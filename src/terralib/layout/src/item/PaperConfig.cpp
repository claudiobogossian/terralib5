/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file PaperConfig.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PaperConfig.h"

te::layout::PaperConfig::PaperConfig() :
  m_paperOrientationType(Portrait),
  m_paperType(A4)
{
  
}

te::layout::PaperConfig::PaperConfig(LayoutAbstractPaperType paperType) :
  m_paperOrientationType(Portrait)
{
  m_paperType = paperType;
}

te::layout::PaperConfig::~PaperConfig()
{

}

void te::layout::PaperConfig::setPaperSize( LayoutAbstractPaperType paperType )
{
  m_paperType = paperType;
}

void te::layout::PaperConfig::getPaperSize( double &w, double &h )
{
  switch(m_paperType)
  {
    case Letter:
      w=216;
      h=279;
      break;
    case Legal:
      w=216;
      h=356;
      break;
    case Executive:
      w=191;
      h=254;
      break;
    case A0 :
      w=841;
      h=1189;
      break;
    case A1 :
      w=594;
      h=841;
      break;
    case A2 :
      w=420;
      h=594;
      break;
    case A3 :
      w=297;
      h=420;
      break;
    case A4 :
      w=210;
      h=297;
      break;
    case A5 :
      w=148;
      h=210;
      break;
    case A6 :
      w=105;
      h=148;
      break;
    case A7	:
      w=74;
      h=105;
      break;
    case A8 :
      w=52;
      h=74;
      break;
    case A9 :	
      w=37;
      h=52;
      break;
  }
}

void te::layout::PaperConfig::setPaperOrientation( LayoutOrientationType orientation )
{
  m_paperOrientationType = orientation;
}

te::layout::LayoutOrientationType te::layout::PaperConfig::getPaperOrientantion()
{
  return m_paperOrientationType;
}
