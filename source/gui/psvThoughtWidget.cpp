#include "psvThoughtWidget.hpp"

#include "ui_psvThoughtWidget.h"

#include <QMenu>

namespace psv
{

ThoughtWidget::ThoughtWidget(QWidget* p_parent):
  QWidget(p_parent),
  m_ui(std::make_unique<Ui::ThoughtWidget>())
{
  m_ui->setupUi(this);
  SetHighlighted(false);

  m_contextMenu = new QMenu(this);
  m_contextMenu->addAction(m_ui->m_addFlagAction);
  m_contextMenu->addAction(m_ui->m_removeFlagsAction);

  SetDisplayMode();

  connect(m_ui->m_editButton, SIGNAL(clicked()), SLOT(SetEditMode()));
  connect(m_ui->m_editButton, SIGNAL(clicked()), SIGNAL(EditionStarted()));
  connect(m_ui->m_saveButton, SIGNAL(clicked()), SLOT(SetDisplayMode()));
  connect(m_ui->m_saveButton, SIGNAL(clicked()), SIGNAL(EditionEnded()));
  connect(m_ui->m_deleteButton, SIGNAL(clicked()), SIGNAL(DeleteRequested()));
  connect(m_ui->m_flags, SIGNAL(customContextMenuRequested(QPoint)),
    SLOT(DisplayContextMenu(QPoint)));
  connect(m_ui->m_addFlagAction, SIGNAL(triggered()), SLOT(AddFlag()));
  connect(m_ui->m_removeFlagsAction, SIGNAL(triggered()), SLOT(RemoveFlags()));
}

ThoughtWidget::~ThoughtWidget() = default;

Thought ThoughtWidget::GetThought() const
{
  Thought thought;

  thought.SetTitle(m_ui->m_title->text().toStdString());
  thought.SetContent(m_ui->m_content->toPlainText().toStdString());
  for(auto i = 0; i < m_ui->m_flags->count(); ++i)
  {
    auto item = m_ui->m_flags->item(i);
    thought.AddFlag(item->text().toStdString());
  }

  return thought;
}

void ThoughtWidget::SetThought(Thought const& p_thought)
{
  m_ui->m_title->setText(QString::fromStdString(p_thought.GetTitle()));
  m_ui->m_content->setPlainText(QString::fromStdString(p_thought.GetContent()));
  m_ui->m_flags->clear();
  for(auto const& flag: p_thought.GetFlags())
  {
    m_ui->m_flags->addItem(QString::fromStdString(flag));
  }
}

void ThoughtWidget::SetHighlighted(bool p_highlighted)
{
  static QString const s_styleSheetTemplate = R"(
    QGroupBox
    {
      border-width: 2px;
      border-style: solid;
      border-radius: 5px;
    }
    QGroupBox:enabled
    {
      border-color: %1;
    }
    QGroupBox:disabled
    {
      border-color: lightgray;
    }
  )";

  setStyleSheet(s_styleSheetTemplate.arg(p_highlighted ? "red" : "black"));
}

void ThoughtWidget::SetEditMode(bool p_editMode)
{
  SetDisplayMode(p_editMode == false);
}

void ThoughtWidget::SetDisplayMode(bool p_displayMode)
{
  m_ui->m_title->setReadOnly(p_displayMode);
  m_ui->m_content->setReadOnly(p_displayMode);
  m_ui->m_deleteButton->setVisible(p_displayMode);
  m_ui->m_editButton->setVisible(p_displayMode);
  m_ui->m_saveButton->setHidden(p_displayMode);

  if(m_ui->m_flags->count() > 0)
  {
    auto newFlags = m_ui->m_flags->item(0)->flags();
    if(p_displayMode)
    {
      newFlags &= ~Qt::ItemIsEditable;
    }
    else
    {
      newFlags |= Qt::ItemIsEditable;
    }

    for(auto i = 0; i < m_ui->m_flags->count(); ++i)
    {
      m_ui->m_flags->item(i)->setFlags(newFlags);
    }
  }
}

void ThoughtWidget::DisplayContextMenu(QPoint const& p_position)
{
  Q_UNUSED(p_position);

  // Edit mode
  if(m_ui->m_saveButton->isVisible())
  {
    m_ui->m_removeFlagsAction->setDisabled(
      m_ui->m_flags->selectedItems().empty());
    m_contextMenu->exec(QCursor::pos());
  }
}

void ThoughtWidget::AddFlag()
{
  auto newItem = new QListWidgetItem(tr("<new flag>"));
  newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
  m_ui->m_flags->addItem(newItem);
  m_ui->m_flags->editItem(newItem);
}

void ThoughtWidget::RemoveFlags()
{
  // Just delete the items the list widget will be cleaned automatically
  for(auto toRemove: m_ui->m_flags->selectedItems())
  {
    delete toRemove;
  }
}

}
