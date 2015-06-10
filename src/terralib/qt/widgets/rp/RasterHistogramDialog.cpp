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
  \file terralib/qt/widgets/rp/RasterHistogramDialog.cpp

  \brief This file defines a class for a RasterHistogram Dialog.
*/

// TerraLib
#include "RasterHistogramDialog.h"
#include "RasterHistogramWidget.h"
#include "ui_RasterHistogramWidgetForm.h"
#include "ui_RasterHistogramDialogForm.h"

// Qt
#include <QGridLayout>

te::qt::widgets::RasterHistogramDialog::RasterHistogramDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::RasterHistogramDialogForm)
{
  m_ui->setupUi(this);

//build form
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  m_widgetRasterHistogram.reset( new te::qt::widgets::RasterHistogramWidget(m_ui->m_widget));
  layout->addWidget(m_widgetRasterHistogram.get(), 0, 0);
  layout->setContentsMargins(0,0,0,0);

  this->setWindowTitle(tr("RasterHistogram"));

  this->resize(640, 480);
}

te::qt::widgets::RasterHistogramDialog::~RasterHistogramDialog()
{
}

te::qt::widgets::RasterHistogramWidget* te::qt::widgets::RasterHistogramDialog::getWidget()
{
  return m_widgetRasterHistogram.get();
}
