#include <Magick++.h>
#include <iostream>
#include <list>

using namespace std;
using namespace Magick;

int main(int argc, char **argv) {
  InitializeMagick(*argv);

  list<Image> imageList;
  try {

    readImages(&imageList, "d.gif");

    Image appended;

    coalesceImages(&imageList, imageList.begin(), imageList.end());
    appendImages(&appended, imageList.begin(), imageList.end());

    appended.write("append.png");
  }catch(Exception &err) {
    cout << "exception" << err.what() << endl;
    return 1;
  }
  return 0;
}
