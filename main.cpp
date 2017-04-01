
#include <string>
#include <iostream>
#include <algorithm>
#include <opencv/highgui.h>
#include "mapmerge.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
  bool verbose = false;
  string outfile = "final.pgm";
  vector<string> infiles;
  float max_distance = 5.;

  // load the images
  if(argc <= 3){
    cerr << "error: format <inputfile1> <inputfile2> <outputfile>" << endl;
    exit(-1);
  }

  infiles.push_back(argv[1]);
  infiles.push_back(argv[2]);
  outfile = argv[3];
  vector<Mat> images;
  for (size_t i=0; i<infiles.size(); i++) {
    Mat image = imread(infiles[i].c_str(), 0); // 0=grayscale
    if (!image.data) {
      cerr << "error: image " << infiles[i] << " not loadable." << endl;
      exit(-1);
    }
    images.push_back(image);
  }
  // create the stitched map
  StitchedMap map(images[0],images[1], max_distance);
  // write to outfile if applicable
  if (outfile.size() != 0) {
    imwrite(outfile, map.get_stitch());
  }

  if (outfile.size() == 0 || verbose) { // generate some output
    cout << "rotation: "          << map.rotation << endl
         << "translation (x,y): " << map.transx << ", " << map.transy << endl
         << "matrix: "            << map.H << endl;
  }

  if (verbose) {
    namedWindow("wrap"); imshow("wrap", map.get_stitch()); imwrite("stitch.pgm", map.get_stitch());
    namedWindow("debug"); imshow("debug", map.get_debug()); imwrite("debug.pgm", map.get_debug());

    while ((waitKey(0)&255) != 10) // RETURN
      ;
  }

  return 0;
}
