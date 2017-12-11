#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void gaussianBlur(Mat image) {
  Mat blurredImage;
  GaussianBlur( image, blurredImage, Size( 9, 9 ), 1.0);

  imshow("Blurred Image" , blurredImage);
  waitKey(0);
}
// we return whether the menu was processed correctly
bool showMenu(Mat image) {
  // show the user the menu
  cout << "Main Menu" << endl;
  cout << "Select your favorite food" << endl;
  cout << "1. Cheese Burger" << endl;
  cout << "2. Ramen" << endl;
  cout << "3. Mashed Potatos" << endl;

  char choice;
  cin >> choice;

  if (choice=='1') {
    gaussianBlur(image);
  } else if (choice=='2') {
    cout << "Yum, with extra naruto." << endl;
  } else if (choice=='3') {
    cout << "Don't forget the gravy." << endl;
  } else {
    return false;
  }
  return true;
}


int main (int argc, char** argv) {

  Mat image;

  image = imread(argv[1], CV_LOAD_IMAGE_COLOR);

  if(!image.data){
      cout<<"Error loading image" << "\n";
      return -1;
  }

  while (!showMenu(image)) {
    cout << "Sorry, that is not a valid choice." << endl;
    cout << "Please try again." << endl << endl << endl;
  }
  cout << endl << "Thank you for playing." << endl;
  return 0;
}
