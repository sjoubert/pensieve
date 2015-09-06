#include "psvLogDialog.hpp"

#include "ui_psvLogDialog.h"

#include <QScrollBar>

namespace psv
{

LogDialog::LogDialog(QAbstractItemModel* p_model, QWidget* p_parent):
  QDialog(p_parent),
  m_ui(std::make_unique<Ui::LogDialog>()),
  m_scrollBarAtMaximum(true)
{
  m_ui->setupUi(this);
  m_ui->m_logView->setModel(p_model);
  connect(p_model, &QAbstractItemModel::rowsAboutToBeInserted,
    this, &LogDialog::SaveScrollBarPosition);
  connect(p_model, &QAbstractItemModel::rowsInserted,
    this, &LogDialog::UpdateScrollBarPosition);
}

LogDialog::~LogDialog() = default;

void LogDialog::SaveScrollBarPosition()
{
  auto scrollBar = m_ui->m_logView->verticalScrollBar();
  m_scrollBarAtMaximum = (scrollBar->sliderPosition() == scrollBar->maximum());
}

void LogDialog::UpdateScrollBarPosition()
{
  if(m_scrollBarAtMaximum)
  {
    /*
     * Process all other events so that we are last and not be messed up by
     * another action on the model
     */
    QApplication::processEvents();
    auto scrollBar = m_ui->m_logView->verticalScrollBar();
    scrollBar->setSliderPosition(scrollBar->maximum());
  }
}

}
