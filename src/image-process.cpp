#include "../inc/image-process.hpp"

using namespace cv;

// 图片旋转操作
void imageRotate(Mat& inputMat, Mat& outputMat, double angle) {
  int cols = inputMat.cols;
  int rows = inputMat.rows;
  Point2f pt(cols / 2., rows / 2.);
  Mat r = getRotationMatrix2D(pt, angle, 1.0);
  warpAffine(inputMat, outputMat, r, Size(cols, rows));
}
