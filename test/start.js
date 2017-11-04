var path = require('path');
var dir = '../test/specs/';

[
    'trainer',
    'tagger'
].forEach((script) => {
    require(path.join(dir, script));
});