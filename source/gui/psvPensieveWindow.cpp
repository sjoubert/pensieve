#include "psvPensieveWindow.hpp"

#include "psvPensieve.hpp"

#include <QApplication>
#include <QCloseEvent>
#include <QHeaderView>
#include <QMenuBar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QSystemTrayIcon>
#include <QToolBar>

#include <thread>

namespace psv
{

PensieveWindow::PensieveWindow(QWidget* p_parent):
  QMainWindow(p_parent)
{
  setWindowIcon(QIcon(":/psv/pensieve"));

  m_pensieveWidget = new PensieveWidget;
  setCentralWidget(m_pensieveWidget);

  auto fileMenu = menuBar()->addMenu(tr("&File"));
  m_addThoughtAction = fileMenu->addAction(tr("&Add thought"));
  m_addThoughtAction->setIcon(style()->standardIcon(
    QStyle::SP_FileDialogNewFolder));
  m_addThoughtAction->setShortcut(QKeySequence::New);
  connect(m_addThoughtAction, SIGNAL(triggered()),
    m_pensieveWidget, SLOT(CreateThought()));
  fileMenu->addSeparator();
  m_downloadDataAction = fileMenu->addAction(tr("&Download"));
  m_downloadDataAction->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
  connect(m_downloadDataAction, SIGNAL(triggered()),
    this, SLOT(DownloadData()));
  m_uploadDataAction = fileMenu->addAction(tr("&Upload"));
  m_uploadDataAction->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
  connect(m_uploadDataAction, SIGNAL(triggered()), this, SLOT(UploadData()));
  fileMenu->addSeparator();
  auto toggleAction = fileMenu->addAction(tr("&Toggle visibility"));
  toggleAction->setShortcut(QKeySequence::Close);
  connect(toggleAction, SIGNAL(triggered()), this, SLOT(ToggleVisibility()));
  fileMenu->addSeparator();
  auto quitAction = fileMenu->addAction(tr("&Quit"));
  quitAction->setShortcut(QKeySequence::Quit);
  quitAction->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
  connect(quitAction, SIGNAL(triggered()),
    QApplication::instance(), SLOT(quit()));

  auto helpMenu = menuBar()->addMenu(tr("&Help"));
  auto aboutQtAction = helpMenu->addAction(tr("&About Qt"));
  aboutQtAction->setIcon(
    QIcon(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));
  connect(aboutQtAction, SIGNAL(triggered()),
    QApplication::instance(), SLOT(aboutQt()));

  auto toolBar = new QToolBar(fileMenu->title());
  addToolBar(Qt::LeftToolBarArea, toolBar);
  toolBar->addAction(m_addThoughtAction);
  toolBar->addSeparator();
  toolBar->addAction(m_downloadDataAction);
  toolBar->addAction(m_uploadDataAction);

  auto systrayMenu = new QMenu;
  systrayMenu->addAction(toggleAction);
  systrayMenu->addSeparator();
  systrayMenu->addAction(quitAction);

  m_systrayIcon.setContextMenu(systrayMenu);
  connect(&m_systrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    this, SLOT(SystrayActivated(QSystemTrayIcon::ActivationReason)));
  UpdateSystrayIcon();
  m_systrayIcon.show();

  connect(m_pensieveWidget, SIGNAL(Modified()),
    this, SLOT(UpdateSystrayIcon()));
  connect(&m_networkManager, SIGNAL(finished(QNetworkReply*)),
    this, SLOT(EndRequest(QNetworkReply*)));
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

  auto count = m_pensieveWidget->GetPensieve().GetThoughts().size();
  if(count > 0)
  {
    QPainter painter(&image);
    painter.setPen(Qt::red); // Red

    auto number = QString::number(count);
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

void PensieveWindow::DownloadData()
{
  SetReadOnly(true);
  m_networkManager.get(QNetworkRequest(QUrl("http://localhost:7142")));
}

void PensieveWindow::UploadData()
{
  SetReadOnly(true);
  m_networkManager.put(QNetworkRequest(QUrl("http://localhost:7142")),
    QByteArray(m_pensieveWidget->GetPensieve().ToJSON().c_str()));
}

void PensieveWindow::EndRequest(QNetworkReply* p_reply)
{
  if(p_reply->operation() == QNetworkAccessManager::GetOperation) // GET
  {
    Pensieve pensieve;
    if(Pensieve::FromJSON(p_reply->readAll().data(), pensieve))
    {
      m_pensieveWidget->SetPensieve(pensieve);
    }
  }

  SetReadOnly(false);
  p_reply->deleteLater();
}

void PensieveWindow::SetReadOnly(bool p_readOnly)
{
  m_pensieveWidget->setDisabled(p_readOnly);
  m_addThoughtAction->setDisabled(p_readOnly);
  m_downloadDataAction->setDisabled(p_readOnly);
  m_uploadDataAction->setDisabled(p_readOnly);
}

}
