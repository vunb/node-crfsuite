#include <iostream>
#include <node.h>

//#include <crfsuite.hpp>
#include "tagger_class.h"

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


Persistent<Function> TaggerClass::constructor;

TaggerClass::TaggerClass() {
  tagger = new CRFSuite::Tagger();
}

TaggerClass::~TaggerClass() {
  delete tagger;
}

void TaggerClass::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "TaggerClass"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "open", Open);
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "tag", Tag);
  NODE_SET_PROTOTYPE_METHOD(tpl, "get_labels", GetLabels);


  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "TaggerClass"), tpl->GetFunction());
}


void TaggerClass::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    TaggerClass* obj = new TaggerClass();

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


void TaggerClass::Open(const v8::FunctionCallbackInfo<v8::Value>& args) { 
  Isolate* isolate = args.GetIsolate();
  TaggerClass* obj = ObjectWrap::Unwrap<TaggerClass>(args.Holder());

  if (args.Length() < 1) {
    isolate->ThrowException(String::NewFromUtf8(isolate, "Path to model file is missing"));
    return;
  }

  if (!args[0]->IsString()) {
    isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid path to the model"));
    return;
  }

  Local<String> path = args[0]->ToString();
  String::Utf8Value utf8_path(path);

  args.GetReturnValue().Set(v8::Boolean::New(isolate, obj->tagger->open(*utf8_path)));
}

void TaggerClass::Close(const v8::FunctionCallbackInfo<v8::Value>& args) {
  TaggerClass* obj = ObjectWrap::Unwrap<TaggerClass>(args.Holder());
  obj->tagger->close();
}

void TaggerClass::Tag(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();
  TaggerClass* obj = ObjectWrap::Unwrap<TaggerClass>(args.Holder());


  if (args.Length() < 1) {
    isolate->ThrowException(String::NewFromUtf8(isolate, "xseq is missing"));
    return;
  }

  if (!args[0]->IsArray()) {
    isolate->ThrowException(String::NewFromUtf8(isolate, "xseq must be an array of arrays"));
    return;
  }

  Local<Array> xseq = Local<Array>::Cast(args[0]);

  CRFSuite::ItemSequence items;
  CRFSuite::StringList labels;

  for (size_t i = 0; i < xseq->Length(); ++i) {
    Local<Value> val = xseq->Get(i);
    if (!val->IsArray()) {
      isolate->ThrowException(String::NewFromUtf8(isolate, "xseq must be an array of arrays"));
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

  labels = obj->tagger->tag(items);

  // Create a new empty array.
  Local<v8::Array> array = v8::Array::New(isolate, labels.size());

  for (size_t i = 0; i < labels.size(); i++) {
    Local<String> value = String::NewFromUtf8(isolate, labels[i].c_str());
    array->Set(i, value);
  }

  args.GetReturnValue().Set(array);
}

void TaggerClass::GetLabels(const v8::FunctionCallbackInfo<v8::Value>& args) { 
  Isolate* isolate = args.GetIsolate();
  TaggerClass* obj = ObjectWrap::Unwrap<TaggerClass>(args.Holder());

  CRFSuite::StringList list = obj->tagger->labels();

  // Create a new empty array.
  Local<v8::Array> array = v8::Array::New(isolate, list.size());

  for (size_t i = 0; i < list.size(); i++) {
    Local<String> value = String::NewFromUtf8(isolate, list[i].c_str());
    array->Set(i, value);
  }

  args.GetReturnValue().Set(array);
}


