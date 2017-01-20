#include <improbable/worker.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <improbable/general/WorldTransform.h>
#include <improbable/player/DroneControls.h>
#include <improbable/math/vector3f.h>
#include <chrono>
#include <thread>

using ::improbable::player::DroneControls;
using ::improbable::general::WorldTransform;
using namespace improbable::math;
using namespace worker;

static const std::string kWorkerType = "RLWorker";
static const std::string kLoggerName = "rlworker.cc";

Entity sphereEntity;
Entity droneEntity;

Coordinates* goal;

void UpdateEntity(worker::Connection& connection, worker::View& view, const worker::EntityId& entity_id) {
	auto& entities = view.Entities;
	if (entities.find(entity_id) == entities.end()) {
		return;
	}
	auto& entity = entities[entity_id];
	if (entity_id != 0 && entity_id != 2) {
		sphereEntity = entity;
		*goal = sphereEntity.Get<WorldTransform>()->position();
	}
	else if (entity_id == 0) {
	}
	else {
		if (goal != nullptr) {
			Coordinates current = entity.Get<WorldTransform>()->position();
			Vector3f force = Vector3f((float)current.x() - (float)goal->x(), (float)current.y() - (float)goal->y(), (float)current.z() - (float)goal->z());
			DroneControls::Update update = DroneControls::Update().set_force(force);
			connection.SendComponentUpdate<DroneControls>(entity_id, update);
		}
	}

	
}




int main(int argc, char** argv) {
  if (argc < 2 || 3 < argc) {
    return argc;
  }

  worker::ConnectionParameters parameters;
  parameters.WorkerType = kWorkerType;
  parameters.WorkerId = argv[1];
  parameters.Network.ConnectionType = worker::NetworkConnectionType::kTcp;
  parameters.Network.UseExternalIp = false;

  std::string hostname = "localhost";
  if (argc == 3) {
    hostname = argv[2];
  }

  worker::Connection connection = worker::Connection(hostname, 7777, parameters);
  worker::View view;

  view.OnAuthorityChange<DroneControls>([&](const worker::AuthorityChangeOp& op) {
	  if (op.HasAuthority) {
		  UpdateEntity(connection, view, op.EntityId);
	  }
  });
  view.OnComponentUpdate<WorldTransform>([&](const worker::ComponentUpdateOp<WorldTransform>& op) {
	  UpdateEntity(connection, view, op.EntityId);
  });

  worker::Dispatcher dispatcher = worker::Dispatcher();
 
  std::cout << kWorkerType << " started" << std::endl;
  for (;;) {
	  // Get the operations since the last time.
	  auto op_list = connection.GetOpList(0);
	  // Process and dispatch.
	  view.Process(op_list);
  }
}