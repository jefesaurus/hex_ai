#pragma once

#include <grpc++/grpc++.h>
#include <random>

#include "base/logging.h"
#include "server/game_protocol.grpc.pb.h"
#include "server/game_protocol.pb.h"

// Logic and data behind the server's behavior.
class HexecutiveImpl final : public hex_service::Hexecutive::Service {
public:
  HexecutiveImpl() : random_generator_(std::random_device()()) {}

private:
  grpc::Status ListGames(grpc::ServerContext *context,
                         const hex_service::ListGamesRequest *request,
                         hex_service::ListGamesReply *reply) override {
    return grpc::Status::OK;
  }

  // Create a new player object
  grpc::Status
  RegisterPlayer(grpc::ServerContext *context,
                 const hex_service::RegisterPlayerRequest *request,
                 hex_service::RegisterPlayerReply *reply) override {
    *(reply->mutable_player()) = NewPlayer(request->username());
    return grpc::Status::OK;
  }

  hex_service::Player NewPlayer(const std::string &username) {
    uint64_t new_id;
    decltype(active_players_.find(0)) it;
    do {
      new_id = GetRandom();
      it = active_players_.find(new_id);
    } while (it != active_players_.end());
    auto it_bool =
        active_players_.emplace(std::make_pair(new_id, hex_service::Player()));
    auto &new_player = it_bool.first->second;
    new_player.set_username(username);
    new_player.set_id(new_id);
    new_player.set_key(GetRandom());
    return new_player;
  }

  uint64_t GetRandom() { return id_dist_(random_generator_); }

  std::mt19937_64 random_generator_;
  std::uniform_int_distribution<uint64_t> id_dist_;

  std::unordered_map<uint64_t, hex_service::Player> active_players_;
};
