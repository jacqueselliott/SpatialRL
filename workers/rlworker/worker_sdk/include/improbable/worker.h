// Copyright (c) Improbable Worlds Ltd, All Rights Reserved
#ifndef IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_WORKER_H
#define IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_WORKER_H
#include "collections.h"
#include "defaults.h"
#include "detail/worker_detail.h"
#include "math/coordinates.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_set>

namespace worker {
using EntityId = std::int64_t;
using ComponentId = std::uint32_t;
enum class NetworkConnectionType { kTcp = 0, kRaknet = 1 };
enum class LocatorCredentialsType { kLoginToken = 0, kSteam = 1 };
enum class LogLevel { kDebug = 1, kInfo = 2, kWarn = 3, kError = 4, kFatal = 5 };
enum class StatusCode {
  /** The command was successfully executed and returned a response. */
  kSuccess = 1,
  /** The command failed for any reason, including timeout. */
  kFailure = 2,
};

/** Type parameter for entity ID reservation request IDs. */
struct ReserveEntityIdRequest {
  ReserveEntityIdRequest() = delete;
};
/** Type parameter for entity creation request IDs. */
struct CreateEntityRequest {
  CreateEntityRequest() = delete;
};
/** Type parameter for entity deletion request IDs. */
struct DeleteEntityRequest {
  DeleteEntityRequest() = delete;
};
/** Type parameter for entity query request IDs. */
struct EntityQueryRequest {
  EntityQueryRequest() = delete;
};
/** Type parameter for outgoing entity command request IDs. */
template <typename>
struct OutgoingCommandRequest {
  OutgoingCommandRequest() = delete;
};
/** Type parameter for incoming entity command request IDs. */
template <typename>
struct IncomingCommandRequest {
  IncomingCommandRequest() = delete;
};

/**
 * Represents an ID for a request. The type parameter should be one of the marker types defined
 * above.
 */
template <typename>
struct RequestId {
  RequestId();
  explicit RequestId(std::uint32_t id);

  bool operator==(const RequestId&) const;
  bool operator!=(const RequestId&) const;

  std::uint32_t Id;
};

/** Parameters for configuring a RakNet connection. Used by NetworkParameters. */
struct RakNetNetworkParameters {
  /** Time (in milliseconds) that RakNet should use for its heartbeat protocol. */
  std::uint32_t HeartbeatTimeoutMillis = defaults::kRakNetHeartbeatTimeoutMillis;
};

/** Parameters for configuring a TCP connection. Used by NetworkParameters. */
struct TcpNetworkParameters {
  /** Number of multiplexed TCP connections. */
  std::uint8_t MultiplexLevel = defaults::kTcpMultiplexLevel;
  /** Size in bytes of the TCP send buffer. */
  std::uint32_t SendBufferSize = defaults::kTcpSendBufferSize;
  /** Size in bytes of the TCP receive buffer. */
  std::uint32_t ReceiveBufferSize = defaults::kTcpReceiveBufferSize;
  /** Whether to enable TCP_NODELAY. */
  bool NoDelay = defaults::kTcpNoDelay;
};

/** Parameters for configuring the network connection. */
struct NetworkParameters {
  /**
   * Set this flag to connect to SpatialOS using the externally-visible IP address. This flag must
   * be set when connecting externally (i.e. from outside the cloud) to a cloud deployment.
   */
  bool UseExternalIp = defaults::kUseExternalIp;
  /** Type of network connection to use when connecting to SpatialOS. */
  NetworkConnectionType ConnectionType = NetworkConnectionType::kTcp;
  /** Connection parameters specific to RakNet connections. */
  RakNetNetworkParameters RakNet;
  /** Connection parameters specific to TCP connections. */
  TcpNetworkParameters Tcp;
};

/**
 * Parameters for configuring protocol logging. If enabled, logs all protocol messages sent and
 * received.
 */
struct ProtocolLoggingParameters {
  /** Log file names are prefixed with this prefix, are numbered, and have the extension .log. */
  std::string LogPrefix = defaults::kLogPrefix;
  /**
   * Maximum number of log files to keep. Note that logs from any previous protocol logging
   * sessions will be overwritten.
   */
  std::uint32_t MaxLogFiles = defaults::kMaxLogFiles;
  /** Once the size of a log file reaches this size, a new log file is created. */
  std::uint32_t MaxLogFileSizeBytes = defaults::kMaxLogFileSizeBytes;
};

/** Parameters for creating a Connection and connecting to SpatialOS. */
struct ConnectionParameters {
  /** Worker type (platform). */
  std::string WorkerType;
  /**
   * Unique worker identifier. Note that this is ignored when connecting via the locator, which
   * generates a unique identifier based on the WorkerType and authentication details.
   */
  std::string WorkerId;
  /** Simulation-specific metadata. */
  std::string Metadata;

