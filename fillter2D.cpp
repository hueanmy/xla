#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;


void conv2(Mat src, int kernel_size) {
  Mat dst,kernel;
  /*
    Tao ma tran loc de nhan chap voi ma tran dau vao
    Cac phan tu cua ma tran deu bang nhau => tien hanh thao tac loc thong thap
  */
  kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);

  /*
    Ap dung nhan chap anh voi ma tran do
  */
  filter2D(src, dst, -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT );
  namedWindow( "nhan chap day ahihi", CV_WINDOW_AUTOSIZE );
  imshow( "nhan chap day ahihi", dst );
}

int main ( int argc, char** argv ) {
  Mat src;

  src = imread(argv[1]);
  if( !src.data )  { return -1; }

  conv2(src,6);

  waitKey(0);
  return 0;
}
