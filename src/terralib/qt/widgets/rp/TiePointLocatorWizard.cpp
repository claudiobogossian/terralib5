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
#include "TiePointLocatorWizard.h"
#include "TiePointLocatorWizardPage.h"

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

      m_tiePointLocatorPage->setReferenceLayer(l);
    }

    return m_layerRefPage->isComplete();
  }
  else if(currentPage() ==  m_layerAdjPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerAdjPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_tiePointLocatorPage->setAdjustLayer(l);

      //show navigators
      m_tiePointLocatorPage->showReferenceNavigator(true);
      m_tiePointLocatorPage->showAdjustNavigator(true);
    }

    return m_layerAdjPage->isComplete();
  }
  else if(currentPage() ==  m_tiePointLocatorPage.get())
  {
    return m_tiePointLocatorPage->isComplete();
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
  m_tiePointLocatorPage.reset(new te::qt::widgets::TiePointLocatorWizardPage(this));

  addPage(m_layerRefPage.get());
  addPage(m_layerAdjPage.get());
  addPage(m_tiePointLocatorPage.get());

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
