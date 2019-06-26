module.exports = function() {
  console.log('start png convert');
  const im = require('../build/Release/im.node');
  const fs = require('fs');
  const buffer = fs.readFileSync('test/1.png');
  const result = im.convert(buffer, buffer.length, { minify: true, isGif: false });

  console.log('width: ', result.width, ', height: ', result.height);

  fs.writeFileSync('test/pngtest.png', result.data);
};
