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
#include "ui_scatterStyleWidgetForm.h"
#include "ScatterStyleWidget.h"
#include <PointSymbolizerWidget.h>
#include "ScatterStyleWidget.h"
#include "Symbol.h"

//QT
#include <qdialog.h>

te::qt::widgets::ScatterStyleWidget::ScatterStyleWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ScatterStyleWidgetForm)
{
    m_ui->setupUi(this);

// connect signal and slots

  connect(m_ui->m_plotStylePushButton, SIGNAL(clicked()), this, SLOT(onPlotStylePushButtonClicked()));
}

te::qt::widgets::ScatterStyleWidget::~ScatterStyleWidget(){}

te::qt::widgets::Symbol* te::qt::widgets::ScatterStyleWidget::getSymbol()
{
  return m_symbol;
}

void te::qt::widgets::ScatterStyleWidget::onPlotStylePushButtonClicked()
{
  QDialog dlg;
  dlg.setWindowTitle("Scatter Style");

  // Point Symbolizer Widget
  te::qt::widgets::PointSymbolizerWidget* pts = new te::qt::widgets::PointSymbolizerWidget(&dlg);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(&dlg);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(pts);

  dlg.exec();
  m_symbol->setSymbolizer(1, static_cast<PointSymbolizerWidget*>(pts)->getSymbolizer());
}
