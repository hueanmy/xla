#include <iostream>
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#pragma GCC diagnostic ignored "-Wwrite-strings"

using namespace std;
using namespace cv;

/*Tang cuong do tuong phan*/
void histogrameq_for_grayImg(Mat image);
void histogrameq_for_colorImg(Mat image);
/*nhan chap*/
void nhanchap(Mat image, int kernel_size);
/*Lam tron anh bang loc thong thap*/
void gaussian(Mat image);
void medianFilter(Mat image, int kernel_size);
/*Phat hien canh bang loc thong cao*/
void laplace(Mat image);
void sobel(Mat image);
void canny(Mat image, int kernel_size);
/*xu ly anh trong mien tan so*/
void dft(Mat image);
/*phan vung anh*/
void kmeans(Mat image);
void watershared(Mat image);
/*hau xu ly anh*/
void morphology(Mat image);
/*resize image*/
void resize(Mat image, int width, int height);
/*thoat chuong trinh*/
void exit();

int selection = 0;
int k = 0;

int main(int argc, char** argv) {
  Mat image;
  image = imread(argv[1]);
  if (!image.data)  { return -1; }

  while (k == 0) {
    selection = 0;
    cout << " Please choose from the following options: \n";
    cout << " 1. Increasing Constrast.\n";
    cout << " 2. Convolution.\n";
    cout << " 3. Smoothing image.\n";
    cout << " 4. Edge detection.\n";
    cout << " 5. Discrete Fourier Transform.\n";
    cout << " 6. Image Segmentation.\n";
    cout << " 7. Morphology.\n";
    cout << " 8. Resize.\n";
    cout << " 9. Exit.\n";
    cout << "Your selection:\t";
    cin >> selection;

  switch (selection) {
    case 1: {
      cout << "Chon phuong phap can lam" << endl;
      cout << "a. Histogram Equalize (for grayscale image)" << endl;
      cout << "b. Histogram Equalize (for color image)" << endl;
      cout << "Your selection: " << endl;
      char select_method;
      cin >> select_method;
      switch(select_method) {
        case 'a': {
          histogrameq_for_grayImg(image);
          break;
        }
        case 'b' : {
          histogrameq_for_colorImg(image);
          break;
        }
      }
      break;
    }
    case 2: {
      cout << "Convolution with 2D filter" << endl;
      cout << "Enter your desired kernel size: " << endl;
      int kernel_size;
      cin >> kernel_size;
      nhanchap(image, kernel_size);
      break;
    }
    case 3: {
      cout << "Smoothing image" << endl;
      cout << "Choose your desired method: " << endl;
      cout << "a. Using Gaussian Filter" << endl;
      cout << "b. Using Median Filter" << endl;
      char select_method;
      cin >> select_method;
      switch(select_method) {
        case 'a': {
          cout << "Using gaussian filter" << endl;
          gaussian(image);
          break;
        }
        case 'b': {
          cout << "Using median filter" << endl;
          cout << "Enter kernel size" << endl;
          int kernel_size;
          cin >> kernel_size;
          if (kernel_size %2 == 0) {
            return 0;
          }
          else {
            medianFilter(image, kernel_size);
          }
        }
      }
      break;
    }
    case 4: {
      cout << "Edge detection" << endl;
      cout << "a. Using Laplacian method" << endl;
      cout << "b. Using Sobel method" << endl;
      cout << "c. Using Canny method" << endl;
      cout << "your selection: " << endl;
      char select_method;
      cin >> select_method;
      switch(select_method) {
        case 'a': {
          laplace(image);
          break;
        }
        case 'b': {
          sobel(image);
          break;
        }
        case 'c': {
          cout << "Enter kernel size" << endl;
          int kernel_size;
          cin >> kernel_size;
          canny(image, kernel_size);
          break;
        }
      }
      break;
    }
    case 5: {
      cout << "Discrete Fourier Transform" << endl;
      dft(image);
      break;
    }
    case 6: {
      cout << "Image Segmentation" << endl;
      cout << "a. Kmeans" << endl;
      cout << "b. Watershared" << endl;
      cout << "Choose your desired method: " << endl;
      char select_method;
      cin >> select_method;
      switch(select_method) {
        case 'a': {
          kmeans(image);
          break;
        }
        case 'b': {
          watershared(image);
          break;
        }
      }
      break;
    }
    case 8: {
      cout << "Enter desired size: " << endl;
      cout << "Enter width: \t";
      int width;
      cin >> width;
      cout << "Enter height: \t";
      int height;
      cin >> height;
      resize(image, width, height);
      break;
    }
    case 9: {
      exit();
      break;
    }
  }
}
 return 0;
}

