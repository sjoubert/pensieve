#include "psvPensieveWindow.hpp"

#include <QApplication>
#include <QHeaderView>
#include <QMenuBar>
#include <QTableView>

namespace psv
{

PensieveWindow::PensieveWindow(QWidget* p_parent):
  QMainWindow(p_parent)
{
  setWindowIcon(QIcon(":/psv/pensieve"));

  auto fileMenu = menuBar()->addMenu(tr("&File"));
  auto quitAction = fileMenu->addAction(tr("&Quit"));
  quitAction->setShortcut(QKeySequence::Quit);
  connect(quitAction, SIGNAL(triggered()),
    QCoreApplication::instance(), SLOT(quit()));

  auto helpMenu = menuBar()->addMenu(tr("&Help"));
  auto aboutQtAction = helpMenu->addAction(tr("&About Qt"));
  connect(aboutQtAction, SIGNAL(triggered()),
    QCoreApplication::instance(), SLOT(aboutQt()));

  auto tableView = new QTableView(this);
  tableView->setModel(&m_model);
  tableView->verticalHeader()->hide();
  tableView->resizeColumnsToContents();
  setCentralWidget(tableView);
}

PensieveWindow::~PensieveWindow() = default;

}
