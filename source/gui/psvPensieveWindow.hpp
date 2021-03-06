#ifndef PSV_PENSIEVEWINDOW_HPP_20150321115845
#define PSV_PENSIEVEWINDOW_HPP_20150321115845

#include "psvPensieveWidget.hpp"
#include "psvLogDialog.hpp"

#include <QLabel>
#include <QStringListModel>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QUrl>

#include <memory>

namespace Ui
{
class PensieveWindow;
}

namespace psv
{

/**
 * Main window of the pensieve GUI
 */
class PensieveWindow:
  public QMainWindow
{
  Q_OBJECT

public:
  /**
   * Construct the window
   * @param p_parent Parent of the new window
   */
  PensieveWindow(QWidget* p_parent = nullptr);

  PensieveWindow(PensieveWindow const&) = delete;

  /**
   * Destruct the pensieve
   */
  ~PensieveWindow();

  PensieveWindow& operator=(PensieveWindow const&) = delete;

protected:
  /**
   * Handle close event to toggle visibility instead of closing the window
   * @param p_event Close event to handle
   */
  void closeEvent(QCloseEvent* p_event) override;

private Q_SLOTS:
  /**
   * Display settings dialog
   */
  void DisplaySettings();

  /**
   * Display log dialog
   */
  void DisplayLog();

  /**
   * Toggle window visibility based on the current status
   */
  void ToggleVisibility();

  /**
   * Display the about dialog
   */
  void About();

  /**
   * Handle systray icon activation
   */
  void SystrayActivated(QSystemTrayIcon::ActivationReason p_reason);

  /**
   * Update systray icon with the number of thoughts
   */
  void UpdateSystrayIcon();

  /**
   * Update network status icon
   * @param p_status New network status
   */
  void UpdateNetworkStatus();

  /**
   * Get data from server
   */
  void DownloadData();

  /**
   * Send data to server
   */
  void UploadData();

  /**
   * Handle end of an HTTP request
   * @param p_reply Network reply that finished
   */
  void EndRequest(QNetworkReply* p_reply);

  /**
   * Start edition mode
   */
  void StartEdition();

  /**
   * End edition mode
   */
  void EndEdition();

private:
  /**
   * Set the read only mode of data
   * @param p_readOnly If it should be read only, editable otherwise
   */
  void SetReadOnly(bool p_readOnly);

  /**
   * Start the update data timer
   *
   * The timer is not started if the update interval is set to 0
   */
  void StartUpdateTimer();

  /**
   * Create a new entry in the log system
   * @param p_message Log message
   */
  void Log(QString const& p_message);

  /// Widget UI
  std::unique_ptr<Ui::PensieveWindow> m_ui;
  /// Main widget
  PensieveWidget m_pensieveWidget;
  /// Systray icon
  QSystemTrayIcon m_systrayIcon;
  /// Network manager
  QNetworkAccessManager m_networkManager;
  /// Server address
  QUrl m_server;
  /// Network status in the status bar
  QLabel m_networkStatusLabel;
  /// Network status update timer
  QTimer m_networkStatusTimer;
  /// Data download timer
  QTimer m_updateDataTimer;
  /// Log model
  QStringListModel m_logModel;
  /// Log dialog
  LogDialog* m_logDialog;
};

}

#endif
