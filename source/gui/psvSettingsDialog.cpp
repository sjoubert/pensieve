#include "psvSettingsDialog.hpp"

#include "ui_psvSettingsDialog.h"

namespace psv
{

QString const SettingsDialog::Settings::SERVER = "server";
QString const SettingsDialog::Settings::FLAGS_FILTER = "flags_filter";
QString const SettingsDialog::Settings::START_HIDDEN = "start_hidden";

SettingsDialog::SettingsDialog(QWidget* p_parent):
  QDialog(p_parent),
  m_ui(std::make_unique<Ui::SettingsDialog>())
{
  m_ui->setupUi(this);
}

SettingsDialog::~SettingsDialog() = default;

void SettingsDialog::SetServer(QString const& p_server)
{
  m_ui->m_server->setText(p_server);
}

QString SettingsDialog::GetServer() const
{
  return m_ui->m_server->text();
}

void SettingsDialog::SetStartHidden(bool p_startHidden)
{
  m_ui->m_startHidden->setChecked(p_startHidden);
}

bool SettingsDialog::GetStartHidden() const
{
  return m_ui->m_startHidden->isChecked();
}

void SettingsDialog::SetFlagsFilter(QRegExp const& p_filter)
{
  m_ui->m_flagsFilter->setText(p_filter.pattern());
}

QRegExp SettingsDialog::GetFlagsFilter() const
{
  return QRegExp(m_ui->m_flagsFilter->text());
}

}
