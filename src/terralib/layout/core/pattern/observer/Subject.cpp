#include "Subject.h"

#include "NewObserver.h"

te::layout::Subject::Subject()
{
}

te::layout::Subject::~Subject()
{
  m_observers.clear();
}

bool te::layout::Subject::attach(NewObserver* observer)
{
  if(observer == 0)
  {
    return false;
  }

  std::vector<NewObserver*>::iterator it  = m_observers.begin();
  while(it != m_observers.end())
  {
    if(*it == observer)
    {
      return false;
    }
    ++it;
  }

  observer->update(this);

  m_observers.push_back(observer);

  return true;
}

bool te::layout::Subject::detach(NewObserver* observer)
{
  if(observer == 0)
  {
    return false;
  }

  std::vector<NewObserver*>::iterator it  = m_observers.begin();
  while(it != m_observers.end())
  {
    if(*it == observer)
    {
      m_observers.erase(it);
      return true;
    }
    ++it;
  }

  return false;
}

void te::layout::Subject::notify()
{
  std::vector<NewObserver*>::iterator it  = m_observers.begin();
  while(it != m_observers.end())
  {
    (*it)->update(this);
    ++it;
  }
}
