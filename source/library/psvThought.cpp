#include "psvThought.hpp"

namespace psv
{

Thought::Thought() = default;

Thought::Thought(Thought const& p_other) = default;

Thought::~Thought() = default;

Thought& Thought::operator=(Thought const& p_other) = default;

void Thought::SetTitle(std::string const& p_title)
{
  m_title = p_title;
}

std::string const& Thought::GetTitle() const
{
  return m_title;
}

void Thought::SetContent(std::string const& p_content)
{
  m_content = p_content;
}

std::string const& Thought::GetContent() const
{
  return m_content;
}

void Thought::AddFlag(std::string const& p_flag)
{
  m_flags.insert(p_flag);
}

void Thought::RemoveFlag(std::string const& p_flag)
{
  m_flags.erase(p_flag);
}

std::set<std::string> const& Thought::GetFlags() const
{
  return m_flags;
}

}
