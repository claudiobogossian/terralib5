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
  \file terralib/qt/widgets/rp/ClassifierWizard.cpp

  \brief A Qt dialog that allows users to run a classifier operation defined by RP module.
*/

// TerraLib 
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/Classifier.h"
#include "../../../rp/Module.h"
#include "ClassifierWizard.h"
#include "ClassifierWizardPage.h"
#include "LayerSearchWidget.h"
#include "LayerSearchWizardPage.h"
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "RasterNavigatorWidget.h"
#include "RasterNavigatorWizardPage.h"

// STL
#include <cassert>

// Qt
#include <QtGui/QMessageBox>


te::qt::widgets::ClassifierWizard::ClassifierWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Classifier"));
  //this->setFixedSize(640, 480);

  addPages();
}

te::qt::widgets::ClassifierWizard::~ClassifierWizard()
{

}

bool te::qt::widgets::ClassifierWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_classifierPage->set(l);
      m_navigatorPage->set(l);
    }

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_classifierPage.get())
  {
    return m_classifierPage->isComplete();
  }
  else if(currentPage() == m_navigatorPage.get())
  {
    return m_navigatorPage->isComplete();
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }

  return true;
}

void te::qt::widgets::ClassifierWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
}

void te::qt::widgets::ClassifierWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_classifierPage.reset(new te::qt::widgets::ClassifierWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));
  m_navigatorPage.reset(new te::qt::widgets::RasterNavigatorWizardPage(this));

  addPage(m_layerSearchPage.get());
  addPage(m_classifierPage.get());
  addPage(m_navigatorPage.get());
  addPage(m_rasterInfoPage.get());

  //for contrast only one layer can be selected
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(false);

  //configure raster navigator
  m_navigatorPage->getWidget()->hidePickerTool(true);

  //connects
  connect(m_navigatorPage->getWidget(), SIGNAL(mapDisplayExtentChanged()), m_classifierPage.get(), SLOT(onMapDisplayExtentChanged()));
  connect(m_navigatorPage->getWidget(), SIGNAL(geomAquired(te::gm::Polygon*, te::qt::widgets::MapDisplay*)), 
    m_classifierPage.get(), SLOT(onGeomAquired(te::gm::Polygon*, te::qt::widgets::MapDisplay*)));
}

bool te::qt::widgets::ClassifierWizard::execute()
{
  //get layer
  std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr l = *list.begin();
  std::auto_ptr<te::da::DataSet> ds = l->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  //run contrast
  te::rp::Classifier algorithmInstance;

  te::rp::Classifier::InputParameters algoInputParams = m_classifierPage->getInputParams();
  algoInputParams.m_inputRasterPtr = inputRst.get();

  te::rp::Classifier::OutputParameters algoOutputParams = m_classifierPage->getOutputParams();
  algoOutputParams.m_rInfo = m_rasterInfoPage->getWidget()->getInfo();
  algoOutputParams.m_rType = m_rasterInfoPage->getWidget()->getType();


  if(algorithmInstance.initialize(algoInputParams))
  {
    if(algorithmInstance.execute(algoOutputParams))
    {
      algoOutputParams.reset();
      
      QMessageBox::information(this, tr("Classifier"), tr("Classifier ended sucessfully"));
    }
    else
    {
      QMessageBox::critical(this, tr("Classifier"), tr("Classifier execution error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str());
      return false;
    }
  }
  else
  {
    QMessageBox::critical(this, tr("Classifier"), tr("Classifier initialization error.") +
      ( " " + te::rp::Module::getLastLogStr() ).c_str() );
    return false;
  }

  return true;
}
