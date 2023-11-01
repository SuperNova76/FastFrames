/**
 * @file Systematic.cc
 * @brief Systematic
 *
 */

#include "FastFrames/Systematic.h"

Systematic::Systematic(const std::string& name) noexcept :
  m_name(name),
  m_isNominal(name == "NOSYS")
{
}