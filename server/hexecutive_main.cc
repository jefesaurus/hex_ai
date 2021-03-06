#include "base/init.h"
#include "base/logging.h"
#include "server/hexecutive.h"

DEFINE_int32(port, 50051, "Port to run Hexecutive on.");

void RunServer() {
  std::string server_address("0.0.0.0:" + std::to_string(FLAGS_port));
  HexecutiveImpl service;

  grpc::ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  LOG(INFO) << "Server listening on " << server_address;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char **argv) {
  Init(&argc, &argv);
  RunServer();
  return 0;
}
