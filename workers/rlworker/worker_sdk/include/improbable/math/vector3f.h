// Copyright (c) Improbable Worlds Ltd, All Rights Reserved
#ifndef IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_MATH_VECTOR3F_H
#define IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_MATH_VECTOR3F_H
#ifdef _MSC_VER
#pragma warning(push, 0)
#if defined(_WIN32) && defined(GetMessage) && defined(UE_GAME)
// The official Protobuf headers redefines GetMessage - this conflicts with UnrealEngine 4, which
// disables the use of Win32 types.
#undef GetMessage
#endif
#endif  // _MSC_VER
#include <improbable/math/vector3f.pb.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif  // _MSC_VER

namespace improbable {
namespace math {

class Vector3f {
public:
  Vector3f(float x, float y, float z) : _x{x}, _y{y}, _z{z} {}
  Vector3f(Vector3f&&) = default;
  Vector3f(const Vector3f&) = default;
  Vector3f& operator=(Vector3f&&) = default;
  Vector3f& operator=(const Vector3f&) = default;
  ~Vector3f() = default;

  bool operator==(const Vector3f& value) const {
    return _x == value._x && _y == value._y && _z == value._z;
  }

  bool operator!=(const Vector3f& value) const {
    return _x != value._x || _y != value._y || _z != value._z;
  }

  float x() const {
    return _x;
  }
  float y() const {
    return _y;
  }
  float z() const {
    return _z;
  }

  float& x() {
    return _x;
  }
  float& y() {
    return _y;
  }
  float& z() {
    return _z;
  }

  Vector3f& set_x(float value) {
    _x = value;
    return *this;
  }
  Vector3f& set_y(float value) {
    _y = value;
    return *this;
  }
  Vector3f& set_z(float value) {
    _z = value;
    return *this;
  }

private:
  float _x;
  float _y;
  float _z;
};

namespace detail {

inline void Serialize_Vector3f(const ::improbable::math::Vector3f& data,
                               ::schema::improbable::math::Vector3f& proto) {
  proto.set_x(data.x());
  proto.set_y(data.y());
  proto.set_z(data.z());
}

inline ::improbable::math::Vector3f
Deserialize_Vector3f(const ::schema::improbable::math::Vector3f& proto) {
  return {proto.x(), proto.y(), proto.z()};
}

}  // ::detail
}  // ::math
}  // ::improbable

#endif  // IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_MATH_VECTOR3F_H
