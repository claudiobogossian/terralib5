/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/common/progress/ProgressManager.cpp

  \brief A singleton class used to manage tasks progresses and their viewers.
*/

// TerraLib
#include "AbstractProgressViewer.h"
#include "ProgressManager.h"
#include "TaskProgress.h"

int te::common::ProgressManager::addViewer(AbstractProgressViewer* apv)
{
  int id = generateViewerId();

  m_viewers.insert(std::map<int, AbstractProgressViewer*>::value_type(id, apv));

  // add current tasks into the new viewer
  std::map<int, TaskProgress*>::iterator it = m_tasks.begin();

  while(it != m_tasks.end())
  {
    apv->addTask(it->second, it->first);
    ++it;
  }

  return id;
}

void te::common::ProgressManager::removeViewer(int viewerId)
{
  std::map<int, AbstractProgressViewer*>::iterator it = m_viewers.find(viewerId);

  if(it != m_viewers.end())
  {
    m_viewers.erase(it);
  }
}

int te::common::ProgressManager::addTask(TaskProgress* tp)
{
  LockWrite l(this);

  int id = generateTaskId();

  m_tasks.insert(std::map<int, TaskProgress*>::value_type(id, tp));

  // add task in viewers
  std::map<int, AbstractProgressViewer*>::iterator itV = m_viewers.begin();

  while(itV != m_viewers.end())
  {
    if(!m_suspendViewers)
      itV->second->addTask(tp, id);

    ++itV;
  }

  return id;
}

void te::common::ProgressManager::removeTask(int taskId)
{
  LockWrite l(this);

  std::map<int, TaskProgress*>::iterator it = m_tasks.find(taskId);

  if(it != m_tasks.end())
  {
    // remove task from viewers
    std::map<int, AbstractProgressViewer*>::iterator itV = m_viewers.begin();

    while(itV != m_viewers.end())
    {
      if(!m_suspendViewers)
        itV->second->removeTask(taskId);

      ++itV;
    }

    m_tasks.erase(it);
  }
}

void te::common::ProgressManager::cancelTask(int taskId)
{
  if(m_suspendViewers)
    return;

  std::map<int, TaskProgress*>::iterator it = m_tasks.find(taskId);

  if(it != m_tasks.end())
  {
    // remove task from viewers
    std::map<int, AbstractProgressViewer*>::iterator itV = m_viewers.begin();

    while(itV != m_viewers.end())
    {
      itV->second->cancelTask(taskId);

      ++itV;
    }
  }
}

void te::common::ProgressManager::cancelTasks(unsigned int type)
{
  if(m_suspendViewers)
    return;

  LockWrite l(this);

  std::vector<TaskProgress*> cancelled;
  std::map<int, TaskProgress*>::iterator it = m_tasks.begin();

  while(it != m_tasks.end())
  {
    if(it->second->getType() == type)
      cancelled.push_back(it->second);

    ++it;
  }

  for(std::size_t i = 0; i < cancelled.size(); ++i)
    cancelled[i]->cancel();
}

void te::common::ProgressManager::setTotalValues(int taskId)
{
  if(m_suspendViewers)
    return;

  std::map<int, AbstractProgressViewer*>::iterator itV = m_viewers.begin();

  while(itV != m_viewers.end())
  {
    itV->second->setTotalValues(taskId);

    ++itV;
  }
}

void te::common::ProgressManager::updateValue(int taskId)
{
  if(m_suspendViewers)
    return;

  std::map<int, AbstractProgressViewer*>::iterator itV = m_viewers.begin();

  while(itV != m_viewers.end())
  {
    itV->second->updateValue(taskId);

    ++itV;
  }
}

void te::common::ProgressManager::updateMessage(int taskId)
{
  if(m_suspendViewers)
    return;

  std::map<int, AbstractProgressViewer*>::iterator itV = m_viewers.begin();

  while(itV != m_viewers.end())
  {
    itV->second->updateMessage(taskId);

    ++itV;
  }
}

void te::common::ProgressManager::clearAll()
{
  m_viewers.clear();
  m_tasks.clear();
}

te::common::ProgressManager::ProgressManager()
  : m_taskCounter(0),
    m_viewerCounter(0),
    m_suspendViewers(false)
{
}

te::common::ProgressManager::~ProgressManager()
{
  clearAll();
}

int te::common::ProgressManager::generateViewerId()
{
  return m_viewerCounter++;
}

int te::common::ProgressManager::generateTaskId()
{
  return m_taskCounter++;
}

