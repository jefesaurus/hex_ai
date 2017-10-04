#pragma once

#include <grpc++/grpc++.h>

#include "server/game_protocol.grpc.pb.h"
#include "server/game_protocol.pb.h"

class HexClient {
public:
  HexClient(std::shared_ptr<grpc::Channel> channel);
  std::string ListGames(const std::string &user);

private:
  std::unique_ptr<hex_service::Hexecutive::Stub> stub_;
};
