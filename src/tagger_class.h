#ifndef _TAGGER_CLASS_H_
#define _TAGGER_CLASS_H_

#include <node.h>
#include <node_object_wrap.h>

#include <crfsuite_api.hpp>

class TaggerClass : public node::ObjectWrap {
public:
  static void Init(v8::Local<v8::Object> exports);

private:
  explicit TaggerClass();
  ~TaggerClass();

  static v8::Persistent<v8::Function> constructor;
  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void Open(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Close(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Tag(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void GetLabels(const v8::FunctionCallbackInfo<v8::Value>& args);

  CRFSuite::Tagger *tagger;
};

#endif
