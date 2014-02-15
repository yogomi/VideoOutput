#ifndef PEN_LINE_H_
#define PEN_LINE_H_

#include <list>

#include <Leap.h>
#include <LeapMath.h>

namespace pen_line {

typedef std::list<Leap::Vector> Line;
typedef std::list<Line> LineList;

struct TracingLine {
  int counter;
  Leap::Vector previous_position;
  struct timeval time_buffer;
  Line line;
};

}

#endif // PEN_LINE_H_
