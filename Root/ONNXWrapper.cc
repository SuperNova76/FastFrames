/**
 * @file ONNXWrapper.cc
 * @brief Helper class to evaluate ONNX models
 *
 */

#include "FastFrames/ONNXWrapper.h"

#include <numeric>
#include <functional>
#include <sstream>


ONNXWrapper::ONNXWrapper(
  const std::string& name, 
  const std::vector<std::string>& filepaths_model_cv) :
  m_model_name(name), 
  m_env(std::make_shared<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "")), 
  m_session_options(std::make_shared<Ort::SessionOptions>()), 
  m_memory_info(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault))
{
  std::stringstream ss;

  // any session options are set via this object
  // use single thread (single CPU core) for the model evaluation
  m_session_options->SetIntraOpNumThreads(1);
  // ONNX can perform optimizations of the model graph to improve performance (ORT_ENABLE_EXTENDED)
  m_session_options->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

  // create the session and load model into memory
  for (const auto& fpath_model : filepaths_model_cv) {
    auto fp = fpath_model;
    LOG(INFO) << "Load model from " << fp << "\n";
    m_sessions.emplace_back(new Ort::Session(*m_env, fp.c_str(), *m_session_options));
  }

  // retrieve the list of input and output tensor names
  Ort::AllocatorWithDefaultOptions allocator;
  Ort::Session* session = m_sessions.front().get();
  for (size_t inode = 0; inode < session->GetInputCount(); inode++) {
    m_input_name_index[ session->GetInputNameAllocated(inode, allocator).get() ] = inode;
    m_input_node_names.push_back(session->GetInputNameAllocated(inode, allocator).get());
  }
  for (size_t inode = 0; inode < session->GetOutputCount(); inode++) {
    m_output_name_index[ session->GetOutputNameAllocated(inode, allocator).get() ] = inode;
    m_output_node_names.push_back(session->GetOutputNameAllocated(inode, allocator).get());
  }

  m_input_names_cstr.resize(m_input_node_names.size());
  m_output_names_cstr.resize(m_output_node_names.size());
  for (size_t i = 0; i < m_input_node_names.size(); ++i)
    m_input_names_cstr[i] = m_input_node_names[i].c_str();
  for (size_t i = 0; i < m_output_node_names.size(); ++i)
    m_output_names_cstr[i] = m_output_node_names[i].c_str();
  
  ss << "Inputs: ";
  for (long unsigned int i=0; i<m_input_name_index.size(); ++i){
    ss << session->GetInputNameAllocated(i, allocator).get() << " ";
  }
  LOG(DEBUG) << ss.str() << "\n";
  ss.str("");

  ss << "Outputs: ";
  for (long unsigned int i=0; i<m_output_name_index.size(); ++i){
    ss << session->GetOutputNameAllocated(i, allocator).get() << " ";
  }
  LOG(DEBUG) << ss.str() << "\n";
  ss.str("");
  

  // first vector -- the individual input nodes
  // // second vector -- the shape of the input node, e.g. for 1xN shape, the vector has two elements with values {1, N}
  size_t input_node_count = session->GetInputCount();
  m_input_shapes = std::vector<std::vector<int64_t>> (input_node_count);
  for (size_t i = 0; i < input_node_count; i++) m_input_shapes[i] = session->GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();

  ss << "input shapes:";
  LOG(DEBUG) << ss.str() << "\n";
  ss.str("");
  for (long unsigned int i=0; i < m_input_shapes.size(); ++i){
    ss << session->GetInputNameAllocated(i, allocator).get() << ": " << "ndims = " << m_input_shapes[i].size();

    Ort::TypeInfo type_info = m_sessions.front()->GetInputTypeInfo(i);
    auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
    ONNXTensorElementDataType type = tensor_info.GetElementType();
    std::vector<int64_t> dims = tensor_info.GetShape();

    ss << ", " << "type = "<<type << ", shape = [";
    for (long unsigned int j=0; j < dims.size(); ++j){
      ss << dims[j] << " ";
    }
    ss << "]";
    LOG(DEBUG) << ss.str() << "\n";
    ss.str("");
  }

  // output shape
  size_t output_node_count = session->GetOutputCount();
  m_output_shapes = std::vector<std::vector<int64_t>> (output_node_count);
  for (size_t i = 0; i < output_node_count; i++) {
    if (session->GetOutputTypeInfo(i).GetONNXType() == ONNXType::ONNX_TYPE_TENSOR) m_output_shapes[i] = session->GetOutputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();
  }
  
  ss << "output shapes:";
  LOG(DEBUG) << ss.str() << "\n";
  ss.str("");
  for (long unsigned int i=0; i < m_output_shapes.size(); ++i){
    ss << session->GetOutputNameAllocated(i, allocator).get() << ": " << "ndims = " << m_output_shapes[i].size();

    Ort::TypeInfo type_info = m_sessions.front()->GetOutputTypeInfo(i);
    if (type_info.GetONNXType() == ONNXType::ONNX_TYPE_TENSOR) {
      auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
      ONNXTensorElementDataType type = tensor_info.GetElementType(); 
      std::vector<int64_t> dims = tensor_info.GetShape();

      ss << ", " << "type= "<<type << ", shape = [";
      for (long unsigned int j=0; j < dims.size(); ++j){
        ss << dims[j] << " ";
      }
      ss << "]";
    }
    LOG(DEBUG) << ss.str() << "\n";
    ss.str("");
  }

} // ONNXWrapper::ONNXWrapper
