#include "psvPensieveWindow.hpp"

#include "psvConfigure.hpp"
#include "psvPensieve.hpp"
#include "psvSettingsDialog.hpp"
#include "ui_psvPensieveWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QSettings>

#include <thread>

namespace psv
{

QString const PensieveWindow::Settings::SERVER = "server";

PensieveWindow::PensieveWindow(QWidget* p_parent):
  QMainWindow(p_parent),
  m_ui(std::make_unique<Ui::PensieveWindow>())
{
  m_ui->setupUi(this);
  setWindowIcon(QIcon(":/psv/pensieve"));
  setCentralWidget(&m_pensieveWidget);

  m_ui->m_addThoughtAction->setIcon(
    style()->standardIcon(QStyle::SP_FileDialogNewFolder));
  m_ui->m_downloadDataAction->setIcon(
    style()->standardIcon(QStyle::SP_ArrowDown));
  m_ui->m_uploadDataAction->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
  m_ui->m_quitAction->setIcon(
    style()->standardIcon(QStyle::SP_DialogCloseButton));
  m_ui->m_aboutQtAction->setIcon(
    QIcon(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));

  m_ui->m_addThoughtAction->setShortcut(QKeySequence::New);
  m_ui->m_toggleVisibilityAction->setShortcut(QKeySequence::Close);
  m_ui->m_quitAction->setShortcut(QKeySequence::Quit);

  auto systrayMenu = new QMenu;
  systrayMenu->addAction(m_ui->m_toggleVisibilityAction);
  systrayMenu->addSeparator();
  systrayMenu->addAction(m_ui->m_quitAction);

  m_systrayIcon.setContextMenu(systrayMenu);
  UpdateSystrayIcon();
  m_systrayIcon.show();

  QApplication::setOrganizationName("qpensieve");
  QSettings settings;
  m_server = settings.value(Settings::SERVER, "").toUrl();

  connect(m_ui->m_addThoughtAction, SIGNAL(triggered()),
    &m_pensieveWidget, SLOT(CreateThought()));
  connect(m_ui->m_downloadDataAction, SIGNAL(triggered()),
    SLOT(DownloadData()));
  connect(m_ui->m_uploadDataAction, SIGNAL(triggered()),
    SLOT(UploadData()));
  connect(m_ui->m_settingsAction, SIGNAL(triggered()), SLOT(DisplaySettings()));
  connect(m_ui->m_toggleVisibilityAction, SIGNAL(triggered()),
    SLOT(ToggleVisibility()));
  connect(m_ui->m_quitAction, SIGNAL(triggered()),
    QApplication::instance(), SLOT(quit()));
  connect(m_ui->m_aboutAction, SIGNAL(triggered()), SLOT(About()));
  connect(m_ui->m_aboutQtAction, SIGNAL(triggered()),
    QApplication::instance(), SLOT(aboutQt()));
  connect(&m_systrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    SLOT(SystrayActivated(QSystemTrayIcon::ActivationReason)));
  connect(&m_pensieveWidget, SIGNAL(Modified()), SLOT(UpdateSystrayIcon()));
  connect(&m_networkManager, SIGNAL(finished(QNetworkReply*)),
    SLOT(EndRequest(QNetworkReply*)));
}

PensieveWindow::~PensieveWindow() = default;

void PensieveWindow::closeEvent(QCloseEvent* p_event)
{
  ToggleVisibility();
  p_event->ignore();
}

void PensieveWindow::DisplaySettings()
{
  // Initialize dialog with current values
  SettingsDialog dialog(this);
  dialog.SetServer(m_server.toString());

  // User input
  if(dialog.exec() == QDialog::Accepted)
  {
    // Save data if needed

    // In memory
    m_server = dialog.GetServer();

    // In settings
    QSettings settings;
    settings.setValue(Settings::SERVER, m_server);
  }
}

void PensieveWindow::ToggleVisibility()
{
  setHidden(isVisible());
}

void PensieveWindow::About()
{
  QMessageBox::about(this, tr("About"),
    tr("Version %1").arg(QString::fromStdString(PENSIEVE_VERSION)));
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

  auto count = m_pensieveWidget.GetPensieve().GetThoughts().size();
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
  m_networkManager.get(QNetworkRequest(m_server));
}

void PensieveWindow::UploadData()
{
  SetReadOnly(true);
  m_networkManager.put(QNetworkRequest(m_server),
    QByteArray(m_pensieveWidget.GetPensieve().ToJSON().c_str()));
}

void PensieveWindow::EndRequest(QNetworkReply* p_reply)
{
  if(p_reply->operation() == QNetworkAccessManager::GetOperation) // GET
  {
    Pensieve pensieve;
    if(Pensieve::FromJSON(p_reply->readAll().data(), pensieve))
    {
      m_pensieveWidget.SetPensieve(pensieve);
    }
  }

  SetReadOnly(false);
  p_reply->deleteLater();
}

void PensieveWindow::SetReadOnly(bool p_readOnly)
{
  m_pensieveWidget.setDisabled(p_readOnly);
  m_ui->m_addThoughtAction->setDisabled(p_readOnly);
  m_ui->m_downloadDataAction->setDisabled(p_readOnly);
  m_ui->m_uploadDataAction->setDisabled(p_readOnly);
}

}
