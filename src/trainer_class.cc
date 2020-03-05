#include <iostream>
#include "trainer_class.h"

Napi::FunctionReference TrainerClass::constructor;

NodeTrainer::NodeTrainer(bool debug) : CRFSuite::Trainer()
{
  this->debug = debug;
}

void NodeTrainer::message(const std::string &msg)
{
  if ( this->debug == true ) std::cout << msg;
}

// TrainerClass::TrainerClass(const Napi::CallbackInfo &info)
//     : Napi::ObjectWrap<TrainerClass>(info), trainer(NULL)
// {
//   Napi::HandleScope scope(info.Env());
// }

Napi::Object TrainerClass::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);
  Napi::Function func = DefineClass(env, "TrainerClass",
                                    {InstanceMethod("init", &TrainerClass::InitTrainer),
                                     InstanceMethod("get_params", &TrainerClass::GetParams),
                                     InstanceMethod("set_params", &TrainerClass::SetParams),
                                     InstanceMethod("append", &TrainerClass::Append),
                                     InstanceMethod("train", &TrainerClass::Train)

                                    });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Trainer", func);
  return exports;
}

TrainerClass::TrainerClass(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<TrainerClass>(info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  bool debug = false;
  if ( info.Length() == 1 ){
    Napi::Object options = info[0].As<Napi::Object>();
    if ( options.Has("debug") ){
      debug = options.Get("debug").ToBoolean().Value();
    }
  }

  this->trainer = new NodeTrainer(debug);
  this->trainer->select("lbfgs", "crf1d");
}

Napi::Value TrainerClass::InitTrainer(const Napi::CallbackInfo &info)
{
  //  Isolate* isolate = args.GetIsolate();
  //  TrainerClass* obj = ObjectWrap::Unwrap<TrainerClass>(args.Holder());
  //  obj->trainer = new NodeTrainer();
  //  args.GetReturnValue().Set(v8::Boolean::New(isolate, obj->trainer->select("lbfgs", "crf1d")));
  return Napi::Boolean::New(info.Env(), true);
}

Napi::Value TrainerClass::GetParams(const Napi::CallbackInfo &info)
{
  CRFSuite::StringList params = this->trainer->params();

  // Create a new empty object.
  Napi::Object obj = Napi::Object::New(info.Env());

  for (size_t i = 0; i < params.size(); i++)
  {
    obj.Set(params[i], this->trainer->get(params[i]));
  }

  return obj;
}

void TrainerClass::SetParams(const Napi::CallbackInfo &info)
{
  if (info.Length() == 0 || !info[0].IsObject())
  {
    Napi::TypeError::New(info.Env(), "Options argument is missing or invalid").ThrowAsJavaScriptException();
  }

  Napi::Object params = info[0].As<Napi::Object>();
  Napi::Array property_names = params.GetPropertyNames();

  for (size_t i = 0; i < property_names.Length(); ++i)
  {
    Napi::Value key = property_names.Get(i);
    Napi::Value value = params.Get(key);

    if (key.IsString())
    {
      this->trainer->set(key.As<Napi::String>().Utf8Value(), value.ToString().Utf8Value());
    }
    else
    {
      Napi::TypeError::New(info.Env(), "Invalid parameter name").ThrowAsJavaScriptException();
    }
  }
}

void TrainerClass::Append(const Napi::CallbackInfo &info)
{
  if (info.Length() < 2)
  {
    Napi::TypeError::New(info.Env(), "Invalid number of arguments").ThrowAsJavaScriptException();
  }

  if (!info[0].IsArray())
  {
    Napi::TypeError::New(info.Env(), "xseq (training data) argument must be an array of arrays").ThrowAsJavaScriptException();
  }

  if (!info[1].IsArray())
  {
    Napi::TypeError::New(info.Env(), "yseq (labels) argument must be an array").ThrowAsJavaScriptException();
  }

  Napi::Array xseq = info[0].As<Napi::Array>(); // Local<Array>::Cast(args[0]);
  Napi::Array yseq = info[1].As<Napi::Array>(); // Local<Array>::Cast(args[1]);

  if (xseq.Length() != yseq.Length())
  {
    Napi::TypeError::New(info.Env(), "xseq and yseq must be of same size").ThrowAsJavaScriptException();
  }

  CRFSuite::ItemSequence items;
  CRFSuite::StringList labels;

  for (size_t i = 0; i < xseq.Length(); ++i)
  {
    Napi::Value val = xseq.Get(i);
    if (!val.IsArray())
    {
      Napi::TypeError::New(info.Env(), "xseq (training data) argument must be an array of arrays").ThrowAsJavaScriptException();
    }

    Napi::Array xxseq = val.As<Napi::Array>();

    CRFSuite::Item item;
    item.empty();

    for (size_t j = 0; j < xxseq.Length(); ++j)
    {
      Napi::String attrName = xxseq.Get(j).ToString();
      item.push_back(CRFSuite::Attribute(attrName.Utf8Value()));
    }
    items.push_back(item);
  }

  for (size_t i = 0; i < yseq.Length(); ++i)
  {
    Napi::String attrName = yseq.Get(i).ToString();
    labels.push_back(attrName.Utf8Value());
  }

  try
  {
    this->trainer->append(items, labels, 0);
  }
  catch (std::invalid_argument &e)
  {
    Napi::TypeError::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
  }
  catch (std::runtime_error &e)
  {
    Napi::TypeError::New(info.Env(), "Out of memory").ThrowAsJavaScriptException();
  }
}

Napi::Value TrainerClass::Train(const Napi::CallbackInfo &info)
{
  if (info.Length() < 1 || !info[0].IsString())
  {
    Napi::TypeError::New(info.Env(), "Path to model file is missing or invalid").ThrowAsJavaScriptException();
  }

  Napi::String path = info[0].As<Napi::String>();
  int32_t status = this->trainer->train(path.Utf8Value(), -1);

  return Napi::Number::New(info.Env(), status);
}
