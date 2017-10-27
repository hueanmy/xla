/*
  Muc dich: tim duong bien cuc bo trong anh
  Khu nhieu bang cach ap dung Gaussian filter
  Chuyen anh goc sang grayscale
  Ap dung toan tu Laplace tren anh grayscale va luu anh output
  Hien thi anh
*/
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wwrite-strings"


using namespace cv;

int main( int argc, char** argv )
{
  Mat src, src_gray, dst;
  int kernel_size = 3;
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  char* window_name = "Laplace Demo";
  int c;

  src = imread( argv[1] );

  if( !src.data )
  { return -1; }

  /*
    Ap dung GaussianBlur de khu nhieu
    src: anh dau vao
    src#2: anh dau ra
    Size: kich co cua ma tran mat na
  */
  GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

  /*
    Chuyen anh sang dang grayscale
  */
  cvtColor( src, src_gray, CV_BGR2GRAY );

  /*
    Tao cua so moi
  */
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /*
    Ap dung toan tu LP
  */
  Mat abs_dst;

  /*
    src_gray: anh dau vao
    dst: anh dau ra
    ddepth: do sau cua anh dau ra
    kernel_size: kich co cua ma tran mat na Sobel, hien tai la 3
    scale, delta, BORDER_DEFAULT: de gai tri mac dinh
  */
  Laplacian( src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );


  /*
    Chuyen output tu dang toan tu LP sang CV_8U image
  */
  convertScaleAbs( dst, abs_dst );
  imshow( window_name, abs_dst );

  waitKey(0);

  return 0;
}
