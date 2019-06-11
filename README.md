# node-crfsuite

This a a fork of the [node-crfsuite](https://github.com/vunb/node-crfsuite) A nodejs binding for crfsuite

[![MIT License](https://img.shields.io/badge/license-MIT_License-green.svg?style=flat-square)](./LICENSE)
[![npm version](https://img.shields.io/npm/v/crfsuite-js.svg?style=flat)](https://www.npmjs.com/package/crfsuite-js)
[![downloads](https://img.shields.io/npm/dm/crfsuite-js.svg)](https://www.npmjs.com/package/crfsuite)
[![Travis](https://travis-ci.org/Trekels/node-crfsuite.svg?branch=master)](https://travis-ci.org/trekels/node-crfsuite)

> This is a link to the CRFSuite library written by Naoaki Okazaki. CRF or Conditional Random Fields are a
class of statistical modeling method often applied in pattern recognition and machine learning and
used for structured prediction.

# Installation

> npm install crfsuite-js --save

# Usage
## CRFSuite Tagger

```js
const crfsuite = require('crfsuite-js');

const tagger = crfsuite.Tagger();

tagger.open('./path/to/crf.model');

let tags = tagger.tag(input);
console.log('Tags: ', tags);
```

## CRFSuite Trainer

```js
const path = require('path');
const crfsuite = require('crfsuite-js');

const trainer = crfsuite.Trainer();

let model_filename = path.resolve('./model.crfsuite');

let xseq = [['walk'], ['walk', 'shop'], ['clean', 'shop']];
let yseq = ['sunny', 'sunny', 'rainy'];

// submit training data to the trainer
trainer.append(xseq, yseq);
trainer.train(model_filename);

// output: ./model.crfsuite
```

## Build from source

```bash
# clone the project
git clone https://github.com/vunb/node-crfsuite.git

# go to working folder
cd node-crfsuite

# install dependencies and tools
npm install

# build node-crfsuite from source
npm run build

# run unit-test
npm test
```

Travis CI - Encrypt Travis CI settings
=============================================

Ref: http://rkh.github.io/travis-encrypt/public/index.html
