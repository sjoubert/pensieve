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

private:
  /// Widget UI
  std::unique_ptr<Ui::LogDialog> m_ui;
};

}

#endif
