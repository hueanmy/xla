/*Chuoi xu ly anh:
  1. Thu nhan anh.
  2. Tien xu ly:
    - Loc nhieu.
    - Histogram_Eq.
    - Resize
  3. Phan vung: K-means.
  4. Hau xu ly: Opening, Closing.
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

Mat gaussianBlur (Mat imageInput);
Mat medianFilter(Mat image, int kernel_size);
Mat histogram(Mat imageInput);
Mat resize(Mat image, int width, int height);
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
  imshow("anh goc", image);
  waitKey(0);

  cout << "|2. TIEN XU LY                                              |" << endl;
  waitKey(0);

  Mat blurredImage = gaussianBlur(image);
  cin.get();

  cout << "| * MEDIAN FILTER: loai bo nhieu muoi tieu tren anh         |" << endl;
  cout << "| Enter kernel size: \t";
  int kernel_size;
  Mat medianImage;
  cin >> kernel_size;
  if (kernel_size %2 == 0) {
    return 0;
  }else {
    medianImage = medianFilter(blurredImage, kernel_size);
    cin.get();
  }
  waitKey(0);

  Mat contrastImage = histogram(medianImage);
  cin.get();

  cout << "| * RESIZE IMAGE                                            |" << endl;
  cout << "| Enter desired size:                                       |" << endl;
  cout << "| Enter width: \t";
  int width;
  cin >> width;
  cout << "| Enter height: \t";
  int height;
  cin >> height;
  Mat resizeImage = resize(contrastImage, width, height);

  cout << "|3. PHAN VUNG ANH                                           |" << endl;
  waitKey(0);

  Mat kmeanImage = kmean(resizeImage);
  cin.get();

  cout << "|4. HAU XU LY MORPHOLOGY: Closing, Openning                 |" << endl;
  waitKey(0);

  Mat morphologyImage = morphology_operations(kmeanImage);
  cin.get();

  cout << "|KET QUA CHUOI XU LY                                        |" << endl;
  imshow("Anh goc", image);
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
  imshow("anh sau khi loc nhieu", imageInput);
  imshow("contrastImage", contrastImage);
  waitKey(0);
  return contrastImage;
}

Mat resize(Mat image, int width, int height) {
  Mat imageDst;
  resize(image, imageDst, cv::Size(), width, height);
  imshow("Anh sau tang cuong va loc nhieu", image);
  imshow("resize image1", imageDst);
  waitKey(0);
  return imageDst;
}

/*HAM LAM TRON ANH BY GAUSS FILTER*/
Mat gaussianBlur (Mat imageInput) {
  Mat blurredImage;
  /**/
  GaussianBlur(imageInput, blurredImage, Size( 9, 9 ), 1.0);  //size(9,9): kich thuoc mat na duoc su dung, w,h la nhung hang xom duoc chon
  imshow("Anh goc", imageInput);
  imshow("Blurred Image" , blurredImage);
  cout << "| * GAUSS FILTER: loai bo nhieu tren anh                    |" << endl;
  waitKey(0);
  return blurredImage;
}

Mat medianFilter(Mat image, int kernel_size){
  Mat imageDst;
  medianBlur( image, imageDst, kernel_size );
  imshow("Anh sau khi gauss", image);
  imshow( "Median filter", imageDst );
  waitKey(0);
  return imageDst;
}

/*HAM PHAN VUNG ANH BY KMEANS*/

