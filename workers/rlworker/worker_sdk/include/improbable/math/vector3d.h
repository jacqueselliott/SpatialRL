// Copyright (c) Improbable Worlds Ltd, All Rights Reserved
#ifndef IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_MATH_VECTOR3D_H
#define IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_MATH_VECTOR3D_H
#ifdef _MSC_VER
#pragma warning(push, 0)
#if defined(_WIN32) && defined(GetMessage) && defined(UE_GAME)
// The official Protobuf headers redefines GetMessage - this conflicts with UnrealEngine 4, which
// disables the use of Win32 types.
#undef GetMessage
#endif
#endif  // _MSC_VER
#include <improbable/math/vector3d.pb.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif  // _MSC_VER

namespace improbable {
namespace math {

class Vector3d {
public:
  Vector3d(double x, double y, double z) : _x{x}, _y{y}, _z{z} {}
  Vector3d(Vector3d&&) = default;
  Vector3d(const Vector3d&) = default;
  Vector3d& operator=(Vector3d&&) = default;
  Vector3d& operator=(const Vector3d&) = default;
  ~Vector3d() = default;

  bool operator==(const Vector3d& value) const {
    return _x == value._x && _y == value._y && _z == value._z;
  }

  bool operator!=(const Vector3d& value) const {
    return _x != value._x || _y != value._y || _z != value._z;
  }

  double x() const {
    return _x;
  }
  double y() const {
    return _y;
  }
  double z() const {
    return _z;
  }

  double& x() {
    return _x;
  }
  double& y() {
    return _y;
  }
  double& z() {
    return _z;
  }

  Vector3d& set_x(double value) {
    _x = value;
    return *this;
  }
  Vector3d& set_y(double value) {
    _y = value;
    return *this;
  }
  Vector3d& set_z(double value) {
    _z = value;
    return *this;
  }

private:
  double _x;
  double _y;
  double _z;
};

namespace detail {

inline void Serialize_Vector3d(const ::improbable::math::Vector3d& data,
                               ::schema::improbable::math::Vector3d& proto) {
  proto.set_x(data.x());
  proto.set_y(data.y());
  proto.set_z(data.z());
}

inline ::improbable::math::Vector3d
Deserialize_Vector3d(const ::schema::improbable::math::Vector3d& proto) {
  return {proto.x(), proto.y(), proto.z()};
}

}  // ::detail
}  // ::math
}  // ::improbable

#endif  // IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_MATH_VECTOR3D_H
