#include "ThreadManager.h"

#include "DrawThread.h"

// Qt
#include <QThreadPool>
#include <QTimer>

te::qt::widgets::ThreadManager::ThreadManager(const std::vector<QRunnable*>& threads, int interval):
QObject(),
m_threads(threads),
m_interval(interval),
m_pool(0)
{
  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), SIGNAL(showFeedback()));
}

te::qt::widgets::ThreadManager::~ThreadManager()
{
}

void te::qt::widgets::ThreadManager::run()
{
  m_timer->start(m_interval);
 
  m_pool = new QThreadPool;

  for(std::vector<QRunnable*>::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
  {
    connect((DrawThread*)*it, SIGNAL(finished()), SLOT(onThreadFinished()));
    m_pool->start(*it);
  }
}

void te::qt::widgets::ThreadManager::stopProccess()
{
  if(m_pool == 0)
    return;

  for(std::vector<QRunnable*>::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
    ((DrawThread*)*it)->m_cancel = true;

  m_pool->waitForDone();
  onThreadFinished();
}

void te::qt::widgets::ThreadManager::onThreadFinished()
{
  if(m_pool == 0)
  {
    emit finished();
    return;
  }

  if(m_pool->activeThreadCount() == 0)
  {
    m_timer->stop();

    delete m_pool;
    m_pool = 0;

    emit finished();
  }
}