Mat kmean (Mat imageInput) {
  Mat kmeanImage(imageInput.size(), imageInput.type());
  Mat points;
  imageInput.convertTo(points, CV_32FC3);
  points = points.reshape(3, imageInput.rows*imageInput.cols); /*o day cho mat phang*/
  Mat_<int> clusters(points.size(), CV_32SC1);
  Mat centers;

  int cluster;
  cout << "| Enter cluster: \t";
  cin >> cluster;
  /*
    points: ma tran diem mau dau vao, cac diem du lieu thuc te can phai nhom. Phai co chinh xac mot diem cho moi hang
      -neu nhu co 50 diem trong mat phang 2D thi nen co 50 hang va 2 cot
    cluster: so luong cac cum trong cac diem du lieu
    clusters: tra ve cum moi diem thuoc ve. Chi ra du doan ban dau cho moi diem
    cvTermCriteria : Đây là tiêu chí chấm dứt vòng lặp. Khi tiêu chí này được thỏa mãn, thuật toán sẽ dừng vòng lặp.
      Trên thực tế, nó phải là một nhóm 3 thông số (type, max_iter, epsilon):
      3.a - loại tiêu chí chấm dứt: Nó có 3 lá cờ như sau:
          cv2.TERM_CRITERIA_EPS - ngăn sự lặp lại thuật toán nếu đạt được độ chính xác nhất định - epsilon đạt được.
          cv2.TERM_CRITERIA_MAX_ITER - dừng thuật toán sau khi số lượng nhất định được lặp đi lặp lại
          cv2.TERM_CRITERIA_EPS max_iter + cv2.TERM_CRITERIA_MAX_ITER - ngăn chặn sự lặp đi
          lặp lại khi một trong các điều kiện trên nó được đáp ứng.
      3.b - max_iter - Một số nguyên xác định số lượng tối đa vòng lặp. ( o day la 10)
      3.c - Độ chính xác - epsilon ( o day la 1)
      attemps: so lan thuat toan duoc chay voi moi vi tri trung tam khac nhau
      centers: ma tran cac trung tam cua moi cum.
      KMEANS_RANDOM_CENTERS: chon diem trung tam ban dau bang random cho moi lan thu
      KMEANS_USE_INITIAL_LABELS:
  */
  kmeans(points, cluster, clusters, cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 12, 1.0), 1, KMEANS_RANDOM_CENTERS, centers);
  /*thoả mãn một chuẩn mà chúng ta đã đưa ra từ trước. Ví du như là đạt đến số lần vòng lặp tối đa đã định nghĩa trước,
  hoặc là đạt được một chuẩn chính xác nào đó .v...
  Các điểm này sẽ thoả mãn tổng khoảng cách giữa dữ liệu test và trọng tâm tương ứng là nhỏ nhất.
  Hoặc cũng có thể là tổng khoảng cách giữa C1 tới điểm đỏ và C2 tới điểm xanh là nhỏ nhất.*/
  MatIterator_<Vec3b> itd = kmeanImage.begin<Vec3b>(),itd_end = kmeanImage.end<Vec3b>();
  for (int i = 0; itd != itd_end; ++itd, ++i) {
        Vec3f &color = centers.at<Vec3f>(clusters(i), 0);
        (*itd)[0] = saturate_cast<uchar>(color[0]);
        (*itd)[1] = saturate_cast<uchar>(color[1]);
        (*itd)[2] = saturate_cast<uchar>(color[2]);
    } /*--> gan mau cho cac vung khac nhau*/
    imshow("anh sau tien xu ly", imageInput);
    imshow("kmeanImage", kmeanImage);
    cout << "| * KMEANS: Phan vung anh                                   |" << endl;
    waitKey(0);
    return kmeanImage;
}

Mat morphology_operations (Mat imageInput) {
  int morph_size = 1;
  Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point(-1,-1) );
  /*
  tham so thu nhat: shape gom 3 kieu:
    - MORPH_RECT(o): cau truc hinh chu nhat
    - MORPH_ELLIPSE(2): cau truc hinh ecliptic
    - MORPH_CROSS(1):
  Size: kich thuoc cua phan tu cau truc.
  Point: vi tri neo trong phan tu, mac dinh la (-1,-1) co nghia neo nam o giua. Luu y rang chi co hinh dang cua
         mot phan tu hinh cheo phu thuoc vao vi tri neo.
  */
  cout<<element;

  Mat closingImage; // result matrix
  Mat openingImage;
  // Apply the specified morphology operation
  morphologyEx( imageInput, openingImage, MORPH_OPEN, element);
  /* dung de xoa nhieu, lam tron
  co: erosion -> gian: dilation*/
  imshow("anh sau khi phan vung", imageInput);
  imshow("openingImage", openingImage);
  cout << "\n| * Openning Image                                          |" << endl;
  waitKey(0);
  morphologyEx( openingImage, closingImage, MORPH_CLOSE, element);
  /* dung de lap khe ho, lam do cac bien
  gianL dilation --> co: erosion
  */
  imshow("openingImage", openingImage);
  imshow("closingImage", closingImage);
  cout << "| * Closing Image                                           |" << endl;
  waitKey(0);
  return closingImage;
}
