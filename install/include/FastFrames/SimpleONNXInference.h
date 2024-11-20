/**
 * @file SimpleONNXInference.h
 * @brief SimpleONNXInference
 *
 */

#pragma once

#ifdef ONNXRUNTIME_AVAILABLE
  #include "FastFrames/ONNXWrapper.h"
#endif

#include "FastFrames/Logger.h"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>

/**
 * @brief Class containing information for inference of a given simple ONNX model
 *
 */
class SimpleONNXInference {
public:

  /**
   * @brief Construct a new SimpleONNXInference object
   *
   * @param name Name of the ONNX model
   */
  explicit SimpleONNXInference(const std::string& name) noexcept;

  /**
   * @brief Deleted default constructor
   *
   */
  SimpleONNXInference()  = delete;

  /**
   * @brief Destroy the SimpleONNXInference object
   *
   */
  ~SimpleONNXInference() = default;

  /**
   * @brief Get the name of the SimpleONNXInference object
   *
   * @return const std::string&
   */
  inline const std::string& name() const {return m_name;}

  /**
   * @brief Sets the paths to the ONNX model files for k-fold inference
   *
   * @param paths List of paths to the ONNX model files
   */
  inline void setModelPaths(const std::vector<std::string>& paths) {
      m_modelPaths = paths;
  }

  /**
   * @brief Returns the list of paths to the ONNX model files
   *
   * @return List of paths to the ONNX model files
   */
  inline const std::vector<std::string>& modelPaths() const {
      return m_modelPaths;
  }

  /**
   * @brief Records the names fo the input columns corresponding to an input layer
   *
   * @param layer_name Name of the input layer
   * @param branch_names Names of the input columns
   */
  void addInput(const std::string& layer_name, const std::vector<std::string>& branch_names) {
      m_inputs.insert({layer_name, branch_names});
  }

  /**
   * @brief Records the names fo the output columns corresponding to an output layer
   *
   * @param layer_name Name of the output layer
   * @param branch_names Names of the output columns
   */
  void addOutput(const std::string& layer_name, const std::vector<std::string>& branch_names) {
      m_outputs.insert({layer_name, branch_names});
  }

  /**
   * @brief Sets the fold-formula for k-fold inference
   *
   * @note Not yet implemented
   */
  inline void setFoldFormula(const std::string& foldFormula) {
      if (foldFormula != "") {
          throw std::runtime_error("Custom fold_formula is not yet supported. It will be implemented later, if necessary.");
      }
      // m_foldFormula = foldFormula;
  }

  /**
   * @brief Initialize the underlying ONNXWrapper object with the given model paths
   *
   */
  inline void initializeModels() {
    #ifdef ONNXRUNTIME_AVAILABLE
      m_onnx =  std::make_unique<ONNXWrapper>(m_name, m_modelPaths);
    #else
      LOG(WARNING) << "You are trying to use ONNX wrapper, but the onnxruntime is not available. Please rerun cmake and recompile with onnxruntime.\n";
      return;
    #endif
  }

  /**
   * @brief Runs k-fold inference
   *
   * @param outputLayer Name of the output layer
   * @param eventNumber Event number of the corresponding event, used for k-fold inference
   * @param inputs Map of input layer names and their corresponding input values
   * @return Output values from the specified the outputLayer
   */
  std::vector<float> runInference(const std::string& outputLayer, const unsigned long long& eventNumber, const std::map<std::string, std::vector<float>>& inputs)   const;

  inline std::vector<std::string> getInputColumnNames() {
      std::vector<std::string> columnNames{"eventNumber"};
      for (const auto &[input_name, branches]: m_inputs) {
          for (const std::string &branch: branches) {
              columnNames.push_back(branch);
          }
      }
      return columnNames;
  }

  /**
   * @brief Returns the map of input layer names and their corresponding input columns
   *
   * @return Map of input layer names and their corresponding input columns
   */
  inline const std::map<std::string, std::vector<std::string>>& getInputMap() {
      return m_inputs;
  }

  /**
   * @brief Returns the map of output layer names and their corresponding output columns
   *
   * @return Map of output layer names and their corresponding output columns
   */
  inline const std::map<std::string, std::vector<std::string>>& getOutputMap() {
      return m_outputs;
  }


  /**
   * @brief Helper class to dispatch one uint64_t and multiple float arguments to a function
   *
   * @note Use the variadicDispatch<N>(f) function to create an object of this class
   */
  template <typename I, typename F>
  class VariadicDispatcher;

  template <std::size_t... N, typename F>
  class VariadicDispatcher<std::index_sequence<N...>, F> {
    template <std::size_t>
    using RepeatedT = float;
    std::decay_t<F> fFunc;

  public:
    explicit VariadicDispatcher(F &&f) : fFunc(f) {}
    auto operator()(unsigned long long eventNumber, RepeatedT<N>... args) { return fFunc(eventNumber, {args...}); }
  };

  /**
   * @brief Helper function to dispatch one uint64_t and multiple float arguments to a function
   *
   * @tparam N Number of float arguments
   * @tparam F Function type
   * @param f Function to dispatch the arguments to
   * @return VariadicDispatcher object
   */
  template <std::size_t N, typename F>
  static auto variadicDispatch(F &&f)
  {
    return VariadicDispatcher<std::make_index_sequence<N>, F>(f);
  }

private:

  std::string m_name;
  std::vector<std::string> m_modelPaths;
  std::map<std::string, std::vector<std::string>> m_inputs;
  std::map<std::string, std::vector<std::string>> m_outputs;
  std::string m_foldFormula;
  #ifdef ONNXRUNTIME_AVAILABLE
    std::unique_ptr<ONNXWrapper> m_onnx;
  #endif
};
