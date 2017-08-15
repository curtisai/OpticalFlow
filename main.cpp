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
#include "src/sampler/sampler.h"

int main(int argc, char** argv)
{
	FTL::Open("../videoplayback.mp4", 10);
	// FTL::Open("../overheadScanLow.mov", 5);
	// FTL::OpenDepth("../DJI_0032.MOV");
	// FTL::OpenDepth("../overheadScanLow.mov", 10, 270);
	return 0;
}
