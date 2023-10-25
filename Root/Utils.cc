#include "FastFrames/Utils.h"

std::string Utils::replaceString(const std::string& original,
                                 const std::string& from,
                                 const std::string& to) {
    
    std::string result(original);
    std::string::size_type n = 0;
    while ((n = result.find(from, n)) != std::string::npos) {
        result.replace(n, from.size(), to);
        n += to.size();
    }

    return result;
}
  
bool Utils::stringEndsWithString(const std::string& toCheck, const std::string& ending) {
    if (ending.size() > toCheck.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), toCheck.rbegin());
}