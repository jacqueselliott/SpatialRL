#include <improbable/worker.h>
#include <cstdint>
#include <iostream>
#include <string>

static const std::string kWorkerType = "RLWorker";
static const std::string kLoggerName = "rlworker.cc";

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
  worker::View view;

  std::cout << kWorkerType << " started" << std::endl;


  for (;;) {
     // Get the operations since the last time.
     //auto op_list = connection.GetOpList(0);
     // Process and dispatch.
     //view.Process(op_list);
   }
}