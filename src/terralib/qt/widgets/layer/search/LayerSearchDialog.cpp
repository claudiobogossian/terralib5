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
  \file terralib/qt/widgets/layer/saerch/LayerSearchDialog.cpp

  \brief This file defines a class for a LayerSearchDialog.
*/

// TerraLib
#include "LayerSearchDialog.h"
#include "LayerSearchWidget.h"
#include "ui_LayerSearchDialogForm.h"


// Qt
#include <QGridLayout>

te::qt::widgets::LayerSearchDialog::LayerSearchDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::LayerSearchDialogForm)
{
//build form
  m_ui->setupUi(this);

  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  m_widget.reset( new te::qt::widgets::LayerSearchWidget(m_ui->m_widget));
  layout->addWidget(m_widget.get(), 0, 0);
  layout->setContentsMargins(0,0,0,0);
}

te::qt::widgets::LayerSearchDialog::~LayerSearchDialog()
{
}

te::qt::widgets::LayerSearchWidget* te::qt::widgets::LayerSearchDialog::getWidget()
{
  return m_widget.get();
}
