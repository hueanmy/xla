#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv){

    Mat image , blurredImage;

    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

    if(!image.data){
        cout<<"Error loading image" << "\n";
        return -1;
    }

    /*
      Ap dung bo loc Gauss
      Object Size bieu thi kich co cua bo loc (la gioi han cua viec lam mo anh)
    */
    GaussianBlur( image, blurredImage, Size( 9, 9 ), 1.0);

    imshow("Blurred Image" , blurredImage);

    waitKey(0);

    return 0;
}
