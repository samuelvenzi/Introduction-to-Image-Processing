#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h> 

cv::Mat changeRedBlue(cv::Mat src_img);
cv::Mat im_chscaledepth(cv::Mat src_img, int depth, float scale);

int main(int argc, char const *argv[])
{
	//std::cout << "Debug"; 

	cv::Mat src_img;

	src_img = cv::imread(argv[1]);
	if (!src_img.data)
	{
		std::cout << "Could not open image." << std::endl;
		return -1;
	}
	cv::imshow("Source", src_img);
	cv::waitKey(10);
	//changeRedBlue(src_img);
	im_chscaledepth(src_img, 3, 1);

	return 0;
}

cv::Mat changeRedBlue(cv::Mat src_img){
	cv::Mat proc_img;

	proc_img = src_img;
	for (int i = 0; i < src_img.cols; ++i)
	{
	 	for (int j = 0; j < src_img.rows; ++j)
		{
			proc_img.at<cv::Vec3b>(j,i)[0] = src_img.at<cv::Vec3b>(j,i)[2];
			proc_img.at<cv::Vec3b>(j,i)[2] = src_img.at<cv::Vec3b>(j,i)[0];
		}
	 }
	cv::imshow("Red and Blue switching", proc_img);
	cv::waitKey();

	return proc_img;
}


cv::Mat im_chscaledepth(cv::Mat src_img, int depth, float scale){
	cv::Mat proc_img, resized_img;
	int depth_range, n_levels;

	n_levels = pow(2,depth);
	depth_range = 256/n_levels;
	std::cout << n_levels << std::endl;
	
	proc_img = src_img;

	for (int i = 0; i < src_img.cols; ++i)
	{
		for (int j = 0; j < src_img.rows; ++j)
		{	
			for (int k = 0; k < 3; ++k)
			{
				int level;
				std::cout << level << std::endl;
				level = proc_img.at<cv::Vec3b>(j,i)[k]/depth_range;
				proc_img.at<cv::Vec3b>(j,i)[k] = level*(255/(n_levels-1));
			}
		}
	}

	cv::imshow("Brightness depth", proc_img);
	cv::waitKey();

	return proc_img;

}