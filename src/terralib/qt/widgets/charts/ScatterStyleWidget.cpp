/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/scatterStyleWidget.cpp

  \brief A widget used to adjust a scatter's style.
*/

//Terralib
#include "../../../se/Graphic.h"
#include "../se/GraphicDialog.h"
#include "ScatterStyle.h"
#include "ScatterStyleWidget.h"
#include "ui_ScatterStyleWidgetForm.h"

// STL
#include <memory>

//QT
#include <QDialog>

te::qt::widgets::ScatterStyleWidget::ScatterStyleWidget(te::qt::widgets::ScatterStyle* initial, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ScatterStyleWidgetForm),
    m_scatterStyle(initial)
{
    m_ui->setupUi(this);

  if(!m_scatterStyle)
    m_scatterStyle = new te::qt::widgets::ScatterStyle(); 

// connect signal and slots
  connect(m_ui->m_plotStylePushButton, SIGNAL(clicked()), this, SLOT(onPlotStylePushButtonClicked()));
}

te::qt::widgets::ScatterStyleWidget::~ScatterStyleWidget()
{
  delete m_scatterStyle;
}

te::qt::widgets::ScatterStyle* te::qt::widgets::ScatterStyleWidget::getScatterStyle()
{
  return m_scatterStyle->clone();
}

void te::qt::widgets::ScatterStyleWidget::onPlotStylePushButtonClicked()
{
  std::auto_ptr<te::se::Graphic> graphic(te::qt::widgets::GraphicDialog::getGraphic(m_scatterStyle->getGraphic(), 0, "Scatter Style"));
  if(graphic.get())
    m_scatterStyle->setGraphic(graphic->clone());
}
