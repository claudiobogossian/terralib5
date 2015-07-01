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
  \file terralib/qt/widgets/se/BasicFillDialog.cpp

  \brief A dialog used to build a basic fill element.
*/

// TerraLib
#include "BasicFillDialog.h"
#include "BasicFillWidget.h"
#include "ui_BasicFillDialogForm.h"

te::qt::widgets::BasicFillDialog::BasicFillDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::BasicFillDialogForm)
{
  m_ui->setupUi(this);

  // Fill Widget
  m_fillWidget = new te::qt::widgets::BasicFillWidget(this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_fillWidgetFrame);
  layout->addWidget(m_fillWidget);
}

te::qt::widgets::BasicFillDialog::~BasicFillDialog()
{
}

te::se::Fill* te::qt::widgets::BasicFillDialog::getFill(const te::se::Fill* initial, QWidget* parent, const QString& title)
{
  BasicFillDialog dlg(parent);
  
  if(!title.isEmpty())
    dlg.setWindowTitle(title);
  
  if(initial)
    dlg.m_fillWidget->setFill(initial);
  
  if(dlg.exec() == QDialog::Accepted)
    return dlg.getFill();
  
  return 0;
}

te::se::Fill* te::qt::widgets::BasicFillDialog::getFill() const
{
  return m_fillWidget->getFill();
}
