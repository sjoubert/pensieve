#include "psvPensieve.hpp"

#include <json/json.h>

#include <sstream>

namespace psv
{

std::string const Pensieve::JSON_THOUGHTS = "thoughts";
std::string const Pensieve::JSON_TITLE = "title";
std::string const Pensieve::JSON_CONTENT = "content";
std::string const Pensieve::JSON_TAGS = "tags";

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

std::string Pensieve::ToJSON() const
{
  // Build JSON
  Json::Value json = Json::objectValue;
  json[JSON_THOUGHTS] = Json::arrayValue;

  for(auto const& thought: GetThoughts())
  {
    Json::Value jsonThought = Json::objectValue;

    jsonThought[JSON_TITLE] = thought.GetTitle();
    jsonThought[JSON_CONTENT] = thought.GetContent();

    jsonThought[JSON_TAGS] = Json::arrayValue;
    for(auto const& tag: thought.GetTags())
    {
      jsonThought[JSON_TAGS].append(tag);
    }

    json[JSON_THOUGHTS].append(jsonThought);
  }

  // Dump JSON
  std::ostringstream out;
  out << json;

  return out.str();
}

bool Pensieve::FromJSON(std::string const& p_json, Pensieve& p_pensieve)
{
  // Build JSON
  Json::Value json;
  std::istringstream in(p_json);
  try
  {
    in >> json;
  }
  catch(...)
  {
    return false;
  }

  // Read JSON
  if(json.isMember(JSON_THOUGHTS) == false)
  {
    return false;
  }
  for(auto const& jsonThought: json[JSON_THOUGHTS])
  {
    if(jsonThought.isMember(JSON_TITLE) &&
      jsonThought.isMember(JSON_CONTENT) &&
      jsonThought.isMember(JSON_TAGS))
    {
      Thought thought;

      thought.SetTitle(jsonThought[JSON_TITLE].asString());
      thought.SetContent(jsonThought[JSON_CONTENT].asString());

      for(auto const& jsonTag: jsonThought[JSON_TAGS])
      {
        thought.AddTag(jsonTag.asString());
      }

      p_pensieve.GetThoughts().push_back(thought);
    }
    else
    {
      return false;
    }
  }

  return true;
}

}
