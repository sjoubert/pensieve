#ifndef PSV_THOUGHTWIDGET_HPP_20150325165657
#define PSV_THOUGHTWIDGET_HPP_20150325165657

#include "psvThought.hpp"

#include <QWidget>

#include <memory>

class QMenu;

namespace Ui
{
class ThoughtWidget;
}

namespace psv
{

/**
 * Widget of a thought
 */
class ThoughtWidget:
  public QWidget
{
  Q_OBJECT

public:
  /**
   * Construct the widget
   * @param p_parent Parent widget
   */
  ThoughtWidget(QWidget* p_parent = nullptr);

  ThoughtWidget(ThoughtWidget const&) = delete;

  /**
   * Destruct the widget
   */
  ~ThoughtWidget();

  ThoughtWidget& operator=(ThoughtWidget const&) = delete;

  /**
   * Get the underlying thought data
   * @return Thought data
   */
  Thought GetThought() const;

  /**
   * Set the thought data to use
   * @param p_thought Thought data
   */
  void SetThought(Thought const& p_thought);

  /**
   * Set the highlighted status of the thought
   * @param p_highlighted New highlighted status
   */
  void SetHighlighted(bool p_highlighted);

Q_SIGNALS:
  /**
   * Emitted when the user request the thought deletion
   */
  void DeleteRequested() const;

  /**
   * Emitted when edition mode is entered
   */
  void EditionStarted() const;

  /**
   * Emitted when edition mode is left
   */
  void EditionEnded() const;

private Q_SLOTS:
  /**
   * Set the edit mode and display the correct widgets for the mode
   * @param p_editMode Set the widget in edit mode if true, display if false
   */
  void SetEditMode(bool p_editMode = true);

  /**
   * Set the display mode and display the correct widgets for the mode
   * @param p_displayMode Set the widget in display mode if true, edit if false
   */
  void SetDisplayMode(bool p_displayMode = true);

  /**
   * Display custom context menu for the flags widget
   * @param p_position Requested position of the menu
   */
  void DisplayContextMenu(QPoint const& p_position);

  /**
   * Add a new flag to the flag list
   */
  void AddFlag();

  /**
   * Remove the selected flags from the flag list
   */
  void RemoveFlags();

private:
  /// Widget UI
  std::unique_ptr<Ui::ThoughtWidget> m_ui;
  /// Flags context menu
  QMenu* m_contextMenu;
};

}

#endif
