#ifndef _TRAINER_CLASS_H_
#define _TRAINER_CLASS_H_

#include <node.h>
#include <node_object_wrap.h>

#include <crfsuite_api.hpp>

class NodeTrainer : public CRFSuite::Trainer { 
public:
  virtual void message(const std::string& msg);
};

class TrainerClass : public node::ObjectWrap {
public:
  static void Init(v8::Local<v8::Object> exports);

private:
  explicit TrainerClass();
  ~TrainerClass();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void InitTrainer(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void GetParams(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void SetParams(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Append(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Train(const v8::FunctionCallbackInfo<v8::Value>& args);

  static v8::Persistent<v8::Function> constructor;

  NodeTrainer *trainer;
};

#endif
