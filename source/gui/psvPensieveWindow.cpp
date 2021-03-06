#include "psvPensieveWindow.hpp"

#include "psvConfigure.hpp"
#include "psvPensieve.hpp"
#include "psvSettingsDialog.hpp"
#include "ui_psvPensieveWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDateTime>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QSettings>

#include <thread>

namespace psv
{

PensieveWindow::PensieveWindow(QWidget* p_parent):
  QMainWindow(p_parent),
  m_ui(std::make_unique<Ui::PensieveWindow>()),
  m_logDialog(nullptr)
{
  m_ui->setupUi(this);
  setWindowIcon(QIcon(":/psv/pensieve"));
  setCentralWidget(&m_pensieveWidget);
  for(auto action: m_ui->m_fileMenu->actions() + m_ui->m_helpMenu->actions())
  {
    action->setStatusTip(action->toolTip());
  }

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

  m_ui->m_statusBar->addPermanentWidget(&m_networkStatusLabel);

  auto systrayMenu = new QMenu(this);
  systrayMenu->addAction(m_ui->m_toggleVisibilityAction);
  systrayMenu->addSeparator();
  systrayMenu->addAction(m_ui->m_quitAction);

  QApplication::setOrganizationName("qpensieve");
  QSettings settings;
  m_server = settings.value(SettingsDialog::Settings::SERVER, "").toUrl();
  m_pensieveWidget.SetTagsFilter(
    settings.value(SettingsDialog::Settings::TAGS_FILTER, "").toRegExp());

  m_systrayIcon.setContextMenu(systrayMenu);
  UpdateSystrayIcon();
  m_systrayIcon.show();

  connect(m_ui->m_addThoughtAction, &QAction::triggered,
    &m_pensieveWidget, &PensieveWidget::CreateThought);
  connect(m_ui->m_downloadDataAction, &QAction::triggered,
    this, &PensieveWindow::DownloadData);
  connect(m_ui->m_uploadDataAction, &QAction::triggered,
    this, &PensieveWindow::UploadData);
  connect(m_ui->m_settingsAction, &QAction::triggered,
    this, &PensieveWindow::DisplaySettings);
  connect(m_ui->m_toggleVisibilityAction, &QAction::triggered,
    this, &PensieveWindow::ToggleVisibility);
  connect(m_ui->m_quitAction, &QAction::triggered,
    QApplication::instance(), &QApplication::quit);
  connect(m_ui->m_logAction, &QAction::triggered,
    this, &PensieveWindow::DisplayLog);
  connect(m_ui->m_aboutAction, &QAction::triggered,
    this, &PensieveWindow::About);
  connect(m_ui->m_aboutQtAction, &QAction::triggered,
    QApplication::instance(), &QApplication::aboutQt);
  connect(&m_systrayIcon, &QSystemTrayIcon::activated,
    this, &PensieveWindow::SystrayActivated);
  connect(&m_pensieveWidget, &PensieveWidget::Modified,
    this, &PensieveWindow::UpdateSystrayIcon);
  connect(&m_networkManager, &QNetworkAccessManager::finished,
    this, &PensieveWindow::EndRequest);
  connect(&m_pensieveWidget, &PensieveWidget::EditionStarted,
    this, &PensieveWindow::StartEdition);
  connect(&m_pensieveWidget, &PensieveWidget::EditionEnded,
    this, &PensieveWindow::EndEdition);
  connect(&m_networkStatusTimer, &QTimer::timeout,
    this, &PensieveWindow::UpdateNetworkStatus);
  connect(&m_updateDataTimer, &QTimer::timeout,
    this, &PensieveWindow::DownloadData);

  // Setup network status
  UpdateNetworkStatus();
  m_networkStatusTimer.start(500);

  // Get initial data and start download timer
  DownloadData();
  m_updateDataTimer.setInterval(
    settings.value(SettingsDialog::Settings::UPDATE_INTERVAL, 0).toInt());
  StartUpdateTimer();
}

PensieveWindow::~PensieveWindow() = default;

void PensieveWindow::closeEvent(QCloseEvent* p_event)
{
  ToggleVisibility();
  p_event->ignore();
}

void PensieveWindow::DisplaySettings()
{
  QSettings settings;

  // Initialize dialog with current values
  SettingsDialog dialog(this);
  dialog.SetServer(m_server.toString());
  dialog.SetTagsFilter(m_pensieveWidget.GetTagsFilter());
  dialog.SetUpdateInterval(m_updateDataTimer.interval());
  dialog.SetStartHidden(
    settings.value(SettingsDialog::Settings::START_HIDDEN, false).toBool());

  // User input
  if(dialog.exec() == QDialog::Accepted)
  {
    // Save data if needed

    // In memory
    m_server = dialog.GetServer();
    m_updateDataTimer.stop();
    m_updateDataTimer.setInterval(dialog.GetUpdateInterval());
    StartUpdateTimer();
    m_pensieveWidget.SetTagsFilter(dialog.GetTagsFilter());
    UpdateSystrayIcon();

    // In settings
    settings.setValue(SettingsDialog::Settings::SERVER, m_server);
    settings.setValue(
      SettingsDialog::Settings::TAGS_FILTER, dialog.GetTagsFilter());
    settings.setValue(
      SettingsDialog::Settings::UPDATE_INTERVAL, dialog.GetUpdateInterval());
    settings.setValue(
      SettingsDialog::Settings::START_HIDDEN, dialog.GetStartHidden());
  }
}

void PensieveWindow::DisplayLog()
{
  if(m_logDialog == nullptr)
  {
    m_logDialog = new LogDialog(&m_logModel, this);
  }

  m_logDialog->show();
  m_logDialog->raise();
  m_logDialog->activateWindow();
}

void PensieveWindow::ToggleVisibility()
{
  setHidden(isVisible());
}

void PensieveWindow::About()
{
  auto text = tr("Version %1\n\n"
    "Developer:\n"
    "    Sylvain Joubert <joubert.sy@gmail.com>\n\n"
    "Application icon:\n"
    "    Denis Sazhin <http://iconka.com>\n"
    "Connected network icon:\n"
    "    gakuseisean (Sean) <http://gakuseisean.deviantart.com>\n"
    "Disconnected network icon:\n"
    "    MazeNL77 (Peter) <http://mazenl77.deviantart.com>\n"
  );

  QMessageBox::about(this, tr("About - QPensieve"),
    text.arg(QString::fromStdString(PENSIEVE_VERSION)));
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

  auto count = m_pensieveWidget.GetHighlightedCount();
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
  m_systrayIcon.setToolTip(tr("%n thought(s) on filter '%1'", nullptr, count)
    .arg(m_pensieveWidget.GetTagsFilter().pattern()));
}

void PensieveWindow::UpdateNetworkStatus()
{
  static auto status = QNetworkAccessManager::UnknownAccessibility;
  if(status != m_networkManager.networkAccessible())
  {
    status = m_networkManager.networkAccessible();

    QPixmap pixmap;
    QString statusString;
    switch(status)
    {
      case QNetworkAccessManager::Accessible:
      {
        pixmap = QPixmap(":/psv/connected");
        statusString = tr("connected");
        break;
      }
      case QNetworkAccessManager::NotAccessible:
      {
        pixmap = QPixmap(":/psv/disconnected");
        statusString = tr("disconnected");
        break;
      }
      default:
      {
        statusString = tr("unknown");
        m_networkStatusLabel.clear();
        break;
      }
    }

    if(pixmap.isNull() == false)
    {
      pixmap = pixmap.scaledToHeight(m_networkStatusLabel.height(),
        Qt::SmoothTransformation);
      m_networkStatusLabel.setPixmap(pixmap);
    }
    Log(tr("Network status changed to: %1").arg(statusString));
  }
}

void PensieveWindow::DownloadData()
{
  SetReadOnly(true);
  m_networkManager.get(QNetworkRequest(m_server));
  Log(tr("Send GET request to %1").arg(m_server.toString()));
}

void PensieveWindow::UploadData()
{
  SetReadOnly(true);
  m_networkManager.put(QNetworkRequest(m_server),
    QByteArray(m_pensieveWidget.GetPensieve().ToJSON().c_str()));
  Log(tr("Send PUT request to %1").arg(m_server.toString()));
}

void PensieveWindow::EndRequest(QNetworkReply* p_reply)
{
  if(p_reply->error() == QNetworkReply::NoError)
  {
    static QString const succeededLog = tr("Request succeeded");
    if(p_reply->operation() == QNetworkAccessManager::GetOperation) // GET
    {
      Pensieve pensieve;
      if(Pensieve::FromJSON(p_reply->readAll().data(), pensieve))
      {
        Log(succeededLog);
        m_pensieveWidget.SetPensieve(pensieve);
      }
      else
      {
        Log(tr("Invalid pensieve content"));
      }
    }
    else // PUT
    {
      Log(succeededLog);
      DownloadData();
    }
  }
  else
  {
    Log(tr("Request failed: %2").arg(p_reply->errorString()));
  }

  SetReadOnly(false);
  p_reply->deleteLater();
}

void PensieveWindow::StartEdition()
{
  m_updateDataTimer.stop();
  m_ui->m_addThoughtAction->setDisabled(true);
  m_ui->m_downloadDataAction->setDisabled(true);
  m_ui->m_uploadDataAction->setDisabled(true);
  m_ui->m_settingsAction->setDisabled(true);
}

void PensieveWindow::EndEdition()
{
  m_ui->m_addThoughtAction->setEnabled(true);
  m_ui->m_downloadDataAction->setEnabled(true);
  m_ui->m_uploadDataAction->setEnabled(true);
  m_ui->m_settingsAction->setEnabled(true);
  UploadData();
  StartUpdateTimer();
}

void PensieveWindow::SetReadOnly(bool p_readOnly)
{
  m_pensieveWidget.setDisabled(p_readOnly);
  m_ui->m_addThoughtAction->setDisabled(p_readOnly);
  m_ui->m_downloadDataAction->setDisabled(p_readOnly);
  m_ui->m_uploadDataAction->setDisabled(p_readOnly);
}

void PensieveWindow::StartUpdateTimer()
{
  if(m_updateDataTimer.interval() > 0)
  {
    m_updateDataTimer.start();
  }
}

void PensieveWindow::Log(QString const& p_message)
{
  QString log = tr("%1: %2")
    .arg(QDateTime::currentDateTime().toString(Qt::ISODate))
    .arg(p_message);

  m_logModel.insertRows(m_logModel.rowCount(), 1);
  m_logModel.setData(m_logModel.index(m_logModel.rowCount() - 1), log);
}

}
