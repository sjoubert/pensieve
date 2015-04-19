#ifndef PSV_SETTINGSDIALOG_HPP_20150329214117
#define PSV_SETTINGSDIALOG_HPP_20150329214117

#include <QDialog>
#include <QRegExp>

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
   * Settings keys
   */
  struct Settings
  {
    /// Server address
    static QString const SERVER;
    /// Regex filter for tags
    static QString const TAGS_FILTER;
    /// Data download interval
    static QString const UPDATE_INTERVAL;
    /// Start the application hidden in the systray
    static QString const START_HIDDEN;
  };

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

  /**
   * Set the tags filter regex
   * @param p_filter Tags filter
   */
  void SetTagsFilter(QRegExp const& p_filter);

  /**
   * Get the tags filter regex
   * @return Tags filter
   */
  QRegExp GetTagsFilter() const;

  /**
   * Set the update interval
   * @param p_msec Update interval in ms
   */
  void SetUpdateInterval(int p_msec);

  /**
   * Get the update interval
   * @return Update interval in ms
   */
  int GetUpdateInterval() const;

  /**
   * Set start hidden value
   * @param p_startHidden Start hidden value
   */
  void SetStartHidden(bool p_startHidden);

  /**
   * Get the start hidden value
   * @return Start hidden value
   */
  bool GetStartHidden() const;

private:
  /// Widget UI
  std::unique_ptr<Ui::SettingsDialog> m_ui;
};

}

#endif
