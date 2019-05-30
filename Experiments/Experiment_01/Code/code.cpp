#include "pch.h"
#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>        // 核心组件
#include<opencv2/highgui/highgui.hpp>  // high-level graphical user interface
#include<opencv2/imgproc/imgproc.hpp>  // 图像处理
#include<opencv2/imgproc/types_c.h>
using namespace cv;
using namespace std;

//彩色图像转化成灰度图像
Mat GrayTransform(Mat src)
{
	Mat dst = Mat::zeros(src.size(), CV_8UC1); //目标图像
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			uchar b = src.at<Vec3b>(i, j)[0];  //B
			uchar g = src.at<Vec3b>(i, j)[1];  //G 
			uchar r = src.at<Vec3b>(i, j)[2];  //R
			dst.at<uchar>(i, j) = (uchar)(b / 3.0 + g / 3.0 + r / 3.0 + 0.1);  //浮点数有误差，+0.1确保就近取整
		}
	}
	return dst;
}

//灰度图像的对数变换
Mat LogTransform(Mat src)
{
	src.convertTo(src, CV_32F);  //将像素值转化为浮点数
	src = src + 1;
	log(src, src);
	src = 1.2 * src;
	normalize(src, src, 0, 255, NORM_MINMAX);
	convertScaleAbs(src, src);
	return src;
}
//灰度图像的伽马变换
Mat GammaTransform(Mat src, double gamma, double parameter)
{
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			src.at<uchar>(i, j) = parameter * pow((double)src.at<uchar>(i, j), gamma);
		}
	}
	normalize(src, src, 0, 255, NORM_MINMAX);  //归一化
	convertScaleAbs(src, src);  //绝对值
	return src;

}
//彩色图像的补色变换
Mat ComplementaryTransform(Mat src)
{
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			src.at<Vec3b>(i, j)[0] = 255 - src.at<Vec3b>(i, j)[0];  //B
			src.at<Vec3b>(i, j)[1] = 255 - src.at<Vec3b>(i, j)[1];  //G 
			src.at<Vec3b>(i, j)[2] = 255 - src.at<Vec3b>(i, j)[2];  //R
		}
	}
	return src;
}
int main()
{
	// 读取图片，读取后格式为Mat
	Mat src = imread("C:/Users/MACHENIKE/Desktop/timg.jpg"); // string格式
	//转化成灰度图像
	Mat grayImg;
	grayImg = GrayTransform(src);
	imwrite("C:/Users/MACHENIKE/Desktop/gray.jpg", grayImg); //保存图片
	//对数变换
	Mat logImg = grayImg;
	logImg = LogTransform(logImg);
	imwrite("C:/Users/MACHENIKE/Desktop/log.jpg", logImg); 
	//伽马变换
	Mat gammaImg = grayImg;
	gammaImg = GammaTransform(gammaImg, 0.4, 1.0);
	imwrite("C:/Users/MACHENIKE/Desktop/gamma.jpg", gammaImg); 
	//补色变换
	Mat compleImg = src;  //拷贝彩色图像
	compleImg = ComplementaryTransform(compleImg);
	imwrite("C:/Users/MACHENIKE/Desktop/comple.jpg", compleImg);
}
