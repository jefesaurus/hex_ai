#include "server/hexecutive_client.h"

// Constructor with "initialization list"
HexClient::HexClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(hex_service::Hexecutive::NewStub(channel)) {}

optional<hex_service::Player> HexClient::ListGames(const std::string &user) {
  // Data we are sending to the server.
  hex_service::RegisterUserRequest request;

  // Container for the data we expect from the server.
  hex_service::RegisterUserReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  grpc::ClientContext context;

  // The actual RPC.
  grpc::Status status = stub_->SayHello(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return reply.message();
  } else {
    return "RPC failed";
  }
}
