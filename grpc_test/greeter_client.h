#pragma once

#include <grpc++/grpc++.h>
#include <memory>
#include <string>

#include "grpc_test/helloworld.grpc.pb.h"
#include "grpc_test/helloworld.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel);
  // Assambles the client's payload, sends it and presents the
  // response back from the server.
  std::string SayHello(const std::string& user);

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};
