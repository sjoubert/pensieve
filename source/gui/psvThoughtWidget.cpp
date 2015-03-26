#include "psvThoughtWidget.hpp"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>

namespace psv
{

ThoughtWidget::ThoughtWidget(QWidget* p_parent):
  QWidget(p_parent)
{
  auto mainLayout = new QVBoxLayout;
  setLayout(mainLayout);

  auto groupBox = new QGroupBox;
  mainLayout->addWidget(groupBox);

  auto dataLayout = new QHBoxLayout;
  groupBox->setLayout(dataLayout);

  auto textsLayout = new QVBoxLayout;
  dataLayout->addLayout(textsLayout);
  dataLayout->setStretch(0, 3);

  m_title = new QLineEdit;
  textsLayout->addWidget(m_title);
  m_content = new QPlainTextEdit;
  textsLayout->addWidget(m_content);

  m_flags = new QListWidget;
  dataLayout->addWidget(m_flags);
  dataLayout->setStretch(1, 1);

  auto buttonLayout = new QHBoxLayout;
  mainLayout->addLayout(buttonLayout);

  buttonLayout->addStretch();
  m_deleteButton = new QPushButton(tr("Delete"));
  buttonLayout->addWidget(m_deleteButton);
  m_editButton = new QPushButton(tr("Edit"));
  buttonLayout->addWidget(m_editButton);
  m_saveButton = new QPushButton(tr("Save"));
  buttonLayout->addWidget(m_saveButton);

  SetDisplayMode();

  connect(m_editButton, SIGNAL(clicked()), this, SLOT(SetEditMode()));
  connect(m_editButton, SIGNAL(clicked()), this, SIGNAL(EditionStarted()));
  connect(m_saveButton, SIGNAL(clicked()), this, SLOT(SetDisplayMode()));
  connect(m_saveButton, SIGNAL(clicked()), this, SIGNAL(EditionEnded()));
  connect(m_deleteButton, SIGNAL(clicked()), this, SIGNAL(DeleteRequested()));
}

ThoughtWidget::~ThoughtWidget() = default;

Thought ThoughtWidget::GetThought() const
{
  Thought thought;

  thought.SetTitle(m_title->text().toStdString());
  thought.SetContent(m_content->toPlainText().toStdString());
  for(auto i = 0; i < m_flags->count(); ++i)
  {
    auto item = m_flags->item(i);
    thought.AddFlag(item->text().toStdString());
  }

  return thought;
}

void ThoughtWidget::SetThought(Thought const& p_thought)
{
  m_title->setText(QString::fromStdString(p_thought.GetTitle()));
  m_content->setPlainText(QString::fromStdString(p_thought.GetContent()));
  m_flags->clear();
  for(auto const& flag: p_thought.GetFlags())
  {
    m_flags->addItem(QString::fromStdString(flag));
  }
}

void ThoughtWidget::SetEditMode(bool p_editMode)
{
  SetDisplayMode(p_editMode == false);
}

void ThoughtWidget::SetDisplayMode(bool p_displayMode)
{
  m_title->setReadOnly(p_displayMode);
  m_content->setReadOnly(p_displayMode);
  m_deleteButton->setVisible(p_displayMode);
  m_editButton->setVisible(p_displayMode);
  m_saveButton->setHidden(p_displayMode);
}

}
