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


bool sphereFound = false;
int foo = 0;
const worker::EntityId& goal_id = 1;

void UpdateEntity(worker::Connection& connection, worker::View& view, const worker::EntityId& entity_id) {

	foo++;
	foo = foo % 7;
	auto& entities = view.Entities;
	if (entities.find(entity_id) == entities.end()) {
		return;
	}
	Entity entity = entities[entity_id];
	if (entity_id == 1) {
		sphereEntity = entities[goal_id];
		sphereFound = true;
	}
	if (entities.find(goal_id) != entities.end()) {
		sphereEntity = entities[goal_id];
		sphereFound = true;
	}

	if (entity_id == 2)
	{
		if (sphereFound) {
			Coordinates droneCoords = entity.Get<WorldTransform>()->position();
			Coordinates sphereCoords = sphereEntity.Get<WorldTransform>()->position();
			Vector3f force = Vector3f((float)sphereCoords.x() - (float)droneCoords.x(), 0.0, (float)sphereCoords.z() - (float)droneCoords.z());
			float norm = sqrt(((float)sphereCoords.x() - (float)droneCoords.x())*((float)sphereCoords.x() - (float)droneCoords.x()) + ((float)sphereCoords.z() - (float)droneCoords.z())*((float)sphereCoords.z() - (float)droneCoords.z()));
			force = Vector3f(((float)sphereCoords.x() - (float)droneCoords.x())/norm, 0.0, ((float)sphereCoords.z() - (float)droneCoords.z())/norm);

			DroneControls::Update update = DroneControls::Update();
			update.set_force(force);
			const std::string& logger_name = "cppworker";
			const std::string& message = "update";
			//connection.SendLogMessage(worker::LogLevel::kError, logger_name, message);
			connection.SendComponentUpdate<DroneControls>(entity_id, update);
		}
		else {
			int decider = foo % 2;
			float xCoord = 2 * decider - 1;
			Vector3f force = Vector3f(xCoord*foo, 0.0, 0.0);
		

			DroneControls::Update update = DroneControls::Update();
			update.set_force(force);
			const std::string& logger_name = "cppworker";
			const std::string& message = "update";
			//connection.SendLogMessage(worker::LogLevel::kError, logger_name, message);
			connection.SendComponentUpdate<DroneControls>(entity_id, update);
			if (entities.find(goal_id) != entities.end()) {
				sphereEntity = entities[goal_id];
				sphereFound = true;
			}
		}

		return;
	}
	/*
	droneEntity = entities[2];
	sphereEntity = entities[1];

	Vector3f force = Vector3f(1.0, 0.0, 0.0);

	Coordinates goal = sphereEntity.Get<WorldTransform>()->position();
	Coordinates current = droneEntity.Get<WorldTransform>()->position();
	//force = Vector3f((float)current.x() - (float)goal.x(), (float)current.y() - (float)goal.y(), (float)current.z() - (float)goal.z());
	DroneControls::Update update = DroneControls::Update().set_force(force);
	connection.SendComponentUpdate<DroneControls>(2, update);
	*/
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
  connection.SendInterestedComponents<WorldTransform>(1);
  connection.SendInterestedComponents<WorldTransform>(2);
  worker::View view;

  view.OnAuthorityChange<DroneControls>([&](const worker::AuthorityChangeOp& op) {
	  if (op.HasAuthority) {
		  UpdateEntity(connection, view, op.EntityId);
	  }
  });
  view.OnComponentUpdate<WorldTransform>([&](const worker::ComponentUpdateOp<WorldTransform>& op) {
	  UpdateEntity(connection, view, op.EntityId);
  });
  view.OnAddEntity([&](const worker::AddEntityOp& op) {
	  UpdateEntity(connection, view, op.EntityId);
  });

  worker::Dispatcher dispatcher = worker::Dispatcher();
 
  std::cout << kWorkerType << " started" << std::endl;
  for (;;) {
	  // Get the operations since the last time.
	  auto op_list = connection.GetOpList(0);
	  // Process and dispatch.
	  view.Process(op_list);
	  //UpdateEntity(connection, view, 2);
  }
}