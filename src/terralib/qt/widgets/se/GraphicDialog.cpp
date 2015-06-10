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
  \file terralib/qt/widgets/se/GraphicDialog.cpp

  \brief A dialog used to build a graphic element.
*/

// TerraLib
#include "GraphicDialog.h"
#include "GraphicSelectorWidget.h"
#include "ui_GraphicDialogForm.h"

te::qt::widgets::GraphicDialog::GraphicDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::GraphicDialogForm)
{
  m_ui->setupUi(this);

  // Graphic Selector
  m_graphicSelector = new GraphicSelectorWidget(this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_graphicSelectorFrame);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(m_graphicSelector);
}

te::qt::widgets::GraphicDialog::~GraphicDialog()
{
}

te::se::Graphic* te::qt::widgets::GraphicDialog::getGraphic(const te::se::Graphic* initial, QWidget* parent, const QString& title)
{
  GraphicDialog dlg(parent);
  
  if(!title.isEmpty())
    dlg.setWindowTitle(title);

  if(initial)
    dlg.m_graphicSelector->setGraphic(initial);

  if(dlg.exec() == QDialog::Accepted)
    return dlg.getGraphic();
  
  return 0;
}

void te::qt::widgets::GraphicDialog::setGraphic(const te::se::Graphic* graphic)
{
  m_graphicSelector->setGraphic(graphic);
}

te::se::Graphic* te::qt::widgets::GraphicDialog::getGraphic() const
{
  return m_graphicSelector->getGraphic();
}

QIcon te::qt::widgets::GraphicDialog::getGraphicIcon(const QSize& size)
{
  return m_graphicSelector->getGraphicIcon(size);
}
