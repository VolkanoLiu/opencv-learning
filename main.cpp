#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "inc/find-target.hpp"
#include "inc/image-process.hpp"
#include "inc/serial.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
  cout << getBuildInformation() << endl;
  FindTarget FT;
  Mat image;
  VideoWriter video("/home/volkano/Downloads/数据集/数据集/out.mp4",
                    VideoWriter::fourcc('m', 'p', '4', 'v'), 10,
                    Size(3263, 700));
  string base = "/home/volkano/Downloads/数据集/数据集/image";
  string tail = ".jpg";
  for (size_t index = 0; index < 266; index++) {
    // Text to be showed on the image
    vector<string> textLines;

    // Path configuration
    string num = to_string(index);
    string path = base + num + tail;
    image = imread(path);

    // Create a windows
    namedWindow("test", WINDOW_GUI_EXPANDED);
    // skip the empty image
    if (image.empty()) {
      cout << "skip image" << index << ".jpg" << endl;
    } else {
      Rect uniqueRect = FT.findUniqueRect(image);
      vector<Rect> targets = FT.findTarget(image);
      Rect targetRect = targets.size() ? targets[0] : Rect(0, 0, 0, 0);
      // check if the uniqueRect and targetRect are detected
      if (uniqueRect.area() && targetRect.area()) {
        Rect overlapRect = FT.getRectOverlapArea(uniqueRect, targetRect);
        double rate = (double)overlapRect.area() / (double)uniqueRect.area();
        rectangle(image, targetRect, Scalar(0, 255, 0), 3);

        // Display nacessacy informations
        stringstream line;
        line << "Accuracy\t" << fixed << setprecision(2) << rate * 100;
        textLines.push_back(line.str());
        line.str("");
        line << "x\t\t" << uniqueRect.x;
        textLines.push_back(line.str());
        line.str("");
        line << "y\t\t" << uniqueRect.y;
        textLines.push_back(line.str());
        line.str("");
        line << "width\t\t" << uniqueRect.width;
        textLines.push_back(line.str());
        line.str("");
        line << "height\t" << uniqueRect.height;
        textLines.push_back(line.str());
        line.str("");
        line << "area\t\t" << uniqueRect.area();
        textLines.push_back(line.str());
      } else {
        if (targetRect.area() != 0) {
          rectangle(image, targetRect, Scalar(0, 255, 0), 3);
        }
        stringstream line;
        line << "Accuracy\tnull";
        textLines.push_back(line.str());
      }

      for (size_t i = 0; i < textLines.size(); i++) {
        addText(image, textLines[i], Point(16, 36 * (i + 1)),
                fontQt("Fira Code", 18, Scalar(0, 0, 255)));
      }

      imshow("test", image);
      video << image;
      waitKey(10);
    }
  }
  // image = imread("/home/volkano/Downloads/数据集/数据集/image1.jpg");
  // vector<Rect> rectangles = FT.findTarget(image);
  // for (auto& rect_n : rectangles) {
  //   rectangle(image, rect_n, Scalar(0, 0, 0));
  // }

  // Rect a(2, 2, 2, 2), b(6, 6, 2, 2);
  // Rect c = FT.getRectOverlapArea(a, b);
  // cout << c.x << '\t' << c.y << '\t' << c.width << '\t' << c.height << '\t'
  //      << endl;

  // imshow("source", image);
  // waitKey();
  return 0;
}