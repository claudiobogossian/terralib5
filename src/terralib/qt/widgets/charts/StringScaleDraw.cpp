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
  \file StringScaleDraw.cpp

  \brief A class to represent a histogram's label.
*/

//Terralib
#include "StringScaleDraw.h"
#include <iostream>

te::qt::widgets::StringScaleDraw::StringScaleDraw( std::set<QString>& labels ) :
QwtScaleDraw(),
m_labels(labels)
{  
}

te::qt::widgets::StringScaleDraw::StringScaleDraw( std::set<std::string>& labels ) :
QwtScaleDraw()
{  

  std::set <std::string>::iterator labelsIt;
  QString qstr;

  for (labelsIt = labels.begin(); labelsIt != labels.end(); labelsIt++)
  {
    qstr = qstr.fromStdString(*labelsIt);
    m_labels.insert(qstr);
  }
}

te::qt::widgets::StringScaleDraw::~StringScaleDraw()
{

}

std::set<QString>& te::qt::widgets::StringScaleDraw::getLabels()
{
  return m_labels;
}

void te::qt::widgets::StringScaleDraw::setLabels( std::set<QString> new_labels)
 {
    m_labels = new_labels;
 }

QwtText te::qt::widgets::StringScaleDraw::label( double value ) const
{
  QString lbl = " ";

  uint index = qRound( value );

  if(m_labels.empty() == false)
  { 
    size_t size = m_labels.size();
    if(index < size)
    {
      std::set<QString>::const_iterator it = m_labels.begin();

      //Increasing the position of the iterator so that it will end at the specified index
      while(index > 0)
      {
        --index;
        ++it;
      }
      lbl = *it;
      std::string test = lbl.toStdString();
    }
  }
  return  QwtText(lbl);
}