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
  \file ChartDisplay.cpp

  \brief A class to represent a chart display.
*/

// TerraLib
#include "../../../se.h"
#include "../../../color/RGBAColor.h"

//Qwt
#include <qwt_plot_grid.h>
#include <qwt_text.h>

//Qt
#include <QtGui/QPen>

#include "Utils.h"
#include "ChartDisplay.h"


te::qt::widgets::ChartDisplay::ChartDisplay(QWidget* parent) : 
  QwtPlot(parent),
  m_titleColor(0),
  m_titleFont(0),
  m_titleBackFill(0),
  m_titleBackStroke(0)
{
  //get default styles
  std::string title = "Teste Chart Display"; 
  setChartTitle(title);
  //==== teste com grid
  QwtPlotGrid *grid = new QwtPlotGrid;
  // mostra a grade horizontal
  grid->enableX(true);
  // mostra a grade vertical
  grid->enableY(true);
  //a grade (mais espacada) e' mostrada em preto e linha solido
  grid->setMajorPen(QPen(Qt::black, 0, Qt::SolidLine));
  //a grade e' mostrada em cinza e pontilhado
  grid->setMinorPen(QPen(Qt::gray, 0, Qt::DotLine));
  grid->attach(this);
}

void te::qt::widgets::ChartDisplay::setChartTitle(const std::string& title)
{
  QwtText* text = Terralib2Qwt(title);
  setTitle(*text);
  delete text;
}         

void te::qt::widgets::ChartDisplay::setChartTitle(const std::string& title,  te::color::RGBAColor* color, 
                   te::se::Font*  font, te::se::Fill* backFill, 
                   te::se::Stroke* backStroke)
{
  QwtText* text = Terralib2Qwt(title, color, font, backFill, backStroke);
  setTitle(*text);
  delete text;
}   

te::qt::widgets::ChartDisplay::~ChartDisplay()
{
  delete m_titleColor;
  delete m_titleFont;
  delete m_titleBackFill;
  delete m_titleBackStroke;
}
