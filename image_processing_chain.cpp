#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat histogram(Mat imageSrc);
Mat gaussianBlur (Mat imageSrc);
Mat kmean (Mat imageSrc);

/*HAM CHINH*/
int main(int argc, char** argv){
  Mat image;
  cout << "\n \n" << endl;
  cout << "|********** CHUOI XU LY ANH- LE THI MAI - 20118994 *********|" << endl;
  cout << "|-----------------------------------------------------------|" << endl;
  cout << "|1. THU NHAN ANH                                            |" << endl;
  image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

  if(!image.data){
    cout<<"Error loading image" << "\n";
    return -1;
  }
  waitKey(0);

  cout << "|2. TIEN XU LY                                              |" << endl;
  waitKey(0);

  Mat blurredImage = gaussianBlur(image);
  cin.get();


  Mat contrastImage = histogram(blurredImage);
  cin.get();

  cout << "|3. PHAN VUNG ANH                                           |" << endl;
  waitKey(0);

  Mat kmeanImage = kmean(contrastImage);
  cin.get();

  cout << "|4. HAU XU LY                                               |" << endl;
  waitKey(0);

}

/*HAM TANG CUONG DO TUONG PHAN CUA ANH BY EQUALIZE HISTOGRAM FOR COLOR IMAGE*/
Mat histogram(Mat imageSrc) {
  Mat imageHsv, contrastImage;
  cvtColor(imageSrc, imageHsv, CV_BGR2HSV);
  vector<Mat> hsvChannels;
  // Tách imageHsv thành 3 kênh màu
  split(imageHsv, hsvChannels);
  // Cân bằng histogram kênh màu v (Value)
  equalizeHist(hsvChannels[2], hsvChannels[2]);
  // Trộn ảnh
  merge(hsvChannels, imageHsv);
  // Chuyển đổi HSV sang RGB để hiển thị
  cvtColor(imageHsv, contrastImage, CV_HSV2BGR);
  cout << "| * EQUALIZE HISTOGRAM: tang cuong do tuong phan cua anh mau|" << endl;
  imshow("contrastImage", contrastImage);
  waitKey(0);
  return contrastImage;
}

/*HAM LAM TRON ANH BY GAUSS FILTER*/
Mat gaussianBlur (Mat imageSrc) {
  Mat blurredImage;
  GaussianBlur(imageSrc, blurredImage, Size( 9, 9 ), 1.0);
  imshow("Blurred Image" , blurredImage);
  cout << "| * GAUSS FILTER: loai bo nhieu tren anh                    |" << endl;
  waitKey(0);
  return blurredImage;
}

/*HAM PHAN VUNG ANH BY KMEANS*/

Mat kmean (Mat imageSrc) {
  Mat kmeanImage(imageSrc.size(), imageSrc.type());
  Mat points;
  imageSrc.convertTo(points, CV_32FC3);
  points = points.reshape(3, imageSrc.rows*imageSrc.cols);
  Mat_<int> clusters(points.size(), CV_32SC1);
  Mat centers;

  const int cluster = 5;
  kmeans(points, cluster, clusters, cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1.0), 1, KMEANS_PP_CENTERS, centers);
  MatIterator_<Vec3b> itd = kmeanImage.begin<Vec3b>(),itd_end = kmeanImage.end<Vec3b>();
  for (int i = 0; itd != itd_end; ++itd, ++i) {
        Vec3f &color = centers.at<Vec3f>(clusters(i), 0);
        (*itd)[0] = saturate_cast<uchar>(color[0]);
        (*itd)[1] = saturate_cast<uchar>(color[1]);
        (*itd)[2] = saturate_cast<uchar>(color[2]);
    }
    imshow("kmeanImage", kmeanImage);
    cout << "| * KMEANS: Phan vung anh                                   |" << endl;
    waitKey(0);
    return kmeanImage;
}
