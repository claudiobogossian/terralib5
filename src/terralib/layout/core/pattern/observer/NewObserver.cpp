#include "NewObserver.h"

#include "Subject.h"

te::layout::NewObserver::NewObserver()
  : m_subject(0)
{
}

te::layout::NewObserver::~NewObserver()
{
  if(m_subject != 0)
    m_subject->detach(this);
}
