#include "ThreadManager.h"

// Qt
#include <QRunnable>
#include <QThreadPool>

te::qt::widgets::ThreadManager::ThreadManager(const std::vector<QRunnable*>& threads):
m_threads(threads)
{
}

te::qt::widgets::ThreadManager::~ThreadManager()
{
}

void te::qt::widgets::ThreadManager::run()
{
  QThreadPool pool;

  for(std::vector<QRunnable*>::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
  {
    pool.start(*it);
  }

  pool.waitForDone();
}