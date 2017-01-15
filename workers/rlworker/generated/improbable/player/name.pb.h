// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: improbable/player/name.proto

#ifndef PROTOBUF_improbable_2fplayer_2fname_2eproto__INCLUDED
#define PROTOBUF_improbable_2fplayer_2fname_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include "improbable/entity_state.pb.h"
// @@protoc_insertion_point(includes)

namespace schema {
namespace improbable {
namespace player {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_improbable_2fplayer_2fname_2eproto();
void protobuf_AssignDesc_improbable_2fplayer_2fname_2eproto();
void protobuf_ShutdownFile_improbable_2fplayer_2fname_2eproto();

class Name;
class NameData;
class Name_Commands;
class Name_Events;

// ===================================================================

class NameData : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:schema.improbable.player.NameData) */ {
 public:
  NameData();
  virtual ~NameData();

  NameData(const NameData& from);

  inline NameData& operator=(const NameData& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const NameData& default_instance();

  void Swap(NameData* other);

  // implements Message ----------------------------------------------

  inline NameData* New() const { return New(NULL); }

  NameData* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const NameData& from);
  void MergeFrom(const NameData& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(NameData* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string field1_entity_name = 1;
  bool has_field1_entity_name() const;
  void clear_field1_entity_name();
  static const int kField1EntityNameFieldNumber = 1;
  const ::std::string& field1_entity_name() const;
  void set_field1_entity_name(const ::std::string& value);
  void set_field1_entity_name(const char* value);
  void set_field1_entity_name(const char* value, size_t size);
  ::std::string* mutable_field1_entity_name();
  ::std::string* release_field1_entity_name();
  void set_allocated_field1_entity_name(::std::string* field1_entity_name);

  // @@protoc_insertion_point(class_scope:schema.improbable.player.NameData)
 private:
  inline void set_has_field1_entity_name();
  inline void clear_has_field1_entity_name();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr field1_entity_name_;
  friend void  protobuf_AddDesc_improbable_2fplayer_2fname_2eproto();
  friend void protobuf_AssignDesc_improbable_2fplayer_2fname_2eproto();
  friend void protobuf_ShutdownFile_improbable_2fplayer_2fname_2eproto();

  void InitAsDefaultInstance();
  static NameData* default_instance_;
};
// -------------------------------------------------------------------

class Name_Events : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:schema.improbable.player.Name.Events) */ {
 public:
  Name_Events();
  virtual ~Name_Events();

  Name_Events(const Name_Events& from);

  inline Name_Events& operator=(const Name_Events& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Name_Events& default_instance();

  void Swap(Name_Events* other);

  // implements Message ----------------------------------------------

  inline Name_Events* New() const { return New(NULL); }

  Name_Events* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Name_Events& from);
  void MergeFrom(const Name_Events& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Name_Events* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:schema.improbable.player.Name.Events)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_improbable_2fplayer_2fname_2eproto();
  friend void protobuf_AssignDesc_improbable_2fplayer_2fname_2eproto();
  friend void protobuf_ShutdownFile_improbable_2fplayer_2fname_2eproto();

  void InitAsDefaultInstance();
  static Name_Events* default_instance_;
};
// -------------------------------------------------------------------

class Name_Commands : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:schema.improbable.player.Name.Commands) */ {
 public:
  Name_Commands();
  virtual ~Name_Commands();

  Name_Commands(const Name_Commands& from);

  inline Name_Commands& operator=(const Name_Commands& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Name_Commands& default_instance();

  void Swap(Name_Commands* other);

  // implements Message ----------------------------------------------

  inline Name_Commands* New() const { return New(NULL); }

  Name_Commands* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Name_Commands& from);
  void MergeFrom(const Name_Commands& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Name_Commands* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:schema.improbable.player.Name.Commands)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_improbable_2fplayer_2fname_2eproto();
  friend void protobuf_AssignDesc_improbable_2fplayer_2fname_2eproto();
  friend void protobuf_ShutdownFile_improbable_2fplayer_2fname_2eproto();

