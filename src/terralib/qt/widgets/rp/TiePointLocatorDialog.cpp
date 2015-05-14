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
#include "TiePointLocatorDialog.h"
#include "TiePointLocatorWidget.h"
#include "ui_TiePointLocatorWidgetForm.h"

// Qt
#include <QGridLayout>

te::qt::widgets::TiePointLocatorDialog::TiePointLocatorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f)
{
//build form
  QGridLayout* layout = new QGridLayout(this);
  m_widget.reset( new te::qt::widgets::TiePointLocatorWidget(this));
  layout->addWidget(m_widget.get(), 0, 0);
  layout->setContentsMargins(0,0,0,0);

  connect(m_widget.get(), SIGNAL(doneAcquiredTiePoints()), this, SLOT(onDoneAcquiredTiePoints()));

  this->setWindowTitle(tr("Tie Points Locator"));
}

te::qt::widgets::TiePointLocatorDialog::~TiePointLocatorDialog()
{
}

te::qt::widgets::TiePointLocatorWidget* te::qt::widgets::TiePointLocatorDialog::getWidget()
{
  return m_widget.get();
}

void te::qt::widgets::TiePointLocatorDialog::setReferenceLayer(te::map::AbstractLayerPtr layer)
{
  m_widget->setReferenceLayer(layer);
}

void te::qt::widgets::TiePointLocatorDialog::setAdjustLayer(te::map::AbstractLayerPtr layer)
{
  m_widget->setAdjustLayer(layer);
}

void  te::qt::widgets::TiePointLocatorDialog::onDoneAcquiredTiePoints()
{
  this->hide();
}


