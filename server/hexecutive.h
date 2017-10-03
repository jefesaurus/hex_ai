#include <grpc++/grpc++.h>

#include "server/game_protocol.grpc.pb.h"
#include "server/game_protocol.pb.h"

// Logic and data behind the server's behavior.
class HexecutiveImpl final : public hex_service::Hexecutive::Service {
  grpc::Status ListGames(grpc::ServerContext* context,
                         const hex_service::ListGamesRequest* request,
                         hex_service::ListGamesReply* reply) override {
    return grpc::Status::OK;
  }
};