  /** Parameters controlling the network connection to SpatialOS. */
  NetworkParameters Network;

  /**
   * Number of messages that can be stored on the send queue. When the send queue is full, calls to
   * Connection::Send functions can block.
   */
  std::uint32_t SendQueueCapacity = defaults::kSendQueueCapacity;
  /**
   * Number of messages that can be stored on the receive queue. When the receive queue is full,
   * SpatialOS can apply QoS and drop messages to the worker.
   */
  std::uint32_t ReceiveQueueCapacity = defaults::kReceiveQueueCapacity;
  /**
   * Number of messages logged by the SDK that can be stored in the log message queue. When the log
   * message queue is full, messages logged by the SDK can be dropped.
   */
  std::uint32_t LogMessageQueueCapacity = defaults::kLogMessageQueueCapacity;
  /**
   * The Connection tracks several internal metrics, such as send and receive queue statistics. This
   * parameter controls how frequently the Connection will return a MetricsOp reporting its built-in
   * metrics. If set to zero, this functionality is disabled.
   */
  std::uint32_t BuiltInMetricsReportPeriodMillis = defaults::kBuiltInMetricsReportPeriodMillis;

  /** Parameters for configuring protocol logging. */
  ProtocolLoggingParameters ProtocolLogging;
  /** Whether to enable protocol logging at startup. */
  bool EnableProtocolLoggingAtStartup = false;
};

/** Parameters for authenticating using a SpatialOS login token. */
struct LoginTokenCredentials {
  /** The token would typically be provided on the command-line by the SpatialOS launcher. */
  std::string Token;
};

/** Parameters for authenticating using Steam credentials. */
struct SteamCredentials {
  /**
   * Steam ticket for the steam app ID and publisher key corresponding to the project name specified
   * in the LocatorParameters. Typically obtained from the steam APIs.
   */
  std::string Ticket;
  /**
   * Deployment tag to request access for. If non-empty, must match the following regex:
   * [A-Za-z0-9][A-Za-z0-9_]*
   */
  std::string DeploymentTag;
};

/** Parameters for authenticating and logging in to a SpatialOS deployment via the locator. */
struct LocatorParameters {
  /** The name of the SpatialOS project. */
  std::string ProjectName;
  /** Type of credentials to use when authenticating via the Locator. */
  LocatorCredentialsType CredentialsType;
  /** Parameters used if the CredentialsType is LOGIN_TOKEN. */
  LoginTokenCredentials LoginToken;
  /** Parameters used if the CredentialsType is STEAM. */
  SteamCredentials Steam;
};

/** Details for a specific deployment obtained via Locator::GetDeploymentList. */
struct Deployment {
  /** The name of the deployment. */
  std::string DeploymentName;
  /** The name of the assembly used by this deployment. */
  std::string AssemblyName;
  /** Description of this deployment. */
  std::string Description;
};

/** A deployment list obtained via Locator::GetDeploymentList. */
struct DeploymentList {
  /** List of accessible deployments for the given project. */
  List<Deployment> Deployments;
  /** Will be non-empty if an error occurred. */
  Option<std::string> Error;
};

/** A queue status update when connecting to a deployment via Locator::Connect. */
struct QueueStatus {
  /** Position in the queue. Decreases as we advance to the front of the queue. */
  std::uint32_t PositionInQueue;
  /** Will be non-empty if an error occurred. */
  Option<std::string> Error;
};

/**
 * A class representing the standard future concept. It can be used for both synchronous and
 * asynchronous interaction.
 */
template <typename T>
class Future {
public:
  ~Future();

  // Noncopyable, but movable.
  Future(Future&&) = default;
  Future(const Future&) = delete;
  Future& operator=(Future&&) = default;
  Future& operator=(const Future&) = delete;

  /**
   * Waits for the result of the future to become available. If timeout_millis is empty, blocks
   * until the result is available. If timeout_millis is non-empty, blocks for at most that many
   * milliseconds.
   *
   * Returns true if the result is now available (in which case Get() can be called without
   * blocking); or false if the result is not available.
   */
  bool Wait(const Option<std::uint32_t>& timeout_millis);

  /**
   * Waits until the result of the future is available, and returns it. This method can only be
   * called once; calling Get() a second time is a fatal error.
   */
  T Get();

private:
  friend class Connection;
  friend class Locator;
  Future(const std::function<Option<T>(const Option<std::uint32_t>&)> get,
         const std::function<void()>& destroy);
  detail::FutureImpl<T> impl;
};

/**
 * A client which can be used to connect to a SpatialOS deployment via a locator service.
 * This is the standard flow used to connect a local worker to a cloud deployment. This object
 * should not be used concurrently by multiple threads.
 */
class Locator {
public:
  /**
   * Creates a client for the locator service.
   *
   * The hostname would typically be either "locator.improbable.io" (for production) or
   * "locator-staging.improbable.io" (for staging).
   */
  Locator(const std::string& hostname, const LocatorParameters& params);

