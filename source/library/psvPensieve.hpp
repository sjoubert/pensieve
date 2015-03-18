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

private:
  /// Collection of thoughts
  std::vector<Thought> m_thoughts;
};

}

#endif
