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
  \file terralib/qt/widgets/se/GlyphMarkDialog.cpp

  \brief A dialog used to build a glyph mark element.
*/

// TerraLib
#include "GlyphMarkDialog.h"
#include "GlyphMarkWidget.h"
#include "ui_GlyphMarkDialogForm.h"

te::qt::widgets::GlyphMarkDialog::GlyphMarkDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::GlyphMarkDialogForm)
{
  m_ui->setupUi(this);

  // Glyph Mark Widget
  m_glyphMarkWidget = new te::qt::widgets::GlyphMarkWidget(this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_glyphMarkWidgetFrame);
  layout->addWidget(m_glyphMarkWidget);
}

te::qt::widgets::GlyphMarkDialog::~GlyphMarkDialog()
{
}

te::se::Mark* te::qt::widgets::GlyphMarkDialog::getMark(const te::se::Mark* initial, QWidget* parent, const QString& title)
{
  GlyphMarkDialog dlg(parent);
  
  if(!title.isEmpty())
    dlg.setWindowTitle(title);
  
  if(initial)
    dlg.m_glyphMarkWidget->setMark(initial);
  
  if(dlg.exec() == QDialog::Accepted)
    return dlg.getMark();
  
  return 0;
}

te::se::Mark* te::qt::widgets::GlyphMarkDialog::getMark() const
{
  return m_glyphMarkWidget->getMark();
}
