#include "psvPensieveWidget.hpp"

#include "psvPensieve.hpp"
#include "psvThoughtWidget.hpp"

#include <QSignalMapper>
#include <QGridLayout>

namespace psv
{

PensieveWidget::PensieveWidget(QWidget* p_parent):
  QScrollArea(p_parent)
{
  m_ui.setupUi(this);

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
    m_thoughtWidgets.push_back(thoughtWidget);

    // Connections
    m_startEditionMapper.setMapping(thoughtWidget, thoughtWidget);
    connect(thoughtWidget, SIGNAL(EditionStarted()),
      &m_startEditionMapper, SLOT(map()));
    connect(thoughtWidget, SIGNAL(EditionEnded()), SLOT(EndEdition()));
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

void PensieveWidget::CreateThought()
{
  auto currentPensieve = GetPensieve();
  currentPensieve.GetThoughts().emplace_back();
  SetPensieve(currentPensieve);
}

void PensieveWidget::StartEdition(QWidget* p_widget)
{
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
}

void PensieveWidget::DeleteThought(QWidget* p_widget)
{
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
      m_ui.m_gridLayout->addWidget(widget, i / columnNumber, i % columnNumber);
      ++i;
    }
  }
}

}
