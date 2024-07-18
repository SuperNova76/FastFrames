/**
 * @file ONNXWrapper.h
 * @brief Helper class to evaluate ONNX models
 *
 */
#pragma once


#ifdef ONNXRUNTIME_AVAILABLE

#include "FastFrames/Logger.h"

#include <onnxruntime_cxx_api.h>
#include <vector>

/**
 * @brief Wrapper class to ONNX runtime library
 */
class ONNXWrapper {
public:
  /**
   * @brief Construct a new ONNXWrapper object
   *
   * @param name Name of the model
   * @param filepaths_model_cv paths to the ONNX model files, used for cross-validation
   */
  ONNXWrapper(const std::string& name, const std::vector<std::string>& filepaths_model_cv);

  /**
   * @brief Destroy the ONNXWrapper object
   *
   */
  virtual ~ONNXWrapper() = default;

  class Inference;
  /**
   * @brief Construct and reutrn a new ONNXWrapper::Inference object for a given ONNXWrapper
   *
   * @return ONNXWrapper::Inference
   */
  inline Inference createInferenceInstance() {
    return Inference(*this);
  }

  /**
   * @brief General purpose method to run inference. Don't directly use this method, use the ONNXWrapper::Inference object instead, if possible.
   *
   * @param input_tensors list of inputs
   * @return std::vector<Ort::Value>
   */
  inline std::vector<Ort::Value> evaluate(
    std::vector<Ort::Value>& input_tensors,
    unsigned index_network = 0
  ) {
    Ort::Session& session = *m_sessions[index_network];

    auto output_tensors = session.Run(Ort::RunOptions{nullptr}, m_input_names_cstr.data(), input_tensors.data(), input_tensors.size(), m_output_names_cstr.data(), m_output_names_cstr.size());
    return output_tensors;
  }

  /**
   * @brief Run inference
   *
   * @param infer ONNXWrapper::Inference object created using createInferenceInstance()
   * @param index_network which model file to use for inference, used from cross-validation
   * @return std::vector<Ort::Value>
   */
  inline void evaluate(Inference& infer, unsigned index_network=0) {
    if (&infer.m_onnx != this) {
      LOG(ERROR) << "This ONNXWrapper::Inference instance is not created from this ONNXWrapper object.\n";
      return;
    }
    infer.m_output_tensors = evaluate(infer.m_input_tensors, index_network);
  }

  /**
   * @brief Helper function to get the index used as the second argument of the evaluate(Inference& infer, unsigned index_network=0) method.
   *
   * @param eventNumber eventNumber of the event for which the inference will be run
   * @return unsigned int
   */
  virtual unsigned getSessionIndex(unsigned long long eventNumber) {
    return eventNumber % m_sessions.size();
  }

  /**
   * @brief Class to hold the inputs and outputs of the ONNX model, required for thread-safety
   *
   */
  class Inference {
    public:
      friend ONNXWrapper;
      /**
       * @brief Clear the staged inputs, needed to re-use an ONNXWrapper::Inference object
       *
       */
      void clearInputs() {m_input_tensors.clear();}

      /**
       * @brief Clear the previously calculated outputs
       *
       */
      void clearOutputs() {m_output_tensors.clear();}

      /**
       * @brief Add a new input tensor to the list of the model inputs to use for inference
       *
       * @param T data type of an individual tensor element
       * @param p_data pointer to the tensor data stored as a flat array
       * @param p_data_element_count total number of elements in the input tensor
       * @param shape pointer to an array holding the shape of the input tensor
       * @param shape_len dimension of the input tensor
       */
      template<typename T>
      void addInputs(
        T* p_data, size_t p_data_element_count, const int64_t* shape, size_t shape_len
      ) {
        m_input_tensors.push_back(
          Ort::Value::CreateTensor<T>(m_onnx.m_memory_info, p_data, p_data_element_count, shape, shape_len)
        );
      }

      /**
       * @brief Add a new input tensor to the list of the model inputs to use for inference
       *
       * @param T data type of an individual tensor element
       * @param values tensor data stored as a std::vector
       * @param shape tensor shape stored as a std::vector
       */
      template<typename T>
      void addInputs(
        std::vector<T>& values, const std::vector<int64_t>& shape
      ) {
        m_input_tensors.push_back(
          Ort::Value::CreateTensor<T>(m_onnx.m_memory_info, values.data(), values.size(), shape.data(), shape.size())
        );
      }

