#include <tiny_dnn/tiny_dnn.h>

#include "base/init.h"
#include "game/hex_io.h"
#include "networks/game_state_checker/data.pb.h"
#include "proto/proto_file.h"

DEFINE_string(file_in, "/tmp/board_proto.binpb", "File with text protos.");

std::unordered_map<tiny_dnn::label_t, std::vector<tiny_dnn::vec_t>> ConvertData(
    const hex_ai::proto::LabeledHexBoards& boards) {
  std::unordered_map<tiny_dnn::label_t, std::vector<tiny_dnn::vec_t>> samples;
  for (const auto& l_board : boards.boards()) {
    const auto& board = l_board.board();
    const auto& label = l_board.connection_status();
    CHECK_EQ(board.size() * board.size(), board.positions().size());
    tiny_dnn::vec_t normalized_board;
    normalized_board.reserve(board.size() * board.size());
    for (const auto& pixel : board.positions()) {
      normalized_board.push_back(pixel / 2.0);
    }
    samples[as_underlying(label)].emplace_back(std::move(normalized_board));
  }
  return samples;
}

void ParseData(const std::string& file_path, double test_ratio,
               std::vector<tiny_dnn::label_t>* train_labels,
               std::vector<tiny_dnn::vec_t>* train_images,
               std::vector<tiny_dnn::label_t>* test_labels,
               std::vector<tiny_dnn::vec_t>* test_images) {
  auto maybe_boards =
      ReadBinaryProto<hex_ai::proto::LabeledHexBoards>(file_path);
  CHECK(maybe_boards);
  auto samples = ConvertData(*maybe_boards);

  // We want the same number of each kind.
  int min_num = std::numeric_limits<int>::max();
  for (const auto& key_boards : samples) {
    min_num = std::min(static_cast<int>(key_boards.second.size()), min_num);
  }

  // And partition into train and test data.
  int train_num = static_cast<int>((1.0 - test_ratio) * min_num);
  for (const auto& key_boards : samples) {
    const auto& key = key_boards.first;
    const auto& boards = key_boards.second;

    for (int i = 0; i < train_num; ++i) {
      train_labels->push_back(key);
      train_images->push_back(boards[i]);
    }

    for (int i = train_num; i < min_num; ++i) {
      test_labels->push_back(key);
      test_images->push_back(boards[i]);
    }
  }
}

void ConstructCnn(double learning_rate, const int n_train_epochs,
                  const int n_minibatch,
                  tiny_dnn::core::backend_t backend_type) {
  tiny_dnn::network<tiny_dnn::sequential> net;

  constexpr int kBoardSize = 11;

  net << tiny_dnn::layers::conv(
             kBoardSize, kBoardSize, 5, 1, 6,  // C1, 1@32x32-in, 6@28x28-out
             tiny_dnn::padding::valid, true, 1, 1, backend_type)
      << tiny_dnn::activation::tanh()
      << tiny_dnn::layers::conv(7, 7, 3, 6, 16, tiny_dnn::padding::valid, true,
                                1, 1, backend_type)
      << tiny_dnn::activation::tanh()
      << tiny_dnn::layers::fc(5 * 5 * 16, 3, true,
                              backend_type)  // F6, 120-in, 10-out
      << tiny_dnn::activation::tanh();

  CHECK_EQ(net.in_data_size(), kBoardSize * kBoardSize);
  CHECK_EQ(net.out_data_size(), 3);

  constexpr double test_ratio = 1.0 / 6.0;
  std::vector<tiny_dnn::label_t> train_labels;
  std::vector<tiny_dnn::vec_t> train_images;
  std::vector<tiny_dnn::label_t> test_labels;
  std::vector<tiny_dnn::vec_t> test_images;
  ParseData(FLAGS_file_in, test_ratio, &train_labels, &train_images,
            &test_labels, &test_images);

  LOG(INFO) << train_labels.size() << ", " << test_labels.size();

  // declare optimization algorithm
  tiny_dnn::adagrad optimizer;

  tiny_dnn::progress_display disp(train_images.size());
  tiny_dnn::timer t;

  optimizer.alpha *= std::min(
      tiny_dnn::float_t(4),
      static_cast<tiny_dnn::float_t>(sqrt(n_minibatch) * learning_rate));

  int epoch = 1;
  // create callback
  auto on_enumerate_epoch = [&]() {
    LOG(INFO) << "Epoch " << epoch << "/" << n_train_epochs << " finished. "
              << t.elapsed() << "s elapsed.";
    ++epoch;
    tiny_dnn::result res = net.test(test_images, test_labels);
    LOG(INFO) << res.num_success << "/" << res.num_total;
    disp.restart(train_images.size());
    t.restart();
  };

  auto on_enumerate_minibatch = [&]() { disp += n_minibatch; };

  // training
  net.train<tiny_dnn::mse>(optimizer, train_images, train_labels, n_minibatch,
                           n_train_epochs, on_enumerate_minibatch,
                           on_enumerate_epoch);

  tiny_dnn::result res = net.test(test_images, test_labels);
  LOG(INFO) << res.num_success << "/" << res.num_total;

  // save
  net.save("net");
}

int main(int argc, char** argv) {
  Init(&argc, &argv);
  double learning_rate = 1;
  int epochs = 30;
  int minibatch_size = 16;
  tiny_dnn::core::backend_t backend_type = tiny_dnn::core::default_engine();
  ConstructCnn(learning_rate, epochs, minibatch_size, backend_type);
  return 0;
}
