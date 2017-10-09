#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h> 

void shiftDFT(cv::Mat &fImage);
cv::Mat create_spectrum_magnitude_display(cv::Mat &complexImg, bool rearrange);
void create_butterworth_notch(cv::Mat &dft_Filter);


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
    
    cv::Mat padded;                            
    int m = cv::getOptimalDFTSize(src_img.rows);
    int n = cv::getOptimalDFTSize(src_img.cols); 
    cv::copyMakeBorder(src_img, padded, 0, m - src_img.rows, 0, n - src_img.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    cv::Mat floatimg;
    padded.convertTo(floatimg, 5);
        
    cv::Mat planes[] = {floatimg, cv::Mat::zeros(padded.size(), 5)};
    
    cv::Mat freq_img;
    cv::merge(planes, 2, freq_img);         
    
    cv::dft(freq_img, freq_img);            

    shiftDFT(freq_img);
    
    cv::Mat filter, filterOutput, imgOutput;
    filter = freq_img.clone();
    create_butterworth_notch(filter);

    cv::mulSpectrums(freq_img, filter, freq_img, 0);

    shiftDFT(freq_img);

    cv::idft(freq_img, src_img); 

    src_img = create_spectrum_magnitude_display(src_img, false);

    cv::Mat myplanes[2];
    cv::split(src_img, myplanes);
    double minimum = -1;
    double maximum = -1;
    cv::Point minloc(-1, -1), maxloc(-1, -1);
    cv::minMaxLoc(myplanes[0], &minimum, &maximum, &minloc, &maxloc);
 
    imgOutput = myplanes[0];

    cv::split(filter, planes);
    cv::normalize(planes[0], filterOutput, 0, 1, CV_MINMAX);


    cv::imshow("spectrum output", src_img);
    cv::imshow("Image output", imgOutput);    
    cv::imshow("Filter output", filterOutput);

    cv::waitKey();
    
    return 0;
}

void shiftDFT(cv::Mat &fImage)
{
    cv::Mat tmp, q0, q1, q2, q3;

    fImage = fImage(cv::Rect(0, 0, fImage.cols & -2, fImage.rows & -2));

    int cx = fImage.cols / 2;
    int cy = fImage.rows / 2;


    q0 = fImage(cv::Rect(0, 0, cx, cy));
    q1 = fImage(cv::Rect(cx, 0, cx, cy));
    q2 = fImage(cv::Rect(0, cy, cx, cy));
    q3 = fImage(cv::Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

cv::Mat create_spectrum_magnitude_display(cv::Mat &complexImg, bool rearrange)
{
    cv::Mat planes[2];

    // compute magnitude spectrum (N.B. for display)
    // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))

    cv::split(complexImg, planes);
    cv::magnitude(planes[0], planes[1], planes[0]);

    cv::Mat mag = (planes[0]).clone();
    mag += cv::Scalar::all(1);
    log(mag, mag);

    if (rearrange)
    {
        shiftDFT(mag);
    }

    cv::normalize(mag, mag, 0, 1, CV_MINMAX);

    return mag;

}

void create_butterworth_notch(cv::Mat &dft_Filter)
{
    cv::Mat tmp = cv::Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);

    cv::Point centre = cv::Point(dft_Filter.rows/2, dft_Filter.cols/2);

    for (int i = 0; i < dft_Filter.rows; i++)
    {
        for (int j = 0; j < dft_Filter.cols; j++)
        {

            tmp.at<float>(i, j) = (float)
                                  (1/(1 + pow((double)10/sqrt((double)pow(i - centre.x - 39, 2)+(double)pow(j - centre.y - 30, 2)), 8)))*
                                  (1/(1 + pow((double)10/sqrt((double)pow(i - centre.x + 39, 2)+(double)pow(i - centre.y + 30, 2)), 8)))*
                                  (1/(1 + pow((double)10/sqrt((double)pow(i - centre.x + 39, 2)+(double)pow(j - centre.y - 30, 2)), 8)))*
                                  (1/(1 + pow((double)10/sqrt((double)pow(i - centre.x - 39, 2)+(double)pow(i - centre.y + 30, 2)), 8)))*
                                  (1/(1 + pow((double)5/sqrt((double)pow(i - centre.x - 78, 2)+(double)pow(j - centre.y - 30, 2)), 8)))*
                                  (1/(1 + pow((double)5/sqrt((double)pow(i - centre.x + 78, 2)+(double)pow(i - centre.y + 30, 2)), 8)))*
                                  (1/(1 + pow((double)5/sqrt((double)pow(i - centre.x + 78, 2)+(double)pow(j - centre.y - 30, 2)), 8)))*
                                  (1/(1 + pow((double)5/sqrt((double)pow(i - centre.x - 78, 2)+(double)pow(i - centre.y + 30, 2)), 8)));
                               
        }
    }

    cv::Mat toMerge[] = {tmp,tmp};
    cv::merge(toMerge, 2, dft_Filter);


}