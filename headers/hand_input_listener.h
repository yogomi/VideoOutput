#ifndef HAND_INPUT_LISTENER_H_
#define HAND_INPUT_LISTENER_H_

#include <map>

#include <Leap.h>

#include "Quaternion.h"
#include "pen_line.h"

#define DEFAULT_CAMERA_X 0
#define DEFAULT_CAMERA_Y 300
#define DEFAULT_CAMERA_Z 600
#define MAX_TRACABLE_POINT_COUNT 10

using namespace Leap;

namespace hand_listener{

class HandInputListener : public Listener {
public:
  virtual void onInit(const Controller& controller);
  virtual void onFrame(const Controller& controller);
  void initialize_world_position();

  void lock();
  void unlock();
  bool rotating;
  Quaternion world_x_quaternion;
  Quaternion world_y_quaternion;
  pen_line::LineList penline_list;
  std::map<int,pen_line::TracingLine> tracing_lines;

  float camera_x_position;
  float camera_y_position;
  float camera_z_position;

private:
  Vector convert_to_world_position_(const Vector &input_vector);
  bool _lock;
  int open_hand_id_(const Frame& frame);
  void trace_finger_(const Hand& hand);
  void rotate_camera_(const Hand& hand);
  void clean_line_map_(const Frame& frame);
};

}

#endif // HAND_INPUT_LISTENER_H_
