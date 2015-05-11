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
  \file AbstractRuler.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractRuler.h"
#include "../../core/PaperConfig.h"

te::layout::AbstractRuler::AbstractRuler() :
  m_visible(true),
  m_longLine(3.),
  m_mediumLine(2.5),
  m_smallLine(1.5),
  m_blockSize(10),
  m_middleBlockSize(5),
  m_smallBlockSize(1),
  m_height(7),
  m_cornerSize(7),
  m_spacingLineText(3.5)
{
  
}

te::layout::AbstractRuler::~AbstractRuler( void )
{

}

void te::layout::AbstractRuler::setVisible( bool visible )
{
  m_visible = visible;
}

bool te::layout::AbstractRuler::isVisible()
{
  return m_visible;
}

double te::layout::AbstractRuler::getLongLine()
{
  return m_longLine;
}

void te::layout::AbstractRuler::setLongLine( double size )
{
  m_longLine = size;
}

double te::layout::AbstractRuler::getMediumLine()
{
  return m_mediumLine;
}

void te::layout::AbstractRuler::setMediumLine( double size )
{
  m_mediumLine = size;
}

double te::layout::AbstractRuler::getSmallLine()
{
  return m_smallLine;
}

void te::layout::AbstractRuler::setSmallLine( double size )
{
  m_smallLine = size;
}

double te::layout::AbstractRuler::getBlockSize()
{
  return m_blockSize;
}

void te::layout::AbstractRuler::setBlockSize( double size )
{
  m_blockSize = size;
}

double te::layout::AbstractRuler::getMiddleBlockSize()
{
  return m_middleBlockSize;
}

void te::layout::AbstractRuler::setMiddleBlockSize( double size )
{
  m_middleBlockSize = size;
}

double te::layout::AbstractRuler::getSmallBlockSize()
{
  return m_smallBlockSize;
}

void te::layout::AbstractRuler::setSmallBlockSize( double size )
{
  m_smallBlockSize = size;
}

double te::layout::AbstractRuler::getHeight()
{
  return m_height;
}

void te::layout::AbstractRuler::setHeight( double size )
{
  m_height = size;
}

double te::layout::AbstractRuler::getCornerSize()
{
  return m_cornerSize;
}

void te::layout::AbstractRuler::setCornerSize( double size )
{
  m_cornerSize = size;
}

void te::layout::AbstractRuler::drawText( QPoint p, std::string txt, double angle, QPainter* painter )
{
  if(txt.empty())
    return;

  painter->translate(p);
  if(angle != 0.)
    painter->rotate(angle);
  
  painter->drawText(p, txt.c_str());
}

double te::layout::AbstractRuler::getSpacingLineText()
{
  return m_spacingLineText;
}

void te::layout::AbstractRuler::setSpacingLineText( double size )
{
  m_spacingLineText = size;
}
