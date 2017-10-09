#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h> 

int main(int argc, char const *argv[])
{
    cv::Mat src_img;

    src_img = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if (!src_img.data)
    {
        std::cout << "Could not open image." << std::endl;
        return -1;
    }
    cv::imshow("Source", src_img);
    
    cv::Mat padded;                            //expand input image to optimal size
    int m = cv::getOptimalDFTSize(src_img.rows);
    int n = cv::getOptimalDFTSize(src_img.cols); // on the border add zero values
    cv::copyMakeBorder(src_img, padded, 0, m - src_img.rows, 0, n - src_img.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    
    
    //   printf("Debug\n");
    cv::Mat floatimg;
    padded.convertTo(floatimg, 5);
        
    cv::Mat planes[] = {floatimg, cv::Mat::zeros(padded.size(), 5)};
    
    cv::Mat freq_img;
    cv::merge(planes, 2, freq_img);         // Add to the expanded another plane with zeros
    cv::dft(freq_img, freq_img);            // this way the result may fit in the source matrix
    
    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    cv::split(freq_img, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    cv::magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    cv::Mat magI = planes[0];

    magI += cv::Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    cv::normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).

    cv::imshow("Input Image"       , src_img   );    // Show the result
    cv::imshow("spectrum magnitude", magI);
    
    cv::waitKey();
    
    return 0;
}