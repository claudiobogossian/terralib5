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
  \file terralib/qt/widgets/se/SelectedChannelWidget.cpp

  \brief A dialog used to build a SelectedChannel element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "ContrastEnhancementWidget.h"
#include "SelectedChannelWidget.h"
#include "ui_SelectedChannelWidgetForm.h"

// Qt


// STL


te::qt::widgets::SelectedChannelWidget::SelectedChannelWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::SelectedChannelWidgetForm)
{
  m_ui->setupUi(this);

  //Constrast Enhancement Widget
  m_contrastWidget = new te::qt::widgets::ContrastEnhancementWidget(this);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_contrastEnhancementFrame);
  layout->addWidget(m_contrastWidget);
}

te::qt::widgets::SelectedChannelWidget::~SelectedChannelWidget()
{

}

void te::qt::widgets::SelectedChannelWidget::updateUi()
{
  
}
