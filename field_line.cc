#include <iostream>

#include "headers/field_line.h"

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

namespace field_line {

FieldLine::FieldLine() {
  set_line_vertexes_();
}

FieldLine::~FieldLine() {
}

void FieldLine::draw() {
  glPushMatrix();
  glPushAttrib(GL_LIGHTING_BIT);
  GLfloat lmodel_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColor3f(0.0f, 0.8f, 0.0f);
  glLineWidth(1.5f);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
  glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
  glMultiDrawArrays(GL_LINE_STRIP, first_indexes_, count_indexes_, line_count_);
  glDisableClientState(GL_VERTEX_ARRAY);
  glPopAttrib();
  glPopMatrix();
  return;
}

void FieldLine::set_line_vertexes_() {
  int index_position = 0;
  int vertex_position = 0;
  int x_max = width_ / 2;
  int z_max = depth_ / 2;
  for (int i=0; i<height_/span_ + 1; i++) {
    first_indexes_[index_position] = vertex_position;
    count_indexes_[index_position] = 2;
    ++index_position;
    line_buffer_[vertex_position][0] = -x_max;
    line_buffer_[vertex_position][1] = i * span_;
    line_buffer_[vertex_position][2] = -z_max;
    ++vertex_position;
    line_buffer_[vertex_position][0] = x_max;
    line_buffer_[vertex_position][1] = i * span_;
    line_buffer_[vertex_position][2] = -z_max;
    ++vertex_position;
  }
  for (int i=0; i<depth_/span_ + 1; i++) {
    first_indexes_[index_position] = vertex_position;
    count_indexes_[index_position] = 2;
    ++index_position;
    line_buffer_[vertex_position][0] = -x_max;
    line_buffer_[vertex_position][1] = 0;
    line_buffer_[vertex_position][2] = z_max - i*span_;
    ++vertex_position;
    line_buffer_[vertex_position][0] = x_max;
    line_buffer_[vertex_position][1] = 0;
    line_buffer_[vertex_position][2] = z_max - i*span_;
    ++vertex_position;
  }
  for (int i=0; i<width_/span_ + 1; i++) {
    first_indexes_[index_position] = vertex_position;
    count_indexes_[index_position] = 3;
    ++index_position;
    line_buffer_[vertex_position][0] = x_max - i*span_;
    line_buffer_[vertex_position][1] = height_;
    line_buffer_[vertex_position][2] = -z_max;
    ++vertex_position;
    line_buffer_[vertex_position][0] = x_max - i*span_;
    line_buffer_[vertex_position][1] = 0;
    line_buffer_[vertex_position][2] = -z_max;
    ++vertex_position;
    line_buffer_[vertex_position][0] = x_max - i*span_;
    line_buffer_[vertex_position][1] = 0;
    line_buffer_[vertex_position][2] = z_max;
    ++vertex_position;
  }
  glGenBuffers(1, &buffer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(line_buffer_)
      , line_buffer_, GL_STATIC_DRAW);
}

} // namespace field_line
