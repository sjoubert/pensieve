#ifndef PSV_PENSIEVEWINDOW_HPP_20150321115845
#define PSV_PENSIEVEWINDOW_HPP_20150321115845

#include <QMainWindow>

namespace psv
{

/**
 * Main window of the pensieve GUI
 */
class PensieveWindow:
  public QMainWindow
{
public:
  /**
   * Construct the window
   */
  PensieveWindow();

  PensieveWindow(PensieveWindow const&) = delete;

  /**
   * Destruct the pensieve
   */
  ~PensieveWindow();

  PensieveWindow& operator=(PensieveWindow const&) = delete;
};

}

#endif
