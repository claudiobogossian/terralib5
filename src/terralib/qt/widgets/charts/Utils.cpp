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
  \file Utils.cpp

  \brief This file contains a set of utility chart functions
*/

// TerraLib
#include "../../../se.h"
#include "Utils.h"

//QT
#include <QPen>


QwtText* te::qt::widgets::Terralib2Qwt(const std::string& text)
{
  QwtText* result = new QwtText(text.c_str()); 
  result->setBackgroundBrush(QBrush(QColor(0, 255, 0)));
  result->setBackgroundPen(QPen(Qt::red, 3, Qt::SolidLine));
  return result;
}

QwtText* te::qt::widgets::Terralib2Qwt(const std::string& text,  te::color::RGBAColor* color, 
                   te::se::Font*  font, te::se::Fill* backFill, 
                   te::se::Stroke* backStroke)
{
  QwtText* result = new QwtText(QString(text.c_str()));
 
  /*
  QColor color
  if(color==0)
  result->setColor();
  result->setFont();

  result->setBackgroundBrush();
  result->setBackgroundPen();
  */
  return result;
}

