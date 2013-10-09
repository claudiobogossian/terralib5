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
  \file terralib/qt/widgets/canvas/DataFrame.h

  \brief The frame data....
*/

// TerraLib
#include "LayoutObject.h"
#include "LayoutEditor.h"

te::qt::widgets::LayoutObject::LayoutObject(te::qt::widgets::LayoutEditor* me, Qt::WindowFlags f) :
  QWidget(me->getAuxWidget(), f),
  m_id(0),
  m_layoutEditor(me)
{
}

te::qt::widgets::LayoutObject::LayoutObject(const LayoutObject& rhs) :
  QWidget((QWidget*)rhs.parent(), rhs.windowFlags())
{
  m_id = rhs.m_id;
  m_layoutEditor = rhs.m_layoutEditor;
}

te::qt::widgets::LayoutObject::~LayoutObject()
{
  hide();
}

te::qt::widgets::LayoutObject& te::qt::widgets::LayoutObject::operator=(const LayoutObject& rhs)
{
  if(this != &rhs)
  {
    setParent((QWidget*)rhs.parent(), rhs.windowFlags());
    m_id = rhs.m_id;
    m_layoutEditor = rhs.m_layoutEditor;
  }

  return *this;
}

te::qt::widgets::LayoutEditor* te::qt::widgets::LayoutObject::getLayoutEditor()
{
  return m_layoutEditor;
}

unsigned int te::qt::widgets::LayoutObject::getId()
{
  return m_id;
}

void te::qt::widgets::LayoutObject::setId(unsigned int id)
{
  m_id = id;
}

void te::qt::widgets::LayoutObject::hide()
{
  QWidget::hide();
}

void te::qt::widgets::LayoutObject::show()
{
  QWidget::show();
}

void te::qt::widgets::LayoutObject::adjust()
{
}

void te::qt::widgets::LayoutObject::sendEventToChildren(bool b)
{
  if(b == true)
    removeEventFilter(m_layoutEditor);
  else
    installEventFilter(m_layoutEditor);
}

void te::qt::widgets::LayoutObject::showSelectionPoints()
{
}

void te::qt::widgets::LayoutObject::hideSelectionPoints()
{
}

double te::qt::widgets::LayoutObject::getScale()
{
  return 1;
}

void te::qt::widgets::LayoutObject::setScale(double)
{
}

bool te::qt::widgets::LayoutObject::isNew()
{
  return false;
}

void te::qt::widgets::LayoutObject::setNew(bool)
{
}

QRectF te::qt::widgets::LayoutObject::getFrameRect()
{
  return QRectF();
}

QPixmap* te::qt::widgets::LayoutObject::getPixmap()
{
  return 0;
}
