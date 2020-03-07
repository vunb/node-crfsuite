# node-crfsuite

A nodejs binding for crfsuite

[![MIT License](https://img.shields.io/badge/license-MIT_License-green.svg?style=flat-square)](./LICENSE)
[![npm version](https://img.shields.io/npm/v/crfsuite.svg?style=flat)](https://www.npmjs.com/package/crfsuite)
[![downloads](https://img.shields.io/npm/dm/crfsuite.svg)](https://www.npmjs.com/package/crfsuite)
[![Travis](https://travis-ci.org/vunb/node-crfsuite.svg?branch=master)](https://travis-ci.org/vunb/node-crfsuite)
[![Appveyor](https://ci.appveyor.com/api/projects/status/9gd460vxd6jbel14/branch/master?svg=true)](https://ci.appveyor.com/project/vunb/node-crfsuite/branch/master)

> This is a link to the CRFSuite library written by Naoaki Okazaki. CRF or Conditional Random Fields are a class of statistical modeling method often applied in pattern recognition and machine learning and used for structured prediction.

# Installation

For most "standard" use cases (on Mac, Linux, or Windows on a x86 or x64 processor), `node-crfsuite` will install easy with:

> npm install crfsuite

# API Usage

## CRFSuite Tagger

```js
const crfsuite = require('crfsuite')
const tagger = new crfsuite.Tagger()

let is_opened = tagger.open('./path/to/crf.model')
console.log('File model is opened:', is_opened)

let tags = tagger.tag(input)
console.log('Tags: ', tags)
```

## CRFSuite Trainer

```js
const path = require('path')
const crfsuite = require('crfsuite')
const trainer = new crfsuite.Trainer({
  debug: true
})

let model_filename = path.resolve('./model.crfsuite')

let xseq = [['walk'], ['walk', 'shop'], ['clean', 'shop']]
let yseq = ['sunny', 'sunny', 'rainy']

// submit training data to the trainer
trainer.append(xseq, yseq)
trainer.train(model_filename)

// output: ./model.crfsuite
```

# Installation Special Cases

We use [node-pre-gyp](https://github.com/mapbox/node-pre-gyp) to compile and publish binaries of the library for most common use cases (Linux, Mac, Windows on standard processor platforms). If you have a special case, `node-crfsuite` will work, but it will compile the binary during the install. Compiling with nodejs is done via [node-gyp](https://github.com/nodejs/node-gyp) which requires Python 2.x, so please ensure you have it installed and in your path for all operating systems. Python 3.x will not work.

* See [node-gyp installation prerequisites](https://github.com/nodejs/node-gyp#installation).

## Build from source

```bash
# clone the project
git clone --recursive https://github.com/vunb/node-crfsuite.git

# go to working folder
cd node-crfsuite

# install dependencies and build the binary
npm install
```

For development:

```bash
# rebuild
npm run build

# run unit-test
npm test
```

# Change Log

### 2019-07-18: Release version 1.0.0

* Convert all sources to use N-API, remove `nan`
* Add typescript declaration file
* Cleanup package dependencies
* CI Tool run & test only in node version 10 LTS
* Add project convention: `.gitattributes`, `.editorconfig`
* Enforced to use `new` keyword to create new **Tagger** and **Trainer**

### 2019-06-09: uses N-API

From `crfsuite@0.9.6` the library uses `N-API` to use the binary in multiple version of Node.

* [Acording to Node.js Foundation](https://medium.com/the-node-js-collection/4f35b781f00e): With N-API, native module developers can compile their module once per platform and architecture, and make it available for any version of Node.js that implements N-API. This holds true even for versions of Node.js that are built with a different VM, e.g. [Node-ChakraCore](https://github.com/nodejs/node-chakracore/).

# Contributing

Pull requests and stars are highly welcome.

For bugs and feature requests, please [create an issue](https://github.com/vunb/node-crfsuite/issues/new).
