#include "psvPensieveWindow.hpp"

#include <QApplication>
#include <QCloseEvent>
#include <QHeaderView>
#include <QMenuBar>
#include <QSystemTrayIcon>
#include <QTableView>

namespace psv
{

PensieveWindow::PensieveWindow(QWidget* p_parent):
  QMainWindow(p_parent)
{
  setWindowIcon(QIcon(":/psv/pensieve"));

  auto fileMenu = menuBar()->addMenu(tr("&File"));
  auto toggleAction = fileMenu->addAction(tr("&Toggle visibility"));
  connect(toggleAction, SIGNAL(triggered()), this, SLOT(ToggleVisibility()));
  fileMenu->addSeparator();
  auto quitAction = fileMenu->addAction(tr("&Quit"));
  quitAction->setShortcut(QKeySequence::Quit);
  connect(quitAction, SIGNAL(triggered()),
    QApplication::instance(), SLOT(quit()));

  auto helpMenu = menuBar()->addMenu(tr("&Help"));
  auto aboutQtAction = helpMenu->addAction(tr("&About Qt"));
  connect(aboutQtAction, SIGNAL(triggered()),
    QApplication::instance(), SLOT(aboutQt()));

  auto tableView = new QTableView(this);
  tableView->setModel(&m_model);
  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();
  setCentralWidget(tableView);

  auto systray = new QSystemTrayIcon(windowIcon(), this);
  systray->setContextMenu(fileMenu);
  systray->show();
  connect(systray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    this, SLOT(SystrayActivated(QSystemTrayIcon::ActivationReason)));
}

PensieveWindow::~PensieveWindow() = default;

void PensieveWindow::closeEvent(QCloseEvent* p_event)
{
  ToggleVisibility();
  p_event->ignore();
}

void PensieveWindow::ToggleVisibility()
{
  setHidden(isVisible());
}

void PensieveWindow::SystrayActivated(
  QSystemTrayIcon::ActivationReason p_reason)
{
  if(p_reason == QSystemTrayIcon::Trigger)
  {
    ToggleVisibility();
  }
}

}
