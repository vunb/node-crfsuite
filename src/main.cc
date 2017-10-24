#include <nan.h>
#include <crfsuite.hpp>
#include "trainer_class.h"
#include "tagger_class.h"

void Initialize(v8::Local<v8::Object> exports) {  
  TrainerClass::Init(exports);
  TaggerClass::Init(exports);
}


NODE_MODULE(crfsuite, Initialize)  