  // Noncopyable, but movable.
  Locator(const Locator&) = delete;
  Locator(Locator&&) = default;
  Locator& operator=(const Locator&) = delete;
  Locator& operator=(Locator&&) = default;

  using QueueStatusCallback = std::function<bool(const QueueStatus&)>;

  /**
   * Queries the current list of deployments for the project given in the LocatorParameters.
   *
   * This function is DEPRECATED and will be removed in the next version! Please use
   * GetDeploymentListAsync below.
   */
  DeploymentList GetDeploymentList();

  /** Queries the current list of deployments for the project given in the LocatorParameters. */
  Future<DeploymentList> GetDeploymentListAsync();

  /**
   * Connects to a specific deployment. The deployment name should be obtained by calling
   * GetDeploymentList. The callback should return false to cancel queuing, or true to continue
   * queueing.
   *
   * This function is DEPRECATED and will be removed in the next version! Please use
   * ConnectAsync below.
   */
  Connection Connect(const std::string& deployment_name, const ConnectionParameters& params,
                     const QueueStatusCallback& callback);

  /**
   * Connects to a specific deployment. The deployment name should be obtained by calling
   * GetDeploymentList. The callback should return false to cancel queuing, or true to continue
   * queueing.
   */
  Future<Connection> ConnectAsync(const std::string& deployment_name,
                                  const ConnectionParameters& params,
                                  const QueueStatusCallback& callback);

private:
  std::unique_ptr<WorkerProtocol_Locator, void (*)(WorkerProtocol_Locator*)> locator;
};

/**
 * Worker Connection API. This is the main way of communicating with SpatialOS, sending and
 * receiving data. This object should not be used concurrently by multiple threads.
 */
class Connection {
public:
  /**
   * Connects to a SpatialOS deployment via a receptionist. This is the flow used to connect a
   * managed worker running in the cloud alongside the deployment, and also to connect any local
   * worker to a (local or remote) deployment via a locally-running receptionist.
   *
   * The hostname and port would typically be provided by SpatialOS on the command-line, if this is
   * a managed worker on the cloud, or otherwise be predetermined (e.g. localhost:7777 for the
   * default receptionist of a locally-running deployment).
   *
   * This function is DEPRECATED and will be removed in the next version! Please use ConnectAsync
   * below.
   */
  Connection(const std::string& hostname, std::uint16_t port, const ConnectionParameters& params);

  static Future<Connection> ConnectAsync(const std::string& hostname, std::uint16_t port,
                                         const ConnectionParameters& params);

  // Noncopyable, but movable.
  Connection(const Connection&) = delete;
  Connection(Connection&&) = default;
  Connection& operator=(const Connection&) = delete;
  Connection& operator=(Connection&&) = default;

  /**
   * Returns true if the Connection object was created correctly and has successfully connected
   * to SpatialOS.
   */
  bool IsConnected() const;

  /**
   * Retrieves the list of operations that have occurred since the last call to this function.
   *
   * If timeout_millis is non-zero, the function will block until there is at least one operation to
   * return, or the timeout has been exceeded. If the timeout is exceeded, an empty list will be
   * returned.
   *
   * If timeout_millis is zero the function is non-blocking.
   */
  OpList GetOpList(std::uint32_t timeout_millis);

  /** Sends a log message for the worker to SpatialOS. */
  void SendLogMessage(LogLevel level, const std::string& logger_name, const std::string& message);

  /**
   * Sends a set of metrics for the worker to SpatialOS. Typically this function should be called
   * periodically (e.g. once every few seconds) to report the worker's status.
   *
   * Typically, the caller should merge built-in SDK metrics reported by the last MetricsOp into
   * the Metrics parameter before calling SendMetrics.
   *
   * Since histogram metrics are diff-based, calling this function clears each histogram in the
   * Metrics parameter.
   */
  void SendMetrics(Metrics& metrics);

  /**
   * Requests SpatialOS to reserve an entity ID. Returns a request ID, which can be used to identify
   * a response to the request via the Dispatcher::OnReserveEntityIdResponse callback.
   *
   * If timeout_millis is not specified, the default timeout will be used.
   */
  RequestId<ReserveEntityIdRequest>
  SendReserveEntityIdRequest(const Option<std::uint32_t>& timeout_millis);

