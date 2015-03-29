#ifndef PSV_SETTINGSDIALOG_HPP_20150329214117
#define PSV_SETTINGSDIALOG_HPP_20150329214117

#include <QDialog>

#include <memory>

namespace Ui
{
class SettingsDialog;
}

namespace psv
{

/**
 * Dialog allowing to edit application settings
 */
class SettingsDialog:
  public QDialog
{
  Q_OBJECT

public:
  /**
   * Construct the widget
   * @param p_parent Parent widget
   */
  SettingsDialog(QWidget* p_parent = nullptr);

  SettingsDialog(SettingsDialog const&) = delete;

  /**
   * Destruct the widget
   */
  ~SettingsDialog();

  SettingsDialog& operator=(SettingsDialog const&) = delete;

  /**
   * Set server address value
   * @param p_server Server address
   */
  void SetServer(QString const& p_server);

  /**
   * Get server address value
   * @return Server address
   */
  QString GetServer() const;

private:
  /// Widget UI
  std::unique_ptr<Ui::SettingsDialog> m_ui;
};

}

#endif
