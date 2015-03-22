#include "psvPensieveWindow.hpp"

#include <QApplication>
#include <QMenuBar>

namespace psv
{

PensieveWindow::PensieveWindow(QWidget* p_parent):
  QMainWindow(p_parent)
{
  setWindowIcon(QIcon(":/psv/pensieve"));

  auto fileMenu = menuBar()->addMenu(tr("&File"));
  auto quitAction = fileMenu->addAction(tr("&Quit"));
  quitAction->setShortcut(QKeySequence::Quit);
  connect(quitAction, &QAction::triggered,
    QCoreApplication::instance(), &QCoreApplication::quit);

  auto helpMenu = menuBar()->addMenu(tr("&Help"));
  auto aboutQtAction = helpMenu->addAction(tr("&About Qt"));
  connect(aboutQtAction, &QAction::triggered, this, QApplication::aboutQt);
}

PensieveWindow::~PensieveWindow() = default;

}
