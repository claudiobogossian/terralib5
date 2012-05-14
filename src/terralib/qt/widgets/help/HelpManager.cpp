#include "HelpManager.h"
#include "HelpManagerImpl.h"

//inline methods implementation.
void te::qt::widgets::HelpManager::setMgerImpl (HelpManagerImpl* impl)
{
  m_impl = impl;
}

te::qt::widgets::HelpManagerImpl* te::qt::widgets::HelpManager::getMgerImpl() const
{
  return m_impl;
}

void te::qt::widgets::HelpManager::showHelp (const QString& htmRef)
{
  if(m_impl != 0)
    m_impl->showHelp(htmRef);
}

void te::qt::widgets::HelpManager::appendDoc(const QString& docRef)
{
  if(m_impl != 0)
    m_impl->appendDoc(docRef);
}

te::qt::widgets::HelpManager::HelpManager() :
te::common::Singleton<HelpManager>(),
  m_impl(0)
{
}
