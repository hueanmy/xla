#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat histogram(Mat imageSrc);
Mat gaussianBlur (Mat imageSrc);

int main(int argc, char** argv){
  Mat image;
  image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

  if(!image.data){
    cout<<"Error loading image" << "\n";
    return -1;
  }

  Mat blurredImage = gaussianBlur(image);
  cin.get();

  histogram(blurredImage);
  cin.get();
}

Mat histogram(Mat imageSrc) {
  Mat imageHsv, imageDst;
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
  cout << "enhancing constrast with equalize histogram for color image" << endl;
  imshow("imageDst", imageDst);
  waitKey(0);
  return imageDst;
}

Mat gaussianBlur (Mat imageSrc) {
  Mat blurredImage;
  GaussianBlur(imageSrc, blurredImage, Size( 9, 9 ), 1.0);
  imshow("Blurred Image" , blurredImage);
  cout << "Blurring with GaussianBlur" << endl;
  waitKey(0);
  return blurredImage;
}
