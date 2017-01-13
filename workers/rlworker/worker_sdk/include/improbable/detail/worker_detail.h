// Copyright (c) Improbable Worlds Ltd, All Rights Reserved
#ifndef IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_DETAIL_WORKER_DETAIL_H
#define IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_DETAIL_WORKER_DETAIL_H
#include "../collections.h"
#include <functional>
#include <memory>
#include <type_traits>

struct WorkerProtocol_AddComponentOp;
struct WorkerProtocol_AddEntityOp;
struct WorkerProtocol_AuthorityChangeOp;
struct WorkerProtocol_ComponentUpdateOp;
struct WorkerProtocol_Connection;
struct WorkerProtocol_CriticalSectionOp;
struct WorkerProtocol_Dispatcher;
struct WorkerProtocol_EntityQueryResponseOp;
struct WorkerProtocol_Locator;
struct WorkerProtocol_LogMessageOp;
struct WorkerProtocol_OpList;
struct WorkerProtocol_RemoveComponentOp;
struct WorkerProtocol_RemoveEntityOp;

namespace worker {
using EntityId = std::int64_t;
using ComponentId = std::uint32_t;

class Connection;
class Dispatcher;
class Entity;
class OpList;
struct Metrics;
namespace query {
struct EntityQuery;
}  // ::query

template <typename>
struct AddComponentOp;
struct AddEntityOp;
struct AuthorityChangeOp;
template <typename>
struct ComponentUpdateOp;
struct CriticalSectionOp;
struct CreateEntityResponseOp;
struct DeleteEntityResponseOp;
struct DisconnectOp;
struct EntityQueryResponseOp;
struct LogMessageOp;
struct MetricsOp;
struct RemoveComponentOp;
struct RemoveEntityOp;
struct ReserveEntityIdResponseOp;

namespace detail {
typedef void ClientObject;
// So that generated code need not include worker_protocol.h.
struct ClientComponentVtable {
  enum ClientObjectType {
    Update = 1,
    Snapshot = 2,
    Request = 3,
    Response = 4,
  };
  worker::ComponentId ComponentId;
  void (*BufferFree)(worker::ComponentId component_id, std::uint8_t* buffer);
  void (*Free)(worker::ComponentId component_id, std::uint8_t object_type, ClientObject* object);
  ClientObject* (*Copy)(worker::ComponentId component_id, std::uint8_t object_type,
                        const ClientObject* object);
  std::uint8_t (*Deserialize)(worker::ComponentId component_id, std::uint8_t object_type,
                              const std::uint8_t* buffer, std::uint32_t length,
                              ClientObject** object_out);
  void (*Serialize)(worker::ComponentId component_id, std::uint8_t object_type,
                    const ClientObject* object, std::uint8_t** buffer_out,
                    std::uint32_t* length_out);
};

// Storage for arbitrary component data.
class ComponentStorageBase {
public:
  virtual ~ComponentStorageBase(){};
  virtual std::unique_ptr<ComponentStorageBase> Copy() const = 0;
};

template <typename T>
class ComponentStorage : public ComponentStorageBase {
public:
  explicit ComponentStorage(const typename T::Data& data) : data{data} {}
  ~ComponentStorage() override {}

  std::unique_ptr<ComponentStorageBase> Copy() const override;
  const Option<typename T::Data>& Get() const;
  Option<typename T::Data>& Get();

private:
  Option<typename T::Data> data;
};

// Callback containers.
template <typename T>
using Callback = std::function<void(const T&)>;
using CallbackKey = std::uint64_t;

template <typename T>
class CallbackMap {
public:
  void Add(CallbackKey key, const Callback<T>& callback);
  bool Remove(CallbackKey key);
  void InvokeAll(const T& op) const;
  void ReverseInvokeAll(const T& op) const;

private:
  Map<CallbackKey, Callback<T>> map;
};

template <typename T>
class ComponentCallbackMap {
public:
  void Add(ComponentId component_id, CallbackKey key, const Callback<T>& callback);
  bool Remove(CallbackKey key);
  void InvokeAll(ComponentId component_id, const T& op) const;
  void ReverseInvokeAll(ComponentId component_id, const T& op) const;

private:
  Map<ComponentId, CallbackMap<T>> map;
};

template <typename T>
struct FutureImpl {
  std::function<Option<T>(const Option<std::uint32_t>&)> get;
  std::function<void()> destroy;
  bool moved_from;
  Option<T> result;
};

struct DispatcherImpl {
  DispatcherImpl(WorkerProtocol_Dispatcher* ptr, void (*deleter)(WorkerProtocol_Dispatcher*))
  : dispatcher{ptr, deleter}, current_callback_key{0} {}

  std::unique_ptr<WorkerProtocol_Dispatcher, void (*)(WorkerProtocol_Dispatcher*)> dispatcher;

