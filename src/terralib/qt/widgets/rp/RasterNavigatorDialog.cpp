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
  \file terralib/qt/widgets/rp/RasterNavigatorDialog.cpp

  \brief This file defines a class for a Raster Navigator Dialog.
*/

// TerraLib
#include "RasterNavigatorWidget.h"
#include "RasterNavigatorDialog.h"
#include "ui_RasterNavigatorWidgetForm.h"

// Qt
#include <QGridLayout>

te::qt::widgets::RasterNavigatorDialog::RasterNavigatorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f)
{
//build form
  QGridLayout* layout = new QGridLayout(this);
  m_navigator.reset( new te::qt::widgets::RasterNavigatorWidget(this));
  layout->addWidget(m_navigator.get(), 0, 0);
  layout->setContentsMargins(0,0,0,0);
  layout->setSizeConstraint(QLayout::SetMinimumSize);
}

te::qt::widgets::RasterNavigatorDialog::~RasterNavigatorDialog()
{
}

te::qt::widgets::RasterNavigatorWidget* te::qt::widgets::RasterNavigatorDialog::getWidget()
{
  return m_navigator.get();
}

void te::qt::widgets::RasterNavigatorDialog::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  m_navigator->set(m_layer);
}

void te::qt::widgets::RasterNavigatorDialog::closeEvent(QCloseEvent* /*e*/)
{
  emit navigatorClosed();
}
