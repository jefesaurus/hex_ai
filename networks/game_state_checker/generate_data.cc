#include "base/init.h"
#include "game/hex_io.h"
#include "mcts/mcts.h"
#include "networks/game_state_checker/data.pb.h"
#include "proto/proto_file.h"

DEFINE_int32(num_trials, 100, "Num games to play out.");
DEFINE_int32(num_iters, 100, "Num tree iterations for each horizontal move.");
DEFINE_string(file_out, "", "Where to store the file with serialized data.");

void GenerateData() {
  // We'll store all of the boards we've seen.
  hex_ai::proto::LabeledHexBoards boards;
  for (int i = 0; i < FLAGS_num_trials; ++i) {
    HexState<11> game;
    while (!game.GameIsOver()) {
      HexTree<11> tree(game);
      const int num_iters = FLAGS_num_iters;
      for (int j = 0; j < num_iters; ++j) {
        tree.RunSimulation();
      }
      const auto best_move = tree.GetBestMove();
      game.MakeMove(best_move.first);

      LOG_EVERY_N(1000, INFO) << i;
      auto* board = boards.add_boards();
      board->mutable_board()->CopyFrom(ToProto<11>(game.Board()));
      board->set_connection_status(ToProto(game.Board().ConnectionStatus()));
    }
  }
  LOG(INFO) << "Num boards: " << boards.boards_size();
  WriteBinaryProto(boards, "/tmp/board_proto.binpb");
}

int main(int argc, char** argv) {
  Init(&argc, &argv);
  GenerateData();
  return 0;
}
