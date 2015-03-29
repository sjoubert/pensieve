#include "psvSettingsDialog.hpp"

#include "ui_psvSettingsDialog.h"

namespace psv
{

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

}
