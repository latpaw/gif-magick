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
  void** buffer = (void**)malloc(buffer_length);
  status = napi_get_buffer_info(env, argv[0], buffer, &length);

  //  cout << "length:" << length << endl;
  //  char* src = (char *)malloc(100);
  //  size_t srcResult;
  //  status = napi_get_value_string_utf8(env, argv[0], src, 100, &srcResult);

  // get the direction of merge image
  //  bool direction;
  //  status = napi_get_value_bool(env, argv[2], &direction);

  napi_value minify;
  const char* key = "minify";
  status = napi_get_named_property(env, argv[2], key, &minify);
  bool minifyBool;
  napi_get_value_bool(env, minify, &minifyBool);

  napi_value direction;
  const char* dkey = "direction";
  status = napi_get_named_property(env, argv[2], dkey, &direction);
  bool directionBool;
  napi_get_value_bool(env, direction, &directionBool);

  napi_value isGif;
  const char* gifKey = "isGif";
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

  try {
    Image appended;

    // read the gif from blob
    if (isGifBool) {

      readImages(&imageList, srcBlob);

      // progressing
      coalesceImages(&imageList, imageList.begin(), imageList.end());
      appendImages(&appended, imageList.begin(), imageList.end(), !directionBool);

    } else {

      appended.read(srcBlob);

    }

    //    appended.gaussianBlur(10,15);
    if(minifyBool){
      cout << "minify" << endl;
      appended.minify();
    }
    cout << "width" << appended.size().width() << minifyBool << endl;
    cout << "height" << appended.size().height() << endl;

    // write into the blob
    appended.write(&blob);

    //    cout << "done" << endl;
  }catch(Magick::Exception &err) {
    cout << "exception" << err.what() << endl;
  }

  napi_value result;

  const void* data = blob.data();
  void** result_data;

  // read the result from blob.data and return to js
  status = napi_create_buffer_copy(env, blob.length(), data, result_data, &result);

  return result;
}

napi_value Init(napi_env env, napi_value exports)
{
  //  napi_status status;
  napi_value convert;
  napi_create_function(env, NULL, 0, Convert, NULL, &convert);
  napi_set_named_property(env, exports, "convert", convert);
  return exports;
}

NAPI_MODULE(im, Init)
