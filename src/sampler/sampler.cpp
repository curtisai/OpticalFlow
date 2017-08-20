#include "sampler.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>

namespace FTL
{
void drawOptFlowMap(const cv::Mat& flow, cv::Mat& cflowmap, int step, double, const cv::Scalar& color)
{
	for(int y = 0; y < cflowmap.rows; y += step)
	{
		for(int x = 0; x < cflowmap.cols; x += step)
		{
			const cv::Point2f& fxy = flow.at<cv::Point2f>(y, x);
			line(cflowmap, cv::Point(x,y), cv::Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), color);
			circle(cflowmap, cv::Point(x,y), 2, color, -1);
		}
	}
}

void drawDepth(const cv::Mat& flow, cv::Mat& depthMap)
{
	depthMap = cv::Mat(flow.rows, flow.cols, CV_8UC(1), cv::Scalar::all(0));
	cv::Mat floatDepth = cv::Mat(flow.rows, flow.cols, CV_32F);
	std::cout << "size of depthMap: rwos " << depthMap.rows << "cols " << depthMap.cols << "\n";
	std::cout << "size of flow: rwos " << flow.rows << "cols " << flow.cols << "\n";
	std::cout << "size of floatDepth: rwos " << floatDepth.rows << "cols " << floatDepth.cols << "\n";
	int t_max = 0;
	for(int y = 0; y < floatDepth.rows; ++y)
	{
		for(int x = 0; x < floatDepth.cols; ++x)
		{
			const cv::Point2f& fxy = flow.at<cv::Point2f>(y, x);
			// std::cout << "access x " << x << " y " << y << "with value x: " << fxy.x <<  " y : " << fxy.y << "\n";
			floatDepth.at<float>(y, x) = fxy.y;
			if (t_max < fxy.y) t_max = fxy.y;
		}
	}
	double min, max;
	cv::minMaxLoc(floatDepth, &min, &max);
	std::cout << max << std::endl;
	// depthMap.forEach<unsigned int>([max](unsigned int& ele, const int *)->void{ele = ele * 255 / static_cast<unsigned int>(max);});
	for(int y = 0; y < depthMap.rows; ++y)
	{
		for(int x = 0; x < depthMap.cols; ++x)
		{
			// const cv::Point2f& fxy = flow.at<cv::Point2f>(y, x);
			const cv::Point2f& fxy = flow.at<cv::Point2f>(y, x);
			// std::cout << "access x " << x << " y " << y << "with value x: " << fxy.x <<  " y : " << fxy.y << "\n";
			// std::cout << "before x " << x << " y " << y << "with value depth: " << depthMap.at<unsigned int>(y, x) << " max : " << max << "\n";
			depthMap.at<unsigned int>(y, x) = static_cast<unsigned int>(floatDepth.at<float>(y, x) * 255 / static_cast<float>(max));
			// std::cout << "after x " << x << " y " << y << "with value depth: " << depthMap.at<unsigned int>(y, x) << " t_max: " << t_max << "\n";
		}
	}
}

void Open(std::string filename, int skip)
{
	cv::VideoCapture capture(filename);
	if( !capture.isOpened() )
		throw "Error when reading steam_avi";

	cv::namedWindow( "w", 1);
	cv::Mat flow, cflow, frame;
	cv::UMat gray, prevgray, uflow;
	int count = 0;
	while ( true )
	{
		capture >> frame;
		++count;
		if ( count % skip )
		{
			continue;
		}
		cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		if( !prevgray.empty() )
		{
			cv::calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
			cvtColor(prevgray, cflow, cv::COLOR_GRAY2BGR);
			uflow.copyTo(flow);
			drawOptFlowMap(flow, cflow, 20, 1.5, cv::Scalar(0, 255, 0));
			imshow("flow", cflow);
		}
		if(cv::waitKey(30)>=0)
			break;
		std::swap(prevgray, gray);
	}
	return;
}

void OpenDepth(std::string filename, int skip, int position)
{
	cv::VideoCapture capture(filename);
	if( !capture.isOpened() )
		throw "Error when reading steam_avi";

	cv::Mat flow, cflow, frame;
	cv::UMat gray, prevgray, uflow;
	int count = 0;
	while ( true )
	{
		capture >> frame;
		++count;
		if ( count % skip )
		{
			continue;
		}
		cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		if( !prevgray.empty() )
		{
			cv::calcOpticalFlowFarneback(prevgray, gray, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
			uflow.copyTo(flow);
			
			if ( count == position ) {
				 // cflow = cv::Mat(flow.rows, flow.cols, CV_8U);
				cv::FileStorage file("flow.ext", cv::FileStorage::WRITE);
				// Write to file!
				file << "flow" << flow;
				file.release();

				drawDepth(flow, cflow);
				break;
			}
		}
		if(cv::waitKey(30)>=0)
			break;
		std::swap(prevgray, gray);
	}
	// cv::imwrite("flow.png", cflow);
	// imshow("flow", cflow);
	// cv::waitKey(30000);
	cv::FileStorage file("some_name.ext", cv::FileStorage::WRITE);
	// Write to file!
	file << "cflow" << cflow;
	file.release();
	IplImage img = cflow;
	return;
}

} // end of namespace FTL
