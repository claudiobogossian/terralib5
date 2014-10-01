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
#include "Frame.h"
#include "LayoutEditor.h"
#include <QWidget>
#include <QToolTip>

te::qt::widgets::Frame::Frame(te::qt::widgets::LayoutEditor* me, Qt::WindowFlags f) :
  LayoutObject(me, f),
  m_selected(0),
  m_dragging(false),
  m_showToolTip(true),
  m_new(false)
{
}

te::qt::widgets::Frame::Frame(const Frame& rhs) :
  LayoutObject(rhs.m_layoutEditor, rhs.windowFlags())
{
  m_frameRect = rhs.m_frameRect;
  m_recSel1 = rhs.m_recSel1;
  m_recSel2 = rhs.m_recSel2;
  m_recSel3 = rhs.m_recSel3;
  m_recSel4 = rhs.m_recSel4;
  m_recSel5 = rhs.m_recSel5;
  m_recSel6 = rhs.m_recSel6;
  m_recSel7 = rhs.m_recSel7;
  m_recSel8 = rhs.m_recSel8;
  m_recSel9 = rhs.m_recSel9;
  m_recSel10 = rhs.m_recSel10;
  m_selected = rhs.m_selected;
  m_auxFrameRect = rhs.m_auxFrameRect;
  m_copyAuxFrameRect = rhs.m_copyAuxFrameRect;
  m_pressPoint = rhs.m_pressPoint;
  m_dragging = rhs.m_dragging;
  m_showToolTip = rhs.m_showToolTip;
  m_new = rhs.m_new;
}

te::qt::widgets::Frame::~Frame()
{
  hide();
}


te::qt::widgets::Frame& te::qt::widgets::Frame::operator=(const Frame& rhs)
{
  if(this != &rhs)
  {
    te::qt::widgets::LayoutObject::operator=(rhs);
    m_frameRect = rhs.m_frameRect;
    m_recSel1 = rhs.m_recSel1;
    m_recSel2 = rhs.m_recSel2;
    m_recSel3 = rhs.m_recSel3;
    m_recSel4 = rhs.m_recSel4;
    m_recSel5 = rhs.m_recSel5;
    m_recSel6 = rhs.m_recSel6;
    m_recSel7 = rhs.m_recSel7;
    m_recSel8 = rhs.m_recSel8;
    m_recSel9 = rhs.m_recSel9;
    m_recSel10 = rhs.m_recSel10;
    m_selected = rhs.m_selected;
    m_auxFrameRect = rhs.m_auxFrameRect;
    m_copyAuxFrameRect = rhs.m_copyAuxFrameRect;
    m_pressPoint = rhs.m_pressPoint;
    m_dragging = rhs.m_dragging;
    m_showToolTip = rhs.m_showToolTip;
    m_new = rhs.m_new;
  }

  return *this;
}

bool te::qt::widgets::Frame::isNew()
{
  return m_new;
}

void te::qt::widgets::Frame::setNew(bool b)
{
  m_new = b;
}

void te::qt::widgets::Frame::showToolTip(bool b)
{
  m_showToolTip = b;

  if(m_showToolTip == false)
    QToolTip::hideText();
}

QRectF te::qt::widgets::Frame::getFrameRect()
{
  return m_frameRect;
}
