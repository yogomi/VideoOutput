#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv)
{
  Mat frame;
  namedWindow("video", 1);
  VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cout << "Cannot open camera" << std::endl;
    return -1;
  }
  while (true) {
    cap >> frame;
    if(frame.empty()) {
      break;
    }
    imshow("video", frame);
    if (waitKey(30) >= 0) {
      break;
    }
  }
  return 0;
}
