#include "DummyWindow.h"

// Terralib
#include "../../src/terralib/Defines.h"
#include "terralib/common/progress/TaskProgress.h"
#include "terralib/common/progress/ProgressManager.h"

// Qt
#include <QSpacerItem>

// System
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  #include <windows.h>
#endif

// OpenMP
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  #include <omp.h>
#endif

#define TOTAL_STEPS 15000

DummyWindow::DummyWindow(QWidget* parent) : QWidget(parent)
{
  this->setWindowTitle("Qt Progress Bar Test");

  m_mainLayout = new QGridLayout(this);
  m_progressPushButton = new QPushButton("Progress Bar...", this);
  m_progressThreadPushButton = new QPushButton("Progress Bar (Thread)...", this);
  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

  QSpacerItem* spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  m_dlgViewer = new te::qt::widgets::ProgressViewerDialog(this);
  m_dlgViewerId = te::common::ProgressManager::getInstance().addViewer(m_dlgViewer);

  m_barViewer = new te::qt::widgets::ProgressViewerBar(this);
  m_barViewerId = te::common::ProgressManager::getInstance().addViewer(m_barViewer);

  m_statusBar->addPermanentWidget(m_barViewer);

  m_mainLayout->addWidget(m_progressPushButton, 0, 0);
  m_mainLayout->addWidget(m_progressThreadPushButton, 1, 0);
  m_mainLayout->addItem(spacer, 2, 0);
  m_mainLayout->addWidget(m_statusBar, 3, 0);
  
  connect(m_progressPushButton, SIGNAL(clicked()), this, SLOT(showProgressBar()));
  connect(m_progressThreadPushButton, SIGNAL(clicked()), this, SLOT(showThreadProgressBar()));
  connect(m_barViewer, SIGNAL(clicked()), this, SLOT(showWidgetViewer()));

  m_widgetViewer = 0;
}

DummyWindow::~DummyWindow()
{
  te::common::ProgressManager::getInstance().removeViewer(m_dlgViewerId);
  delete m_dlgViewer;

  if(m_widgetViewer)
  {
    te::common::ProgressManager::getInstance().removeViewer(m_WidgetViewerId);
  }

  if(m_barViewer)
  {
    te::common::ProgressManager::getInstance().removeViewer(m_barViewerId);
  }
}

void DummyWindow::showProgressBar()
{
  te::common::TaskProgress task;
  task.setTotalSteps(TOTAL_STEPS);
  task.setMessage("Qt Progress Test");
  task.useTimer(true);

  for(unsigned int i = 0; i < TOTAL_STEPS; ++i)
  {
    if(!task.isActive())
    {
      break;
    }

    #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
    Sleep(5);
    #endif

    task.pulse();
  }
}

void DummyWindow::showThreadProgressBar()
{
  te::common::TaskProgress task;
  task.setTotalSteps(TOTAL_STEPS);
  task.setMessage("Qt Thread Progress Test");
  task.useTimer(false);
  task.useMultiThread(true);

  #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
    omp_set_num_threads( 4 );
    #pragma omp parallel for
  #endif
  for(int i = 0; i < TOTAL_STEPS; ++i)
  {
    if(task.isActive())
    {
      #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        Sleep(5);
      #endif

      task.pulse();
    }
  }
}

void DummyWindow::showWidgetViewer()
{
  if(m_widgetViewer == 0)
  {
    m_widgetViewer = new te::qt::widgets::ProgressViewerWidget(this);
    m_WidgetViewerId = te::common::ProgressManager::getInstance().addViewer(m_widgetViewer);
  }

  if(m_widgetViewer->isVisible())
  {
    m_widgetViewer->hide();
  }
  else
  {
    m_widgetViewer->show();
  }
}
