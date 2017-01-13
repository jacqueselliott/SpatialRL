// Generated by SpatialOS codegen. DO NOT EDIT!
// source: EntityAcl in improbable/standard_library.schema.

namespace Improbable
{

/// <summary>
/// Deprecated and will be removed in a future version. Consider migrating to the simplified
/// <c>EntityAcl.Writer</c> interface (but see its documentation for semantic differences).
/// </summary>
[global::Improbable.Entity.Component.WriterInterface]
[global::Improbable.Entity.Component.ComponentId(50)]
public interface EntityAclWriter : EntityAclReader
{
  EntityAcl.Updater Update { get; }
}

/// <summary>
/// Deprecated and will be removed in a future version. Consider migrating to the simplified
/// <c>EntityAcl.Reader</c> interface (but see its documentation for semantic differences).
/// </summary>
[global::Improbable.Entity.Component.ReaderInterface]
[global::Improbable.Entity.Component.ComponentId(50)]
public interface EntityAclReader
{
  bool IsAuthoritativeHere { get; }
  event global::System.Action PropertyUpdated;
  event global::System.Action<bool> AuthorityChanged;
  // Field read = 1.
  global::Improbable.Collections.Option<global::Improbable.WorkerPredicate> Read { get; }
  event global::System.Action<global::Improbable.Collections.Option<global::Improbable.WorkerPredicate>> ReadUpdated;
  // Field component_acl = 2.
  global::Improbable.Collections.Option<global::Improbable.ComponentAcl> ComponentAcl { get; }
  event global::System.Action<global::Improbable.Collections.Option<global::Improbable.ComponentAcl>> ComponentAclUpdated;
}

public partial class EntityAcl : global::Improbable.Entity.Component.IComponentFactory
{
  [global::Improbable.Entity.Component.WriterInterface]
  [global::Improbable.Entity.Component.ComponentId(50)]
  public interface Writer : Reader, global::Improbable.Entity.Component.IComponentWriter
  {
    /// <summary>
    /// Sends a component update.
    /// </summary>
    /// <remarks>
    /// Unlike the deprecated <c>EntityAclWriter</c> interface, changes made by the update are not
    /// applied to the local copy of the component (returned by the <c>Data</c> property) until the
    /// update is processed by the connection. Therefore, the <c>ComponentUpdated</c> event is not
    /// triggered immediately, but queued to be triggered at a later time. Additionally, the
    /// update will be sent even if it would have no effect on the current local copy of the
    /// component data; discarding of no-op updates should be done manually. The behaviour is
    /// undefined if the update is mutated after it is sent; use <c>Send(update.DeepCopy())</c> if
    /// you intend to hold on to the update and modify it later.
    /// </remarks>
    void Send(Update update);
    /// <summary>
    /// Returns the CommandReceiver for this component.
    /// </summary>
    ICommandReceiver CommandReceiver { get; }
  }

  [global::Improbable.Entity.Component.ReaderInterface]
  [global::Improbable.Entity.Component.ComponentId(50)]
  public interface Reader
  {
    /// <summary>
    /// Whether the local worker currently has authority over this component.
    /// </summary>
    bool HasAuthority { get; }
    /// <summary>
    /// Retrieves the local copy of the component data.
    /// </summary>
    global::Improbable.EntityAclData Data { get; }

    /// <summary>
    /// Triggered whenever an update is received for this component.
    /// </summary>
    /// <remarks>
    /// Unlike the deprecated <c>EntityAclReader</c> interface, registered handlers are not
    /// invoked immediately upon registration. Additionally, callbacks are not automatically removed
    /// OnDisable - visualizers must clean up themselves.
    /// </remarks>
    event global::System.Action<Update> ComponentUpdated;
    /// <summary>
    /// Triggered whenever the local worker's authority over this component changes.
    /// </summary>
    /// <remarks>
    /// Unlike the deprecated <c>EntityAclReader</c> interface, registered handlers are not
    /// invoked immediately upon registration. Additionally, callbacks are not automatically removed
    /// OnDisable - visualizers must clean up themselves.
    /// </remarks>
    event global::System.Action<bool> AuthorityChanged;
  }

