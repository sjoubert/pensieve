#include "psvPensieveWindow.hpp"

#include "psvPensieve.hpp"

#include <QApplication>
#include <QCloseEvent>
#include <QHeaderView>
#include <QMenuBar>
#include <QPainter>
#include <QSystemTrayIcon>

namespace psv
{

PensieveWindow::PensieveWindow(QWidget* p_parent):
  QMainWindow(p_parent)
{
  setWindowIcon(QIcon(":/psv/pensieve"));

  m_pensieveWidget = new PensieveWidget;
  setCentralWidget(m_pensieveWidget);

  auto fileMenu = menuBar()->addMenu(tr("&File"));
  auto addThoughtAction = fileMenu->addAction(tr("&Add thought"));
  addThoughtAction->setShortcut(QKeySequence::New);
  connect(addThoughtAction, SIGNAL(triggered()),
    m_pensieveWidget, SLOT(CreateThought()));
  fileMenu->addSeparator();
  auto toggleAction = fileMenu->addAction(tr("&Toggle visibility"));
  connect(toggleAction, SIGNAL(triggered()), this, SLOT(ToggleVisibility()));
  fileMenu->addSeparator();
  auto quitAction = fileMenu->addAction(tr("&Quit"));
  quitAction->setShortcut(QKeySequence::Quit);
  connect(quitAction, SIGNAL(triggered()),
    QApplication::instance(), SLOT(quit()));

  auto helpMenu = menuBar()->addMenu(tr("&Help"));
  auto aboutQtAction = helpMenu->addAction(tr("&About Qt"));
  connect(aboutQtAction, SIGNAL(triggered()),
    QApplication::instance(), SLOT(aboutQt()));

  auto systrayMenu = new QMenu;
  systrayMenu->addAction(toggleAction);
  systrayMenu->addSeparator();
  systrayMenu->addAction(quitAction);

  m_systrayIcon.setContextMenu(systrayMenu);
  connect(&m_systrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    this, SLOT(SystrayActivated(QSystemTrayIcon::ActivationReason)));
  UpdateSystrayIcon();
  m_systrayIcon.show();

  connect(m_pensieveWidget, SIGNAL(Modified()),
    this, SLOT(UpdateSystrayIcon()));

  // Add temporary fake data
  Pensieve pensieve;
  Thought thought;
  thought.SetTitle("toto");
  thought.SetContent("l'escargot qui porte sa maison sur le dos");
  thought.AddFlag("animal");
  thought.AddFlag("joke");
  pensieve.GetThoughts().push_back(thought);
  for(int i = 0; i < 9; ++i){
  thought = Thought();
  thought.SetTitle("meuh");
  thought.SetContent("la vache qui produit du lait");
  thought.AddFlag("animal");
  thought.AddFlag("wtf");
  pensieve.GetThoughts().push_back(thought);}
  m_pensieveWidget->SetPensieve(pensieve);
}

PensieveWindow::~PensieveWindow() = default;

void PensieveWindow::closeEvent(QCloseEvent* p_event)
{
  ToggleVisibility();
  p_event->ignore();
}

void PensieveWindow::ToggleVisibility()
{
  setHidden(isVisible());
}

void PensieveWindow::SystrayActivated(
  QSystemTrayIcon::ActivationReason p_reason)
{
  if(p_reason == QSystemTrayIcon::Trigger)
  {
    ToggleVisibility();
  }
}

void PensieveWindow::UpdateSystrayIcon()
{
  QPixmap image(":/psv/pensieve");

  auto count = m_pensieveWidget->GetPensieve().GetThoughts().size();
  if(count > 0)
  {
    QPainter painter(&image);
    painter.setPen(Qt::red); // Red

    auto number = QString::number(count);
    auto newFont = painter.font();
    newFont.setBold(true); // Bold

    // Compute a size that can handle the text
    newFont.setPixelSize(image.height());
    bool foundSize = false;
    while(foundSize == false)
    {
      auto size = QFontMetrics(newFont).size(Qt::TextSingleLine, number);
      size *= 1.25; // Avoid covering the whole image
      if((size.height() <= image.height() && size.width() <= image.height()) ||
        newFont.pixelSize() <= 0)
      {
        foundSize = true;
      }
      else
      {
        newFont.setPixelSize(newFont.pixelSize() - 1);
      }
    }
    painter.setFont(newFont);

    painter.drawText(image.rect(), Qt::AlignHCenter | Qt::AlignBottom, number);
  }

  m_systrayIcon.setIcon(QIcon(image));
}

}
