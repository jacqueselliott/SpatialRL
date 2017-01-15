// Generated by SpatialOS codegen. DO NOT EDIT!
// source: improbable/terrain/Terrain.schema
#include <improbable/terrain/Terrain.h>
#include <improbable/entity_state.pb.h>
#include <improbable/terrain/Terrain.pb.h>

namespace improbable {
namespace terrain {

// Implementation of TerrainData.
//----------------------------------------------------------------

TerrainData::TerrainData() {}

bool TerrainData::operator==(const TerrainData&) const {
  return true;
}

bool TerrainData::operator!=(const TerrainData& value) const {
  return !operator==(value);
}

// Implementation of Terrain.
//----------------------------------------------------------------

constexpr const ::worker::ComponentId Terrain::ComponentId;

bool Terrain::Update::operator==(const Terrain::Update&) const {
  return true;
}

bool Terrain::Update::operator!=(const Terrain::Update& value) const {
  return !operator==(value);
}

Terrain::Update Terrain::Update::FromInitialData(const ::improbable::terrain::TerrainData&) {
  return Terrain::Update{};
}

::improbable::terrain::TerrainData Terrain::Update::ToInitialData() const {
  return ::improbable::terrain::TerrainData();
}

void Terrain::Update::ApplyTo(::improbable::terrain::TerrainData&) const {
}

// Serialization helpers.
//----------------------------------------------------------------

namespace detail {

void Serialize_TerrainData(
    const ::improbable::terrain::TerrainData&,
    ::schema::improbable::terrain::TerrainData&) {}

::improbable::terrain::TerrainData Deserialize_TerrainData(
    const ::schema::improbable::terrain::TerrainData&) {
  return {};
}

namespace {

void VtableBufferFree_Terrain(::worker::ComponentId, std::uint8_t* buffer) {
  delete[] buffer;
}

void VtableFree_Terrain(
    ::worker::ComponentId, std::uint8_t object_type,
    ::worker::detail::ClientObject* object) {
  if (object_type == ::worker::detail::ClientComponentVtable::Update ||
      object_type == ::worker::detail::ClientComponentVtable::Snapshot) {
    delete static_cast< ::improbable::terrain::Terrain::Update* >(object);
  } else if (object_type == ::worker::detail::ClientComponentVtable::Request ||
             object_type == ::worker::detail::ClientComponentVtable::Response) {
    delete static_cast< GenericCommandObject_Terrain* >(object);
  }
}

::worker::detail::ClientObject* VtableCopy_Terrain(
    ::worker::ComponentId, std::uint8_t object_type,
    const ::worker::detail::ClientObject* object) {
  if (object_type == ::worker::detail::ClientComponentVtable::Update ||
      object_type == ::worker::detail::ClientComponentVtable::Snapshot) {
    return new ::improbable::terrain::Terrain::Update(*static_cast<const ::improbable::terrain::Terrain::Update*>(object));
  } else if (object_type == ::worker::detail::ClientComponentVtable::Request ||
             object_type == ::worker::detail::ClientComponentVtable::Response) {
    return new GenericCommandObject_Terrain(
        *static_cast<const GenericCommandObject_Terrain*>(object));
  }
  return nullptr;
}

std::uint8_t VtableDeserialize_Terrain(
    ::worker::ComponentId, std::uint8_t object_type, const std::uint8_t* buffer,
    std::uint32_t length, ::worker::detail::ClientObject** object) {
  if (object_type == ::worker::detail::ClientComponentVtable::Update) {
    ::schema::improbable::EntityComponentUpdate proto_wrapper;
    if (!proto_wrapper.ParseFromArray(buffer, length)) {
      return false;
    }
    *object = new ::improbable::terrain::Terrain::Update;
    {
    }
    {
    }
  } else if (object_type == ::worker::detail::ClientComponentVtable::Snapshot) {
    ::schema::improbable::EntityState proto_wrapper;
    if (!proto_wrapper.ParseFromArray(buffer, length)) {
      return false;
    }
    *object = new ::improbable::terrain::Terrain::Update;
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

void VtableSerialize_Terrain(
    ::worker::ComponentId, std::uint8_t object_type,
    const ::worker::detail::ClientObject* object, std::uint8_t** buffer,
    std::uint32_t* length) {
  static_cast<void>(object);
  if (object_type == ::worker::detail::ClientComponentVtable::Update) {
    ::schema::improbable::EntityComponentUpdate proto_wrapper;
    {
      proto_wrapper.mutable_entity_state()->MutableExtension(::schema::improbable::terrain::Terrain::component_extension);
    }
    {
      proto_wrapper.mutable_entity_event()->MutableExtension(::schema::improbable::terrain::Terrain::event_extension);
    }

    *length = proto_wrapper.ByteSize();
    *buffer = new std::uint8_t[*length];
    proto_wrapper.SerializeToArray(*buffer, *length);
  } else if (object_type == ::worker::detail::ClientComponentVtable::Snapshot) {
    ::schema::improbable::EntityState proto_wrapper;
    proto_wrapper.MutableExtension(::schema::improbable::terrain::Terrain::component_extension);


    *length = proto_wrapper.ByteSize();
    *buffer = new std::uint8_t[*length];
    proto_wrapper.SerializeToArray(*buffer, *length);
  } else if (object_type == ::worker::detail::ClientComponentVtable::Request) {
    ::schema::improbable::EntityCommand proto_wrapper;
    proto_wrapper.MutableExtension(::schema::improbable::terrain::Terrain::command_extension);


    *length = proto_wrapper.ByteSize();
    *buffer = new std::uint8_t[*length];
    proto_wrapper.SerializeToArray(*buffer, *length);
  } else if (object_type == ::worker::detail::ClientComponentVtable::Response) {
    ::schema::improbable::EntityCommand proto_wrapper;
    proto_wrapper.MutableExtension(::schema::improbable::terrain::Terrain::command_extension);


    *length = proto_wrapper.ByteSize();
    *buffer = new std::uint8_t[*length];
    proto_wrapper.SerializeToArray(*buffer, *length);
  }
}

}  // anonymous namespace

const ::worker::detail::ClientComponentVtable& Vtable_Terrain() {
  static const ::worker::detail::ClientComponentVtable vtable = {
      1002,
      &detail::VtableBufferFree_Terrain,
      &detail::VtableFree_Terrain,
      &detail::VtableCopy_Terrain,
      &detail::VtableDeserialize_Terrain,
      &detail::VtableSerialize_Terrain,
  };
  return vtable;
}

}  // ::detail

}  // ::terrain
}  // ::improbable