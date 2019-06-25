module.exports = function() {
  console.log('start gif convert');
  const im = require('../build/Release/im.node');
  const fs = require('fs');
  // console.log(im.convert)
  const buffer = fs.readFileSync('test/2.gif');
  // console.log('raw buffer', buffer, buffer.length, typeof buffer);
  const result = im.convert(buffer, buffer.length, { minify: false });
  // console.log("result js",world);

  console.log('width: ', result.width, ', height: ', result.height);
  fs.writeFileSync('test/hello.png', result.data);
};