      /**
       * @brief Sets the input tensor for a specific input layer to use for inference
       *
       * @param T data type of an individual tensor element
       * @param node_name name of the input layer
       * @param p_data pointer to the tensor data stored as a flat array
       * @param p_data_element_count total number of elements in the input tensor
       * @param shape pointer to an array holding the shape of the input tensor
       * @param shape_len dimension of the input tensor
       */
      template<typename T>
      inline void setInputs(const std::string& node_name, T* p_data, size_t p_data_element_count, const int64_t* shape, size_t shape_len) {
        // resize m_input_tensors in case it is not of the same size as m_input_node_names

        // m_input_tensors.resize(m_input_node_names.size());
        // Does not compile. Ort::Value is not DefaultInsertable.
        // m_input_tensors.resize(m_input_node_names.size(), Ort::Value(nullptr));
        // Does not compile either. Ort::Value is not CopyInsertable.
        for (size_t i = 0; i < m_onnx.m_input_node_names.size()-m_input_tensors.size(); i++) {
          m_input_tensors.emplace_back(nullptr);
        }

        // find the node index from the node name
        try {
          unsigned node = m_onnx.m_input_name_index.at(node_name);
          m_input_tensors[node] = Ort::Value::CreateTensor<T>(m_onnx.m_memory_info, p_data, p_data_element_count, shape, shape_len);
        } catch (std::out_of_range& ex) {
          LOG(ERROR) << "Fail to assign input values to node " << node_name << ": " << ex.what() << "\n";
        }
      }

      /**
       * @brief Sets the input tensor for a specific input layer to use for inference
       *
       @param T data type of an individual tensor element
       * @param values tensor data stored as a std::vector
       * @param shape tensor shape stored as a std::vector
       */
      template<typename T>
      void setInputs(const std::string& node_name, std::vector<T>& values, const std::vector<int64_t>& shape) {
        setInputs(node_name, values.data(), values.size(), shape.data(), shape.size());
      }

      /**
       * @brief Gets the model output after running inference
       *
       * @param T data type of the full output tensor
       * @param node index of the output layer
       * @return pointer to the output tensor
       */
      template<typename T>
      T* getOutputs(unsigned node) {
        return m_output_tensors.at(node).GetTensorMutableData<T>();
      }

      /**
       * @brief Gets the model output after running inference
       *
       * @param T data type of the full output tensor
       * @param node name of the output layer
       * @return pointer to the output tensor
       */
      template<typename T>
      T* getOutputs(const std::string& node_name) {
        try {
          // find the node index from the node name
          unsigned node = m_onnx.m_output_name_index.at(node_name);
          return getOutputs<T>(node);
        } catch (std::out_of_range& ex) {
          LOG(ERROR) << "Fail to retrieve output from " << node_name << ": " << ex.what() << "\n";
          return nullptr;
        }
      }

    protected:
      ONNXWrapper& m_onnx;
      std::vector<Ort::Value> m_input_tensors;
      std::vector<Ort::Value> m_output_tensors;

    private:
      Inference() = delete;
      explicit Inference(ONNXWrapper& onnx): m_onnx(onnx) {};
  };


protected:

  std::string m_model_name;
  // ort
  std::shared_ptr<Ort::Env> m_env;
  std::shared_ptr<Ort::SessionOptions> m_session_options;
  std::vector<std::shared_ptr<Ort::Session>> m_sessions;
  std::vector<std::string> m_input_node_names;
  std::vector<std::string> m_output_node_names;
  std::vector<const char*> m_input_names_cstr;
  std::vector<const char*> m_output_names_cstr;
  std::vector<std::vector<int64_t>> m_input_shapes;
  std::vector<std::vector<int64_t>> m_output_shapes;

  Ort::MemoryInfo m_memory_info; // where to allocate the tensors

  // map from node name to node index
  std::unordered_map<std::string, unsigned> m_input_name_index;
  std::unordered_map<std::string, unsigned> m_output_name_index;
};

#endif // ONNXRUNTIME_AVAILABLE