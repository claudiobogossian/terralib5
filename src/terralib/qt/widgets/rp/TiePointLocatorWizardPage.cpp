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
  \file terralib/qt/widgets/rp/TiePointLocatorWizardPage.cpp

  \brief This file defines a class for a tie point locator Wizard page.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"


#include "../../../raster/Grid.h"
#include "../../../raster/Raster.h"
#include "../../../se/Fill.h"
#include "../../../se/Mark.h"
#include "../../../se/Stroke.h"
#include "../../../se/Utils.h"
#include "../../widgets/canvas/Canvas.h"
#include "../../widgets/canvas/MapDisplay.h"
#include "../../widgets/Utils.h"
#include "TiePointLocatorWidget.h"
#include "TiePointLocatorWizardPage.h"
#include "ui_TiePointLocatorWidgetForm.h"

// Qt
#include <QGridLayout>
#include <QPixmap>

te::qt::widgets::TiePointLocatorWizardPage::TiePointLocatorWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
//build form
  QGridLayout* layout = new QGridLayout(this);
  m_tiePointWidget.reset( new te::qt::widgets::TiePointLocatorWidget(this));
  layout->addWidget(m_tiePointWidget.get(), 0, 0);


//configure page
  this->setTitle(tr("Tie Point Locator"));
  this->setSubTitle(tr("Used to acquire a set of tie points."));
}

te::qt::widgets::TiePointLocatorWizardPage::~TiePointLocatorWizardPage()
{
}

bool te::qt::widgets::TiePointLocatorWizardPage::isComplete() const
{
  return true;
}

te::qt::widgets::TiePointLocatorWidget* te::qt::widgets::TiePointLocatorWizardPage::getWidget()
{
  return m_tiePointWidget.get();
}

void te::qt::widgets::TiePointLocatorWizardPage::setReferenceLayer(te::map::AbstractLayerPtr layer)
{
  m_tiePointWidget->setReferenceLayer(layer);
}

void te::qt::widgets::TiePointLocatorWizardPage::setAdjustLayer(te::map::AbstractLayerPtr layer)
{
  m_tiePointWidget->setAdjustLayer(layer);
}
