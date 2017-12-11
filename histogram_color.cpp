#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#pragma GCC diagnostic ignored "-Wwrite-strings"

using namespace std;
using namespace cv;

int main(int argc, char** argv ){
//Mat imageSrc = imread("test.jpg", CV_LOAD_IMAGE_COLOR);
Mat imageSrc, imageHsv, imageDst;
imageSrc = imread( argv[1], 1);

cvtColor(imageSrc, imageHsv, CV_BGR2HSV);

vector<Mat> hsvChannels;
// Tách imageHsv thành 3 kênh màu
split(imageHsv, hsvChannels);

// Cân bằng histogram kênh màu v (Value)
equalizeHist(hsvChannels[2], hsvChannels[2]);

// Trộn ảnh
merge(hsvChannels, imageHsv);

// Chuyển đổi HSV sang RGB để hiển thị
cvtColor(imageHsv, imageDst, CV_HSV2BGR);

imshow("imageSrc", imageSrc);

imshow("imageDst", imageDst);

waitKey(0);
}


