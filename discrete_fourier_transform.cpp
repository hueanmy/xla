/*
    Muc dich: DFT chuyen anh tu mien khong gian sang mien tan so
*/
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;
static void help(void)
{
    cout << endl
        <<  "This program demonstrated the use of the discrete Fourier transform (DFT). " << endl
        <<  "The dft of an image is taken and it's power spectrum is displayed."          << endl
        <<  "Usage:"                                                                      << endl
        <<  "./discrete_fourier_transform [image_name -- default ../data/lena.jpg]"       << endl;
}
int main(int argc, char ** argv)
{
    help();
    const char* filename = argc >=2 ? argv[1] : "../data/lena.jpg";
    Mat I = imread(filename, IMREAD_GRAYSCALE);
    if( I.empty()){
        cout << "Error opening image" << endl;
        return -1;
    }
    /*
        Hieu nang cua DFT phu thuoc vao kich co cua anh
        Kich co toi uu cua anh la mot boi so cua 2, 3, 5
        => phuong phap: lay kich co toi uu cua anh + mo rong bien (border) cua anh
    */
    Mat padded;
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols );
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

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
    imshow("Input Image"       , I   );    // Show the result
    imshow("spectrum magnitude", magI);
    waitKey();
    return 0;
}
