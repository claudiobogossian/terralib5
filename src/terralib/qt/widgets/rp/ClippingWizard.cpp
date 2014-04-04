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
  \file terralib/qt/widgets/rp/ClippingWizard.cpp

  \brief A Qt dialog that allows users to run a clipping operation defined by RP module.
*/

// TerraLib 
#include "../../../common/progress/ProgressManager.h"
#include "../../../geometry/Envelope.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../raster/Interpolator.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "ClippingWizard.h"
#include "ClippingWizardPage.h"
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QMessageBox>


te::qt::widgets::ClippingWizard::ClippingWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Clipping"));
  //this->setFixedSize(640, 580);

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_clipping.html");

  addPages();
}

te::qt::widgets::ClippingWizard::~ClippingWizard()
{

}

bool te::qt::widgets::ClippingWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_clippingPage->set(l);
    }

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_clippingPage.get())
  {
    bool res = m_clippingPage->isComplete();
    
    if(!res)
      QMessageBox::warning(this, tr("Warning"), tr("Select at least one band."));

    return res;
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }

  return true;
}

void te::qt::widgets::ClippingWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();

  m_clippingPage->setList(layerList);
}

void te::qt::widgets::ClippingWizard::setLayer(te::map::AbstractLayerPtr layer)
{
  m_clippingPage->set(layer);
}

te::map::AbstractLayerPtr te::qt::widgets::ClippingWizard::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::ClippingWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_clippingPage.reset(new te::qt::widgets::ClippingWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));

  addPage(m_layerSearchPage.get());
  addPage(m_clippingPage.get());
  addPage(m_rasterInfoPage.get());

  //for contrast only one layer can be selected
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(false);
}

bool te::qt::widgets::ClippingWizard::execute()
{
  if(m_rasterInfoPage->getWidget()->fileExists())
  {
    QMessageBox::warning(this, tr("Clipping"), tr("File already exists."));
    return false;
  }

  if(m_clippingPage->isExtentClipping())
    return executeExtentClipping();
  else if(m_clippingPage->isDimensionClipping())
    return executeDimensionClipping();
  else if(m_clippingPage->isLayerClipping())
    return executeLayerClipping();

  return false;
}

bool te::qt::widgets::ClippingWizard::executeExtentClipping()
{
  //get raster
  te::map::AbstractLayerPtr layer = m_clippingPage->get();
  std::auto_ptr<te::da::DataSet> ds = layer->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  //get parameters
  te::gm::Envelope env;

  m_clippingPage->getExtentClipping(env);

  std::map<std::string, std::string> info = m_rasterInfoPage->getWidget()->getInfo();

  //execute clipping
  te::rst::Raster* outputRst = inputRst->trim(&env, info);

  if(outputRst)
  {
    delete outputRst;

    //set output layer
    m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                  m_rasterInfoPage->getWidget()->getInfo());
  }
  return true;
}

bool te::qt::widgets::ClippingWizard::executeDimensionClipping()
{
  //get raster
  te::map::AbstractLayerPtr layer = m_clippingPage->get();
  std::auto_ptr<te::da::DataSet> ds = layer->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  //get parameters
  int x, y, width, height;

  m_clippingPage->getDimensionClipping(x, y, width, height);

  std::map<std::string, std::string> info = m_rasterInfoPage->getWidget()->getInfo();
  
  //execute clipping
  te::rst::Raster* outputRst = inputRst->resample(te::rst::Interpolator::NearestNeighbor, y, x, height, width, height, width, info);

  if(outputRst)
  {
    delete outputRst;

    //set output layer
    m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                  m_rasterInfoPage->getWidget()->getInfo());
  }

  return true;
}

bool te::qt::widgets::ClippingWizard::executeLayerClipping()
{
  return false;
}