  void InitAsDefaultInstance();
  static Name_Commands* default_instance_;
};
// -------------------------------------------------------------------

class Name : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:schema.improbable.player.Name) */ {
 public:
  Name();
  virtual ~Name();

  Name(const Name& from);

  inline Name& operator=(const Name& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Name& default_instance();

  void Swap(Name* other);

  // implements Message ----------------------------------------------

  inline Name* New() const { return New(NULL); }

  Name* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Name& from);
  void MergeFrom(const Name& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(Name* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef Name_Events Events;
  typedef Name_Commands Commands;

  // accessors -------------------------------------------------------

  static const int kComponentExtensionFieldNumber = 1001;
  static ::google::protobuf::internal::ExtensionIdentifier< ::schema::improbable::EntityState,
      ::google::protobuf::internal::MessageTypeTraits< ::schema::improbable::player::NameData >, 11, false >
    component_extension;
  static const int kEventExtensionFieldNumber = 1001;
  static ::google::protobuf::internal::ExtensionIdentifier< ::schema::improbable::EntityEvent,
      ::google::protobuf::internal::MessageTypeTraits< ::schema::improbable::player::Name_Events >, 11, false >
    event_extension;
  static const int kCommandExtensionFieldNumber = 1001;
  static ::google::protobuf::internal::ExtensionIdentifier< ::schema::improbable::EntityCommand,
      ::google::protobuf::internal::MessageTypeTraits< ::schema::improbable::player::Name_Commands >, 11, false >
    command_extension;
  // @@protoc_insertion_point(class_scope:schema.improbable.player.Name)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_improbable_2fplayer_2fname_2eproto();
  friend void protobuf_AssignDesc_improbable_2fplayer_2fname_2eproto();
  friend void protobuf_ShutdownFile_improbable_2fplayer_2fname_2eproto();

  void InitAsDefaultInstance();
  static Name* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// NameData

// optional string field1_entity_name = 1;
inline bool NameData::has_field1_entity_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void NameData::set_has_field1_entity_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void NameData::clear_has_field1_entity_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void NameData::clear_field1_entity_name() {
  field1_entity_name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_field1_entity_name();
}
inline const ::std::string& NameData::field1_entity_name() const {
  // @@protoc_insertion_point(field_get:schema.improbable.player.NameData.field1_entity_name)
  return field1_entity_name_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void NameData::set_field1_entity_name(const ::std::string& value) {
  set_has_field1_entity_name();
  field1_entity_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:schema.improbable.player.NameData.field1_entity_name)
}
inline void NameData::set_field1_entity_name(const char* value) {
  set_has_field1_entity_name();
  field1_entity_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:schema.improbable.player.NameData.field1_entity_name)
}
inline void NameData::set_field1_entity_name(const char* value, size_t size) {
  set_has_field1_entity_name();
  field1_entity_name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:schema.improbable.player.NameData.field1_entity_name)
}
inline ::std::string* NameData::mutable_field1_entity_name() {
  set_has_field1_entity_name();
  // @@protoc_insertion_point(field_mutable:schema.improbable.player.NameData.field1_entity_name)
  return field1_entity_name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* NameData::release_field1_entity_name() {
  // @@protoc_insertion_point(field_release:schema.improbable.player.NameData.field1_entity_name)
  clear_has_field1_entity_name();
  return field1_entity_name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void NameData::set_allocated_field1_entity_name(::std::string* field1_entity_name) {
  if (field1_entity_name != NULL) {
    set_has_field1_entity_name();
  } else {
    clear_has_field1_entity_name();
  }
  field1_entity_name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), field1_entity_name);
  // @@protoc_insertion_point(field_set_allocated:schema.improbable.player.NameData.field1_entity_name)
}

// -------------------------------------------------------------------

// Name_Events

// -------------------------------------------------------------------

// Name_Commands

// -------------------------------------------------------------------

// Name

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace player
}  // namespace improbable
}  // namespace schema

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_improbable_2fplayer_2fname_2eproto__INCLUDED