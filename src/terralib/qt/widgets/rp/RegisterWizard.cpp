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
  \file terralib/qt/widgets/rp/RegisterWizard.cpp

  \brief This file defines the RegisterWizard class.
*/

// TerraLib 
#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Grid.h"
#include "../../../raster/Raster.h"
#include "../../../rp/Register.h"
#include "../../../rp/Module.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "RegisterWizard.h"
#include "TiePointLocatorDialog.h"
#include "TiePointLocatorWidget.h"
#include "Utils.h"


// STL
#include <cassert>
#include <memory>

// Qt
#include <QApplication>
#include <QMessageBox>


te::qt::widgets::RegisterWizard::RegisterWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Register"));
  //this->setFixedSize(640, 480);

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_register.html");

  addPages();
}

te::qt::widgets::RegisterWizard::~RegisterWizard()
{

}

bool te::qt::widgets::RegisterWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerRefPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerRefPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      m_refLayer = *list.begin();

      m_tiePointLocatorDialog->setReferenceLayer(m_refLayer);
    }

    return m_layerRefPage->isComplete();
  }
  else if(currentPage() ==  m_layerAdjPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerAdjPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      m_adjLayer = *list.begin();

      m_tiePointLocatorDialog->setAdjustLayer(m_adjLayer);
    }

    m_tiePointLocatorDialog->showMaximized();

    return m_layerAdjPage->isComplete();
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    std::vector< te::gm::GTParameters::TiePoint > tiePoints;

    m_tiePointLocatorDialog->getWidget()->getTiePoints(tiePoints);

    if(tiePoints.empty())
    {
      QMessageBox::warning(this, tr("Register"), tr("Tie Points not aquired."));
      return false;
    }

    return execute();
  }

  return true;
}

void te::qt::widgets::RegisterWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerRefPage->getSearchWidget()->setList(layerList);
  m_layerAdjPage->getSearchWidget()->setList(layerList);

  m_layerRefPage->getSearchWidget()->filterOnlyByRaster();
  m_layerAdjPage->getSearchWidget()->filterOnlyByRaster();
}

te::map::AbstractLayerPtr te::qt::widgets::RegisterWizard::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::RegisterWizard::addPages()
{
  m_layerRefPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_layerAdjPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));
  m_tiePointLocatorDialog.reset(new te::qt::widgets::TiePointLocatorDialog(this));

  addPage(m_layerRefPage.get());
  addPage(m_layerAdjPage.get());
  addPage(m_rasterInfoPage.get());

  //for register only one layer can be selected
  m_layerRefPage->setSubTitle(tr("Allows selection of layers using filters for selection. Select the layer to be used as REFERENCE."));
  m_layerRefPage->getSearchWidget()->enableMultiSelection(false);
  m_layerAdjPage->setSubTitle(tr("Allows selection of layers using filters for selection. Select the layer to be used as ADJUST."));
  m_layerAdjPage->getSearchWidget()->enableMultiSelection(false);
}

bool te::qt::widgets::RegisterWizard::execute()
{
  if(m_rasterInfoPage->getWidget()->fileExists())
  {
    QMessageBox::warning(this, tr("Register"), tr("File already exists."));
    return false;
  }

  //get raster
  std::auto_ptr<te::da::DataSet> ds(m_adjLayer->getData());
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  std::vector<unsigned int> vec;
  for(size_t t = 0; t < inputRst->getNumberOfBands(); ++t)
  {
    vec.push_back((unsigned int)t);
  }

  std::vector< te::gm::GTParameters::TiePoint > tiePoints;
  m_tiePointLocatorDialog->getWidget()->getTiePoints(tiePoints);

  int srid;
  m_tiePointLocatorDialog->getWidget()->getOutputSRID(srid);

  double resX, resY;
  m_tiePointLocatorDialog->getWidget()->getOutputResolution(resX, resY);

  //input parameters
  te::rp::Register::InputParameters algoInputParams;
  algoInputParams.m_inputRasterPtr = inputRst.release();
  algoInputParams.m_inputRasterBands = vec;
  algoInputParams.m_tiePoints = tiePoints;
  algoInputParams.m_outputSRID = srid;
  algoInputParams.m_outputResolutionX = resX;
  algoInputParams.m_outputResolutionY = resY;

  //output parameters
  te::rp::Register::OutputParameters algoOutputParams;
  algoOutputParams.m_rType = m_rasterInfoPage->getWidget()->getType();
  algoOutputParams.m_rInfo = m_rasterInfoPage->getWidget()->getInfo();


  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
    te::rp::Register algorithmInstance;
    
    if(!algorithmInstance.initialize(algoInputParams))
    {
      QMessageBox::warning(this, tr("Register"), tr("Algorithm initialization error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str());

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      return false;
    }
    
    if(!algorithmInstance.execute(algoOutputParams))
    {
      QMessageBox::warning(this, tr("Register"), tr("Register Error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str());

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      return false;
    }
    else
    {
      QMessageBox::warning(this, tr("Register"), tr("Register Done!"));
      
      algoOutputParams.reset();

      //set output layer
      m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                   m_rasterInfoPage->getWidget()->getInfo());
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Register"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Register"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  return true;
}

