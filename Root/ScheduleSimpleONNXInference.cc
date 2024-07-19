#include "FastFrames/MainFrame.h"

ROOT::RDF::RNode MainFrame::scheduleSimpleONNXInference(ROOT::RDF::RNode node) {
    for (const auto& infer : m_config->simpleONNXInferences()) {
        std::vector<std::string> inputColumns = infer->getInputColumnNames();
        for (const auto& output: infer->getOutputMap()) {
            auto inferenceFunc = [infer, output](unsigned long long eventNumber, const std::vector<float> &inputValues) {
                std::map<std::string, std::vector<float>> inputs;
                size_t inputColumnsIndex = 0;
                for (const auto& input : infer->getInputMap()) {
                    std::string inputName = input.first;
                    size_t nVars = input.second.size();
                    inputs.insert({inputName, std::vector<float>(inputValues.begin() + inputColumnsIndex, inputValues.begin() + inputColumnsIndex + nVars)});
                    inputColumnsIndex += nVars;
                }
                return infer->runInference(output.first, eventNumber, inputs);
            };
            const std::string intermediateColumn = infer->name() + output.first + "_NOSYS";
            switch (inputColumns.size()-1) {
                case 1: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<1>(inferenceFunc), inputColumns); break;
                case 2: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<2>(inferenceFunc), inputColumns); break;
                case 3: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<3>(inferenceFunc), inputColumns); break;
                case 4: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<4>(inferenceFunc), inputColumns); break;
                case 5: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<5>(inferenceFunc), inputColumns); break;
                case 6: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<6>(inferenceFunc), inputColumns); break;
                case 7: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<7>(inferenceFunc), inputColumns); break;
                case 8: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<8>(inferenceFunc), inputColumns); break;
                case 9: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<9>(inferenceFunc), inputColumns); break;
                case 10: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<10>(inferenceFunc), inputColumns); break;
                case 11: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<11>(inferenceFunc), inputColumns); break;
                case 12: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<12>(inferenceFunc), inputColumns); break;
                case 13: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<13>(inferenceFunc), inputColumns); break;
                case 14: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<14>(inferenceFunc), inputColumns); break;
                case 15: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<15>(inferenceFunc), inputColumns); break;
                case 16: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<16>(inferenceFunc), inputColumns); break;
                case 17: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<17>(inferenceFunc), inputColumns); break;
                case 18: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<18>(inferenceFunc), inputColumns); break;
                case 19: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<19>(inferenceFunc), inputColumns); break;
                case 20: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<20>(inferenceFunc), inputColumns); break;
                case 21: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<21>(inferenceFunc), inputColumns); break;
                case 22: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<22>(inferenceFunc), inputColumns); break;
                case 23: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<23>(inferenceFunc), inputColumns); break;
                case 24: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<24>(inferenceFunc), inputColumns); break;
                case 25: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<25>(inferenceFunc), inputColumns); break;
                case 26: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<26>(inferenceFunc), inputColumns); break;
                case 27: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<27>(inferenceFunc), inputColumns); break;
                case 28: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<28>(inferenceFunc), inputColumns); break;
                case 29: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<29>(inferenceFunc), inputColumns); break;
                case 30: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<30>(inferenceFunc), inputColumns); break;
                case 31: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<31>(inferenceFunc), inputColumns); break;
                case 32: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<32>(inferenceFunc), inputColumns); break;
                case 33: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<33>(inferenceFunc), inputColumns); break;
                case 34: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<34>(inferenceFunc), inputColumns); break;
                case 35: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<35>(inferenceFunc), inputColumns); break;
                case 36: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<36>(inferenceFunc), inputColumns); break;
                case 37: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<37>(inferenceFunc), inputColumns); break;
                case 38: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<38>(inferenceFunc), inputColumns); break;
                case 39: node = this->systematicDefine(node, intermediateColumn, SimpleONNXInference::variadicDispatch<39>(inferenceFunc), inputColumns); break;
                default: throw std::invalid_argument("Number of model inputs has to be within 1-39");
            }
            size_t outputIndex = 0;
            for (const std::string &outputColumnName: output.second) {
                if (outputColumnName != "") {
                    auto vecToScalarBranchesFunc = [outputIndex](const std::vector<float> &vec) {return vec[outputIndex];};
                    node = this->systematicDefine(node, outputColumnName, vecToScalarBranchesFunc, {intermediateColumn});
                }
                outputIndex++;
            }
        }
    }
    return node;
}
