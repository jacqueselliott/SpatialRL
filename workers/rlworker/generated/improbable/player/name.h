// Generated by SpatialOS codegen. DO NOT EDIT!
// source: improbable/player/name.schema
#ifndef SPATIALOS_SCHEMA_improbable_player_name_schema_4286168017219417399_INCLUDED
#define SPATIALOS_SCHEMA_improbable_player_name_schema_4286168017219417399_INCLUDED
#include <improbable/collections.h>
#include <improbable/math/coordinates.h>
#include <improbable/math/vector3d.h>
#include <improbable/math/vector3f.h>
#include <improbable/worker.h>
#include <cstddef>
#include <cstdint>
#include <set>
#include <string>

// Schema forward declarations.
//----------------------------------------------------------------

namespace schema {
namespace improbable {
namespace player {

class NameData;

}  // ::improbable
}  // ::player
}  // ::schema

namespace improbable {
namespace player {

// Forward declarations.
//----------------------------------------------------------------

class NameData;

namespace detail {
const ::worker::detail::ClientComponentVtable& Vtable_Name();
struct GenericCommandObject_Name {
  std::uint32_t CommandId;
  ::worker::Variant<> CommandObject;
};
}  // ::detail

// Enumerations.
//----------------------------------------------------------------

// Data classes.
//----------------------------------------------------------------

class NameData {
public:
  // Main constructor.
  NameData(const std::string& entity_name);

  // Copyable and moveable.
  NameData(NameData&&) = default;
  NameData(const NameData&) = default;
  NameData& operator=(NameData&&) = default;
  NameData& operator=(const NameData&) = default;
  ~NameData() = default;

  bool operator==(const NameData&) const;
  bool operator!=(const NameData&) const;

  // Field entity_name = 1.
  const std::string& entity_name() const;
  std::string& entity_name();
  NameData& set_entity_name(const std::string&);

private:
  std::string _entity_name;
};

// Component metaclasses and update classes.
//----------------------------------------------------------------

/**
 * This type can be supplied as the template parameter to all component-related templated types
 * and methods in the C++ worker API. For example:
 *
 *   using ::improbable::player::Name;
 *
 *   Name::Update update;
 *   connection.SendComponentUpdate< Name >(entity_id, update);
 *
 *   dispatcher.OnComponentUpdate< Name >(
 *       [&](const worker::ComponentUpdateOp< Name >& op) {
 *         worker::EntityId entity_id = op.GetEntityId();
 *         Name::Update update = op.GetUpdate();
 *       });
 *
 *   Name::Data data = entity.Get< Name >();
 */
struct Name : ::worker::detail::ComponentMetaclassBase<
    Name, detail::Vtable_Name> {
  static constexpr const ::worker::ComponentId ComponentId = 1001;
  using Data = ::improbable::player::NameData;
  using GenericCommandObject = detail::GenericCommandObject_Name;

  class Update {
  public:
    // Default-constructible, copyable and movable.
    Update() = default;
    Update(Update&&) = default;
    Update(const Update&) = default;
    Update& operator=(Update&&) = default;
    Update& operator=(const Update&) = default;
    ~Update() = default;

    bool operator==(const Update&) const;
    bool operator!=(const Update&) const;

    /** Creates an Update from a ::improbable::player::NameData object. */
    static Update FromInitialData(const ::improbable::player::NameData& data);

    /**
     * Converts to a ::improbable::player::NameData
     * object. It is an error to call this function unless *all* of the optional fields in this
     * update are filled in.
     */
    ::improbable::player::NameData ToInitialData() const;

    /**
     * Replaces fields in the given ::improbable::player::NameData
     * object with the corresponding fields in this update, where present.
     */
    void ApplyTo(::improbable::player::NameData&) const;

    // Field entity_name = 1.
    const ::worker::Option< std::string >& entity_name() const;
    ::worker::Option< std::string >& entity_name();
    Update& set_entity_name(const std::string&);

  private:
    ::worker::Option< std::string > _entity_name;
  };

};

// Implementation details.
//----------------------------------------------------------------

namespace detail {

void Serialize_NameData(
    const ::improbable::player::NameData&,
    ::schema::improbable::player::NameData&);

::improbable::player::NameData Deserialize_NameData(
    const ::schema::improbable::player::NameData&);

}  // ::detail
}  // ::player
}  // ::improbable

#endif  // SPATIALOS_SCHEMA_improbable_player_name_schema_4286168017219417399_INCLUDED