  /**
   * Requests SpatialOS to create an entity. Returns a request ID, which can be used to identify
   * a response to the request via the Dispatcher::OnCreateEntityResponse callback.
   *
   * prefab_name exists for legacy purposes only and will be removed (see WIT-1189).
   *
   * If an entity ID is provided, it must have been reserved using SendReserveEntityIdRequest().
   *
   * If timeout_millis is not specified, the default timeout will be used.
   */
  RequestId<CreateEntityRequest>
  SendCreateEntityRequest(const Entity& entity, const Option<std::string> prefab_name,
                          const Option<EntityId>& entity_id,
                          const Option<std::uint32_t>& timeout_millis);

  /**
   * Requests SpatialOS to delete an entity. Returns a request ID, which can be used to identify a
   * response to the request via the Dispatcher::OnDeleteEntityResponse callback.
   *
   * If timeout_millis is not specified, the default timeout will be used.
   */
  RequestId<DeleteEntityRequest>
  SendDeleteEntityRequest(EntityId entity_id, const Option<std::uint32_t>& timeout_millis);

  /**
   * Queries SpatialOS for remote entity data. Returns a request ID, which can be used to identify a
   * response to the request via the Dispatcher::OnEntityQueryResponse callback.
   *
   * If timeout_millis is not specified, the default timeout will be used.
   */
  RequestId<EntityQueryRequest> SendEntityQueryRequest(const query::EntityQuery& entity_query,
                                                       const Option<std::uint32_t>& timeout_millis);

  /**
   * Sends the set of interested components for the given entity to SpatialOS. Usually, the
   * client is sent all components. However, certain bridge settings can restrict the components
   * that will be checked out on a worker by default. This function explicitly requests checkout of
   * the given components for a specific entity.
   *
   * Subsequent calls replace previous calls for the same entity.
   *
   * Component template parameters should be automatically generated component metaclasses.
   */
  template <typename... Component>
  void SendInterestedComponents(EntityId entity_id);

  /** Same as the other overload for SendInterestedComponents, but works on raw component IDs. */
  void SendInterestedComponents(EntityId entity_id, const List<ComponentId>& component_ids);

  /**
   * Sends an update for an entity's component to SpatialOS. The template parameter T should be a
   * generated component API metaclass. Note that the sent component update is added as an operation
   * to the operation list and will be returned by a subsequent call to GetOpList().
   */
  template <typename T>
  void SendComponentUpdate(EntityId entity_id, const typename T::Update& update);

  /**
   * Sends a command request to a component on a specific target entity. Returns a request ID, which
   * can be used to identify a response to the command via the Dispatcher::OnCommandResponse
   * callback. The template parameter T should be a generated command API metaclass.
   *
   * If timeout_millis is not specified, the default timeout will be used.
   */
  template <typename T>
  RequestId<OutgoingCommandRequest<T>>
  SendCommandRequest(EntityId entity_id, const typename T::Request& request,
                     const Option<std::uint32_t>& timeout_millis);

  /**
   * Sends a response to an incoming command request for a component on an entity over which this
   * worker has authority. The request ID should match an incoming command request via the
   * Dispatcher::OnCommandRequest callback. The template parameter T should be a generated command
   * API metaclass.
   */
  template <typename T>
  void SendCommandResponse(RequestId<IncomingCommandRequest<T>> request_id,
                           const typename T::Response& response);

  /**
   * Returns the value of the worker flag with the given name. If no worker flag exists with the
   * given name, an empty option is returned.
   *
   * Worker flags are remotely configurable and may change during the runtime of the worker,
   * including addition and deletion.
   */
  Option<std::string> GetWorkerFlag(const std::string& flag_name) const;

  /**
   * Enables or disables protocol logging. Logging uses the parameters specified when the Connection
   * was created. Enabling it when already enabled, or disabling it when already disabled, do
   * nothing.
   *
   * Note that logs from any previous protocol logging sessions will be overwritten.
   */
  void SetProtocolLoggingEnabled(bool enabled);

private:
  friend class Locator;
  explicit Connection(WorkerProtocol_Connection* connection);
  std::unique_ptr<WorkerProtocol_Connection, void (*)(WorkerProtocol_Connection*)> connection;
};

/**
 * An opaque list of operations retrieved from Connection::GetOpList(). It is usually passed to
 * Dispatcher::Process(), which dispatches the operations to the appropriate callbacks.
 */
class OpList {
public:
  // Noncopyable, but movable.
  OpList(const OpList&) = delete;
  OpList(OpList&&) = default;
  OpList& operator=(const OpList&) = delete;
  OpList& operator=(OpList&&) = default;

private:
  friend class Connection;
  friend class Dispatcher;
  explicit OpList(WorkerProtocol_OpList* op_list);
  std::unique_ptr<WorkerProtocol_OpList, void (*)(WorkerProtocol_OpList*)> op_list;
};

/**
 * A histogram metric tracks observations of a given value by bucket. This corresponds to a
 * Prometheus histogram metric. This object should not be used concurrently by multiple threads.
 */
class HistogramMetric {
public:
  /** A histogram bucket. */
  struct Bucket {
    /** The upper bound. */
    double UpperBound;
    /** The number of observations that were less than or equal to the upper bound. */
    std::uint32_t Samples;
  };

