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
    \file Observable.h

    \brief Abstract class to represent an observable. "Model" part of MVC component.

    \ingroup layout
*/

// TerraLib
#include "Subject.h"
#include "Observer.h"

te::layout::Subject::Subject()
{

}

te::layout::Subject::~Subject()
{
  m_observers.clear();
}

bool te::layout::Subject::attach(Observer* observer)
{
  if(observer == 0)
  {
    return false;
  }

  std::vector<Observer*>::iterator it  = m_observers.begin();
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

bool te::layout::Subject::detach(Observer* observer)
{
  if(observer == 0)
  {
    return false;
  }

  std::vector<Observer*>::iterator it  = m_observers.begin();
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
  std::vector<Observer*>::iterator it  = m_observers.begin();
  while(it != m_observers.end())
  {
    (*it)->update(this);
    ++it;
  }
}
