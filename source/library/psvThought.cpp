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

void Thought::AddTag(std::string const& p_tag)
{
  m_tags.insert(p_tag);
}

void Thought::RemoveTag(std::string const& p_tag)
{
  m_tags.erase(p_tag);
}

std::set<std::string> const& Thought::GetTags() const
{
  return m_tags;
}

}
