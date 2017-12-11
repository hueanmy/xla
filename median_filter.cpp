// Loai bo nhieu xung/nhieu muoi tieu
// bo loc thong thap: lam tron anh
// Thuc hien: 
// 	- lay so lieu vao tung cua so 3x3.
// 	- tao mang 1 chieu, sap tang dan va phan tu f(x,y) duoc thay bang phan tu
// 	chinh giua.
// For blurring and smoothing image
#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
  Mat src, dst;
  float sum;

  src = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  if( !src.data )
  { return -1; }

  float Kernel[3][3] = {
    {1/9.0, 1/9.0, 1/9.0},
    {1/9.0, 1/9.0, 1/9.0},
    {1/9.0, 1/9.0, 1/9.0}
  };
  /*
    Tinh toan 8 gia tri pixel xung quanh pixel dang xet
    Thay the ket qua vao pixel do
    Lap lai qua trinh tren anh
  */
  for(int y = 0; y < src.rows; y++)
    for(int x = 0; x < src.cols; x++)
      dst.at<uchar>(y,x) = 0.0;
  for(int y = 1; y < src.rows - 1; y++){
    for(int x = 1; x < src.cols - 1; x++){
      sum = 0.0;
      for(int k = -1; k <= 1;k++){
        for(int j = -1; j <=1; j++){
          sum = sum + Kernel[j+1][k+1]*src.at<uchar>(y - j, x - k);
        }
      }
      dst.at<uchar>(y,x) = sum;
    }
  }

  namedWindow("after");
  imshow("after", dst);

  namedWindow("before");
  imshow("before", src);

  waitKey();
  return 0;
}
