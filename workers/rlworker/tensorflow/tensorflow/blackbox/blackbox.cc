#include <fstream>
#include <vector>
#include <random>

#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/stringpiece.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/command_line_flags.h"

// These are all common classes it's handy to reference with no namespace.
using tensorflow::Flag;
using tensorflow::TensorShape;
using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;
using tensorflow::int32;
using namespace tensorflow;

Status LoadGraph(string graph_file_name,
                 std::unique_ptr<tensorflow::Session>* session) 
{
  tensorflow::GraphDef graph_def;
  Status load_graph_status =
      ReadBinaryProto(tensorflow::Env::Default(), graph_file_name, &graph_def);
  if (!load_graph_status.ok()) 
  {
    return tensorflow::errors::NotFound("Failed to load compute graph at '",
                                        graph_file_name, "'");
  }
  session->reset(tensorflow::NewSession(tensorflow::SessionOptions()));
  Status session_create_status = (*session)->Create(graph_def);
  if (!session_create_status.ok()) 
  {
    return session_create_status;
  }
  return Status::OK();
}

Tensor convert_to_tensor(std::vector<float>pos)
{
  Tensor input(DT_FLOAT, TensorShape({sizeof(pos)}));
  std::copy_n(pos.begin(), pos.size(), input.flat<float>().data());
  return input;
}

int getForce(std::vector<float> drone_pos, std::vector<float> drone_velocity, std::vector<float> goal_pos) {
  std::vector<float> inputTensor;

  inputTensor.push_back(drone_pos[0]);
  inputTensor.push_back(drone_pos[2]);
  inputTensor.push_back(drone_velocity[0]);
  inputTensor.push_back(drone_velocity[2]);
  inputTensor.push_back(goal_pos[0]);
  inputTensor.push_back(goal_pos[2]);

  //SORT OUT GRAPH HERE ====
  string root_dir = "";
  string graph = "../../bazel-bin/tensorflow/blackbox/models/graph.pb";
  string input_layer = "input_state";
  string output = "predict";
  //======

  std::unique_ptr<tensorflow::Session> session;
  string graph_path = tensorflow::io::JoinPath(root_dir, graph);
  Status load_graph_status = LoadGraph(graph_path, &session);
  if (!load_graph_status.ok()) {
    LOG(ERROR) << load_graph_status;
    return -1;
  }

  double p = ((double) rand() / (RAND_MAX)) + 1;
  std::vector<std::pair<string, Tensor>> inputs = 
  {
    { input_layer, convert_to_tensor(inputTensor)}
  };

  if (p > 0.05)
  {
    // Actually run the state through the model.
    std::vector<Tensor> outputs;
    Status run_status = session->Run({inputs},
                                     {output}, {}, &outputs);
    if (!run_status.ok()) 
    {
      LOG(ERROR) << "Running model failed: " << run_status;
      return -1;
    }

    session->Close();

    auto taken_action = outputs[0].scalar<float>()(0);
    return (int) taken_action;
  }
  else
  {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 3); // define the range
    return (int) distr(eng);
  }
}

int main() {
  return 0;
}

