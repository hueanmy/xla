#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
    Mat im = imread("test.jpg");
    Mat im2(im.size(), im.type());
    if (im.empty()) return -1;

    Mat points;
    im.convertTo(points, CV_32FC3);
    points = points.reshape(3, im.rows*im.cols);

    Mat_<int> clusters(points.size(), CV_32SC1);
    Mat centers;

    int cluster;
    count << "Enter cluster: \t";
    cin >> cluster;

    kmeans(points, cluster, clusters, cvTermCriteria(CV_TERMCRIT_EPS |
        CV_TERMCRIT_ITER, 10, 1.0), 1, KMEANS_PP_CENTERS, centers);

    MatIterator_<Vec3b> itd = im2.begin<Vec3b>(),itd_end = im2.end<Vec3b>();
    for (int i = 0; itd != itd_end; ++itd, ++i) {
        Vec3f &color = centers.at<Vec3f>(clusters(i), 0);
        (*itd)[0] = saturate_cast<uchar>(color[0]);
        (*itd)[1] = saturate_cast<uchar>(color[1]);
        (*itd)[2] = saturate_cast<uchar>(color[2]);
    }
    imshow("Input", im);
    imshow("Output", im2);
    waitKey(0);
}
