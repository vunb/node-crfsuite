var path = require('path');
var dir = '../test/specs/';

[
    'tagger'
].forEach((script) => {
    require(path.join(dir, script));
});