  /// <summary>
  /// Deprecated and will be removed in the next major version. Please migrate to the simplified
  /// <c>EntityAcl.Writer</c> interface (but see its documentation for semantic differences).
  /// </summary>
  public interface Updater
  {
    void FinishAndSend();
    // Field read = 1.
    Updater Read(global::Improbable.Collections.Option<global::Improbable.WorkerPredicate> newValue);
    // Field component_acl = 2.
    Updater ComponentAcl(global::Improbable.Collections.Option<global::Improbable.ComponentAcl> newValue);
  }

  public interface ICommandReceiver
  {
  }

  // Implementation details below here.
  //-----------------------------------

  private readonly global::System.Collections.Generic.Dictionary<global::Improbable.EntityId, Impl> implMap =
      new global::System.Collections.Generic.Dictionary<global::Improbable.EntityId, Impl>();
  private bool loggedInvalidAddComponent = false;
  private bool loggedInvalidRemoveComponent = false;

  public void UnregisterWithConnection(global::Improbable.Worker.Connection connection, global::Improbable.Worker.Dispatcher dispatcher) {
    loggedInvalidAddComponent = false;
    loggedInvalidRemoveComponent = false;
    implMap.Clear();
  }

  public void RegisterWithConnection(global::Improbable.Worker.Connection connection, global::Improbable.Worker.Dispatcher dispatcher,
                                     global::Improbable.Entity.Component.ComponentFactoryCallbacks callbacks)
  {
    dispatcher.OnAddComponent<EntityAcl>((op) => {
      if (!implMap.ContainsKey(op.EntityId))
      {
        var impl = new Impl(connection, op.EntityId, op.Data.Get());
        implMap.Add(op.EntityId, impl);
        if (callbacks.OnComponentAdded != null)
        {
          callbacks.OnComponentAdded(op.EntityId, this, impl);
        }
      }
      else if (!loggedInvalidAddComponent)
      {
        global::Improbable.Worker.ClientError.LogClientException(new global::System.InvalidOperationException(
            "Component EntityAcl added to entity " + op.EntityId + ", but it already exists." +
            "This error will be reported once only for each component."));
        loggedInvalidAddComponent = true;
      }
    });

    dispatcher.OnRemoveComponent<EntityAcl>((op) => {
      Impl impl;
      if (implMap.TryGetValue(op.EntityId, out impl))
      {
        if (callbacks.OnComponentRemoved != null)
        {
          callbacks.OnComponentRemoved(op.EntityId, this, impl);
        }
        implMap.Remove(op.EntityId);
      }
      else if (!loggedInvalidRemoveComponent)
      {
        global::Improbable.Worker.ClientError.LogClientException(new global::System.InvalidOperationException(
            "Component EntityAcl removed from entity " + op.EntityId + ", but it does not exist." +
            "This error will be reported once only for each component."));
        loggedInvalidRemoveComponent = true;
      }
    });

    dispatcher.OnAuthorityChange<EntityAcl>((op) => {
      Impl impl;
      if (implMap.TryGetValue(op.EntityId, out impl))
      {
        impl.On_AuthorityChange(op.HasAuthority);
        if (callbacks.OnAuthorityChanged != null)
        {
          callbacks.OnAuthorityChanged(op.EntityId, this, op.HasAuthority, impl);
        }
      }
    });

    dispatcher.OnComponentUpdate<EntityAcl>((op) => {
      Impl impl;
      if (implMap.TryGetValue(op.EntityId, out impl))
      {
        impl.On_ComponentUpdate(op.Update.Get());
        if (callbacks.OnComponentUpdated != null)
        {
          callbacks.OnComponentUpdated(op.EntityId, this, impl);
        }
      }
    });
  }

  public object GetComponentForEntity(global::Improbable.EntityId entityId)
  {
    Impl impl;
    implMap.TryGetValue(entityId, out impl);
    return impl;
  }

  public void DisconnectEventHandlersWithTarget(global::Improbable.EntityId entityId, object actionTarget)
  {
    Impl impl;
    if (implMap.TryGetValue(entityId, out impl))
    {
      impl.DisconnectEventHandlers(actionTarget);
    }
  }