void resize(Mat image, int width, int height) {
  Mat imageDst;
  resize(image, imageDst, cv::Size(), width, height);
  imshow("resize image1", imageDst);
  waitKey(0);
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

void histogrameq_for_grayImg(Mat image){
  Mat image_dst;
  char* source_window = "Source image";
  char* equalized_window = "Equalized Image";

  cvtColor( image, image, CV_BGR2GRAY );

  equalizeHist( image, image_dst );

  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  namedWindow( equalized_window, CV_WINDOW_AUTOSIZE );

  imshow( source_window, image );
  imshow( equalized_window, image_dst );

  waitKey(0);
}

void histogrameq_for_colorImg(Mat image){
  Mat imageHsv, imageDst;
  cvtColor(image, imageHsv, CV_BGR2HSV);

  vector<Mat> hsvChannels;
  // Tách imageHsv thành 3 kênh màu
  split(imageHsv, hsvChannels);

  // Cân bằng histogram kênh màu v (Value)
  equalizeHist(hsvChannels[2], hsvChannels[2]);

  // Trộn ảnh
  merge(hsvChannels, imageHsv);

  // Chuyển đổi HSV sang RGB để hiển thị
  cvtColor(imageHsv, imageDst, CV_HSV2BGR);

  imshow("imageSrc", image);

  imshow("imageDst", imageDst);

  waitKey(0);
}

void gaussian(Mat image){
  Mat blurredImage;
  /*
    Ap dung bo loc Gauss
    Object Size bieu thi kich co cua bo loc (la gioi han cua viec lam mo anh)
  */
  GaussianBlur( image, blurredImage, Size( 9, 9 ), 1.0);
  imshow("Blurred Image" , blurredImage);
  waitKey(0);
}

void medianFilter(Mat image, int kernel_size){
  Mat imageDst;
  imshow("Original Image", image);
  medianBlur( image, imageDst, kernel_size );
  imshow( "Median filter", imageDst );
  waitKey(0);
}

void laplace(Mat image){
  Mat image_gray, image_dst;
  int kernel_size = 3;
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  char* window_name = "Laplace Demo";
  int c;
  /*
    Ap dung GaussianBlur de khu nhieu
    src: anh dau vao
    src#2: anh dau ra
    Size: kich co cua ma tran mat na
  */
  GaussianBlur( image, image, Size(3,3), 0, 0, BORDER_DEFAULT );
  /*
    Chuyen anh sang dang grayscale
  */
  cvtColor( image, image_gray, CV_BGR2GRAY );
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
  Laplacian( image_gray, image_dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
  /*
    Chuyen output tu dang toan tu LP sang CV_8U image
  */
  convertScaleAbs( image_dst, abs_dst );
  imshow( window_name, abs_dst );
  waitKey(0);
}

void sobel(Mat image){
  Mat temp_image;
  Mat sobel_image;
  Sobel(image, temp_image, CV_32F, 1, 1);
  convertScaleAbs(temp_image, sobel_image, 1, 0);
  imshow("Sobel", sobel_image);
  waitKey(0);
}

void canny(Mat image, int kernel_size) {
  Mat canny_image;
  Canny(image, canny_image, 50, 200, kernel_size);
  imshow("Canny", canny_image);
  waitKey(0);
}

void dft(Mat image) {
  cvtColor(image, image, CV_BGR2GRAY );
  Mat padded;
  int m = getOptimalDFTSize( image.rows );
  int n = getOptimalDFTSize( image.cols );
  copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

  /*
      Ket qua cua DFT la mot so phuc
      Moi phan tu cua anh khi qua DFT deu sinh ra 2 gia tri
      Gia tri mien tan so > Gia tri mien khong gian
      Ta can luu cac gia tri nay it nhat la float
      Convert anh, sau do convert qua channel khac de luu cac gia tri phuc
  */
  Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
  Mat complexI;
  merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

  /*
      Khoi chay ham dft()
  */
  dft(complexI, complexI);            // this way the result may fit in the source matrix
  // compute the magnitude and switch to logarithmic scale
  // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))

  /*
      Chuyen cac gia tri phuc va thuc sang do lon
  */

  split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
  magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
  Mat magI = planes[0];

  /*
      Dai dong (dynamic range) cua he so Fourier la qua lon de hien thi tren man hinh
      => cac gia tri cao (high values) duoc hien thi boi mau trang
      => cac gia tri thap (low values) duoc hien thi mau den
  */

  magI += Scalar::all(1);                    // switch to logarithmic scale
  log(magI, magI);
  // crop the spectrum, if it has an odd number of rows or columns

  /*
      Tra lai anh ve ban dau (do luc dau da bien doi anh)
  */
  magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
  // rearrange the quadrants of Fourier image  so that the origin is at the image center
  int cx = magI.cols/2;
  int cy = magI.rows/2;
  Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
  Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
  Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
  Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right
  Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
  q0.copyTo(tmp);
  q3.copyTo(q0);
  tmp.copyTo(q3);
  q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
  q2.copyTo(q1);
  tmp.copyTo(q2);
  normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
                                          // viewable image form (float between values 0 and 1).
  imshow("Input Image", image);    // Show the result
  imshow("spectrum magnitude", magI);
  waitKey();
}

void kmeans(Mat image) {
  Mat image2(image.size(), image.type());

  Mat points;
  image.convertTo(points, CV_32FC3);
  points = points.reshape(3, image.rows*image.cols);

  Mat_<int> clusters(points.size(), CV_32SC1);
  Mat centers;

  const int cluster = 5;
  kmeans(points, cluster, clusters, cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1.0), 1, KMEANS_PP_CENTERS, centers);

  MatIterator_<Vec3b> itd = image2.begin<Vec3b>(),itd_end = image2.end<Vec3b>();
  for (int i = 0; itd != itd_end; ++itd, ++i) {
      Vec3f &color = centers.at<Vec3f>(clusters(i), 0);
      (*itd)[0] = saturate_cast<uchar>(color[0]);
      (*itd)[1] = saturate_cast<uchar>(color[1]);
      (*itd)[2] = saturate_cast<uchar>(color[2]);
  }
  imshow("Input", image);
  imshow("Output", image2);
  waitKey(0);
}

void watershared(Mat image) {
  //todo
}

void morphology(Mat image) {
  //todo
}

void exit() {
  cout << endl;
  cout << "See you soon!\n";
  k = 1;
}
