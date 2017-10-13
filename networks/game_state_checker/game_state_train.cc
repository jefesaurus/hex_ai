#include <tiny_dnn/tiny_dnn.h>

#include "base/init.h"
#include "game/hex_io.h"
#include "networks/game_state_checker/data.pb.h"
#include "proto/proto_file.h"

DEFINE_string(file_in, "/tmp/board_proto.binpb", "File with text protos.");

void ConvertData(const hex_ai::proto::LabeledHexBoard& l_board,
                 std::vector<tiny_dnn::label_t>* labels,
                 std::vector<tiny_dnn::vec_t>* samples) {
  const auto& board = l_board.board();
  const auto& label = l_board.connection_status();
  CHECK_EQ(board.size() * board.size(), board.positions().size());
  samples->emplace_back();
  auto& board_image = samples->back();
  board_image.reserve(board.size() * board.size());
  for (const auto& pixel : board.positions()) {
    board_image.push_back(pixel / 2.0);
  }
  labels->push_back(as_underlying(label));
}

void ParseData(const std::string& file_path,
               std::vector<tiny_dnn::label_t>* labels,
               std::vector<tiny_dnn::vec_t>* samples) {
  auto maybe_boards =
      ReadBinaryProto<hex_ai::proto::LabeledHexBoards>(file_path);
  CHECK(maybe_boards);
  for (const auto& board : maybe_boards->boards()) {
    ConvertData(board, labels, samples);
  }
}

void ConstructCnn(double learning_rate, const int n_train_epochs,
                  const int n_minibatch,
                  tiny_dnn::core::backend_t backend_type) {
  // input: 32x32x1 (1024 dimensions)  output: 10
  tiny_dnn::network<tiny_dnn::sequential> net;

  constexpr int kBoardSize = 11;

  net << tiny_dnn::layers::conv(kBoardSize, kBoardSize, 5, 1, 6,
                                tiny_dnn::padding::valid, true, 1, 1,
                                backend_type)
      << tiny_dnn::activation::tanh()
      << tiny_dnn::layers::ave_pool(7, 7, 6 /* in channels */,
                                    1 /* pool size */)
      << tiny_dnn::activation::tanh()
      << tiny_dnn::layers::fc(7 * 7 * 6, 3, true,
                              backend_type)  // F6, 120-in, 10-out
      << tiny_dnn::activation::tanh();

  CHECK_EQ(net.in_data_size(), kBoardSize * kBoardSize);
  CHECK_EQ(net.out_data_size(), 3);

  std::vector<tiny_dnn::label_t> all_labels;
  std::vector<tiny_dnn::vec_t> all_images;
  ParseData(FLAGS_file_in, &all_labels, &all_images);

  const int num_data = all_labels.size();
  constexpr double test_ratio = .2;
  const int num_train = num_data - test_ratio * num_data;

  std::vector<tiny_dnn::label_t> train_labels(all_labels.begin(),
                                              all_labels.begin() + num_train);
  std::vector<tiny_dnn::vec_t> train_images(all_images.begin(),
                                            all_images.begin() + num_train);

  std::vector<tiny_dnn::label_t> test_labels(all_labels.begin() + num_train,
                                             all_labels.end());
  std::vector<tiny_dnn::vec_t> test_images(all_images.begin() + num_train,
                                           all_images.end());

  LOG(INFO) << "Num training samples: " << num_train;
  LOG(INFO) << "Num testing samples: " << test_labels.size();

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
