#include <Magick++.h>
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <node_api.h>
#include <iostream>
#include <list>

using namespace v8;
using namespace node;
using namespace std;
using namespace Magick;

napi_value Convert(napi_env env, napi_callback_info info)
{
  napi_status status;

  // 3 args, all napi_value
  size_t argc = 3;
  napi_value argv[3];

  // read the argv
  status = napi_get_cb_info(env, info, &argc, argv, 0, 0);

  // get buffer length
  double buffer_length;
  status = napi_get_value_double(env, argv[1], &buffer_length);

  // get buffer
  size_t length;
  void **buffer = (void **)malloc(buffer_length);
  status = napi_get_buffer_info(env, argv[0], buffer, &length);

  //  cout << "length:" << length << endl;
  //  char* src = (char *)malloc(100);
  //  size_t srcResult;
  //  status = napi_get_value_string_utf8(env, argv[0], src, 100, &srcResult);

  // get the direction of merge image
  //  bool direction;
  //  status = napi_get_value_bool(env, argv[2], &direction);

  napi_value minify;
  const char *key = "minify";
  status = napi_get_named_property(env, argv[2], key, &minify);
  bool minifyBool;
  napi_get_value_bool(env, minify, &minifyBool);

  napi_value direction;
  const char *dkey = "direction";
  status = napi_get_named_property(env, argv[2], dkey, &direction);
  bool directionBool;
  napi_get_value_bool(env, direction, &directionBool);

  napi_value isGif;
  const char *gifKey = "isGif";
  status = napi_get_named_property(env, argv[2], gifKey, &isGif);
  bool isGifBool;
  napi_get_value_bool(env, isGif, &isGifBool);

  // init imageList to store gif
  list<Image> imageList;
  cout << "start convert" << endl;

  // the output in blob
  Magick::Blob blob;
  //  cout << length << endl;

  // read the buffer into blob
  Magick::Blob srcBlob(*buffer, (size_t)length);
  //  cout << "blob length:" << srcBlob.length() << endl;

  Image appended;

  try
  {

    // read the gif from blob
    if (isGifBool)
    {

      readImages(&imageList, srcBlob);

      // progressing
      coalesceImages(&imageList, imageList.begin(), imageList.end());
      appendImages(&appended, imageList.begin(), imageList.end(), !directionBool);
    }
    else
    {
      appended.read(srcBlob);
      appended.magick("PNG");
    }

    appended.quality(100);
    appended.compressType(BZipCompression);
    //    appended.gaussianBlur(10,15);
    if (minifyBool)
    {
      cout << "minify" << endl;
      appended.minify();
    }
    // cout << "width" << appended.size().width() << minifyBool << endl;
    // cout << "height" << appended.size().height() << endl;

    // write into the blob
    appended.write(&blob);

    //    cout << "done" << endl;
  }
  catch (Magick::Exception &err)
  {
    cout << "exception" << err.what() << endl;
  }

  napi_value result;

  napi_value resultBuffer;

  const void *data = blob.data();
  void **result_data;

  // read the result from blob.data and return to js
  status = napi_create_buffer_copy(env, blob.length(), data, result_data, &resultBuffer);

  status = napi_create_object(env, &result);
  napi_set_named_property(env, result, "data", resultBuffer);

  napi_value width;
  status = napi_create_int32(env, appended.size().width(), &width);
  napi_set_named_property(env, result, "width", width);

  napi_value height;
  status = napi_create_int32(env, appended.size().height(), &height);
  napi_set_named_property(env, result, "height", height);

  return result;
}

napi_value Thumb(napi_env env, napi_callback_info info)
{
  napi_status status;

  // 3 args, all napi_value
  size_t argc = 3;
  napi_value argv[3];

  // read the argv
  status = napi_get_cb_info(env, info, &argc, argv, 0, 0);

  // get buffer length
  double buffer_length;
  status = napi_get_value_double(env, argv[1], &buffer_length);

  // get buffer
  size_t length;
  void **buffer = (void **)malloc(buffer_length);
  status = napi_get_buffer_info(env, argv[0], buffer, &length);

  napi_value aspect;
  const char *akey = "aspect";
  status = napi_get_named_property(env, argv[2], akey, &aspect);
  bool aspectBool;
  napi_get_value_bool(env, aspect, &aspectBool);

  napi_value width;
  const char *wkey = "width";
  status = napi_get_named_property(env, argv[2], wkey, &width);
  double widthValue = 0;
  napi_get_value_double(env, width, &widthValue);

  napi_value height;
  const char *hkey = "height";
  status = napi_get_named_property(env, argv[2], hkey, &height);
  double heightValue = 0;
  napi_get_value_double(env, height, &heightValue);

  Magick::Blob srcBlob(*buffer, (size_t)length);
  Image image;
  image.read(srcBlob);
  Geometry geo;
  size_t imageNewWidth = widthValue;
  geo.width(imageNewWidth);
  if (heightValue > 1)
  {
    size_t imageNewHeight = heightValue;
    geo.height(heightValue);
  }
  if (aspectBool)
  {
    geo.aspect(aspectBool);
  }
  image.resize(geo);

  Magick::Blob blob;
  image.write(&blob);

  napi_value result;
  napi_value resultBuffer;

  const void *data = blob.data();
  void **result_data;

  // read the result from blob.data and return to js
  status = napi_create_buffer_copy(env, blob.length(), data, result_data, &resultBuffer);

  status = napi_create_object(env, &result);
  napi_set_named_property(env, result, "data", resultBuffer);

  return result;
}

napi_value Init(napi_env env, napi_value exports)
{
  //  napi_status status;
  napi_value convert;
  napi_create_function(env, NULL, 0, Convert, NULL, &convert);
  napi_set_named_property(env, exports, "convert", convert);

  napi_value thumb;
  napi_create_function(env, NULL, 0, Thumb, NULL, &thumb);
  napi_set_named_property(env, exports, "thumb", thumb);
  return exports;
}

NAPI_MODULE(im, Init)
