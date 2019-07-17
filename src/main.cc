#include <crfsuite.hpp>
#include "trainer_class.h"
#include "tagger_class.h"

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
  TrainerClass::Init(exports);
  TaggerClass::Init(exports);
  return exports;
}

NODE_MODULE(crfsuite, Initialize)
