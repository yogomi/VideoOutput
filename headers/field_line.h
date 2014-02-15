#ifdef __APPLE__
#include <OpenGL/gl.h>
#elif __linux__
#include <GL/gl.h>
#endif

namespace field_line {

class FieldLine {
public:
  FieldLine();
  ~FieldLine();
  void draw();
private:
  static const int span_ = 50;
  static const int width_ = 1000;
  static const int height_ = 600;
  static const int depth_ = 600;
  static const int x_line_count_ = height_ / span_ + 1 + depth_ / span_ + 1;
  static const int yz_line_count_ = width_ / span_ + 1;
  static const int line_count_ = x_line_count_ + yz_line_count_;
  GLuint buffer_id_;
  GLint first_indexes_[line_count_];
  GLint count_indexes_[line_count_];
  GLfloat line_buffer_[x_line_count_ * 2 + yz_line_count_ * 3][3];
  void set_line_vertexes_();
};

} // namespace field_line
