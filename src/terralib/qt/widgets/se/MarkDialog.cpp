/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/MarkDialog.cpp

  \brief A dialog used to build a mark element.
*/

// TerraLib
#include "ui_MarkDialogForm.h"
#include "MarkDialog.h"
#include "MarkWidget.h"

te::qt::widgets::MarkDialog::MarkDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::MarkDialogForm)
{
  m_ui->setupUi(this);

  // Mark Widget
  m_markWidget = new te::qt::widgets::MarkWidget(this);

  // Adjusting...
  QGridLayout* markLayout = new QGridLayout(m_ui->m_markWidgetFrame);
  markLayout->addWidget(m_markWidget);
}

te::qt::widgets::MarkDialog::~MarkDialog()
{
}

te::se::Mark* te::qt::widgets::MarkDialog::getMark(const te::se::Mark* initialMark, QWidget* parent, const QString& title)
{
  MarkDialog dlg(parent);
  
  if(!title.isEmpty())
    dlg.setWindowTitle(title);
  
  if(initialMark)
    dlg.m_markWidget->setMark(initialMark);
  
  if(dlg.exec() == QDialog::Accepted)
    return dlg.getMark();
  
  return 0;
}

te::se::Mark* te::qt::widgets::MarkDialog::getMark() const
{
  return m_markWidget->getMark();
}
