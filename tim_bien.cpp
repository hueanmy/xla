#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#pragma GCC diagnostic ignored "-Wwrite-strings"

using namespace cv;


Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";

void CannyThreshold(int, void*)
{
  // lam mo anh voi kernel size = 3
  blur( src_gray, detected_edges, Size(3,3) );

  // Su dung Canny
  // Canny (source_image_in_gray_scale, output, users_value, threetimes lower thresshold, kernel_size)
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  // Tao output cua Canny la mot mat na, full 0 la den tuyen luon :)
  dst = Scalar::all(0);

  // chi lay vien (vien tren nen den)
  src.copyTo( dst, detected_edges);
  imshow( window_name, dst );
}


int main( int argc, char** argv )
{
  src = imread( argv[1] );

  if( !src.data )
  { return -1; }

  // Tao ma tran cung kich thuoc anh
  dst.create( src.size(), src.type() );

  // Chuyen anh ve grayscale
  cvtColor( src, src_gray, CV_BGR2GRAY );

  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  // tao trackbar
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

  CannyThreshold(0, 0);

  waitKey(0);

  return 0;
}
