#include <iostream>
#include <node.h>

#include "trainer_class.h"

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Array;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;


Persistent<Function> TrainerClass::constructor;

void NodeTrainer::message(const std::string& msg) { 
  std::cout << msg;
}

TrainerClass::TrainerClass() : trainer(NULL) {

}

TrainerClass::~TrainerClass() {
  if (trainer)
    delete trainer;
}

void TrainerClass::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "TrainerClass"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "init", InitTrainer);
  NODE_SET_PROTOTYPE_METHOD(tpl, "get_params", GetParams);
  NODE_SET_PROTOTYPE_METHOD(tpl, "set_params", SetParams);
  NODE_SET_PROTOTYPE_METHOD(tpl, "append", Append);
  NODE_SET_PROTOTYPE_METHOD(tpl, "train", Train);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "TrainerClass"), tpl->GetFunction());
}


void TrainerClass::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    TrainerClass* obj = new TrainerClass();

    obj->trainer = new NodeTrainer();
    obj->trainer->select("lbfgs", "crf1d");

    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Context> context = isolate->GetCurrentContext();
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}


void TrainerClass::InitTrainer(const FunctionCallbackInfo<Value>& args) {
//  Isolate* isolate = args.GetIsolate();
//  TrainerClass* obj = ObjectWrap::Unwrap<TrainerClass>(args.Holder());
//  obj->trainer = new NodeTrainer();
//  args.GetReturnValue().Set(v8::Boolean::New(isolate, obj->trainer->select("lbfgs", "crf1d")));
}

void TrainerClass::GetParams(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  TrainerClass* obj = ObjectWrap::Unwrap<TrainerClass>(args.Holder());

  CRFSuite::StringList params = obj->trainer->params();

  // Create a new empty array.
  Local<v8::Object> array = v8::Object::New(isolate);

  for (size_t i = 0; i < params.size(); i++) {
    Local<String> name = String::NewFromUtf8(isolate, params[i].c_str());
    Local<String> value = String::NewFromUtf8(isolate, obj->trainer->get(params[i]).c_str());

    array->Set(name, value);
  }

  args.GetReturnValue().Set(array);
}

void TrainerClass::SetParams(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() == 0) { 
    isolate->ThrowException(String::NewFromUtf8(isolate, "Options argument is missing"));
    return;
  }

  if (!args[0]->IsObject()) { 
    isolate->ThrowException(String::NewFromUtf8(isolate, "Not an associative array"));
    return;
  }

  Local<Object> params = Local<Object>::Cast(args[0]);
  Local<Array> property_names = params->GetOwnPropertyNames();
  TrainerClass* obj = ObjectWrap::Unwrap<TrainerClass>(args.Holder());

  for (size_t i = 0; i < property_names->Length(); ++i) {
    Local<Value> key = property_names->Get(i);
    Local<Value> value = params->Get(key);

    if (key->IsString()) {
        String::Utf8Value utf8_key(key);
        String::Utf8Value utf8_value(value->ToString());

        obj->trainer->set(*utf8_key, *utf8_value);
    } else {
      isolate->ThrowException(String::NewFromUtf8(isolate, "invalid parameter name"));
    }
  }

//  TrainerClass* obj = ObjectWrap::Unwrap<TrainerClass>(args.Holder());
}

void TrainerClass::Append(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() < 2) { 
    isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid number of arguments"));
    return;
  }

  if (!args[0]->IsArray()) { 
    isolate->ThrowException(String::NewFromUtf8(isolate, "xseq (training data) argument must be an array of arrays"));
    return;
  }

  if (!args[1]->IsArray()) { 
    isolate->ThrowException(String::NewFromUtf8(isolate, "yseq (labels) argument must be an array"));
    return;
  }

  Local<Array> xseq = Local<Array>::Cast(args[0]);
  Local<Array> yseq = Local<Array>::Cast(args[1]);

  if (xseq->Length() != yseq->Length()) { 
    isolate->ThrowException(String::NewFromUtf8(isolate, "xseq and yseq must be of same size"));
    return;
  }

  CRFSuite::ItemSequence items;
  CRFSuite::StringList labels;

  for (size_t i = 0; i < xseq->Length(); ++i) {
    Local<Value> val = xseq->Get(i);
    if (!val->IsArray()) { 
      isolate->ThrowException(String::NewFromUtf8(isolate, "xseq (training data) argument must be an array of arrays"));
      return;
    }

    Local<Array> xxseq = Local<Array>::Cast(val);

    CRFSuite::Item item;
    item.empty();

    for (size_t j = 0; j < xxseq->Length(); ++j) {
      Local<String> attrName = xxseq->Get(j)->ToString();
      String::Utf8Value utf8_key(attrName);
      item.push_back(CRFSuite::Attribute(*utf8_key));
    }
    items.push_back(item);
  }

  for (size_t i = 0; i < yseq->Length(); ++i) {
      Local<String> attrName = yseq->Get(i)->ToString();
      String::Utf8Value utf8_key(attrName);
      labels.push_back(*utf8_key);
  }

  TrainerClass* obj = ObjectWrap::Unwrap<TrainerClass>(args.Holder());

  try {
    obj->trainer->append(items, labels, 0);
  } catch (std::invalid_argument &e) {
    isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid arguments"));
  } catch (std::runtime_error &e) {
    isolate->ThrowException(String::NewFromUtf8(isolate, "Out of memory"));
  }
}

void TrainerClass::Train(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() < 1) { 
    isolate->ThrowException(String::NewFromUtf8(isolate, "Path to model file is missing"));
    return;
  }

  if (!args[0]->IsString()) { 
    isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid path to model file"));
    return;
  }

  Local<String> path = args[0]->ToString();
  String::Utf8Value utf8_path(path);

  TrainerClass* obj = ObjectWrap::Unwrap<TrainerClass>(args.Holder());

  args.GetReturnValue().Set(v8::Number::New(isolate, obj->trainer->train(*utf8_path, -1)));
}
