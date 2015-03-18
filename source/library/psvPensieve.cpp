#include "psvPensieve.hpp"

namespace psv
{

Pensieve::Pensieve() = default;

Pensieve::Pensieve(Pensieve const& p_other) = default;

Pensieve::~Pensieve() = default;

Pensieve& Pensieve::operator=(Pensieve const& p_other) = default;

std::vector<Thought> const& Pensieve::GetThoughts() const
{
  return m_thoughts;
}

std::vector<Thought>& Pensieve::GetThoughts()
{
  return m_thoughts;
}

}
