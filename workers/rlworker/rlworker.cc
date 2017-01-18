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
using ::improbable::math::Vector3f;


static const std::string kWorkerType = "RLWorker";
static const std::string kLoggerName = "rlworker.cc";

void RunEventLoop(worker::Connection& connection, worker::Dispatcher& dispatcher) {
	static const unsigned kFramesPerSecond = 60;

	auto time = std::chrono::high_resolution_clock::now();
	while (true) {
		auto op_list = connection.GetOpList(0 /* non-blocking */);

		// Invoke user-provided callbacks.
		dispatcher.Process(op_list);

		// Do other work here...

		time += std::chrono::microseconds(1000000 / kFramesPerSecond);
		std::this_thread::sleep_until(time);

		float x = 1;
		float y = 0;
		float z = 0;
		
		Vector3f direction = Vector3f(x, y, z);
		DroneControls::Update update;
		update.set_force(direction);
		connection.SendComponentUpdate<DroneControls>(2, update);	
	}
}


int main(int argc, char** argv) {
  if (argc < 2 || 3 < argc) {
    return argc;
  }

  worker::ConnectionParameters parameters;
  parameters.WorkerType = "rlworker";
  parameters.WorkerId = argv[1];
  parameters.Network.ConnectionType = worker::NetworkConnectionType::kTcp;
  parameters.Network.UseExternalIp = false;

  std::string hostname = "localhost";
  if (argc == 3) {
    hostname = argv[2];
  }

  worker::Connection connection = worker::Connection(hostname, 7777, parameters);
  worker::View view = worker::View();

  worker::Dispatcher dispatcher = worker::Dispatcher();

  std::cout << kWorkerType << " started" << std::endl;
  //TODO: continue and work out the dispatcher
  for (;;) {
	  RunEventLoop(connection, dispatcher);
  }
}