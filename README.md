# node-crfsuite

A nodejs binding for crfsuite

[![MIT License](https://img.shields.io/badge/license-MIT_License-green.svg?style=flat-square)](./LICENSE)
[![npm version](https://img.shields.io/npm/v/crfsuite.svg?style=flat)](https://www.npmjs.com/package/crfsuite)
[![downloads](https://img.shields.io/npm/dm/crfsuite.svg)](https://www.npmjs.com/package/crfsuite)
[![Travis](https://travis-ci.org/vunb/node-crfsuite.svg?branch=master)](https://travis-ci.org/vunb/node-crfsuite)
[![Appveyor](https://ci.appveyor.com/api/projects/status/9gd460vxd6jbel14/branch/master?svg=true)](https://ci.appveyor.com/project/vunb/node-crfsuite/branch/master)

> This is a link to the CRFSuite library written by Naoaki Okazaki. CRF or Conditional Random Fields are a class of statistical modeling method often applied in pattern recognition and machine learning and used for structured prediction.

# Installation

Using npm:

> npm install crfsuite --save

# API Usage

See [docs](http://node-crfsuite.rtfd.io) and [examples](./examples).

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

# Contributing

Pull requests and stars are highly welcome.

For bugs and feature requests, please [create an issue](https://github.com/vunb/node-crfsuite/issues/new).
