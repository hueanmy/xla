#include <iostream>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;


int selection = 0;
int k = 0;
void nhanchap(Mat image, int kernel_size);
void canny();
void exit();

int main(int argc, char** argv) {
  Mat image;
  image = imread(argv[1]);
  if (!image.data)  { return -1; }

  while (k == 0) {
    selection = 0;
    cout << " Please choose from the following options: \n";
    cout << " 1. Nhan Chap.\n";
    cout << " 2. Set launch codes.\n";
    cout << " 3. Exit.\n";
    cout << "Your selection:\t";
    cin >> selection;

  switch (selection) {
    case 1: {
      nhanchap(image, 6);
      break;
    }
    case 2: {
      canny();
      break;
    }
    case 3: {
      return 0;
    }
  }
}
 return 0;
}

void nhanchap(Mat image, int kernel_size) {
  cout << "Nhan Chap" << endl;
  Mat dst,kernel;
  /*
    Tao ma tran loc de nhan chap voi ma tran dau vao
    Cac phan tu cua ma tran deu bang nhau => tien hanh thao tac loc thong thap
  */
  kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);

  /*
    Ap dung nhan chap anh voi ma tran do
  */
  filter2D(image, dst, -1 , kernel, Point( -1, -1 ), 0, BORDER_DEFAULT );
  namedWindow( "nhan chap day ahihi", CV_WINDOW_AUTOSIZE );
  imshow( "nhan chap day ahihi", dst );
  waitKey(0);
}

void canny() {
  cout << "canny" << endl;
}

void exit() {
  cout << endl;
  cout << "See you soon!\n";
  k = 1;
}
