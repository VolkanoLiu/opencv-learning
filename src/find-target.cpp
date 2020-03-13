#include "../inc/find-target.hpp"

std::vector<cv::Rect> FindTarget::findRect(cv::Mat &image_input,
                                           cv::Scalar lowScalar,
                                           cv::Scalar highScalar) {
  cv::Mat image_preprocess;
  cv::inRange(image_input, lowScalar, highScalar, image_preprocess);
  cv::Mat verticalKernel, horizonalKernel;
  cv::Mat image_verticalLine, image_horizonalLine, image_line;

  verticalKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 45));
  horizonalKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(45, 1));
  // 对二值化后的图片进行腐蚀和膨胀处理，以获得垂直与水平线
  cv::erode(image_preprocess, image_verticalLine, verticalKernel);
  cv::dilate(image_verticalLine, image_verticalLine, verticalKernel);
  cv::erode(image_preprocess, image_horizonalLine, horizonalKernel);
  cv::dilate(image_horizonalLine, image_horizonalLine, horizonalKernel);
  image_line = image_verticalLine + image_horizonalLine;

  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(image_line, contours, hierarchy, cv::RETR_CCOMP,
                   cv::CHAIN_APPROX_NONE);

  std::vector<cv::Rect> rectangles;
  for (auto &contour : contours) {
    cv::approxPolyDP(contour, contour, 5, true);
    if (cv::isContourConvex(contour) && contour.size() == 4) {
      rectangles.push_back(cv::boundingRect(contour));
    }
  }
  std::sort(
      rectangles.begin(), rectangles.end(),
      [](cv::Rect &a, cv::Rect &b) -> int { return (a.width > b.width); });
  return rectangles;
}

cv::Rect FindTarget::findUniqueRect(cv::Mat &image_input) {
  std::vector<cv::Rect> rectangles = findRect(
      image_input, cv::Scalar(230, 230, 230), cv::Scalar(255, 255, 255));
  if (rectangles.size() < 2) {
    return cv::Rect(0, 0, 0, 0);
  } else {
    // 将得到的两个轮廓平均处理
    cv::Rect rectangle;
    rectangle.x = (rectangles[0].x + rectangles[1].x) / 2;
    rectangle.y = (rectangles[0].y + rectangles[1].y) / 2;
    rectangle.width = (rectangles[0].width + rectangles[1].width) / 2;
    rectangle.height = (rectangles[0].height + rectangles[1].height) / 2;
    return rectangle;
  }
}

std::vector<cv::Rect> FindTarget::findTarget(cv::Mat &image_input) {
  cv::Mat image_preprocess;
  cv::inRange(image_input, cv::Scalar(10, 10, 110), cv::Scalar(95, 95, 255),
              image_preprocess);
  cv::medianBlur(image_preprocess, image_preprocess, 7);
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(image_preprocess, contours, hierarchy, cv::RETR_CCOMP,
                   cv::CHAIN_APPROX_NONE);

  std::vector<cv::Rect> rectangles;
  for (auto &contour : contours) {
    if (cv::contourArea(contour) > 10000) {
      rectangles.push_back(cv::boundingRect(contour));
    }
  }
  return rectangles;
}

cv::Rect FindTarget::getRectOverlapArea(cv::Rect &a, cv::Rect &b) {
  int ax_1 = a.x, ay_1 = a.y, ax_2 = a.x + a.width, ay_2 = a.y + a.height;
  int bx_1 = b.x, by_1 = b.y, bx_2 = b.x + b.width, by_2 = b.y + b.height;

  int left = std::max(ax_1, bx_1);
  int right = std::min(ax_2, bx_2);
  int top = std::max(ay_1, by_1);
  int bottom = std::min(ay_2, by_2);
  if (left < right && top < bottom) {
    return cv::Rect(left, top, right - left, bottom - top);
  } else {
    return cv::Rect(0, 0 ,0 ,0);
  }
}