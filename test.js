const im = require('./build/Release/im.node');
const fs = require('fs')
// console.log(im.convert)
const buffer = fs.readFileSync('h.gif');
// console.log('raw buffer', buffer, buffer.length, typeof buffer);
const world = im.convert(buffer, buffer.length, {"minify": true});
// console.log("result js",world);

fs.writeFileSync('hello.png',world);
