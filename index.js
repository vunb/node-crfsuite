var binary = require('node-pre-gyp');
var path = require('path')

var binaryPath = binary.find(path.resolve(path.join(__dirname, './package.json')));
var binding = require(binaryPath);

module.exports = {
    Trainer: binding.TrainerClass,
    Tagger: binding.TaggerClass
};