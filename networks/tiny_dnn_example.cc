#define CNN_TASK_SIZE 8
#define CNN_USE_AVX
#include <tiny_dnn/tiny_dnn.h>

#include "base/init.h"

void ConstructCnn(double learning_rate, const int n_train_epochs,
                  const int n_minibatch,
                  tiny_dnn::core::backend_t backend_type) {
// connection table [Y.Lecun, 1998 Table.1]
#define O true
#define X false
  // clang-format off
static const bool tbl[] = {
    O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
    O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
    O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
    X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
    X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
    X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
};
// clang-format on
#undef O
#undef X

  // input: 32x32x1 (1024 dimensions)  output: 10
  tiny_dnn::network<tiny_dnn::sequential> net;

  net << tiny_dnn::layers::conv(32, 32, 5, 1, 6,  // C1, 1@32x32-in, 6@28x28-out
                                tiny_dnn::padding::valid, true, 1, 1,
                                backend_type)
      << tiny_dnn::activation::tanh()
      << tiny_dnn::layers::ave_pool(28, 28, 6,
                                    2)  // S2, 6@28x28-in, 6@14x14-out
      << tiny_dnn::activation::tanh()
      << tiny_dnn::layers::conv(
             14, 14, 5, 6, 16,  // C3, 6@14x14-in, 16@10x10-out
             tiny_dnn::core::connection_table(tbl, 6, 16),
             tiny_dnn::padding::valid, true, 1, 1, backend_type)
      << tiny_dnn::activation::tanh()
      << tiny_dnn::layers::ave_pool(10, 10, 16,
                                    2)  // S4, 16@10x10-in, 16@5x5-out
      << tiny_dnn::activation::tanh()
      << tiny_dnn::layers::conv(5, 5, 5, 16, 120,  // C5, 16@5x5-in, 120@1x1-out
                                tiny_dnn::padding::valid, true, 1, 1,
                                backend_type)
      << tiny_dnn::activation::tanh()
      << tiny_dnn::layers::fc(120, 10, true,
                              backend_type)  // F6, 120-in, 10-out
      << tiny_dnn::activation::tanh();

  CHECK_EQ(net.in_data_size(), 32 * 32);
  CHECK_EQ(net.out_data_size(), 10);

  // load MNIST dataset
  std::vector<tiny_dnn::label_t> train_labels;
  std::vector<tiny_dnn::vec_t> train_images;

  std::vector<tiny_dnn::label_t> test_labels;
  std::vector<tiny_dnn::vec_t> test_images;

  const std::string data_dir = "external/tiny_dnn/data";
  tiny_dnn::parse_mnist_labels(data_dir + "/train-labels.idx1-ubyte",
                               &train_labels);
  tiny_dnn::parse_mnist_images(data_dir + "/train-images.idx3-ubyte",
                               &train_images, -1.0, 1.0, 2, 2);
  tiny_dnn::parse_mnist_labels(data_dir + "/t10k-labels.idx1-ubyte",
                               &test_labels);
  tiny_dnn::parse_mnist_images(data_dir + "/t10k-images.idx3-ubyte",
                               &test_images, -1.0, 1.0, 2, 2);

  // declare optimization algorithm
  tiny_dnn::adagrad optimizer;
#ifdef CNN_SINGLE_THREAD
  LOG(INFO) << "SINGLE THREAD :(";
#endif

  LOG(INFO) << "Start training";

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
