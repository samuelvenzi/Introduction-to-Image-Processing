#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h> 

int main(int argc, char const *argv[])
{
	cv::Mat src_img, freq_img;

	src_img = cv::imread(argv[1]);
	if (!src_img.data)
	{
		std::cout << "Could not open image." << std::endl;
		return -1;
	}
	cv::imshow("Source", src_img);
	cv::waitKey(10);

	// cv::Mat padded;                            //expand input image to optimal size
 //    int m = cv::getOptimalDFTSize(src_img.rows);
 //    int n = cv::getOptimalDFTSize(src_img.cols); // on the border add zero values
 //    copyMakeBorder(src_img, padded, 0, m - src_img.rows, 0, n - src_img.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));


	// printf("Debug\n");
 //    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
	
 //    cv::merge(planes, 2, freq_img);         // Add to the expanded another plane with zeros

    cv::dft(src_img, freq_img);            // this way the result may fit in the source matrix



	
	return 0;
}