/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/rp/TiePointLocatorWizard.cpp

  \brief This file defines the TiePointLocatorWizard class.
*/

// TerraLib 
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../raster/Raster.h"
#include "LayerSearchWidget.h"
#include "LayerSearchWizardPage.h"
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "TiePointLocatorDialog.h"
#include "TiePointLocatorWizard.h"

// STL
#include <cassert>

// Qt
#include <QtGui/QMessageBox>


te::qt::widgets::TiePointLocatorWizard::TiePointLocatorWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Tie Point Locator"));
  this->setFixedSize(640, 480);

  addPages();
}

te::qt::widgets::TiePointLocatorWizard::~TiePointLocatorWizard()
{

}

bool te::qt::widgets::TiePointLocatorWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerRefPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerRefPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_tiePointLocatorDialog->setReferenceLayer(l);
    }

    return m_layerRefPage->isComplete();
  }
  else if(currentPage() ==  m_layerAdjPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerAdjPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_tiePointLocatorDialog->setAdjustLayer(l);
    }

    m_tiePointLocatorDialog->showMaximized();

    return m_layerAdjPage->isComplete();
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }

  return true;
}

void te::qt::widgets::TiePointLocatorWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerRefPage->getSearchWidget()->setList(layerList);
  m_layerAdjPage->getSearchWidget()->setList(layerList);
}

void te::qt::widgets::TiePointLocatorWizard::addPages()
{
  m_layerRefPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_layerAdjPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));
  m_tiePointLocatorDialog.reset(new te::qt::widgets::TiePointLocatorDialog(this));

  addPage(m_layerRefPage.get());
  addPage(m_layerAdjPage.get());
  addPage(m_rasterInfoPage.get());

  //for contrast only one layer can be selected
  m_layerRefPage->setSubTitle(tr("Allows selection of layers using filters for selection. Select the layer to be used as REFERENCE."));
  m_layerRefPage->getSearchWidget()->enableMultiSelection(false);
  m_layerAdjPage->setSubTitle(tr("Allows selection of layers using filters for selection. Select the layer to be used as ADJUST."));
  m_layerAdjPage->getSearchWidget()->enableMultiSelection(false);
}

bool te::qt::widgets::TiePointLocatorWizard::execute()
{
  return true;
}
