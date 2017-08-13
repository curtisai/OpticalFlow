// =====================================================================================
//
//       Filename:  main.cpp
//
//    Description:  just a main
//
//        Version:  1.0
//        Created:  08/13/2017 00:52:39
//       Revision:  none
//       Compiler:  g++
//
//         Author:  curtisai, notadish@gmail.com
//   Organization:  
//
// =====================================================================================
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>

static void drawOptFlowMap(const cv::Mat& flow, cv::Mat& cflowmap, int step, double, const cv::Scalar& color)
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

int main(int argc, char** argv)
{
	std::string filename = "DJI_0032.MOV";
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
		if ( count % 3 )
		{
			continue;
		}
		cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		if( !prevgray.empty() )
		{
			cv::calcOpticalFlowFarneback(prevgray, gray, uflow, 0.2, 3, 15, 3, 5, 1.2, 0);
			cvtColor(prevgray, cflow, cv::COLOR_GRAY2BGR);
			uflow.copyTo(flow);
			drawOptFlowMap(flow, cflow, 16, 1.5, cv::Scalar(0, 255, 0));
			imshow("flow", cflow);
		}
		if(cv::waitKey(30)>=0)
			break;
		std::swap(prevgray, gray);
	}
}
