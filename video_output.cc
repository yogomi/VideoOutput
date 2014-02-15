// Copyright 2014 Makoto Yano

#include <math.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#elif __linux__
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#include <opencv2/opencv.hpp>
#include <LeapMath.h>

#include "headers/field_line.h"
#include "headers/Quaternion.h"
#include "headers/hand_input_listener.h"

field_line::FieldLine *background_line;

/////////////////////////////////
// for Leap

hand_listener::HandInputListener listener;

void init_opengl() {
  glEnable(GL_DEPTH_TEST);

  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 100.0 };
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 0.0 };
  GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
  glClearColor(0, 0, 0, 1.0f);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glShadeModel(GL_SMOOTH);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  background_line = new field_line::FieldLine();
  listener.initialize_world_position();
  float hard = Leap::PI / 32;
  float s = sin(hard);
  Quaternion rotate_quaternion(cos(hard), 1*s, 0*s, 0*s);
}

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
  output.open("output.m4v", codec, 60
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
