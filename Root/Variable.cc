/**
 * @file Variable.cc
 * @brief Variable
 *
 */

#include "FastFrames/Variable.h"

Variable::Variable(const std::string& name) noexcept :
  m_name(name),
  m_definition(""),
  m_title("title"),
  m_binning(Binning())
{
}
