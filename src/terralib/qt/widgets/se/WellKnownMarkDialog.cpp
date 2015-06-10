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
  \file terralib/qt/widgets/se/WellKnownMarkDialog.cpp

  \brief A dialog used to build a well known mark element.
*/

// TerraLib
#include "ui_WellKnownMarkDialogForm.h"
#include "WellKnownMarkDialog.h"
#include "WellKnownMarkWidget.h"

te::qt::widgets::WellKnownMarkDialog::WellKnownMarkDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::WellKnownMarkDialogForm)
{
  m_ui->setupUi(this);

  // Mark Widget
  m_markWidget = new te::qt::widgets::WellKnownMarkWidget(this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_markWidgetFrame);
  layout->addWidget(m_markWidget);
}

te::qt::widgets::WellKnownMarkDialog::~WellKnownMarkDialog()
{
}

te::se::Mark* te::qt::widgets::WellKnownMarkDialog::getMark(const te::se::Mark* initial, QWidget* parent, const QString& title)
{
  WellKnownMarkDialog dlg(parent);
  
  if(!title.isEmpty())
    dlg.setWindowTitle(title);
  
  if(initial)
    dlg.m_markWidget->setMark(initial);
  
  if(dlg.exec() == QDialog::Accepted)
    return dlg.getMark();
  
  return 0;
}

te::se::Mark* te::qt::widgets::WellKnownMarkDialog::getMark() const
{
  return m_markWidget->getMark();
}
