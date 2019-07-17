#include "tagger_class.h"

Napi::FunctionReference TaggerClass::constructor;

TaggerClass::TaggerClass(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<TaggerClass>(info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  tagger = new CRFSuite::Tagger();
}

Napi::Object TaggerClass::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);
  Napi::Function func = DefineClass(env, "TaggerClass",
                                    {InstanceMethod("open", &TaggerClass::Open),
                                     InstanceMethod("close", &TaggerClass::Close),
                                     InstanceMethod("tag", &TaggerClass::Tag),
                                     InstanceMethod("get_labels", &TaggerClass::GetLabels)

                                    });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Tagger", func);
  return exports;
}

Napi::Value TaggerClass::Open(const Napi::CallbackInfo &info)
{
  if (info.Length() < 1 || !info[0].IsString())
  {
    Napi::TypeError::New(info.Env(), "Path to model file is missing or invalid").ThrowAsJavaScriptException();
  }

  Napi::String path = info[0].As<Napi::String>();
  return Napi::Boolean::New(info.Env(), this->tagger->open(path.Utf8Value()));
}

Napi::Value TaggerClass::Close(const Napi::CallbackInfo &info)
{
  this->tagger->close();
  return Napi::Boolean::New(info.Env(), true);
}

Napi::Value TaggerClass::Tag(const Napi::CallbackInfo &info)
{
  if (info.Length() < 1)
  {
    Napi::TypeError::New(info.Env(), "xseq is missing").ThrowAsJavaScriptException();
  }
  else if (!info[0].IsArray())
  {
    Napi::TypeError::New(info.Env(), "xseq must be an array of arrays").ThrowAsJavaScriptException();
  }

  Napi::Array xseq = info[0].As<Napi::Array>();

  CRFSuite::ItemSequence items;
  CRFSuite::StringList labels;

  for (size_t i = 0; i < xseq.Length(); ++i)
  {
    Napi::Value val = xseq.Get(i);
    if (!val.IsArray())
    {
      Napi::TypeError::New(info.Env(), "xseq must be an array of arrays").ThrowAsJavaScriptException();
    }

    Napi::Array xxseq = val.As<Napi::Array>();

    CRFSuite::Item item;
    item.empty();

    for (size_t j = 0; j < xxseq.Length(); ++j)
    {
      Napi::String observable = xxseq.Get(j).ToString();
      item.push_back(CRFSuite::Attribute(observable.Utf8Value()));
    }
    items.push_back(item);
  }

  labels = this->tagger->tag(items);

  // Create a new empty array.
  Napi::Array result = Napi::Array::New(info.Env(), labels.size());

  for (size_t i = 0; i < labels.size(); i++)
  {
    Napi::String value = Napi::String::New(info.Env(), labels[i]);
    result.Set(i, value);
  }

  return result;
}

Napi::Value TaggerClass::GetLabels(const Napi::CallbackInfo &info)
{
  CRFSuite::StringList list = this->tagger->labels();

  // Create a new empty array.
  Napi::Array result = Napi::Array::New(info.Env(), list.size());

  for (size_t i = 0; i < list.size(); i++)
  {
    Napi::String value = Napi::String::New(info.Env(), list[i]);
    result.Set(i, value);
  }

  return result;
}
