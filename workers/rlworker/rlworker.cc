#include <improbable/worker.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <improbable/general/WorldTransform.h>

#include <improbable/general/WorldVelocity.h>
#include <improbable/player/DroneControls.h>
#include <improbable/math/vector3f.h>
#include <chrono>
#include <thread>
// #include "tensorflow/tensorflow/blackbox/blackbox.cc"

using ::improbable::player::DroneControls;
using ::improbable::general::WorldTransform;
using ::improbable::general::WorldVelocity;
using namespace improbable::math;
using namespace worker;

static const std::string kWorkerType = "RLWorker";
static const std::string kLoggerName = "rlworker.cc";

Entity sphereEntity;

Vector3f intToForce(int action);

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
			
			Vector3f droneVelocity = entity.Get<WorldVelocity>()->velocity();

			// int action = getForce(CoordinatesToVector(droneCoords),
			// 					  Vector3fToVector(droneVelocity),
			// 					  CoordinatesToVector(sphereCoords));

			int action = 1;

			if(action != -1) {
				Vector3f force = intToForce(action);
				DroneControls::Update update = DroneControls::Update();
				update.set_force(force);
				const std::string& logger_name = "cppworker";
				const std::string& message = "update";
				connection.SendComponentUpdate<DroneControls>(entity_id, update);
			}

			else {
				//IF NEED RESET ADD HERE
			}
			
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

}

std::vector<float> Vector3fToVector(Vector3f inputVector) {
	std::vector<float> ans;

	ans.push_back(inputVector.x());
	ans.push_back(inputVector.y());
	ans.push_back(inputVector.z());

	return ans;
}

std::vector<float> CoordinatesToVector(Coordinates inputVector) {
	std::vector<float> ans;

	ans.push_back(inputVector.x());
	ans.push_back(inputVector.y());
	ans.push_back(inputVector.z());

	return ans;
}


Vector3f intToForce(int action) {
	if(action == 0) {
		Vector3f fuck = Vector3f(1.0,0.0,0.0);
		return fuck;
		//fuck = Vector3f((float)1.0, (float)0.0, (float)0.0);
	}
	else if(action == 1) {
		Vector3f fuck = Vector3f(0.0,0.0,1.0);
		return fuck;
		//fuck = Vector3f(0.0, 0.0, 1.0);
	}

	else if(action == 2) {
		Vector3f fuck = Vector3f(0.0,0.0,-1.0);
		return fuck;
		//fuck = Vector3f(0.0, 0.0, -1.0);
	}

	else if(action == 3) {
		Vector3f fuck = Vector3f(-1.0,0.0,0.0);
		return fuck;
		//fuck = Vector3f((float)-1.0, (float)0.0, (float)0,0);
	}
	Vector3f fuck = Vector3f(0.0,0.0,0.0);

	return fuck;
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
  connection.SendInterestedComponents<WorldVelocity>(2);

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