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
  \file terralib/qt/widgets/rp/RasterInfoDialog.cpp

  \brief This file defines a class for a RasterInfoDialog.
*/

// TerraLib
#include "RasterInfoDialog.h"
#include "RasterInfoWidget.h"
#include "ui_RasterInfoDialogForm.h"

// Qt
#include <QGridLayout>

te::qt::widgets::RasterInfoDialog::RasterInfoDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::RasterInfoDialogForm)
{
//build form
  m_ui->setupUi(this);

  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  m_widget.reset( new te::qt::widgets::RasterInfoWidget(true, m_ui->m_widget, 0));
  layout->addWidget(m_widget.get(), 0, 0);
  layout->setContentsMargins(0,0,0,0);
}

te::qt::widgets::RasterInfoDialog::~RasterInfoDialog()
{
}

te::qt::widgets::RasterInfoWidget* te::qt::widgets::RasterInfoDialog::getWidget()
{
  return m_widget.get();
}



