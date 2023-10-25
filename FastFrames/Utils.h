#pragma once

#include <string>

namespace Utils {

  std::string replaceString(const std::string& original,
                            const std::string& from,
                            const std::string& to);

  bool stringEndsWithString(const std::string& toCheck, const std::string& ending);

};
