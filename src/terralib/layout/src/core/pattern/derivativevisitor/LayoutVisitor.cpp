/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file LayoutVisitor.h
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutVisitor.h"
#include "LayoutVisitable.h"

te::layout::LayoutVisitor::LayoutVisitor() :
  m_visitable(0)
{

}

te::layout::LayoutVisitor::~LayoutVisitor( void )
{

}

void te::layout::LayoutVisitor::visit( LayoutVisitable* visitable )
{
  if(m_visitable)
  {
    if(visitable)
    {
      if(m_visitable != visitable)
      {
        m_visitable->unacceptVisitor(this);
        m_visitable = visitable;
      }
    }
    visitDependent();
  }
  else
  {
    if(visitable)
    {
      m_visitable = visitable;
      visitDependent();
    }
  }
}
