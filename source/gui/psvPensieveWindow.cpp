#include "psvPensieveWindow.hpp"

#include <QApplication>
#include <QCloseEvent>
#include <QHeaderView>
#include <QMenuBar>
#include <QPainter>
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

  m_systrayIcon.setContextMenu(fileMenu);
  connect(&m_systrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    this, SLOT(SystrayActivated(QSystemTrayIcon::ActivationReason)));
  UpdateSystrayIcon();
  m_systrayIcon.show();

  connect(&m_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
    this, SLOT(UpdateSystrayIcon()));
  connect(&m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
    this, SLOT(UpdateSystrayIcon()));
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

void PensieveWindow::UpdateSystrayIcon()
{
  QPixmap image(":/psv/pensieve");

  if(m_model.rowCount() > 0)
  {
    QPainter painter(&image);
    painter.setPen(Qt::red); // Red

    auto number = QString::number(m_model.rowCount());
    auto newFont = painter.font();
    newFont.setBold(true); // Bold

    // Compute a size that can handle the text
    newFont.setPixelSize(image.height());
    bool foundSize = false;
    while(foundSize == false)
    {
      auto size = QFontMetrics(newFont).size(Qt::TextSingleLine, number);
      size *= 1.25; // Avoid covering the whole image
      if((size.height() <= image.height() && size.width() <= image.height()) ||
        newFont.pixelSize() <= 0)
      {
        foundSize = true;
      }
      else
      {
        newFont.setPixelSize(newFont.pixelSize() - 1);
      }
    }
    painter.setFont(newFont);

    painter.drawText(image.rect(), Qt::AlignHCenter | Qt::AlignBottom, number);
  }

  m_systrayIcon.setIcon(QIcon(image));
}

}
