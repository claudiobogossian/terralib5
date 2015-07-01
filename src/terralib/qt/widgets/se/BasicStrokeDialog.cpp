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
  \file terralib/qt/widgets/se/BasicStrokeDialog.cpp

  \brief A dialog used to build a basic stroke element.
*/

// TerraLib
#include "BasicStrokeDialog.h"
#include "BasicStrokeWidget.h"
#include "ui_BasicStrokeDialogForm.h"

te::qt::widgets::BasicStrokeDialog::BasicStrokeDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::BasicStrokeDialogForm)
{
  m_ui->setupUi(this);

  // Stroke Widget
  m_strokeWidget = new te::qt::widgets::BasicStrokeWidget(this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_strokeWidgetFrame);
  layout->addWidget(m_strokeWidget);
}

te::qt::widgets::BasicStrokeDialog::~BasicStrokeDialog()
{
}

te::se::Stroke* te::qt::widgets::BasicStrokeDialog::getStroke(const te::se::Stroke* initial, QWidget* parent, const QString& title)
{
  BasicStrokeDialog dlg(parent);
  
  if(!title.isEmpty())
    dlg.setWindowTitle(title);
  
  if(initial)
    dlg.m_strokeWidget->setStroke(initial);
  
  if(dlg.exec() == QDialog::Accepted)
    return dlg.getStroke();
  
  return 0;
}

te::se::Stroke* te::qt::widgets::BasicStrokeDialog::getStroke() const
{
  return m_strokeWidget->getStroke();
}
