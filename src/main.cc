#include <crfsuite.hpp>
#include "trainer_class.h"
#include "tagger_class.h"

#if NAPI_VERSION
// N-API code goes here
#else
// NAN code goes here
#include <nan.h>
#endif

void Initialize(v8::Local<v8::Object> exports) {
  TrainerClass::Init(exports);
  TaggerClass::Init(exports);
}


NODE_MODULE(crfsuite, Initialize)
