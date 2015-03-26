#include "psvPensieveWidget.hpp"

#include "psvPensieve.hpp"
#include "psvThoughtWidget.hpp"

#include <QScrollArea>
#include <QSignalMapper>
#include <QGridLayout>

namespace psv
{

PensieveWidget::PensieveWidget(QWidget* p_parent):
  QWidget(p_parent)
{
  auto layout = new QHBoxLayout;
  setLayout(layout);

  auto scrollArea = new QScrollArea;
  scrollArea->setWidgetResizable(true);
  layout->addWidget(scrollArea);

  auto widget = new QWidget;
  widget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  scrollArea->setWidget(widget);

  m_mainLayout = new QGridLayout;
  widget->setLayout(m_mainLayout);

  m_startEditionMapper = new QSignalMapper(this);
  connect(m_startEditionMapper, SIGNAL(mapped(QWidget*)),
    this, SLOT(StartEdition(QWidget*)));
  m_deletionMapper = new QSignalMapper(this);
  connect(m_deletionMapper, SIGNAL(mapped(QWidget*)),
    this, SLOT(DeleteThought(QWidget*)));
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

    // Connections
    m_startEditionMapper->setMapping(thoughtWidget, thoughtWidget);
    connect(thoughtWidget, SIGNAL(EditionStarted()),
      m_startEditionMapper, SLOT(map()));
    connect(thoughtWidget, SIGNAL(EditionEnded()), this, SLOT(EndEdition()));
    m_deletionMapper->setMapping(thoughtWidget, thoughtWidget);
    connect(thoughtWidget, SIGNAL(DeleteRequested()),
      m_deletionMapper, SLOT(map()));

    m_thoughtWidgets.push_back(thoughtWidget);
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
      m_mainLayout->addWidget(widget, i / columnNumber, i % columnNumber);
      ++i;
    }
  }
}

}
