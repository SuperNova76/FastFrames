/**
 * @file FastFramesExecutor.h
 * @brief Main executor wrapper
 *
 */

#pragma once

#include <memory>

class ConfigSetting;

/**
 * @brief Wrapper class that calls MainFrame or the inherited class
 *
 */
class FastFramesExecutor {

public:

  /**
   * @brief Construct a new FastFramesExecutor object
   *
   * @param config The config
   */
  explicit FastFramesExecutor(const std::shared_ptr<ConfigSetting>& config) noexcept;

  /**
   * @brief Deleted default constructor
   *
   */
  FastFramesExecutor() = delete;

  /**
   * @brief Destroy the Fast Frames Executor object
   *
   */
  ~FastFramesExecutor() = default;

  /**
   * @brief Main method that does all the work
   *
   */
  void runFastFrames() const;

  /**
   * @brief Set the run ntuples flag
   *
   * @param flag
   */
  void setRunNtuples(const bool flag) {m_runNtuples = flag;}

  /**
   * @brief Run ntuples?
   *
   * @return true
   * @return false
   */
  bool runNtuples() const {return m_runNtuples;}

private:

  std::shared_ptr<ConfigSetting> m_config;
  bool m_runNtuples = false;

};
