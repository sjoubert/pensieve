#ifndef PSV_LOGDIALOG_HPP_20150412164812
#define PSV_LOGDIALOG_HPP_20150412164812

#include <QDialog>
#include <QAbstractItemModel>

#include <memory>

namespace Ui
{
class LogDialog;
}

namespace psv
{

/**
 * Dialog displaying the application logs
 */
class LogDialog:
  public QDialog
{
  Q_OBJECT

public:
  /**
   * Construct the widget
   * @param p_model Log data model
   * @param p_parent Parent widget
   */
  LogDialog(QAbstractItemModel* p_model, QWidget* p_parent = nullptr);

  LogDialog(LogDialog const&) = delete;

  /**
   * Destruct the widget
   */
  ~LogDialog();

  LogDialog& operator=(LogDialog const&) = delete;

private Q_SLOTS:
  /**
   * Save the scroll bar position
   */
  void SaveScrollBarPosition();

  /**
   * Update the scroll bar position to follow the last log if needed
   *
   * The scroll bar position is only updated if it was already displaying the
   * last log before the last log update, otherwise it is left unchanged.
   */
  void UpdateScrollBarPosition();

private:
  /// Widget UI
  std::unique_ptr<Ui::LogDialog> m_ui;
  /// Was the scroll bar at the maximum position before the last log addition
  bool m_scrollBarAtMaximum;
};

}

#endif
