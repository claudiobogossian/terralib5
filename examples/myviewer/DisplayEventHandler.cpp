/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file MapDisplay.cpp

  \brief A widget to control the display of a set of layers.
 */

// TerraLib
#include "DisplayEventHandler.h"
#include "MyDisplay.h" 

// Qt
#include <QtGui/QResizeEvent>


QPixmap* DisplayEventHandler::getDisplayPixmap()
{
  QWidget* p = (QWidget*)parent();
  while(p)
  {
    if(p->objectName() == "MyDisplay")
      return ((te::qt::widgets::MapDisplay*)p)->getDisplayPixmap();
    p = (QWidget*)p->parent();
  }
  return NULL;
}

void DisplayEventHandler::setRepaint(bool s)
{
  QWidget* p = (QWidget*)parent();
  while(p)
  {
    if(p->objectName() == "MyDisplay")
    {
      ((MyDisplay*)p)->setRepaint(s);
      break;
    }
    p = (QWidget*)p->parent();
  }
}