  #pragma warning disable 0618
  public class Impl : EntityAclWriter, Writer, Updater
  #pragma warning restore 0618
  {
    private readonly global::Improbable.Worker.Connection connection;
    private readonly global::Improbable.EntityId entityId;
    private readonly CommandReceiverImpl commandReceiver;

    private Data data;
    private bool hasAuthority = false;
    private readonly global::System.Collections.Generic.List<global::System.Action<Update>> componentUpdateCallbacks =
        new global::System.Collections.Generic.List<global::System.Action<Update>>();
    private readonly global::System.Collections.Generic.List<global::System.Action<bool>> authorityChangeCallbacks =
        new global::System.Collections.Generic.List<global::System.Action<bool>>();

    public uint ComponentId { get { return 50; } }

    public Impl(global::Improbable.Worker.Connection connection, global::Improbable.EntityId entityId, Data initialData)
    {
      this.connection = connection;
      this.entityId = entityId;
      this.commandReceiver = new CommandReceiverImpl();
      data = initialData.DeepCopy();
    }

    public ICommandReceiver CommandReceiver
    {
      get { return commandReceiver; }
    }

    internal CommandReceiverImpl CommandReceiverInternal
    {
      get { return commandReceiver; }
    }

    public event global::System.Action<Update> ComponentUpdated
    {
      add { componentUpdateCallbacks.Add(value); }
      remove { componentUpdateCallbacks.Remove(value); }
    }

    event global::System.Action<bool> Reader.AuthorityChanged
    {
      add { authorityChangeCallbacks.Add(value); }
      remove { authorityChangeCallbacks.Remove(value); }
    }

    public bool HasAuthority
    {
      get { return hasAuthority; }
    }

    public global::Improbable.EntityAclData Data
    {
      get { return data.Value; }
    }

    public void Send(Update update)
    {
      connection.SendComponentUpdate(entityId, update);
    }

    internal void On_AuthorityChange(bool newAuthority)
    {
      hasAuthority = newAuthority;
      for (int i = 0; i < authorityChangeCallbacks.Count; ++i)
      {
        authorityChangeCallbacks[i](newAuthority);
      }
    }

    internal void On_ComponentUpdate(Update update)
    {
      update.ApplyTo(data);
      for (int i = 0; i < componentUpdateCallbacks.Count; ++i)
      {
        componentUpdateCallbacks[i](update);
      }
    }

    public global::Improbable.EntityId EntityId
    {
      get { return entityId; }
    }

    // Implementation of deprecated interface.
    //----------------------------------------

    private EntityAcl.Update currentUpdate = null;

    public Updater Update
    {
      get
      {
        if (currentUpdate == null)
        {
          currentUpdate = new EntityAcl.Update();
        }
        else
        {
          global::Improbable.Worker.ClientError.LogClientException(new global::System.InvalidOperationException(
              "Update for component EntityAcl called with an update already in-flight! " +
              "Each Update must be followed by a FinishAndSend() before another update is made."));
        }
        return this;
      }
    }

    public void FinishAndSend()
    {
      if (currentUpdate != null)
      {
        if (FinishAndSend_ResolveDiff(currentUpdate)) {
          On_ComponentUpdate(currentUpdate);
          connection.SendComponentUpdate(entityId, currentUpdate, /* legacy semantics */ true);
        }
        currentUpdate = null;
      }
      else
      {
        global::Improbable.Worker.ClientError.LogClientException(new global::System.InvalidOperationException(
            "FinishAndSend() for component EntityAcl called with no update in-flight!"));
      }
    }

    public bool IsAuthoritativeHere
    {
      get { return HasAuthority; }
    }

    private readonly global::Improbable.Entity.Component.DeprecatedEvent<global::System.Action<bool>, global::System.Action<bool>> authorityChangedWrapper =
        new global::Improbable.Entity.Component.DeprecatedEvent<global::System.Action<bool>, global::System.Action<bool>>((x) => x);

    event global::System.Action<bool> EntityAclReader.AuthorityChanged
    {
      add { ((Reader) this).AuthorityChanged += authorityChangedWrapper.Add(value, value.Target); value(HasAuthority); }
      remove { ((Reader) this).AuthorityChanged -= authorityChangedWrapper.Remove(value, value.Target); }
    }

    private readonly global::Improbable.Entity.Component.DeprecatedEvent<global::System.Action<Update>, global::System.Action> propertyUpdatedWrapper =
        new global::Improbable.Entity.Component.DeprecatedEvent<global::System.Action<Update>, global::System.Action>(PropertyUpdated_Wrap);

    private static global::System.Action<Update> PropertyUpdated_Wrap(global::System.Action action)
    {
      return (update) => { action(); };
    }

    public event global::System.Action PropertyUpdated
    {
      add { ComponentUpdated += propertyUpdatedWrapper.Add(value, value.Target); value(); }
      remove { ComponentUpdated -= propertyUpdatedWrapper.Remove(value, value.Target); }
    }

    public global::Improbable.Collections.Option<global::Improbable.WorkerPredicate> Read
    {
      get { return Data.read; }
    }

    private readonly global::Improbable.Entity.Component.DeprecatedEvent<global::System.Action<Update>, global::System.Action<global::Improbable.Collections.Option<global::Improbable.WorkerPredicate>>> readWrapper =
        new global::Improbable.Entity.Component.DeprecatedEvent<global::System.Action<Update>, global::System.Action<global::Improbable.Collections.Option<global::Improbable.WorkerPredicate>>>(ReadUpdated_Wrap);

    private static global::System.Action<Update> ReadUpdated_Wrap(global::System.Action<global::Improbable.Collections.Option<global::Improbable.WorkerPredicate>> action)
    {
      return (update) => { if (update.read.HasValue) { action(update.read.Value); } };
    }

    public event global::System.Action<global::Improbable.Collections.Option<global::Improbable.WorkerPredicate>> ReadUpdated
    {
      add { ComponentUpdated += readWrapper.Add(value, value.Target); value(Data.read); }
      remove { ComponentUpdated -= readWrapper.Remove(value, value.Target); }
    }

    Updater Updater.Read(global::Improbable.Collections.Option<global::Improbable.WorkerPredicate> newValue)
    {
      currentUpdate.SetRead(newValue);
      return this;
    }

    public global::Improbable.Collections.Option<global::Improbable.ComponentAcl> ComponentAcl
    {
      get { return Data.componentAcl; }
    }

    private readonly global::Improbable.Entity.Component.DeprecatedEvent<global::System.Action<Update>, global::System.Action<global::Improbable.Collections.Option<global::Improbable.ComponentAcl>>> componentAclWrapper =
        new global::Improbable.Entity.Component.DeprecatedEvent<global::System.Action<Update>, global::System.Action<global::Improbable.Collections.Option<global::Improbable.ComponentAcl>>>(ComponentAclUpdated_Wrap);

    private static global::System.Action<Update> ComponentAclUpdated_Wrap(global::System.Action<global::Improbable.Collections.Option<global::Improbable.ComponentAcl>> action)
    {
      return (update) => { if (update.componentAcl.HasValue) { action(update.componentAcl.Value); } };
    }

    public event global::System.Action<global::Improbable.Collections.Option<global::Improbable.ComponentAcl>> ComponentAclUpdated
    {
      add { ComponentUpdated += componentAclWrapper.Add(value, value.Target); value(Data.componentAcl); }
      remove { ComponentUpdated -= componentAclWrapper.Remove(value, value.Target); }
    }

    Updater Updater.ComponentAcl(global::Improbable.Collections.Option<global::Improbable.ComponentAcl> newValue)
    {
      currentUpdate.SetComponentAcl(newValue);
      return this;
    }

    private bool FinishAndSend_ResolveDiff(Update update)
    {
      bool isNoOp = true;
      if (update.read.HasValue)
      {
        if (update.read.Value == Data.read)
        {
          update.read.Clear();
        }
        else
        {
          isNoOp = false;
        }
      }
      if (update.componentAcl.HasValue)
      {
        if (update.componentAcl.Value == Data.componentAcl)
        {
          update.componentAcl.Clear();
        }
        else
        {
          isNoOp = false;
        }
      }
      return !isNoOp;
    }

    internal void DisconnectEventHandlers(object actionTarget)
    {
      readWrapper.DisconnectTarget(actionTarget);
      componentAclWrapper.DisconnectTarget(actionTarget);
    }

    internal class CommandReceiverImpl : ICommandReceiver
    {
    }
  }
}

}
