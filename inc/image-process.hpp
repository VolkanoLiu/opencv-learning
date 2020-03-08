#ifndef __IMAGE_PROCESS_HPP
#define __IMAGE_PROCESS_HPP

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

void imageRotate(Mat& inputMat, Mat& outputMat, double angle);

#endif