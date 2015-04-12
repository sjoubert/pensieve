#include "psvLogDialog.hpp"

#include "ui_psvLogDialog.h"

namespace psv
{

LogDialog::LogDialog(QAbstractItemModel* p_model, QWidget* p_parent):
  QDialog(p_parent),
  m_ui(std::make_unique<Ui::LogDialog>())
{
  m_ui->setupUi(this);
  m_ui->m_logView->setModel(p_model);
}

LogDialog::~LogDialog() = default;

}
