#include "psvPensieve.hpp"

#include <json/json.h>

#include <sstream>

namespace psv
{

std::string const Pensieve::JSON_THOUGHTS = "thoughts";
std::string const Pensieve::JSON_TITLE = "title";
std::string const Pensieve::JSON_CONTENT = "content";
std::string const Pensieve::JSON_FLAGS = "flags";

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

    jsonThought[JSON_FLAGS] = Json::arrayValue;
    for(auto const& flag: thought.GetFlags())
    {
      jsonThought[JSON_FLAGS].append(flag);
    }

    json[JSON_THOUGHTS].append(jsonThought);
  }

  // Dump JSON
  std::ostringstream out;
  out << json;

  return out.str();
}

void Pensieve::FromJSON(std::string const& p_json)
{
  // Build JSON
  Json::Value json;
  std::istringstream in(p_json);
  in >> json;

  // Read JSON
  GetThoughts().clear();
  for(auto const& jsonThought: json[JSON_THOUGHTS])
  {
    Thought thought;

    thought.SetTitle(jsonThought[JSON_TITLE].asString());
    thought.SetContent(jsonThought[JSON_CONTENT].asString());

    for(auto const& jsonFlag: jsonThought[JSON_FLAGS])
    {
      thought.AddFlag(jsonFlag.asString());
    }

    GetThoughts().push_back(thought);
  }
}

}
