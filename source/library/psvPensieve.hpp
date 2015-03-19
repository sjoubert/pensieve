#ifndef PSV_PENSIEVE_HPP_20150318215604
#define PSV_PENSIEVE_HPP_20150318215604

#include "psvThought.hpp"

#include <vector>

namespace psv
{

/**
 * Container of Thought
 *
 * This is the central class of pensieve, it holds the data in memory
 */
class Pensieve
{
public:
  /**
   * Construct an empty pensieve
   */
  Pensieve();

  /**
   * Copy construct a pensieve from another pensieve
   * @param p_other Source of the copy
   */
  Pensieve(Pensieve const& p_other);

  /**
   * Destruct the pensieve
   */
  ~Pensieve();

  /**
   * Copy assign a pensieve from another pensieve
   * @param p_other Source of the assignment
   * @return A reference to this pensieve
   */
  Pensieve& operator=(Pensieve const& p_other);

  /**
   * Get the thoughts
   * @return Collection of the current thought
   */
  std::vector<Thought> const& GetThoughts() const;

  /**
   * @copydoc GetThoughts()const
   */
  std::vector<Thought>& GetThoughts();

  /**
   * Convert the pensieve into a JSON string
   * @return JSON representation of the current data
   */
  std::string ToJSON() const;

  /**
   * Set pensieve's data from a JSON string
   * @param p_json JSON representation of the data
   */
  void FromJSON(std::string const& p_json);

private:
  /// JSON key for thoughts
  static std::string const JSON_THOUGHTS;
  /// JSON key for title
  static std::string const JSON_TITLE;
  /// JSON key for content
  static std::string const JSON_CONTENT;
  /// JSON key for flags
  static std::string const JSON_FLAGS;

  /// Collection of thoughts
  std::vector<Thought> m_thoughts;
};

}

#endif
