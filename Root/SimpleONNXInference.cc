/**
 * @file SimpleONNXInference.cc
 * @brief SimpleONNXInference
 *
 */

#include "FastFrames/SimpleONNXInference.h"
#include "FastFrames/Logger.h"

SimpleONNXInference::SimpleONNXInference(const std::string& name) noexcept :
m_name(name)
{
}


#ifdef ONNXRUNTIME_AVAILABLE
    std::vector<float> SimpleONNXInference::runInference(const std::string& outputLayer, const unsigned long long& eventNumber, const std::map<std::string, std::vector<float>>& inputs)    const {
        if (m_onnx == nullptr) {
            throw std::runtime_error("ONNX model not loaded");
        }
        ONNXWrapper::Inference infer = m_onnx->createInferenceInstance();
        std::vector<float> input_vector;
        for (auto input: inputs) {
            input_vector.clear();
            input_vector.insert(input_vector.begin(), input.second.begin(), input.second.end());
            infer.setInputs(input.first, input_vector, {1, static_cast<int64_t>(input_vector.size())});
        }
        unsigned int fold = m_onnx->getSessionIndex(eventNumber);
        m_onnx->evaluate(infer, fold);
        const float *output =  infer.getOutputs<float>(outputLayer);
        std::vector<float> results;
        for (size_t i = 0; i < m_outputs.at(outputLayer).size(); i++) {
            results.push_back(output[i]);
        }
        return results;
    }
#else
    std::vector<float> SimpleONNXInference::runInference(const std::string& outputLayer, [[maybe_unused]] const unsigned long long& eventNumber, [[maybe_unused]] const std::map<std::string, std::vector<float>>& inputs)    const {
        std::vector<float> results;
        for (size_t i = 0; i < m_outputs.at(outputLayer).size(); i++) {
            results.push_back(0);
        }
        return results;
    }
#endif

