#ifndef PSV_THOUGHT_HPP_20150318222225
#define PSV_THOUGHT_HPP_20150318222225

#include <string>
#include <set>

namespace psv
{

/**
 * A thought is the central element of a pensieve
 *
 * It has some characteristics such as a title, a content,...
 */
class Thought
{
public:
  /**
   * Construct an empty thought
   */
  Thought();

  /**
   * Copy construct a thought from another thought
   * @param p_other Source of the copy
   */
  Thought(Thought const& p_other);

  /**
   * Destruct the thought
   */
  ~Thought();

  /**
   * Copy assign a thought from another thought
   * @param p_other Source of the assignment
   * @return A reference to this thought
   */
  Thought& operator=(Thought const& p_other);

  /**
   * Set the title
   * @param p_title New title
   */
  void SetTitle(std::string const& p_title);

  /**
   * Get the current title
   * @return Current title value
   */
  std::string const& GetTitle() const;

  /**
   * Set the content
   * @param p_content New content
   */
  void SetContent(std::string const& p_content);

  /**
   * Get the current content
   * @return Current content value
   */
  std::string const& GetContent() const;

  /**
   * Add a tag to the though
   * @param p_tag New tag
   */
  void AddTag(std::string const& p_tag);

  /**
   * Remove a tag from the thought
   * @param p_tag Tag to remove
   */
  void RemoveTag(std::string const& p_tag);

  /**
   * Get the tags
   * @return Collection of the current tags
   */
  std::set<std::string> const& GetTags() const;

private:
  /// Title, destined to be a summary/short text
  std::string m_title;
  /// Content
  std::string m_content;
  /// Collection of associated tags
  std::set<std::string> m_tags;
};

}

#endif
