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
#include "EditorInfo.h"
#include "LayoutEditor.h"
#include <QWidget>

te::qt::widgets::EditorInfo::EditorInfo(te::qt::widgets::LayoutEditor* me, Qt::WindowFlags f) :
  LayoutObject(me, f)
{
  setWindowTitle("EditorInfo");
}

te::qt::widgets::EditorInfo::EditorInfo(const EditorInfo& rhs) :
  LayoutObject(rhs.m_layoutEditor, rhs.windowFlags())
{
  setWindowTitle("EditorInfo");
  m_rect = rhs.m_rect;
  m_paperViewRect = rhs.m_paperViewRect;
  m_paperSize = rhs.m_paperSize;
  m_showRulerGrid = rhs.m_showRulerGrid;
  m_rulerGridLines = rhs.m_rulerGridLines;
  m_resize = rhs.m_resize;
  m_move = rhs.m_move;
}

te::qt::widgets::EditorInfo::~EditorInfo()
{
  hide();
}


te::qt::widgets::EditorInfo& te::qt::widgets::EditorInfo::operator=(const EditorInfo& rhs)
{
  if(this != &rhs)
  {
    te::qt::widgets::LayoutObject::operator=(rhs);
    setWindowTitle("EditorInfo");

    m_rect = rhs.m_rect;
    m_paperViewRect = rhs.m_paperViewRect;
    m_paperSize = rhs.m_paperSize;
    m_showRulerGrid = rhs.m_showRulerGrid;
    m_rulerGridLines = rhs.m_rulerGridLines;
    m_resize = rhs.m_resize;
    m_move = rhs.m_move;
  }

  return *this;
}

