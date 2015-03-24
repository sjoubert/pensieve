#ifndef PSV_PENSIEVEWINDOW_HPP_20150321115845
#define PSV_PENSIEVEWINDOW_HPP_20150321115845

#include "psvPensieveModel.hpp"

#include <QMainWindow>
#include <QSystemTrayIcon>

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

private:
  /// Data mode
  PensieveModel m_model;
};

}

#endif
