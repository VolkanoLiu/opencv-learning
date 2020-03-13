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
  FindTarget FT;
  Mat image;
  VideoWriter video("/home/volkano/Downloads/数据集/数据集/out.mp4",
                    VideoWriter::fourcc('m', 'p', '4', 'v'), 10,
                    Size(3263, 700));
  string base = "/home/volkano/Downloads/数据集/数据集/image";
  string tail = ".jpg";

  unsigned frames = 0;  // 图片的帧数
  vector<double> rates;

  auto start = chrono::high_resolution_clock::now();

  for (size_t index = 0; index < 266; index++) {
    vector<string> textLines;  // 显示在视频中的字符串

    // 新建一个窗口
    namedWindow("test", WINDOW_GUI_EXPANDED);

    // 读取图片
    string num = to_string(index);
    string path = base + num + tail;
    image = imread(path);

    // 跳过空图片
    if (image.empty()) {
      cout << "skip image" << index << ".jpg" << endl;
    } else {
      frames++;

      Rect uniqueRect = FT.findUniqueRect(image);
      vector<Rect> targets = FT.findTarget(image);
      Rect targetRect = targets.size() ? targets[0] : Rect(0, 0, 0, 0);
      // 检查白色边框与标靶是否被识别到
      if (uniqueRect.area() && targetRect.area()) {
        Rect overlapRect = FT.getRectOverlapArea(uniqueRect, targetRect);
        double rate = (double)overlapRect.area() / (double)uniqueRect.area();
        rates.push_back(rate);
        rectangle(image, targetRect, Scalar(0, 0, 0), 3);

        // 在图片上显示一些数据
        stringstream line;
        line << "Image name\timage" << index << ".jpg";
        textLines.push_back(line.str());
        line.str("");
        line << "Frames\t" << frames;
        textLines.push_back(line.str());
        line.str("");
        line << "Match rate\t" << fixed << setprecision(2) << rate * 100 << "%";
        textLines.push_back(line.str());
        line.str("");
        line << "--白色标记框数据--";
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
          rectangle(image, targetRect, Scalar(0, 0, 0), 3);
        }
        stringstream line;
        line << "Image name\timage" << index << ".jpg";
        textLines.push_back(line.str());
        line.str("");
        line << "Accuracy\tnull";
        textLines.push_back(line.str());
      }

      for (size_t i = 0; i < textLines.size(); i++) {
        addText(image, textLines[i], Point(16, 36 * (i + 1)),
                fontQt("Fira Code", 18, Scalar(0, 0, 255)));
      }

      video << image;

      // cv::waitKey(10);
    }
  }

  double rate_sum = 0;
  for (auto iter = rates.begin(); iter != rates.end(); iter++) {
    rate_sum += *iter;
  }
  rate_sum /= rates.size();

  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

  cout << "+----------------------+" << endl;
  cout << "|    Program result    |" << endl;
  cout << "+----------+-----------+" << endl;
  cout << "|" << setiosflags(ios::left) << setw(10) << "Total time"
       << "|" << setiosflags(ios::right) << setw(8) << duration.count() << " ms"
       << "|" << endl;
  cout << "+----------+-----------+" << endl;
  cout << "|" << setiosflags(ios::left) << setw(10) << "  Frames  "
       << "|" << setiosflags(ios::right) << setw(8) << frames << "   |" << endl;
  cout << "+----------+-----------+" << endl;
  cout << "|" << setiosflags(ios::left) << setw(10) << "Match rate"
       << "|" << setiosflags(ios::right) << setw(8) << rate_sum * 100 << " % "
       << "|" << endl;
  cout << "+----------+-----------+" << endl;
  return 0;
}