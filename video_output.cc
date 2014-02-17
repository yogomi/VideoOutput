// Copyright 2014 Makoto Yano

#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#elif __linux__
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#include <opencv2/opencv.hpp>
#include <LeapMath.h>

#include "headers/pen_line.h"
#include "headers/field_line.h"
#include "headers/Quaternion.h"
#include "headers/hand_input_listener.h"

field_line::FieldLine *background_line;

/////////////////////////////////
// for Leap

hand_listener::HandInputListener listener;

/////////////////////////////////
// for OpenCV
cv::Size window_size(800, 600);
cv::VideoWriter video_output;

void reshape_func(int width, int height) {
  glViewport(0, 0, width, height);
}

void apply_world_quaternion(const Quaternion &q) {
  GLfloat m[16];
  float x2 = q[1] * q[1] * 2.0f;
  float y2 = q[2] * q[2] * 2.0f;
  float z2 = q[3] * q[3] * 2.0f;
  float xy = q[1] * q[2] * 2.0f;
  float yz = q[2] * q[3] * 2.0f;
  float zx = q[3] * q[1] * 2.0f;
  float xw = q[1] * q[0] * 2.0f;
  float yw = q[2] * q[0] * 2.0f;
  float zw = q[3] * q[0] * 2.0f;

  m[0] = 1.0f - y2 - z2;
  m[1] = xy + zw;
  m[2] = zx - yw;
  m[3] = 0.0f;

  m[4] = xy - zw;
  m[5] = 1.0 - z2 - x2;
  m[6] = yz + xw;
  m[7] = 0.0f;

  m[8] = zx + yw;
  m[9] = yz - xw;
  m[10] = 1.0f - x2 - y2;
  m[11] = 0.0f;

  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
  glMultMatrixf(m);
}

void output_view() {
  cv::Mat frame(window_size, CV_8UC3);
  glReadPixels(0, 0, frame.cols, frame.rows
      , GL_RGB, GL_UNSIGNED_BYTE, frame.data);
  cv::cvtColor(frame, frame, CV_RGB2BGR);
  cv::flip(frame, frame, 0);
  video_output << frame;
}

void display_func(void) {
  output_view();

  listener.lock();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(
    60.0f,
    static_cast<float>(glutGet(GLUT_WINDOW_WIDTH))
    / static_cast<float>(glutGet(GLUT_WINDOW_HEIGHT)), 2.0f, 200000.0f);
  gluLookAt(0, 0, 0
      , 0, 0,  -300, 0, 1, 0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslated(listener.camera_x_position
      , -listener.camera_y_position
      , -listener.camera_z_position);
  apply_world_quaternion(listener.world_x_quaternion);
  apply_world_quaternion(listener.world_y_quaternion);

  background_line->draw();

  glPushAttrib(GL_LIGHTING_BIT);
  GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  for (pen_line::LineList::iterator line = listener.penline_list.begin()
      ; line != listener.penline_list.end(); line++) {
    if (line->size() > 3) {
      glPushMatrix();
      glLineWidth(3);
      pen_line::Line::iterator point = line->begin();
      glColor3f(point->x, point->y, point->z);
      ++point;
      glBegin(GL_LINE_STRIP);
      for (; point != line->end(); point++) {
        glVertex3f(point->x, point->y, point->z);
      }
      glEnd();
      glPopMatrix();
    }
  }
  glPopAttrib();

  glPushAttrib(GL_LIGHTING_BIT);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  for (std::map<int, pen_line::TracingLine>::iterator tracing_line_map
        = listener.tracing_lines.begin()
      ; tracing_line_map != listener.tracing_lines.end()
      ; tracing_line_map++) {
    if ((*tracing_line_map).second.line.size() > 3) {
      glPushMatrix();
      glLineWidth(3);
      pen_line::Line::iterator point = (*tracing_line_map).second.line.begin();
      glColor3f(point->x, point->y, point->z);
      ++point;
      glBegin(GL_LINE_STRIP);
      for (; point != (*tracing_line_map).second.line.end(); point++) {
        glVertex3f(point->x, point->y, point->z);
      }
      glEnd();
      glPopMatrix();
    }
  }

  glPopAttrib();
  listener.unlock();
  glutSwapBuffers();
}

void key_func(unsigned char key, int x, int y) {
  switch (toupper(key)) {
  case 'Q':
    exit(0);
    break;
  case 'I':
    listener.initialize_world_position();
    break;
  }
}

void idle_func(void) {
  glutPostRedisplay();
}

static void ExitNx() {
  std::cout << "cccc" << std::endl;
  video_output.release();
}

void init_opengl() {
  glutDisplayFunc(display_func);
  glutReshapeFunc(reshape_func);
  glutKeyboardFunc(key_func);
  glutIdleFunc(idle_func);
  atexit(ExitNx);

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
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowSize(window_size.width, window_size.height);

  int mainHandle = glutCreateWindow("Sample 1");
  glutSetWindow(mainHandle);

  init_opengl();
  Leap::Controller controller;
  controller.addListener(listener);

  int codec =  CV_FOURCC('m', 'p', '4', 'v');

  video_output.open("output.m4v", codec, 60, window_size);

  if (!video_output.isOpened()) {
    std::cout << "Cannot open video output" << std::endl;
    return -1;
  }

  glutMainLoop();
  return 0;
}
