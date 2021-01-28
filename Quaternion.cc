#include<math.h>

#include "headers/Quaternion.h"

Quaternion::Quaternion(float w, float x, float y, float z)
{
  q[0] = w;
  q[0] = w;
  q[1] = x;
  q[2] = y;
  q[3] = z;
}

float Quaternion::operator [] (int i) const
{
  return q[i];
}

float &Quaternion::operator [] (int i)
{
  return q[i];
}

Quaternion& Quaternion::operator =(const Quaternion& a)
{
  q[0] = a[0];
  q[1] = a[1];
  q[2] = a[2];
  q[3] = a[3];
  return *this;
}

Quaternion &Quaternion::operator +=(const Quaternion &a)
{
  for(int i = 0; i < 4; ++i){
    q[i] += a[i];
  }
  return *this;
}

Quaternion &Quaternion::negative()
{
  for(int i = 0; i < 4; ++i){
    q[i] = -q[i];
  }
  return *this;
}


Quaternion &Quaternion::operator *=(const Quaternion &a)
{
  return *this = *this * a;
}

Quaternion &Quaternion::inverse()
{
  float n = norm(*this);
  q[0] /= n;
  for(int i = 0; i < 4; ++i){
    q[i] /= -n;
  }
  return *this;
}

Quaternion operator *(const Quaternion &a, const Quaternion &b)
{
  Quaternion r(0, 0, 0, 0);
  r[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
  r[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
  r[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
  r[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];
  return r;
}


Quaternion conj( const Quaternion &a )
{
  return Quaternion( a[0], -a[1], -a[2], -a[3] );
}

float norm(const Quaternion &a)
{
  return a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3];
}

float abs( const Quaternion &a )
{
  return sqrt(norm(a));
}
