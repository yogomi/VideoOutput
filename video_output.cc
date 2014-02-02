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

  cv::VideoWriter output;
  int codec =  CV_FOURCC('m', 'p', '4', 'v');
  cv::Size capture_size =
    cv::Size(static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH)),
          static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT)));

  std::cout << "Size width = " << capture_size.width << std::endl;
  std::cout << "Size height = " << capture_size.height << std::endl;
  output.open("output.m4v", codec, 30
              , capture_size);

  if (!output.isOpened()) {
    std::cout << "Cannot open video output" << std::endl;
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
    output << frame;
  }
  output.release();
  cap.release();
  return 0;
}
