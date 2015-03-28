#ifndef PSV_THOUGHTWIDGET_HPP_20150325165657
#define PSV_THOUGHTWIDGET_HPP_20150325165657

#include "psvThought.hpp"

#include <QWidget>

class QLabel;
class QLineEdit;
class QListWidget;
class QMenu;
class QPlainTextEdit;
class QPushButton;

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
  /// Title edit widget
  QLineEdit* m_title;
  /// Content edit widget
  QPlainTextEdit* m_content;
  /// Flags widget
  QListWidget* m_flags;
  /// Remove button
  QPushButton* m_deleteButton;
  /// Edit button
  QPushButton* m_editButton;
  /// Save button
  QPushButton* m_saveButton;
  /// Flags context menu
  QMenu* m_contextMenu;
  /// Remove flags action of the context menu
  QAction* m_removeFlagsAction;
};

}

#endif
