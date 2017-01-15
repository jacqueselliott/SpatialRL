// Generated by SpatialOS codegen. DO NOT EDIT!
// source: improbable/gameplay/GoalSphere.schema
#include <improbable/gameplay/GoalSphere.h>
#include <improbable/entity_state.pb.h>
#include <improbable/gameplay/GoalSphere.pb.h>

namespace improbable {
namespace gameplay {

// Implementation of Collision.
//----------------------------------------------------------------

Collision::Collision(std::int64_t time)
: _time{time} {}

bool Collision::operator==(const Collision& value) const {
  return
      _time == value._time;
}

bool Collision::operator!=(const Collision& value) const {
  return !operator==(value);
}

std::int64_t Collision::time() const {
  return _time;
}

std::int64_t& Collision::time() {
  return _time;
}

Collision& Collision::set_time(std::int64_t value) {
  _time = value;
  return *this;
}

// Implementation of GoalSphereData.
//----------------------------------------------------------------

GoalSphereData::GoalSphereData(bool collided)
: _collided{collided} {}

bool GoalSphereData::operator==(const GoalSphereData& value) const {
  return
      _collided == value._collided;
}

bool GoalSphereData::operator!=(const GoalSphereData& value) const {
  return !operator==(value);
}

bool GoalSphereData::collided() const {
  return _collided;
}

bool& GoalSphereData::collided() {
  return _collided;
}

GoalSphereData& GoalSphereData::set_collided(bool value) {
  _collided = value;
  return *this;
}

// Implementation of GoalSphere.
//----------------------------------------------------------------

constexpr const ::worker::ComponentId GoalSphere::ComponentId;

bool GoalSphere::Update::operator==(const GoalSphere::Update& value) const {
  return
      _collided == value._collided &&
      _collide == value._collide;
}

bool GoalSphere::Update::operator!=(const GoalSphere::Update& value) const {
  return !operator==(value);
}

GoalSphere::Update GoalSphere::Update::FromInitialData(const ::improbable::gameplay::GoalSphereData& data) {
  GoalSphere::Update update;
  update._collided.emplace(data.collided());
  return update;
}

::improbable::gameplay::GoalSphereData GoalSphere::Update::ToInitialData() const {
  return ::improbable::gameplay::GoalSphereData(*_collided);
}

void GoalSphere::Update::ApplyTo(::improbable::gameplay::GoalSphereData& data) const {
  if (_collided) {
    data.set_collided(*_collided);
  }
}

const ::worker::Option< bool >& GoalSphere::Update::collided() const {
  return _collided;
}

::worker::Option< bool >& GoalSphere::Update::collided() {
  return _collided;
}

GoalSphere::Update& GoalSphere::Update::set_collided(bool value) {
  _collided.emplace(value); return *this;
}

const ::worker::List< ::improbable::gameplay::Collision >& GoalSphere::Update::collide() const {
  return _collide;
}

::worker::List< ::improbable::gameplay::Collision >& GoalSphere::Update::collide() {
  return _collide;
}

GoalSphere::Update& GoalSphere::Update::add_collide(const ::improbable::gameplay::Collision& value) {
  _collide.emplace_back(value);
  return *this;
}

// Serialization helpers.
//----------------------------------------------------------------

namespace detail {

void Serialize_Collision(
    const ::improbable::gameplay::Collision& data,
    ::schema::improbable::gameplay::Collision& proto) {
  proto.set_field1_time(data.time());
}

::improbable::gameplay::Collision Deserialize_Collision(
    const ::schema::improbable::gameplay::Collision& proto) {
  ::improbable::gameplay::Collision result{
      proto.field1_time()};
  return result;
}

void Serialize_GoalSphereData(
    const ::improbable::gameplay::GoalSphereData& data,
    ::schema::improbable::gameplay::GoalSphereData& proto) {
  proto.set_field1_collided(data.collided());
}

::improbable::gameplay::GoalSphereData Deserialize_GoalSphereData(
    const ::schema::improbable::gameplay::GoalSphereData& proto) {
  ::improbable::gameplay::GoalSphereData result{
      proto.field1_collided()};
  return result;
}

namespace {

void VtableBufferFree_GoalSphere(::worker::ComponentId, std::uint8_t* buffer) {
  delete[] buffer;
}

void VtableFree_GoalSphere(
    ::worker::ComponentId, std::uint8_t object_type,
    ::worker::detail::ClientObject* object) {
  if (object_type == ::worker::detail::ClientComponentVtable::Update ||
      object_type == ::worker::detail::ClientComponentVtable::Snapshot) {
    delete static_cast< ::improbable::gameplay::GoalSphere::Update* >(object);
  } else if (object_type == ::worker::detail::ClientComponentVtable::Request ||
             object_type == ::worker::detail::ClientComponentVtable::Response) {
    delete static_cast< GenericCommandObject_GoalSphere* >(object);
  }
}

::worker::detail::ClientObject* VtableCopy_GoalSphere(
    ::worker::ComponentId, std::uint8_t object_type,
    const ::worker::detail::ClientObject* object) {
  if (object_type == ::worker::detail::ClientComponentVtable::Update ||
      object_type == ::worker::detail::ClientComponentVtable::Snapshot) {
    return new ::improbable::gameplay::GoalSphere::Update(*static_cast<const ::improbable::gameplay::GoalSphere::Update*>(object));
  } else if (object_type == ::worker::detail::ClientComponentVtable::Request ||
             object_type == ::worker::detail::ClientComponentVtable::Response) {
    return new GenericCommandObject_GoalSphere(
        *static_cast<const GenericCommandObject_GoalSphere*>(object));
  }
  return nullptr;
}

std::uint8_t VtableDeserialize_GoalSphere(
    ::worker::ComponentId, std::uint8_t object_type, const std::uint8_t* buffer,
    std::uint32_t length, ::worker::detail::ClientObject** object) {
  if (object_type == ::worker::detail::ClientComponentVtable::Update) {
    ::schema::improbable::EntityComponentUpdate proto_wrapper;
    if (!proto_wrapper.ParseFromArray(buffer, length)) {
      return false;
    }
    *object = new ::improbable::gameplay::GoalSphere::Update;
    auto client_object = static_cast< ::improbable::gameplay::GoalSphere::Update* >(*object);
    {
      const auto& proto = proto_wrapper.entity_state().GetExtension(::schema::improbable::gameplay::GoalSphere::component_extension);

      std::set<std::uint32_t> fields_to_clear{proto_wrapper.fields_to_clear().begin(),
                                              proto_wrapper.fields_to_clear().end()};
      if (proto.has_field1_collided()) {
        client_object->collided().emplace(proto.field1_collided());
      }
    }
    {
      const auto& proto = proto_wrapper.entity_event().GetExtension(::schema::improbable::gameplay::GoalSphere::event_extension);

      for (const auto& item : proto.event_collide()) {
        client_object->collide().emplace_back(::improbable::gameplay::detail::Deserialize_Collision(item));
      }
    }
  } else if (object_type == ::worker::detail::ClientComponentVtable::Snapshot) {
    ::schema::improbable::EntityState proto_wrapper;
    if (!proto_wrapper.ParseFromArray(buffer, length)) {
      return false;
    }
    *object = new ::improbable::gameplay::GoalSphere::Update;
    auto client_object = static_cast< ::improbable::gameplay::GoalSphere::Update* >(*object);
    const auto& proto = proto_wrapper.GetExtension(::schema::improbable::gameplay::GoalSphere::component_extension);

    client_object->collided().emplace(proto.field1_collided());
  } else if (object_type == ::worker::detail::ClientComponentVtable::Request) {
    ::schema::improbable::EntityCommand proto_wrapper;
    if (!proto_wrapper.ParseFromArray(buffer, length)) {
      return false;
    }
  } else if (object_type == ::worker::detail::ClientComponentVtable::Response) {
    ::schema::improbable::EntityCommand proto_wrapper;
    if (!proto_wrapper.ParseFromArray(buffer, length)) {
      return false;
    }
  }
  return true;
}

void VtableSerialize_GoalSphere(
    ::worker::ComponentId, std::uint8_t object_type,
    const ::worker::detail::ClientObject* object, std::uint8_t** buffer,
    std::uint32_t* length) {
  static_cast<void>(object);
  if (object_type == ::worker::detail::ClientComponentVtable::Update) {
    ::schema::improbable::EntityComponentUpdate proto_wrapper;
    const auto& data = *static_cast<const ::improbable::gameplay::GoalSphere::Update*>(object);
    {
      auto& proto = *proto_wrapper.mutable_entity_state()->MutableExtension(::schema::improbable::gameplay::GoalSphere::component_extension);
      if (data.collided()) {
        proto.set_field1_collided(*data.collided());
      }
    }
    {
      auto& proto = *proto_wrapper.mutable_entity_event()->MutableExtension(::schema::improbable::gameplay::GoalSphere::event_extension);
      for (const auto& item : data.collide()) {
        ::improbable::gameplay::detail::Serialize_Collision(item, *proto.add_event_collide());
      }
    }

    *length = proto_wrapper.ByteSize();
    *buffer = new std::uint8_t[*length];
    proto_wrapper.SerializeToArray(*buffer, *length);
  } else if (object_type == ::worker::detail::ClientComponentVtable::Snapshot) {
    ::schema::improbable::EntityState proto_wrapper;
    const auto& data = *static_cast<const ::improbable::gameplay::GoalSphere::Update*>(object);
    auto& proto = *proto_wrapper.MutableExtension(::schema::improbable::gameplay::GoalSphere::component_extension);

    proto.set_field1_collided(*data.collided());

    *length = proto_wrapper.ByteSize();
    *buffer = new std::uint8_t[*length];
    proto_wrapper.SerializeToArray(*buffer, *length);
  } else if (object_type == ::worker::detail::ClientComponentVtable::Request) {
    ::schema::improbable::EntityCommand proto_wrapper;
    proto_wrapper.MutableExtension(::schema::improbable::gameplay::GoalSphere::command_extension);


    *length = proto_wrapper.ByteSize();
    *buffer = new std::uint8_t[*length];
    proto_wrapper.SerializeToArray(*buffer, *length);
  } else if (object_type == ::worker::detail::ClientComponentVtable::Response) {
    ::schema::improbable::EntityCommand proto_wrapper;
    proto_wrapper.MutableExtension(::schema::improbable::gameplay::GoalSphere::command_extension);


    *length = proto_wrapper.ByteSize();
    *buffer = new std::uint8_t[*length];
    proto_wrapper.SerializeToArray(*buffer, *length);
  }
}

}  // anonymous namespace

const ::worker::detail::ClientComponentVtable& Vtable_GoalSphere() {
  static const ::worker::detail::ClientComponentVtable vtable = {
      1005,
      &detail::VtableBufferFree_GoalSphere,
      &detail::VtableFree_GoalSphere,
      &detail::VtableCopy_GoalSphere,
      &detail::VtableDeserialize_GoalSphere,
      &detail::VtableSerialize_GoalSphere,
  };
  return vtable;
}

}  // ::detail

}  // ::gameplay
}  // ::improbable
