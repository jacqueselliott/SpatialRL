// Copyright (c) Improbable Worlds Ltd, All Rights Reserved
#ifndef IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_DETAIL_WORKER_I_H
#define IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_DETAIL_WORKER_I_H

// Not strictly necessary, since this file shouldn't be included on its own, but may as well make it
// standalone for the benefit of automated tooling.
#include "../worker.h"

namespace std {
template <typename T>
struct hash<worker::RequestId<T>> {
  std::size_t operator()(const worker::RequestId<T>& key) const {
    return std::hash<std::uint32_t>{}(key.Id);
  }
};
}  // ::std

namespace worker {
namespace detail {

template <typename T>
std::unique_ptr<ComponentStorageBase> ComponentStorage<T>::Copy() const {
  return std::unique_ptr<ComponentStorageBase>{new ComponentStorage{*data}};
}

template <typename T>
const Option<typename T::Data>& ComponentStorage<T>::Get() const {
  return data;
}

template <typename T>
void EnsureStaticInitializationHasOccurred() {
  T::ClassInstance.EnsureMetaclassIsRegistered();
}

template <typename T>
Option<typename T::Data>& ComponentStorage<T>::Get() {
  return data;
}

template <typename T>
void CallbackMap<T>::Add(CallbackKey key, const Callback<T>& callback) {
  map.emplace(key, callback);
}

template <typename T>
bool CallbackMap<T>::Remove(CallbackKey key) {
  auto it = map.find(key);
  if (it == map.end()) {
    return false;
  }
  map.erase(it);
  return true;
}

template <typename T>
void CallbackMap<T>::InvokeAll(const T& op) const {
  for (const auto& pair : map) {
    pair.second(op);
  }
}

template <typename T>
void CallbackMap<T>::ReverseInvokeAll(const T& op) const {
  for (auto it = map.rbegin(), end = map.rend(); it != end; ++it) {
    it->second(op);
  }
}

template <typename T>
void ComponentCallbackMap<T>::Add(ComponentId component_id, CallbackKey key,
                                  const Callback<T>& callback) {
  map[component_id].Add(key, callback);
}

template <typename T>
bool ComponentCallbackMap<T>::Remove(CallbackKey key) {
  for (auto& pair : map) {
    if (pair.second.Remove(key)) {
      return true;
    }
  }
  return false;
}

template <typename T>
void ComponentCallbackMap<T>::InvokeAll(ComponentId component_id, const T& op) const {
  auto it = map.find(component_id);
  if (it != map.end()) {
    it->second.InvokeAll(op);
  }
}

template <typename T>
void ComponentCallbackMap<T>::ReverseInvokeAll(ComponentId component_id, const T& op) const {
  auto it = map.find(component_id);
  if (it != map.end()) {
    it->second.ReverseInvokeAll(op);
  }
}

// Storage for ClassInstance.
template <typename Self, VtableFunction Vtable>
const ComponentMetaclassBase<Self, Vtable> ComponentMetaclassBase<Self, Vtable>::ClassInstance;

template <typename Self, VtableFunction VTable>
ComponentId ComponentMetaclassBase<Self, VTable>::GetComponentId() const {
  return Self::ComponentId;
}

template <typename Self, VtableFunction Vtable>
ComponentInfo ComponentMetaclassBase<Self, Vtable>::CreateComponentInfo() const {
  ComponentInfo info;
  info.Vtable = &Vtable();
  info.AddToEntityWithInitialData = [](const ClientObject* update, Entity& entity) {
    entity.Add<Self>(static_cast<const typename Self::Update*>(update)->ToInitialData());
  };
  info.ExtractAsUpdate = [](const Entity& entity) {
    auto component_data = entity.Get<Self>();
    if (component_data) {
      return std::unique_ptr<ClientObject, void (*)(ClientObject*)>(
          new typename Self::Update(Self::Update::FromInitialData(*component_data)),
          [](ClientObject* update) { delete static_cast<typename Self::Update*>(update); });
    }
    return std::unique_ptr<ClientObject, void (*)(ClientObject*)>(nullptr, {});
  };
  info.SetInitialDelegation = [](Entity& entity) { entity.SetAuthority<Self>(true); };
  info.TrackComponent = [](Dispatcher& dispatcher, Map<EntityId, Entity>& entities) {
    dispatcher.OnAddComponent<Self>([&entities](const AddComponentOp<Self>& op) {
      auto it = entities.find(op.EntityId);
      if (it != entities.end()) {
        // Explicit type to avoid needing `entity.template Get<T>()` syntax due to `entity` being a
        // dependent name, which causes obscure compiler bugs in Visual Studio. See
        // https://connect.microsoft.com/VisualStudio/feedback/details/2362045.
        Entity& entity = it->second;
        entity.Add<Self>(op.Data);
      }
    });
    dispatcher.OnRemoveComponent<Self>([&entities](const RemoveComponentOp& op) {
      auto it = entities.find(op.EntityId);
      if (it != entities.end()) {
        Entity& entity = it->second;
        entity.Remove<Self>();
      }
    });
    dispatcher.OnAuthorityChange<Self>([&entities](const AuthorityChangeOp& op) {
      auto it = entities.find(op.EntityId);
      if (it != entities.end()) {
        it->second.SetAuthority<Self>(op.HasAuthority);
      }
    });
    dispatcher.OnComponentUpdate<Self>([&entities](const ComponentUpdateOp<Self>& op) {
      auto it = entities.find(op.EntityId);
      if (it != entities.end()) {
        Entity& entity = it->second;
        if (entity.Get<Self>()) {
          entity.Update<Self>(op.Update);
        }
      }
    });
  };
  return info;
}

template <typename Self, VtableFunction Vtable>
ComponentMetaclassBase<Self, Vtable>::ComponentMetaclassBase() {
  auto metaclasses = GetComponentMetaclasses();
  metaclasses.emplace_back(this);
  SetComponentMetaclasses(metaclasses);
}
}  // ::detail

template <typename T>
RequestId<T>::RequestId() : Id{0} {}

template <typename T>
RequestId<T>::RequestId(std::uint32_t id) : Id{id} {}

template <typename T>
bool RequestId<T>::operator==(const RequestId& other) const {
  return Id == other.Id;
}

template <typename T>
bool RequestId<T>::operator!=(const RequestId& other) const {
  return !operator==(other);
}

template <typename T>
Future<T>::~Future() {
  impl.destroy();
}

template <typename T>
bool Future<T>::Wait(const Option<std::uint32_t>& timeout_millis) {
  if (!impl.result) {
    impl.result = impl.get(timeout_millis);
  }
  return static_cast<bool>(impl.result);
}

template <typename T>
T Future<T>::Get() {
  if (impl.moved_from) {
    std::terminate();
  }
  if (!impl.result) {
    Wait({});
  }
  impl.moved_from = true;
  return std::move(*impl.result);
}

template <typename T>
Future<T>::Future(const std::function<Option<T>(const Option<std::uint32_t>&)> get,
                  const std::function<void()>& destroy)
: impl{get, destroy, false, {}} {}

template <typename T>
void Connection::SendComponentUpdate(EntityId entity_id, const typename T::Update& update) {
  detail::EnsureStaticInitializationHasOccurred<T>();
  detail::SendClientComponentUpdate(connection.get(), entity_id, T::ComponentId, &update);
}

template <typename T>
RequestId<OutgoingCommandRequest<T>>
Connection::SendCommandRequest(EntityId entity_id, const typename T::Request& request,
                               const Option<std::uint32_t>& timeout_millis) {
  detail::EnsureStaticInitializationHasOccurred<typename T::ComponentMetaclass>();
  typename T::ComponentMetaclass::GenericCommandObject generic_request{T::CommandId, request};
  return RequestId<OutgoingCommandRequest<T>>{detail::SendClientCommandRequest(
      connection.get(), entity_id, T::ComponentMetaclass::ComponentId, &generic_request,
      T::CommandId, timeout_millis)};
}

template <typename T>
void Connection::SendCommandResponse(RequestId<IncomingCommandRequest<T>> request_id,
                                     const typename T::Response& response) {
  detail::EnsureStaticInitializationHasOccurred<typename T::ComponentMetaclass>();
  typename T::ComponentMetaclass::GenericCommandObject generic_response{T::CommandId, response};
  detail::SendClientCommandResponse(connection.get(), request_id.Id,
                                    T::ComponentMetaclass::ComponentId, &generic_response);
}

template <typename... Component>
void Connection::SendInterestedComponents(EntityId entity_id) {
  SendInterestedComponents(entity_id, {Component::ComponentId...});
}

template <typename T>
bool Entity::HasAuthority() const {
  detail::EnsureStaticInitializationHasOccurred<T>();
  return component_authority_flags.find(T::ComponentId) != component_authority_flags.end();
}

template <typename T>
void Entity::SetAuthority(bool has_authority) {
  detail::EnsureStaticInitializationHasOccurred<T>();
  if (has_authority) {
    component_authority_flags.insert(T::ComponentId);
  } else {
    component_authority_flags.erase(T::ComponentId);
  }
}

template <typename T>
const Option<typename T::Data>& Entity::Get() const {
  detail::EnsureStaticInitializationHasOccurred<T>();
  auto it = components.find(T::ComponentId);
  if (it == components.end()) {
    static const Option<typename T::Data> empty_option{};
    return empty_option;
  }
  return static_cast<detail::ComponentStorage<T>&>(*it->second).Get();
}

template <typename T>
void Entity::Add(const typename T::Data& data) {
  detail::EnsureStaticInitializationHasOccurred<T>();
  auto it = components.find(T::ComponentId);
  if (it == components.end()) {
    components.emplace(T::ComponentId, std::unique_ptr<detail::ComponentStorageBase>{
                                           new detail::ComponentStorage<T>{data}});
  }
}

template <typename T>
void Entity::Update(const typename T::Update& update) {
  detail::EnsureStaticInitializationHasOccurred<T>();
  auto it = components.find(T::ComponentId);
  if (it != components.end()) {
    update.ApplyTo(*static_cast<detail::ComponentStorage<T>&>(*it->second).Get());
  }
}

template <typename T>
void Entity::Remove() {
  detail::EnsureStaticInitializationHasOccurred<T>();
  components.erase(T::ComponentId);
}

template <typename T>
Dispatcher::CallbackKey Dispatcher::OnAddComponent(const Callback<AddComponentOp<T>>& callback) {
  detail::EnsureStaticInitializationHasOccurred<T>();
  auto wrapper_callback = [callback](const detail::DispatcherImpl::ComponentUpdateWrapperOp& op) {
    callback(AddComponentOp<T>{op.EntityId,
                               static_cast<const typename T::Update*>(op.Update)->ToInitialData()});
  };
  impl.add_component_callbacks.Add(T::ComponentId, impl.current_callback_key, wrapper_callback);
  return impl.current_callback_key++;
}

template <typename T>
Dispatcher::CallbackKey Dispatcher::OnRemoveComponent(const Callback<RemoveComponentOp>& callback) {
  detail::EnsureStaticInitializationHasOccurred<T>();
  impl.remove_component_callbacks.Add(T::ComponentId, impl.current_callback_key, callback);
  return impl.current_callback_key++;
}

template <typename T>
Dispatcher::CallbackKey Dispatcher::OnAuthorityChange(const Callback<AuthorityChangeOp>& callback) {
  detail::EnsureStaticInitializationHasOccurred<T>();
  impl.authority_change_callbacks.Add(T::ComponentId, impl.current_callback_key, callback);
  return impl.current_callback_key++;
}

template <typename T>
Dispatcher::CallbackKey
Dispatcher::OnComponentUpdate(const Callback<ComponentUpdateOp<T>>& callback) {
  detail::EnsureStaticInitializationHasOccurred<T>();
  auto wrapper_callback = [callback](const detail::DispatcherImpl::ComponentUpdateWrapperOp& op) {
    callback(ComponentUpdateOp<T>{op.EntityId, *static_cast<const typename T::Update*>(op.Update)});
  };
  impl.component_update_callbacks.Add(T::ComponentId, impl.current_callback_key, wrapper_callback);
  return impl.current_callback_key++;
}

template <typename T>
Dispatcher::CallbackKey
Dispatcher::OnCommandRequest(const Callback<CommandRequestOp<T>>& callback) {
  detail::EnsureStaticInitializationHasOccurred<typename T::ComponentMetaclass>();
  auto wrapper_callback = [callback](const detail::DispatcherImpl::CommandRequestWrapperOp& op) {
    const auto& generic_request =
        *static_cast<const typename T::ComponentMetaclass::GenericCommandObject*>(op.Request);
    if (generic_request.CommandId != T::CommandId) {
      return;
    }
    CommandRequestOp<T> wrapper{
        RequestId<IncomingCommandRequest<T>>{op.RequestId},
        op.EntityId,
        op.TimeoutMillis,
        op.CallerWorkerId,
        /* CallerClaims */ {},
        *generic_request.CommandObject.template data<typename T::Request>()};
    for (std::uint32_t i = 0; i < op.CallerClaimCount; ++i) {
      wrapper.CallerClaims.emplace_back(op.CallerClaim[i]);
    }
    callback(wrapper);
  };
  impl.command_request_callbacks.Add(T::ComponentMetaclass::ComponentId, impl.current_callback_key,
                                     wrapper_callback);
  return impl.current_callback_key++;
}

template <typename T>
Dispatcher::CallbackKey
Dispatcher::OnCommandResponse(const Callback<CommandResponseOp<T>>& callback) {
  detail::EnsureStaticInitializationHasOccurred<typename T::ComponentMetaclass>();
  auto wrapper_callback = [callback](const detail::DispatcherImpl::CommandResponseWrapperOp& op) {
    if (op.CommandId != T::CommandId) {
      return;
    }
    CommandResponseOp<T> wrapper{RequestId<OutgoingCommandRequest<T>>{op.RequestId},
                                 op.EntityId,
                                 static_cast<StatusCode>(op.StatusCode),
                                 op.Message,
                                 {}};
    if (op.Response) {
      const auto& generic_response =
          *static_cast<const typename T::ComponentMetaclass::GenericCommandObject*>(op.Response);
      wrapper.Response.emplace(
          *generic_response.CommandObject.template data<typename T::Response>());
    }
    callback(wrapper);
  };
  impl.command_response_callbacks.Add(T::ComponentMetaclass::ComponentId, impl.current_callback_key,
                                      wrapper_callback);
  return impl.current_callback_key++;
}

}  // ::worker

#endif  // IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_DETAIL_WORKER_I_H
