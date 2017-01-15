#include <improbable/worker.h>
#include <cstdint>
#include <iostream>
#include <string>

static const std::string kWorkerType = "RLWorker";
static const std::string kLoggerName = "rlworker.cc";

int main(int argc, char** argv) {
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " <hostname> <port> <worker id>" << std::endl;
    std::cout << "For example: " << argv[0] << " localhost 7777 RLWorker0" << std::endl;
    return 1;
  }

  const std::string hostname = argv[1];
  const std::uint16_t port = static_cast<std::uint16_t>(std::stoul(argv[2]));
  const std::string worker_id = argv[3];

  worker::ConnectionParameters params;
  params.WorkerType = kWorkerType;
  params.WorkerId = worker_id;
  params.Network.UseExternalIp = true;
  params.Network.Tcp.MultiplexLevel = 4;
  worker::Connection connection{hostname, port, params};

  worker::View view;

  for (;;) {
    // Get the operations since the last time.
    auto op_list = connection.GetOpList(0);
    // Process and dispatch.
    view.Process(op_list);
  }
}