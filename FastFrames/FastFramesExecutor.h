#pragma once

#include <memory>

class ConfigSetting;

class FastFramesExecutor {

public:

  explicit FastFramesExecutor(const std::shared_ptr<ConfigSetting>& config);

  FastFramesExecutor() = delete;

  ~FastFramesExecutor() = default;

  void runFastFrames() const;

private:

  std::shared_ptr<ConfigSetting> m_config;

};