  /**
   * Creates a histogram with the given bucket boundaries. Each bucket boundary is an upper bound;
   * the bucket tracks all observations with a value less than or equal to the bound. A final bucket
   * with a boundary of +INF is added automatically.
   */
  HistogramMetric(const List<double>& bucket_upper_bounds);
  /** Creates a histogram with a single bucket. */
  HistogramMetric();

  /** Clears all recorded oservations. Automatically called by Connection::SendMetrics. */
  void ClearObservations();
  /** Records a sample and adds it to the corresponding buckets. */
  void RecordObservation(double value);

  /** Returns the buckets for inspection. */
  const List<Bucket>& Buckets() const;
  /** Returns the sum of all observations since the last call to ClearObservations. */
  double Sum() const;

private:
  List<Bucket> buckets;
  double sum;
};

/**
 * A set of metrics sent up from a worker to SpatialOS.
 *
 * Keys for the contained metrics should match the following regex: [a-zA-Z_][a-zA-Z0-9_]*
 */
struct Metrics {
  /** Copies all metrics from another Metrics object into this one, overwriting existing values. */
  void Merge(const Metrics& metrics);

  /**
   * The load value of this worker. A value of 0 indicates that the worker is completely unloaded;
   * a value greater than 1 indicates that the worker is overloaded. The load value directs
   * SpatialOS's load-balancing strategy for managed workers (spinning them up, spinning them down,
   * and assigning work between them).
   */
  Option<double> Load;

  /** Gauge metrics for the worker. */
  Map<std::string, double> GaugeMetrics;
  /** Histogram metrics for the worker. */
  Map<std::string, HistogramMetric> HistogramMetrics;
};

/**
 * Stores the complete data for an entity's components. This is used both for representing the
 * initial set of components for an entity by the AddEntity operation, and inside the (optional)
 * View.
 *
 * Note that an Entity object is simply a local data structure, and changes made here are not
 * automatically reflected across the SpatialOS simulation. To synchronize component state with
 * SpatialOS, use Connection::SendComponentUpdate.
 *
 * This object should not be modified concurrently by multiple threads.
 */
class Entity {
public:
  Entity() {}
  Entity(const Entity& entity);
  Entity(Entity&&) = default;
  Entity& operator=(const Entity& entity);
  Entity& operator=(Entity&&) = default;

  /**
   * Retrieves a flag representing authority over the given component. The template parameter T
   * should be a generated component API metaclass.
   */
  template <typename T>
  bool HasAuthority() const;

  /**
   * Overrides the flag representing authority over the given component. The template parameter T
   * should be a generated component API metaclass.
   */
  template <typename T>
  void SetAuthority(bool has_authority);

  /**
   * Retrieves data for the given component. Returns an empty option if the entity does not have the
   * given component. The template parameter T should be a generated component API metaclass.
   */
  template <typename T>
  const Option<typename T::Data>& Get() const;

  /**
   * Creates the given component with initial data. Has no effect if the entity already has the
   * given component. The template parameter T should be a generated component API metaclass.
   */
  template <typename T>
  void Add(const typename T::Data& data);

  /**
   * Applies an update to the given component. Has no effect if the entity does not have the given
   * component. The template parameter T should be a generated component API metaclass.
   */
  template <typename T>
  void Update(const typename T::Update& update);

  /**
   * Removes a component. The template parameter T should be a generated component API metaclass.
   */
  template <typename T>
  void Remove();

