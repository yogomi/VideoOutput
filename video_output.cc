// Copyright 2014 Makoto Yano

#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  cv::Mat frame;
  cv::namedWindow("video", 1);
  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cout << "Cannot open camera" << std::endl;
    return -1;
  }
  while (true) {
    cap >> frame;
    if (frame.empty()) {
      break;
    }
    cv::imshow("video", frame);
    if (cv::waitKey(30) >= 0) {
      break;
    }
  }
  return 0;
}
