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

//Coordinates goal = Coordinates(2048.0, 0.0, 2048.0);

void UpdateEntity(worker::Connection& connection, worker::View& view, const worker::EntityId& entity_id) {
	auto& entities = view.Entities;
	/*if (entities.find(entity_id) == entities.end()) {
		return;
	}*/
	/*if (entities.find(1) == entities.end()) {
		return;
	}*/
	sphereEntity = entities[1];
	Coordinates goal = sphereEntity.Get<WorldTransform>()->position();

	droneEntity = entities[2];
	Coordinates current = droneEntity.Get<WorldTransform>()->position();
	double x = current.x();
	float fx = (float)x;

	double y = current.y();
	float fy = (float)y;

	double z = current.z();
	float fz = (float)z;

	Vector3f force = Vector3f(0.0, 1.0, 0.0);;

	//Vector3f force = Vector3f(12.12-fx, 0.0, 0.0);

	DroneControls::Update update = DroneControls::Update().set_force(force);
	connection.SendComponentUpdate<DroneControls>(2, update);
	/*force = Vector3f(0.0, 1.0, 0.0);
	update = DroneControls::Update().set_force(force);
	connection.SendComponentUpdate<DroneControls>(2, update);
	*/
	/*auto& entity = entities[entity_id];
	if (entity_id == 2)
	{

		Coordinates current = entity.Get<WorldTransform>()->position();
		//Vector3f force = Vector3f((float)current.x() - (float)goal.x(), (float)current.y() - (float)goal.y(), (float)current.z() - (float)goal.z());
		Vector3f force = Vector3f(1.0, 0.0, 0.0);
		DroneControls::Update update = DroneControls::Update().set_force(force);
		connection.SendComponentUpdate<DroneControls>(2, update);

		if (goal != nullptr) {

			Coordinates current = entity.Get<WorldTransform>()->position();
			Vector3f force = Vector3f((float)current.x() - (float)goal->x(), (float)current.y() - (float)goal->y(), (float)current.z() - (float)goal->z());
			//Vector3f force = Vector3f(1.0, 0.0, 0.0);
			DroneControls::Update update = DroneControls::Update().set_force(force);
			connection.SendComponentUpdate<DroneControls>(2, update);

		}
		else {
			Vector3f force = Vector3f(1.0, 0.0, 0.0);
			DroneControls::Update update = DroneControls::Update().set_force(force);
			connection.SendComponentUpdate<DroneControls>(2, update);
		}
	}


	else if (entity_id == 0) {
	}*/
	/*else {
		if (goal != nullptr) {
			Coordinates current = entity.Get<WorldTransform>()->position();
			Vector3f force = Vector3f((float)current.x() - (float)goal->x(), (float)current.y() - (float)goal->y(), (float)current.z() - (float)goal->z());
			DroneControls::Update update = DroneControls::Update().set_force(force);
			connection.SendComponentUpdate<DroneControls>(entity_id, update);
		}
	}*/


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
	  //view.Process(op_list);
	  UpdateEntity(connection, view, 0);
  }
}