  /**
   * Creates and returns a vector containing the ComponentId of the components present in this
   * entity.
   */
  std::vector<ComponentId> GetComponentIds() const;

private:
  std::unordered_set<ComponentId> component_authority_flags;
  Map<ComponentId, std::unique_ptr<detail::ComponentStorageBase>> components;
};

namespace query {
/** Constrains a query to match only entities with a particular ID. */
struct EntityIdConstraint {
  worker::EntityId EntityId;
};

/** Constraints a query to match only entities that have a specific component. */
struct ComponentConstraint {
  worker::ComponentId ComponentId;
};

/** Constrains a query to match only entities whose position lies within a given sphere. */
struct SphereConstraint {
  improbable::math::Coordinates Position;
  double Radius;
};

struct AndConstraint;
struct OrConstraint;
using Constraint =
    Variant<EntityIdConstraint, SphereConstraint, ComponentConstraint, AndConstraint, OrConstraint>;

/** Constrains a query by the conjuction of one or more constraints. */
struct AndConstraint : List<Constraint> {
  using List<Constraint>::List;
};

/** Constrains a query by the disjuction of one or more constraints. */
struct OrConstraint : List<Constraint> {
  using List<Constraint>::List;
};

/** Indicates that a query should return the number of entities it matched. */
struct CountResultType {};

/** Indicates that a query should return a component data snapshot for each matched entity. */
struct SnapshotResultType {
  /** If nonempty, filters the components returned in the snapshot for each entity. */
  Option<List<ComponentId>> ComponentIds;
};

using ResultType = Variant<CountResultType, SnapshotResultType>;

/** Represents a global query for entity data across the simulation. */
struct EntityQuery {
  query::Constraint Constraint;
  query::ResultType ResultType;
};
}  // ::query

/**
 * Data for an operation that indicates the Connection has disconnected and can no longer be
 * used.
 */
struct DisconnectOp {
  std::string Reason;
};

/** Data for an operation that provides a log message from the SDK. */
struct LogMessageOp {
  LogLevel Level;
  std::string Message;
};

/** Data for an operation that provides a report on built-in metrics from the SDK. */
struct MetricsOp {
  worker::Metrics Metrics;
};

/**
 * Data for an operation that indicates the message stream received by the worker is entering or
 * leaving a critical section.
 *
 * Nested critical sections are currently not supported by the protocol.
 */
struct CriticalSectionOp {
  bool InCriticalSection;
};

/**
 * Data for an operation that indicates an entity has been added to the worker's view of the
 * simulation.
 */
struct AddEntityOp {
  /**
   * The ID of the entity that was added to the worker's view of the simulation.
   */
  worker::EntityId EntityId;
  /**
   * This exists for legacy purposes only and will be removed (see WIT-1189). Please use a
   * component to send and receive this information.
   */
  std::string PrefabName;
  /**
   * This exists for legacy purposes only and will be removed (see WIT-1189). Please use a
   * component to send and receive this information.
   */
  std::string PrefabContext;
};

/**
 * Data for an operation that indicates an entity has been removed from the worker's view of the
 * simulation.
 */
struct RemoveEntityOp {
  worker::EntityId EntityId;
};

/** A response indicating the result of an entity ID reservation request. */
struct ReserveEntityIdResponseOp {
  /**
   * The outgoing request ID for which there was a response. Matches a previous call to
   * Connection::SendReserveEntityIdRequest.
   */
  worker::RequestId<ReserveEntityIdRequest> RequestId;
  /** Status code of the response. */
  worker::StatusCode StatusCode;
  /** A description of the status. Will contain the reason for failure if unsuccessful. */
  std::string Message;
  /**
   * If successful, a newly allocated entity ID, which is guaranteed to be unused in the current
   * deployment.
   */
  Option<worker::EntityId> EntityId;
};

/** A response indicating the result of an entity creation request. */
struct CreateEntityResponseOp {
  /**
   * The outgoing request ID for which there was a response. Matches a previous call to
   * Connection::SendCreateEntityRequest.
   */
  worker::RequestId<CreateEntityRequest> RequestId;
  /** Status code of the response. */
  worker::StatusCode StatusCode;
  /** A description of the status. Will contain the reason for failure if unsuccessful. */
  std::string Message;
  /** If successful, the entity ID of the newly created entity. */
  Option<worker::EntityId> EntityId;
};

/** A response indicating the result of an entity deletion request. */
struct DeleteEntityResponseOp {
  /**
   * The outgoing request ID for which there was a response. Matches a previous call to
   * Connection::SendDeleteEntityRequest.
   */
  worker::RequestId<DeleteEntityRequest> RequestId;
  /** The ID of the target entity of this request. */
  worker::EntityId EntityId;
  /** Status code of the response. */
  worker::StatusCode StatusCode;
  /** A description of the status. Will contain the reason for failure if unsuccessful. */
  std::string Message;
};

/** A response indicating the result of an entity query request. */
struct EntityQueryResponseOp {
  /**
   * The outgoing request ID for which there was a response. Matches a previous call to
   * Connection::SendEntityQueryRequest.
   */
  worker::RequestId<EntityQueryRequest> RequestId;
  /** Status code of the response. */
  worker::StatusCode StatusCode;
  /** A description of the status. Will contain the reason for failure if unsuccessful. */
  std::string Message;
  /**
   * The number of entities that matched the query.
   *
   * Note that a best-effort attempt is made to count the entities even when the status code is not
   * StatusCode::kSuccess. In this case, the count can still be non-zero, but should be considered a
   * lower bound (i.e. there might be entities matching the query that were not counted).
   */
  std::size_t ResultCount;
  /**
   * The result of the query. Not used for CountResultType queries.
   *
   * Note that a best-effort attempt is made to get results even when the status code is not
   * StatusCode::kSuccess. In this case, the result can still be non-empty, but should be considered
   * incomplete (i.e. there might be entities matching the query that were not returned).
   */
  Map<EntityId, Entity> Result;
};

/**
 * Data for an operation that indicates a component has been added to an existing entity in the
 * worker's view of the simulation. The template parameter T should be a generated component
 * metaclass.
 */
template <typename T>
struct AddComponentOp {
  worker::EntityId EntityId;
  typename T::Data Data;
};

/**
 * Data for an operation that indicates a component has been removed from an existing entity in the
 * worker's view of the simulation.
 */
struct RemoveComponentOp {
  worker::EntityId EntityId;
};

/**
 * Data for an operation that indicates the worker's authority over a component for an entity has
 * been changed.
 */
struct AuthorityChangeOp {
  worker::EntityId EntityId;
  bool HasAuthority;
};

/**
 * Data for an operation that indicates the component for an entity has been updated. The template
 * parameter T should be a generated component metaclass.
 */
template <typename T>
struct ComponentUpdateOp {
  worker::EntityId EntityId;
  typename T::Update Update;
};

/**
 * Data for an operation that indicates a command request has been received for a component on an
 * entity over which this worker has authority. The template parameter T should be a generated
 * component API metaclass. The worker should respond to the command by calling
 * Connection::SendCommandResponse<T> with the given request ID.
 */
template <typename T>
struct CommandRequestOp {
  /**
   * The incoming request ID, which should be passed to Connection::SendCommandResponse in order to
   * respond to this request.
   */
  worker::RequestId<IncomingCommandRequest<T>> RequestId;
  /** The ID of the target entity of this request. */
  worker::EntityId EntityId;
  /**
   * An upper bound on the timeout of this request. Any response sent after the timeout has expired
   * will be ignored by the SDK.
   */
  std::uint32_t TimeoutMillis;
  /** The ID of the worker that initiated this request. */
  std::string CallerWorkerId;
  /** The list of claims of the worker that initiated this request. */
  List<std::string> CallerClaims;
  /** The request data. */
  typename T::Request Request;
};

/**
 * Data for an operation that indicates a command response has been received for a request
 * previously issued by this worker. The template parameter T should be a generated component API
 * metaclass. The request ID will match a previous call to Connection::SendCommandRequest<T>.
 */
template <typename T>
struct CommandResponseOp {
  /**
   * The outgoing request ID, which matches a the request ID returned by a previous call to
   * Connection::SendCommandRequest.
   */
  worker::RequestId<OutgoingCommandRequest<T>> RequestId;
  /** The target entity ID of the original request. */
  worker::EntityId EntityId;
  /** The status code of the command response. */
  worker::StatusCode StatusCode;
  /** A description of the status. Will contain the reason for failure if unsuccessful. */
  std::string Message;
  /** The command response data. Present exactly when the status code is StatusCode::kSuccess. */
  Option<typename T::Response> Response;
};

/**
 * A Dispatcher processes OpLists retrieved from the Connection and invokes appropriate callbacks.
 * This object should not be modified concurrently by multiple threads.
 */
class Dispatcher {
public:
  template <typename T>
  using Callback = std::function<void(const T&)>;
  using CallbackKey = std::uint64_t;

