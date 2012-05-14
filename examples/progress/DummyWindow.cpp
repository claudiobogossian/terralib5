#include "DummyWindow.h"

// Terralib
#include "terralib/common/progress/ProgressManager.h"
#include "terralib/qt/widgets/progress/QtProgress.h"
#include "terralib/qt/widgets/progress/QtWidgetProgress.h"

// System
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  #include <windows.h>
#endif

// OpenMP
#include <omp.h>

#define TOTAL_STEPS 15000

DummyWindow::DummyWindow(QWidget* parent) : QWidget(parent)
{
  this->setWindowTitle("Qt Progress Bar Test");

  m_mainLayout = new QGridLayout(this);
  m_progressPushButton = new QPushButton("Progress Bar...", this);
  m_progressThreadPushButton = new QPushButton("Progress Bar (Thread)...", this);
  m_progressThreadPushButton2 = new QPushButton("Progress Bar Two (Thread)...", this);
  m_progressStatusBar = new QPushButton("Progress Status Bar...", this);
  m_statusBar = new QStatusBar(this);

  m_mainLayout->addWidget(m_progressPushButton, 0, 0);
  m_mainLayout->addWidget(m_progressThreadPushButton, 1, 0);
  m_mainLayout->addWidget(m_progressThreadPushButton2, 2, 0);
  m_mainLayout->addWidget(m_progressStatusBar, 3, 0);
  m_mainLayout->addWidget(m_statusBar, 4, 0);

  connect(m_progressPushButton, SIGNAL(clicked()), this, SLOT(showProgressBar()));
  connect(m_progressThreadPushButton, SIGNAL(clicked()), this, SLOT(showThreadProgressBar()));
  connect(m_progressThreadPushButton2, SIGNAL(clicked()), this, SLOT(showThreadProgressBar2()));
  connect(m_progressStatusBar, SIGNAL(clicked()), this, SLOT(showStatusProgressBar()));

  this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

DummyWindow::~DummyWindow()
{

}

void DummyWindow::showProgressBar()
{
  te::qt::widgets::QtProgress* qtProgress = new te::qt::widgets::QtProgress(this);

  int id = te::common::ProgressManager::getInstance().setProgress(qtProgress);

  te::common::ProgressManager::getInstance().reset(id);
  te::common::ProgressManager::getInstance().setTotalSteps(TOTAL_STEPS, id);
  te::common::ProgressManager::getInstance().setTitle("Qt Progress Test", id);
  te::common::ProgressManager::getInstance().setMessage("Sleeper Progress", id);
  te::common::ProgressManager::getInstance().useProgressTimer(true, id);
  te::common::ProgressManager::getInstance().startTimer(id);

  for(unsigned int i = 0; i < TOTAL_STEPS; ++i)
  {
    if(!te::common::ProgressManager::getInstance().isActive(id))
    {
      break;
    }

    #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
    Sleep(5);
    #endif

    te::common::ProgressManager::getInstance().pulse(id);
  }

  te::common::ProgressManager::getInstance().reset();

  delete qtProgress;
}

void DummyWindow::showThreadProgressBar()
{

  te::qt::widgets::QtProgress* qtProgress = new te::qt::widgets::QtProgress(this);

  int id = te::common::ProgressManager::getInstance().setProgress(qtProgress);

  te::common::ProgressManager::getInstance().reset(id);
  te::common::ProgressManager::getInstance().setTotalSteps(TOTAL_STEPS, id);
  te::common::ProgressManager::getInstance().setTitle("Qt Thread Progress Test", id);
  te::common::ProgressManager::getInstance().setMessage("Sleeper Progress", id);
  te::common::ProgressManager::getInstance().setMultiThreadProgress(true, id);
  te::common::ProgressManager::getInstance().useProgressTimer(false, id);
  te::common::ProgressManager::getInstance().setModal(false, id);

  omp_set_num_threads( 1 );

  #pragma omp parallel for
  for(int i = 0; i < TOTAL_STEPS; ++i)
  {
    if(te::common::ProgressManager::getInstance().isActive(id))
    {
      #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        Sleep(5);
      #endif

      te::common::ProgressManager::getInstance().pulse(id);
    }
  }

  te::common::ProgressManager::getInstance().reset();

  delete qtProgress;
}

void DummyWindow::showThreadProgressBar2()
{

  te::qt::widgets::QtProgress* qtProgress = new te::qt::widgets::QtProgress(this);

  int id = te::common::ProgressManager::getInstance().setProgress(qtProgress);

  te::common::ProgressManager::getInstance().reset(id);
  te::common::ProgressManager::getInstance().setTotalSteps(TOTAL_STEPS, id);
  te::common::ProgressManager::getInstance().setTitle("Qt Thread Progress Test", id);
  te::common::ProgressManager::getInstance().setMessage("Sleeper Progress", id);
  te::common::ProgressManager::getInstance().setMultiThreadProgress(true, id);
  te::common::ProgressManager::getInstance().useProgressTimer(false, id);
  te::common::ProgressManager::getInstance().setModal(false, id);

  omp_set_num_threads( 1 );

  #pragma omp parallel for
  for(int i = 0; i < TOTAL_STEPS; ++i)
  {
    if(te::common::ProgressManager::getInstance().isActive(id))
    {
      #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        Sleep(5);
      #endif

      te::common::ProgressManager::getInstance().pulse(id);
    }
  }

  te::common::ProgressManager::getInstance().reset();

  delete qtProgress;
}

void DummyWindow::showStatusProgressBar()
{
  te::qt::widgets::QtWidgetProgress* qtProgress = new te::qt::widgets::QtWidgetProgress(this);

  m_statusBar->addPermanentWidget(qtProgress);

  qtProgress->show();

  int id = te::common::ProgressManager::getInstance().setProgress(qtProgress);

  te::common::ProgressManager::getInstance().reset(id);
  te::common::ProgressManager::getInstance().setTotalSteps(TOTAL_STEPS, id);
  te::common::ProgressManager::getInstance().setTitle("Qt Progress Test", id);
  te::common::ProgressManager::getInstance().setMessage("Sleeper Progress", id);
  te::common::ProgressManager::getInstance().setMultiThreadProgress(true, id);
  te::common::ProgressManager::getInstance().useProgressTimer(false, id);

  omp_set_num_threads( 1 );

  #pragma omp parallel for
  for(int i = 0; i < TOTAL_STEPS; ++i)
  {
    if(te::common::ProgressManager::getInstance().isActive(id))
    {
      #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        Sleep(5);
      #endif

      te::common::ProgressManager::getInstance().pulse(id);
    } 
  }

  te::common::ProgressManager::getInstance().reset();

  m_statusBar->removeWidget(qtProgress);

  delete qtProgress;
}
