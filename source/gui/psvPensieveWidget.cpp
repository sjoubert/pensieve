#include "psvPensieveWidget.hpp"

#include "psvPensieve.hpp"
#include "psvThought.hpp"
#include "psvThoughtWidget.hpp"
#include "ui_psvPensieveWidget.h"

namespace psv
{

PensieveWidget::PensieveWidget(QWidget* p_parent):
  QScrollArea(p_parent),
  m_ui(std::make_unique<Ui::PensieveWidget>())
{
  m_ui->setupUi(this);

  connect(&m_startEditionMapper, SIGNAL(mapped(QWidget*)),
    SLOT(StartEdition(QWidget*)));
  connect(&m_deletionMapper, SIGNAL(mapped(QWidget*)),
    SLOT(DeleteThought(QWidget*)));
}

PensieveWidget::~PensieveWidget() = default;

Pensieve PensieveWidget::GetPensieve() const
{
  Pensieve pensieve;

  for(auto widget: m_thoughtWidgets)
  {
    auto thought = qobject_cast<ThoughtWidget*>(widget);
    if(thought)
    {
      pensieve.GetThoughts().push_back(thought->GetThought());
    }
  }

  return pensieve;
}

void PensieveWidget::SetPensieve(Pensieve const& p_pensieve)
{
  // Clean data
  for(auto widget: m_thoughtWidgets)
  {
    delete widget;
  }
  m_thoughtWidgets.clear();

  // Add new data
  for(auto const& thought: p_pensieve.GetThoughts())
  {
    auto thoughtWidget = new ThoughtWidget;
    thoughtWidget->SetThought(thought);
    thoughtWidget->SetHighlighted(ShouldBeHighlighted(thought));
    m_thoughtWidgets.push_back(thoughtWidget);

    // Connections
    m_startEditionMapper.setMapping(thoughtWidget, thoughtWidget);
    connect(thoughtWidget, SIGNAL(EditionStarted()),
      &m_startEditionMapper, SLOT(map()));
    connect(thoughtWidget, &ThoughtWidget::EditionEnded,
      this, &PensieveWidget::EndEdition);
    m_deletionMapper.setMapping(thoughtWidget, thoughtWidget);
    connect(thoughtWidget, SIGNAL(DeleteRequested()),
      &m_deletionMapper, SLOT(map()));
  }
  LayoutWidgets();

  // Notification
  Q_EMIT(Modified());
}

void PensieveWidget::resizeEvent(QResizeEvent* p_event)
{
  QWidget::resizeEvent(p_event);
  LayoutWidgets();
}

void PensieveWidget::SetTagsFilter(QRegExp const& p_filter)
{
  m_tagsFilter = p_filter;
  // Update highlighted thoughts
  SetPensieve(GetPensieve());
}

QRegExp const& PensieveWidget::GetTagsFilter() const
{
  return m_tagsFilter;
}

unsigned int PensieveWidget::GetHighlightedCount() const
{
  auto thoughts = GetPensieve().GetThoughts();
  using namespace  std::placeholders;
  return std::count_if(thoughts.begin(), thoughts.end(),
    std::bind(&PensieveWidget::ShouldBeHighlighted, this, _1));
}

void PensieveWidget::CreateThought()
{
  Q_EMIT(EditionStarted());

  auto currentPensieve = GetPensieve();
  currentPensieve.GetThoughts().emplace_back();
  SetPensieve(currentPensieve);

  Q_EMIT(EditionEnded());
}

void PensieveWidget::StartEdition(QWidget* p_widget)
{
  Q_EMIT(EditionStarted());

  for(auto widget: m_thoughtWidgets)
  {
    if(widget != p_widget)
    {
      widget->setEnabled(false);
    }
  }
}

void PensieveWidget::EndEdition()
{
  for(auto widget: m_thoughtWidgets)
  {
    widget->setEnabled(true);
  }

  // Simulate data send/receive to server
  SetPensieve(GetPensieve());

  Q_EMIT(EditionEnded());
}

void PensieveWidget::DeleteThought(QWidget* p_widget)
{
  Q_EMIT(EditionStarted());

  auto found =
    std::find(m_thoughtWidgets.begin(), m_thoughtWidgets.end(), p_widget);
  if(found != m_thoughtWidgets.end())
  {
    delete *found;
    m_thoughtWidgets.erase(found);
    LayoutWidgets();

    // Simulate data send/receive to server
    SetPensieve(GetPensieve());
  }

  Q_EMIT(EditionEnded());
}

void PensieveWidget::LayoutWidgets()
{
  if(m_thoughtWidgets.empty() == false)
  {
    unsigned int i = 0;
    unsigned int columnNumber =
      std::max(width() / m_thoughtWidgets[0]->sizeHint().width(), 1);
    for(auto widget: m_thoughtWidgets)
    {
      m_ui->m_gridLayout->addWidget(widget, i / columnNumber, i % columnNumber);
      ++i;
    }
  }
}

bool PensieveWidget::ShouldBeHighlighted(Thought const& p_thought) const
{
  for(auto const& tag: p_thought.GetTags())
  {
    if(m_tagsFilter.indexIn(QString::fromStdString(tag)) != -1)
    {
      return true;
    }
  }

  return m_tagsFilter.pattern().isEmpty();
}

}
