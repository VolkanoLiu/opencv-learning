#ifndef __FIND_TARGET_HPP
#define __FIND_TARGET_HPP

#include <algorithm>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

class FindTarget {
 public:
  // cv::Mat image;
  std::vector<cv::Rect> findRect(cv::Mat& image_input,
                                         cv::Scalar lowScalar,
                                         cv::Scalar highScalar);

  cv::Rect findUniqueRect(cv::Mat &image_input);

  std::vector<cv::Rect> findTarget(cv::Mat& image_input);

  cv::Rect getRectOverlapArea(cv::Rect &a, cv::Rect &b);
};

#endif