  struct ComponentUpdateWrapperOp {
    worker::EntityId EntityId;
    // A WorkerProtocol_ClientObject provided by C API.
    const void* Update;
  };
  struct CommandRequestWrapperOp {
    std::uint32_t RequestId;
    worker::EntityId EntityId;
    std::uint32_t TimeoutMillis;
    const char* CallerWorkerId;
    std::uint32_t CallerClaimCount;
    const char** CallerClaim;
    // A WorkerProtocol_ClientObject provided by C API.
    const void* Request;
  };
  struct CommandResponseWrapperOp {
    std::uint32_t RequestId;
    worker::EntityId EntityId;
    std::uint8_t StatusCode;
    const char* Message;
    // A WorkerProtocol_ClientObject provided by C API. Will be nullptr if StatusCode is not
    // StatucCode::kSuccess.
    const void* Response;
    std::uint32_t CommandId;
  };

  CallbackKey current_callback_key;
  CallbackMap<DisconnectOp> disconnect_callbacks;
  CallbackMap<LogMessageOp> log_message_callbacks;
  CallbackMap<MetricsOp> metrics_callbacks;
  CallbackMap<CriticalSectionOp> critical_section_callbacks;
  CallbackMap<AddEntityOp> add_entity_callbacks;
  CallbackMap<RemoveEntityOp> remove_entity_callbacks;
  CallbackMap<ReserveEntityIdResponseOp> reserve_entity_id_response_callbacks;
  CallbackMap<CreateEntityResponseOp> create_entity_response_callbacks;
  CallbackMap<DeleteEntityResponseOp> delete_entity_response_callbacks;
  CallbackMap<EntityQueryResponseOp> entity_query_response_callbacks;
  ComponentCallbackMap<ComponentUpdateWrapperOp> add_component_callbacks;
  ComponentCallbackMap<RemoveComponentOp> remove_component_callbacks;
  ComponentCallbackMap<AuthorityChangeOp> authority_change_callbacks;
  ComponentCallbackMap<ComponentUpdateWrapperOp> component_update_callbacks;
  ComponentCallbackMap<CommandRequestWrapperOp> command_request_callbacks;
  ComponentCallbackMap<CommandResponseWrapperOp> command_response_callbacks;
};

// Helper functions to send client objects through the connection.
void SendClientComponentUpdate(WorkerProtocol_Connection* connection, EntityId entity_id,
                               ComponentId component_id, const ClientObject* update);
std::uint32_t SendClientCommandRequest(WorkerProtocol_Connection* connection, EntityId entity_id,
                                       ComponentId component_id, const ClientObject* request,
                                       std::uint32_t command_id,
                                       const Option<std::uint32_t>& timeout_millis);
void SendClientCommandResponse(WorkerProtocol_Connection* connection, std::uint32_t request_id,
                               ComponentId component_id, const ClientObject* response);

// Component-specific handling code dispatched to by generic methods using StaticLookupTable.
struct ComponentInfo {
  const ClientComponentVtable* Vtable;
  std::function<void(const ClientObject*, Entity&)> AddToEntityWithInitialData;
  std::function<std::unique_ptr<ClientObject, void (*)(ClientObject*)>(const Entity&)>
      ExtractAsUpdate;
  std::function<void(Entity&)> SetInitialDelegation;
  std::function<void(Dispatcher&, Map<EntityId, Entity>&)> TrackComponent;
};

// This function forces initialization of the static variable mentioned above by calling a no-op
// method on it. It should be called by *every* API function templated on a generated component
// type, so that user code cannot mention a particular state without forcing the component-specific
// initialization to have occurred.
template <typename>
void EnsureStaticInitializationHasOccurred();

// Base class for component metaclasses implementing static initialization and ID lookup. (Update
// and data types are not mentioned here, because we encourage clients to make use of those
// typedefs, and putting them in a templated base class gives much more verbose error messages.)
class ComponentMetaclass {
public:
  virtual ComponentId GetComponentId() const = 0;
  virtual ComponentInfo CreateComponentInfo() const = 0;

  // A list of all known metaclasses. It is wrapped in a function
  // to make sure that it is initialized before it is used. This is populated in the constructor
  // of each metaclass in the generated code for each component.
  static List<const ComponentMetaclass*>& AllMetaclasses();
};

using VtableFunction = const ClientComponentVtable&();
template <typename, VtableFunction>
class ComponentMetaclassBase : public ComponentMetaclass {
public:
  static const ComponentMetaclassBase ClassInstance;
  // Ensures that instances of this class are not optimized out by the linker.
  void EnsureMetaclassIsRegistered() const {}

  ComponentId GetComponentId() const override;
  ComponentInfo CreateComponentInfo() const override;

private:
  ComponentMetaclassBase();
};

}  // ::detail
}  // ::worker

#endif  // IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_DETAIL_WORKER_DETAIL_H
