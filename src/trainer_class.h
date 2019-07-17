#ifndef _TRAINER_CLASS_H_
#define _TRAINER_CLASS_H_

#include <napi.h>
#include <crfsuite_api.hpp>

class NodeTrainer : public CRFSuite::Trainer
{
public:
  virtual void message(const std::string &msg);
};

class TrainerClass : public Napi::ObjectWrap<TrainerClass>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  explicit TrainerClass(const Napi::CallbackInfo &info);

  /**
   * Destructor
   */
  ~TrainerClass()
  {
    if (trainer)
      delete trainer;
  }

private:
  static Napi::FunctionReference constructor;
  NodeTrainer *trainer;

  Napi::Value InitTrainer(const Napi::CallbackInfo &info);
  Napi::Value GetParams(const Napi::CallbackInfo &info);
  void SetParams(const Napi::CallbackInfo &info);
  void Append(const Napi::CallbackInfo &info);
  Napi::Value Train(const Napi::CallbackInfo &info);
};

#endif
