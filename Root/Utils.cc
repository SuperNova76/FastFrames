/**
 * @file Utils.cc
 * @brief Helper functions
 *
 */

#include "FastFrames/Utils.h"

#include "FastFrames/Logger.h"

std::unique_ptr<TChain> Utils::chainFromFiles(const std::string& treeName,
                                              const std::vector<std::string>& files) {

    std::unique_ptr<TChain> chain = std::make_unique<TChain>(treeName.c_str());

    for (const auto& ifile : files) {
        chain->AddFile(ifile.c_str());
    }

    return chain;
}
