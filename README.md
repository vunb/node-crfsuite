# node-crfsuite

This a a fork of the [node-crfsuite](https://github.com/vunb/node-crfsuite) A nodejs binding for crfsuite

[![MIT License](https://img.shields.io/badge/license-MIT_License-green.svg?style=flat-square)](./LICENSE)
[![npm version](https://img.shields.io/npm/v/crfsuite.svg?style=flat)](https://www.npmjs.com/package/crfsuite)
[![downloads](https://img.shields.io/npm/dm/crfsuite.svg)](https://www.npmjs.com/package/crfsuite)
[![Travis](https://travis-ci.org/vunb/node-crfsuite.svg?branch=master)](https://travis-ci.org/vunb/node-crfsuite)

> This is a link to the CRFSuite library written by Naoaki Okazaki. CRF or Conditional Random Fields are a class of statistical modeling method often applied in pattern recognition and machine learning and used for structured prediction.

# Installation

For most "standard" use cases (on Mac, Linux, or Windows on a x86 or x64 processor), `node-crfsuite` will install nice and easy with:

> npm install crfsuite --save

## CRFSuite Tagger

```js
const crfsuite = require('crfsuite')
const tagger = crfsuite.Tagger()

let is_opened = tagger.open('./path/to/crf.model')
console.log('File model is opened:', is_opened)

let tags = tagger.tag(input)
console.log('Tags: ', tags)
```

## CRFSuite Trainer

```js
const path = require('path')
const crfsuite = require('crfsuite')
const trainer = crfsuite.Trainer()

let model_filename = path.resolve('./model.crfsuite')

let xseq = [['walk'], ['walk', 'shop'], ['clean', 'shop']]
let yseq = ['sunny', 'sunny', 'rainy']

// submit training data to the trainer
trainer.append(xseq, yseq)
trainer.train(model_filename)

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