  Dispatcher();

  // Noncopyable, but movable.
  Dispatcher(const Dispatcher&) = delete;
  Dispatcher(Dispatcher&&) = default;
  Dispatcher& operator=(const Dispatcher&) = delete;
  Dispatcher& operator=(Dispatcher&&) = default;

  /**
   * Registers a callback to be invoked when the Connection has disconnected and can no longer be
   * used.
   */
  CallbackKey OnDisconnect(const Callback<DisconnectOp>& callback);

  /** Registers a callback to be invoked when the SDK logs a message. */
  CallbackKey OnLogMessage(const Callback<LogMessageOp>& callback);

  /** Registers a callback to be invoked when the SDK reports built-in metrics. */
  CallbackKey OnMetrics(const Callback<MetricsOp>& callback);

  /**
   * Registers a callback to be invoked when the message stream enters or leaves a critical section.
   */
  CallbackKey OnCriticalSection(const Callback<CriticalSectionOp>& callback);

  /**
   * Registers a callback to be invoked when an entity is added to the worker's view of the
   * simulation.
   */
  CallbackKey OnAddEntity(const Callback<AddEntityOp>& callback);

  /**
   * Registers a callback to be invoked when an entity is removed from the worker's view of the
   * simulation.
   */
  CallbackKey OnRemoveEntity(const Callback<RemoveEntityOp>& callback);

  /** Registers a callback to be invoked when an entity ID reservation response is received. */
  CallbackKey OnReserveEntityIdResponse(const Callback<ReserveEntityIdResponseOp>& callback);

