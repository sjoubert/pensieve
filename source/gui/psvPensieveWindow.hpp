#ifndef PSV_PENSIEVEWINDOW_HPP_20150321115845
#define PSV_PENSIEVEWINDOW_HPP_20150321115845

#include "psvPensieveWidget.hpp"

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QSystemTrayIcon>

class QAction;

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
   * Toggle window visibility based on the current status
   */
  void ToggleVisibility();

  /**
   * Handle systray icon activation
   */
  void SystrayActivated(QSystemTrayIcon::ActivationReason p_reason);

  /**
   * Update systray icon with the number of thoughts
   */
  void UpdateSystrayIcon();

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

private:
  /**
   * Set the read only mode of data
   * @param p_readOnly If it should be read only, editable otherwise
   */
  void SetReadOnly(bool p_readOnly);

  /// Data mode
  PensieveWidget* m_pensieveWidget;
  /// Systray icon
  QSystemTrayIcon m_systrayIcon;
  /// Network manager
  QNetworkAccessManager m_networkManager;
  /// Add thought
  QAction* m_addThoughtAction;
  /// Download data
  QAction* m_downloadDataAction;
  /// Upload data
  QAction* m_uploadDataAction;
};

}

#endif
