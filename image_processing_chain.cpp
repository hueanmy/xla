#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat histogram(Mat imageInput);
Mat gaussianBlur (Mat imageInput);
Mat kmean (Mat imageInput);
Mat morphology_operations (Mat imageInput);

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

  cout << "|4. HAU XU LY MORPHOLOGY: Closing, Openning                 |" << endl;
  waitKey(0);

  Mat morphologyImage = morphology_operations(kmeanImage);
  cin.get();
  waitKey(0);

  cout << "|KET QUA CHUOI XU LY                                        |" << endl;
  imshow("Result of proccessing image chain", morphologyImage);
  waitKey(0);

  cout << "|********************* HEN GAP LAI *************************|" << endl;
}

/*HAM TANG CUONG DO TUONG PHAN CUA ANH BY EQUALIZE HISTOGRAM FOR COLOR IMAGE*/
Mat histogram(Mat imageInput) {
  Mat imageHsv, contrastImage;
  cvtColor(imageInput, imageHsv, CV_BGR2HSV);
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
Mat gaussianBlur (Mat imageInput) {
  Mat blurredImage;
  GaussianBlur(imageInput, blurredImage, Size( 9, 9 ), 1.0);  //size(9,9): kich thuoc mat na duoc su dung
  imshow("Blurred Image" , blurredImage);
  cout << "| * GAUSS FILTER: loai bo nhieu tren anh                    |" << endl;
  waitKey(0);
  return blurredImage;
}

/*HAM PHAN VUNG ANH BY KMEANS*/

Mat kmean (Mat imageInput) {
  Mat kmeanImage(imageInput.size(), imageInput.type());
  Mat points;
  imageInput.convertTo(points, CV_32FC3);
  points = points.reshape(3, imageInput.rows*imageInput.cols);
  Mat_<int> clusters(points.size(), CV_32SC1);
  Mat centers;

  int cluster;
  cout << "Enter cluster: \t";
  cin >> cluster;
  /*
    points:
    cluster: so nhom yeu cau tai thoi diem cuoi cung.
    cvTermCriteria : Đây là tiêu chí chấm dứt vòng lặp. Khi tiêu chí này được thỏa mãn, thuật toán sẽ dừng vòng lặp. Trên thực tế, nó phải là một nhóm 3 thông số (type, max_iter, epsilon):
      3.a - loại tiêu chí chấm dứt: Nó có 3 lá cờ như sau:
          cv2.TERM_CRITERIA_EPS - ngăn sự lặp lại thuật toán nếu đạt được độ chính xác nhất định - epsilon đạt được.
          cv2.TERM_CRITERIA_MAX_ITER - dừng thuật toán sau khi số lượng nhất định được lặp đi lặp lại
          cv2.TERM_CRITERIA_EPS max_iter + cv2.TERM_CRITERIA_MAX_ITER - ngăn chặn sự lặp đi lặp lại khi một trong các điều kiện trên nó được đáp ứng
      3.b - max_iter - Một số nguyên xác định số lượng tối đa vòng lặp.
      3.c - Độ chính xác - epsilon
  */
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

Mat morphology_operations (Mat imageInput) {
  int morph_size = 0;
  Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
  cout<<element;

  Mat morphologyImage; // result matrix
    // Apply the specified morphology operation
  // for (int i=1;i<10;i++){
  //   morphologyEx( imageInput, morphologyImage, MORPH_OPEN, element, Point(-1,-1), i );
  //   morphologyEx( imageInput, morphologyImage, MORPH_CLOSE, element, Point(-1,-1), i );
  //   //morphologyEx( src, dst, MORPH_TOPHAT, element ); // here iteration=1
  //   }
  morphologyEx( imageInput, morphologyImage, MORPH_OPEN, element);
  morphologyEx( imageInput, morphologyImage, MORPH_CLOSE, element);
  // morphologyEx( imageInput, morphologyImage, MORPH_GRADIENT, element);
  imshow("morphologyImage", morphologyImage);
  waitKey(0);
  return morphologyImage;
}
