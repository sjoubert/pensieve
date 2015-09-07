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
  m_contextMenu->addAction(m_ui->m_addTagAction);
  m_contextMenu->addAction(m_ui->m_removeTagsAction);
  for(auto action: m_contextMenu->actions())
  {
    action->setStatusTip(action->toolTip());
  }

  SetDisplayMode();

  connect(m_ui->m_editButton, SIGNAL(clicked()),
    this, SLOT(SetEditMode()));
  connect(m_ui->m_editButton, &QPushButton::clicked,
    this, &ThoughtWidget::EditionStarted);
  connect(m_ui->m_saveButton, SIGNAL(clicked()),
    this, SLOT(SetDisplayMode()));
  connect(m_ui->m_saveButton, &QPushButton::clicked,
    this, &ThoughtWidget::EditionEnded);
  connect(m_ui->m_deleteButton, &QPushButton::clicked,
    this, &ThoughtWidget::DeleteRequested);
  connect(m_ui->m_tags, &QWidget::customContextMenuRequested,
    this, &ThoughtWidget::DisplayContextMenu);
  connect(m_ui->m_addTagAction, &QAction::triggered,
    this, &ThoughtWidget::AddTag);
  connect(m_ui->m_removeTagsAction, &QAction::triggered,
    this, &ThoughtWidget::RemoveTags);
}

ThoughtWidget::~ThoughtWidget() = default;

Thought ThoughtWidget::GetThought() const
{
  Thought thought;

  thought.SetTitle(m_ui->m_title->text().toStdString());
  thought.SetContent(m_ui->m_content->toPlainText().toStdString());
  for(auto i = 0; i < m_ui->m_tags->count(); ++i)
  {
    auto item = m_ui->m_tags->item(i);
    thought.AddTag(item->text().toStdString());
  }

  return thought;
}

void ThoughtWidget::SetThought(Thought const& p_thought)
{
  m_ui->m_title->setText(QString::fromStdString(p_thought.GetTitle()));
  m_ui->m_content->setPlainText(QString::fromStdString(p_thought.GetContent()));
  m_ui->m_tags->clear();
  for(auto const& tag: p_thought.GetTags())
  {
    m_ui->m_tags->addItem(QString::fromStdString(tag));
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

  if(m_ui->m_tags->count() > 0)
  {
    auto newTags = m_ui->m_tags->item(0)->flags();
    if(p_displayMode)
    {
      newTags &= ~Qt::ItemIsEditable;
    }
    else
    {
      newTags |= Qt::ItemIsEditable;
    }

    for(auto i = 0; i < m_ui->m_tags->count(); ++i)
    {
      m_ui->m_tags->item(i)->setFlags(newTags);
    }
  }
}

void ThoughtWidget::DisplayContextMenu(QPoint const& p_position)
{
  Q_UNUSED(p_position);

  // Edit mode
  if(m_ui->m_saveButton->isVisible())
  {
    m_ui->m_removeTagsAction->setDisabled(
      m_ui->m_tags->selectedItems().empty());
    m_contextMenu->exec(QCursor::pos());
  }
}

void ThoughtWidget::AddTag()
{
  auto newItem = new QListWidgetItem(tr("<new tag>"));
  newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
  m_ui->m_tags->addItem(newItem);
  m_ui->m_tags->editItem(newItem);
}

void ThoughtWidget::RemoveTags()
{
  // Just delete the items the list widget will be cleaned automatically
  for(auto toRemove: m_ui->m_tags->selectedItems())
  {
    delete toRemove;
  }
}

}
