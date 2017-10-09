#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h> 

int main(int argc, char const *argv[])
{
	cv::Mat src_img, proc_img, proc_img_int;

	src_img = cv::imread(argv[1]);
	if (!src_img.data)
	{
		std::cout << "Could not open image." << std::endl;
		return -1;
	}

	cv::Mat kernel;
	
	kernel = (cv::Mat_<float>(3,3) << 
        -1,  -1, -1,
        -1, 8, -1,
        -1,  -1, -1);

	printf("Debug\n");
	cv::imshow("Source", src_img);
	cv::waitKey(10);
	cv::imshow("Kernel", kernel);
	cv::waitKey(10);


	//cv::Laplacian(src_img, proc_img, 8);

	cv::GaussianBlur(src_img, proc_img_int, cv::Size(3,3), 1);

	cv::filter2D(proc_img_int, proc_img, -1, kernel);

	cv::imshow("Laplacian", proc_img);
	cv::waitKey();


	return 0;
}