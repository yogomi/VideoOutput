#ifndef QUATERNION_H
#define QUATERNION_H


class Quaternion {
protected:
  float q[4];
public:
  Quaternion(float w = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f);
  float operator [] (int i) const;
  float &operator [] (int i);
  Quaternion& operator=(const Quaternion& q);
  Quaternion &operator += (const Quaternion &);
  Quaternion &negative();
  Quaternion &operator *= (const Quaternion &);
  Quaternion &inverse();
};

Quaternion operator * (const Quaternion &a, const Quaternion &b);
Quaternion conj( const Quaternion &a );
float norm(const Quaternion &a);
float abs( const Quaternion &a );

#endif