  /** Registers a callback to be invoked when an entity creation response is received. */
  CallbackKey OnCreateEntityResponse(const Callback<CreateEntityResponseOp>& callback);

  /** Registers a callback to be invoked when an entity delete response is received. */
  CallbackKey OnDeleteEntityResponse(const Callback<DeleteEntityResponseOp>& callback);

  /** Registers a callback to be invoked when an entity query response is recieved. */
  CallbackKey OnEntityQueryResponse(const Callback<EntityQueryResponseOp>& callback);

  /**
   * Registers a callback to be invoked when a particular component (indicated by the template
   * parameter T, which should be a generated component API metaclass) is added to an existing
   * entity in the worker's view fo the sumulation.
   */
  template <typename T>
  CallbackKey OnAddComponent(const Callback<AddComponentOp<T>>& callback);

  /**
   * Registers a callback to be invoked when a particular component (indicated by the template
   * parameter T, which should be a generated component API metaclass) is removed from an existing
   * entity in the worker's view of the simulation.
   */
  template <typename T>
  CallbackKey OnRemoveComponent(const Callback<RemoveComponentOp>& callback);

  /**
   * Registers a callback to be invoked when the worker is granted authority over a particular
   * component (indicated by the template parameter T, which should be a generated component API
   * metaclass) for some entity, or when the worker's authority over that component is revoked.
   */
  template <typename T>
  CallbackKey OnAuthorityChange(const Callback<AuthorityChangeOp>& callback);

  /**
   * Registers a callback to be invoked when a particular component (indicated by the template
   * parameter T, which should be a generated component API metaclass) is updated for an entity.
   */
  template <typename T>
  CallbackKey OnComponentUpdate(const Callback<ComponentUpdateOp<T>>& callback);

  /**
   * Registers a callback to be invoked when a command request is received for a particular
   * component (indicated by the template parameter T, which should be a generated command API
   * metaclass).
   */
  template <typename T>
  CallbackKey OnCommandRequest(const Callback<CommandRequestOp<T>>& callback);

  /**
   * Registers a callback to be invoked when a command response is received for a particular
   * component (indicated by the template parameter T, which should be a generated command API
   * metaclass).
   */
  template <typename T>
  CallbackKey OnCommandResponse(const Callback<CommandResponseOp<T>>& callback);

  /**
   * Unregisters a callback identified by its CallbackKey, as returned from the registration
   * function. If the key does not exist, the application will terminate.
   */
  void Remove(CallbackKey key);

  /** Processes an OpList and invokes registered callbacks. */
  void Process(const OpList& op_list) const;

private:
  detail::DispatcherImpl impl;
};

/**
 * The View is an optional data structure that maintains the known state of entities in the worker's
 * view of the simulation. This object should not be modified concurrently by multiple threads. In
 * particular, a call to Process() can modify the Entities map as a side-effect, and so should not
 * be made concurrently with any manual modification of the Entities map.
 */
class View : public Dispatcher {
public:
  View();

  // Noncopyable, but movable.
  View(const View&) = delete;
  View(View&&) = default;
  View& operator=(const View&) = delete;
  View& operator=(View&&) = default;

  /** The state of the universe. */
  Map<EntityId, Entity> Entities;
};

/**
 * Returns a list of all registered component types.
 *
 * This list is local to the current binary. If the Connection has been created in another
 * binary (for example, in a dynamically-loaded plugin), then this list must be shared with the
 * Connection's binary by calling SetComponentMetaclasses.
 *
 * This function must not be called during static initialization.
 */
const List<const detail::ComponentMetaclass*>& GetComponentMetaclasses();

/**
 * Set a new list of component metaclasses.
 *
 * The Connection, Dispatcher and View classes use this information to send and receive component
  updates. To have any effect, this function must be called before attempting to connect to
  SpatialOS or to load a snapshot.
 *
 * This function must not be called during static initialization.
 */
void SetComponentMetaclasses(const List<const detail::ComponentMetaclass*>& new_metaclasses);

/** The snapshot of an Entity. */
struct SnapshotEntity : public Entity {
public:
  /** This exists for legacy purposes only and will be removed (see WIT-1189). */
  std::string Prefab;
};

/** Load a snapshot from a file. Returns an error message if an error occured. */
Option<std::string> LoadSnapshot(const std::string& path,
                                 std::unordered_map<EntityId, SnapshotEntity>& entities_output);

/** Saves a snapshot to a file. Returns an error message if an error occured. */
Option<std::string> SaveSnapshot(const std::string& path,
                                 const std::unordered_map<EntityId, SnapshotEntity>& entities);

}  // ::worker

// Templated implementations.
#include "detail/worker.i.h"
#endif  // IMPROBABLE_WORKER_SDK_INCLUDE_IMPROBABLE_WORKER_H
