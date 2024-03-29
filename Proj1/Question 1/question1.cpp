// Aluno: Samuel Venzi Lima Moneiro de Oliveira
// Matricula: 14/0162241

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

	cv::Mat src_img, proc_img;

	src_img = cv::imread(argv[1]);
	if (!src_img.data)
	{
		std::cout << "Could not open image." << std::endl;
		return -1;
	}
	cv::imshow("Source", src_img);
	cv::waitKey(10);
	//proc_img = changeRedBlue(src_img);
	proc_img = im_chscaledepth(src_img, 5, 0.5);

	cv::imshow("Proc", proc_img);
	cv::waitKey();

	return 0;
}

cv::Mat changeRedBlue(cv::Mat src_img){
	cv::Mat proc_img;

	proc_img = src_img;
	for (int i = 0; i < src_img.cols; ++i)
	{
	 	for (int j = 0; j < src_img.rows; ++j)
		{
			proc_img.at<cv::Vec3b>(j,i)[0] = src_img.at<cv::Vec3b>(j,i)[2];    //troca das componentes
			proc_img.at<cv::Vec3b>(j,i)[2] = src_img.at<cv::Vec3b>(j,i)[0];
		}
	 }

	return proc_img;
}


cv::Mat im_chscaledepth(cv::Mat src_img, int depth, float scale){
	cv::Mat proc_img;
	int depth_range, n_levels;

	n_levels = pow(2,depth);
	depth_range = 256/n_levels;
	
	proc_img = src_img;

	for (int i = 0; i < src_img.cols; ++i)
	{
		for (int j = 0; j < src_img.rows; ++j)
		{	
			for (int k = 0; k < 3; ++k)
			{
				int level;
				level = proc_img.at<cv::Vec3b>(j,i)[k]/depth_range;       //mudanca de niveis de brilho
				proc_img.at<cv::Vec3b>(j,i)[k] = level*(255/(n_levels-1));
			}
		}
	}

	int n_cols, n_rows, x, y, new_x, new_y;
	
	x = src_img.cols;
	y = src_img.rows;
	new_x = x*scale;
	new_y = y*scale;

	cv::Mat resized_cols = cv::Mat(y, new_x, CV_8UC(3));
	cv::Mat resized_img = cv::Mat(new_y, new_x, CV_8UC(3));

	for (int i = 0; i < src_img.cols; ++i)
	{
		for (int j = 0; j < src_img.rows; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{														//redimensionamento
				int aux_i;
				aux_i = (int)(scale*i);
				if (aux_i < new_x)
					resized_cols.at<cv::Vec3b>(j,aux_i)[k] = proc_img.at<cv::Vec3b>(j,i)[k];	
			}
		}	
	}

	if (scale > 1)
	{
		for (int i = 0; i < resized_cols.cols; ++i)
		{
			int sum = 0;
			for (int lines = 0; lines < resized_cols.rows; ++lines)
			{
				sum += resized_cols.at<cv::Vec3b>(lines,i)[0];
			}
			for (int j = 0; j < resized_cols.rows; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{	
					if (sum == 0)
					{
						resized_cols.at<cv::Vec3b>(j,i)[k] = resized_cols.at<cv::Vec3b>(j,i-1)[k];	
					}
				}
			}
		}
	}


	for (int i = 0; i < resized_cols.rows; ++i)
	{
		for (int j = 0; j < resized_cols.cols; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{	
				int aux_i;
				aux_i = (int)(scale*i);
				if (aux_i < new_y)
					resized_img.at<cv::Vec3b>(aux_i,j)[k] = resized_cols.at<cv::Vec3b>(i,j)[k];
			}
		}
	}

	if (scale > 1)
	{
		for (int i = 0; i < resized_img.rows; ++i)
		{	
			int sum = 0;
			for (int columns = 0; columns < resized_img.cols; ++columns)
			{
				sum += resized_img.at<cv::Vec3b>(i,columns)[0];
			}
			for (int j = 0; j < resized_img.cols; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{	
					if (sum == 0)
					{
						resized_img.at<cv::Vec3b>(i,j)[k] = resized_img.at<cv::Vec3b>(i-1,j)[k];	
					}
				}
			}	
		}
	}


	return resized_img;
}