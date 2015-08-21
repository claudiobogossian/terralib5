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
  \file Visitable.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Visitable.h"
#include "AbstractVisitor.h"

void te::layout::Visitable::acceptVisitor( AbstractVisitor* visitor )
{
  m_visitors.insert(visitor);
  
  //allow the incoming visitor to start visiting
  visitor->visit(this);
}

void te::layout::Visitable::updateVisitors()
{
  std::set<AbstractVisitor*>::iterator it;
  for(it = m_visitors.begin(); it != m_visitors.end(); ++it)
  {
    (*it)->visit(this);
  }
}

void te::layout::Visitable::unacceptVisitor( AbstractVisitor* visitor )
{
  if(!visitor)
    return;

  visitor->disassociate();
  m_visitors.erase(visitor);
}

void te::layout::Visitable::unacceptAllVisitor()
{
  std::set<AbstractVisitor*>::iterator it;

  it = m_visitors.begin();
  while(it != m_visitors.end())
  {
    AbstractVisitor* visitor = *it;
    ++it;
    unacceptVisitor(visitor);
  }
}

