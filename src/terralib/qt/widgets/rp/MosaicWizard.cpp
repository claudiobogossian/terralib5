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
  \file terralib/qt/widgets/rp/MosaicWizard.cpp

  \brief A Qt dialog that allows users to run a mosaic operation defined by RP module.
*/

// TerraLib 
#include "../../../common/progress/ProgressManager.h"
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "MosaicWizard.h"
#include "MosaicWizardPage.h"
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QMessageBox>
#include <QApplication>


te::qt::widgets::MosaicWizard::MosaicWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Mosaic"));
  //this->setFixedSize(640, 480);

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_mosaic.html");

  addPages();
}

te::qt::widgets::MosaicWizard::~MosaicWizard()
{

}

bool te::qt::widgets::MosaicWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    m_mosaicPage->setList(list);

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_mosaicPage.get())
  {
    return m_mosaicPage->isComplete();
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }

  return true;
}

void te::qt::widgets::MosaicWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(true);
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::MosaicWizard::getOutputLayers()
{
  return m_outputLayerList;
}

void te::qt::widgets::MosaicWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_mosaicPage.reset(new te::qt::widgets::MosaicWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));

  addPage(m_layerSearchPage.get());
  addPage(m_mosaicPage.get());
  addPage(m_rasterInfoPage.get());
}

bool te::qt::widgets::MosaicWizard::execute()
{
  if(m_mosaicPage->isGeoMosaic())
    return executeGeoMosaic();
  else if(m_mosaicPage->isTiePointMosaic())
    return executeTiePointMosaic();
  else if(m_mosaicPage->isSequenceMosaic())
    return executeSequenceMosaic();

  return false;
}

bool te::qt::widgets::MosaicWizard::executeGeoMosaic()
{
//run Geo Mosaic
  te::rp::GeoMosaic algorithmInstance;

  te::rp::GeoMosaic::InputParameters algoInputParams = m_mosaicPage->getInputGeoParams();

  //get rasters
  std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

  std::list<te::map::AbstractLayerPtr>::iterator it = list.begin();
  std::vector<std::size_t> bands;

  std::vector<const te::rst::Raster*> rasters;

  while(it != list.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSet> ds = l->getData();
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    te::rst::Raster* rst = ds->getRaster(rpos).release();

    rasters.push_back(rst);

    std::vector<unsigned int> bands;
    
    for(unsigned int i = 0; i < rst->getNumberOfBands(); ++i)
      bands.push_back(i);

    algoInputParams.m_inputRastersBands.push_back( bands );

    ++it;
  }

  te::rp::FeederConstRasterVector feeder(rasters);
  algoInputParams.m_feederRasterPtr = &feeder;
  algoInputParams.m_enableProgress = true;

  te::rp::GeoMosaic::OutputParameters algoOutputParams = m_mosaicPage->getOutputGeoParams();
  algoOutputParams.m_rInfo = m_rasterInfoPage->getWidget()->getInfo();
  algoOutputParams.m_rType = m_rasterInfoPage->getWidget()->getType();

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
    if(algorithmInstance.initialize(algoInputParams))
    {
      if(algorithmInstance.execute(algoOutputParams))
      {
        algoOutputParams.reset();

        //set output layer
        te::map::AbstractLayerPtr outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                     m_rasterInfoPage->getWidget()->getInfo());

        m_outputLayerList.push_back(outputLayer);
      
        QMessageBox::information(this, tr("Mosaic"), tr("Mosaic ended sucessfully"));
      }
      else
      {
        QMessageBox::critical(this, tr("Mosaic"), tr("Mosaic execution error.") +
          ( " " + te::rp::Module::getLastLogStr() ).c_str());

        te::common::ProgressManager::getInstance().removeViewer(id);

        QApplication::restoreOverrideCursor();

        te::common::FreeContents(rasters);

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Mosaic"), tr("Mosaic initialization error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str() );

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      te::common::FreeContents(rasters);

      return false;
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Mosaic"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    te::common::FreeContents(rasters);

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Mosaic"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    te::common::FreeContents(rasters);

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  te::common::FreeContents(rasters);

  return true;
}

bool te::qt::widgets::MosaicWizard::executeTiePointMosaic()
{
//run Tie Point Mosaic
  te::rp::TiePointsMosaic algorithmInstance;

  te::rp::TiePointsMosaic::InputParameters algoInputParams = m_mosaicPage->getInputTPParams();
  algoInputParams.m_enableProgress = true;

//get rasters
  std::list<te::map::AbstractLayerPtr> list;
  list.push_back(m_mosaicPage->getTiePointMosaicLayerA());
  list.push_back(m_mosaicPage->getTiePointMosaicLayerB());

  std::list<te::map::AbstractLayerPtr>::iterator it = list.begin();
  std::vector<std::size_t> bands;

  std::vector<const te::rst::Raster*> rasters;

  while(it != list.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSet> ds = l->getData();
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    te::rst::Raster* rst = ds->getRaster(rpos).release();

    rasters.push_back(rst);

    std::vector<unsigned int> bands;
    
    for(unsigned int i = 0; i < rst->getNumberOfBands(); ++i)
      bands.push_back(i);

    algoInputParams.m_inputRastersBands.push_back( bands );

    ++it;
  }

  te::rp::FeederConstRasterVector feeder(rasters);
  algoInputParams.m_feederRasterPtr = &feeder;

  te::rp::TiePointsMosaic::OutputParameters algoOutputParams = m_mosaicPage->getOutputTPParams();
  algoOutputParams.m_rInfo = m_rasterInfoPage->getWidget()->getInfo();
  algoOutputParams.m_rType = m_rasterInfoPage->getWidget()->getType();

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
    if(algorithmInstance.initialize(algoInputParams))
    {
      if(algorithmInstance.execute(algoOutputParams))
      {
        algoOutputParams.reset();

        //set output layer
        te::map::AbstractLayerPtr outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                     m_rasterInfoPage->getWidget()->getInfo());

        m_outputLayerList.push_back(outputLayer);
      
        QMessageBox::information(this, tr("Mosaic"), tr("Mosaic ended sucessfully"));
      }
      else
      {
        QMessageBox::critical(this, tr("Mosaic"), tr("Mosaic execution error.") +
          ( " " + te::rp::Module::getLastLogStr() ).c_str());

        te::common::ProgressManager::getInstance().removeViewer(id);

        QApplication::restoreOverrideCursor();

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Mosaic"), tr("Mosaic initialization error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str() );

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      return false;
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Mosaic"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Mosaic"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  return true;
}

bool te::qt::widgets::MosaicWizard::executeSequenceMosaic()
{
//run Sequence Mosaic
  te::rp::SequenceMosaic algorithmInstance;

  te::rp::SequenceMosaic::InputParameters algoInputParams = m_mosaicPage->getInputSeqParams();

  //get rasters
  std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

  std::list<te::map::AbstractLayerPtr>::iterator it = list.begin();
  std::vector<std::size_t> bands;

  std::vector<const te::rst::Raster*> rasters;

  while(it != list.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSet> ds = l->getData();
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    te::rst::Raster* rst = ds->getRaster(rpos).release();

    rasters.push_back(rst);

    std::vector<unsigned int> bands;
    
    for(unsigned int i = 0; i < rst->getNumberOfBands(); ++i)
      bands.push_back(i);

    algoInputParams.m_inputRastersBands.push_back( bands );

    ++it;
  }

  te::rp::FeederConstRasterVector feeder(rasters);
  algoInputParams.m_feederRasterPtr = &feeder;

  algoInputParams.m_outDataSetsNamePrefix = m_rasterInfoPage->getWidget()->getShortName();
  algoInputParams.m_outDataSetsNameSufix = m_rasterInfoPage->getWidget()->getExtension();

  std::auto_ptr<te::da::DataSource> ds = m_rasterInfoPage->getWidget()->getDataSource();

  te::rp::SequenceMosaic::OutputParameters algoOutputParams = m_mosaicPage->getOutputSeqParams();
  algoOutputParams.m_outputDSPtr = ds.get();
  //algoOutputParams.m_rInfo = m_rasterInfoPage->getWidget()->getInfo();
  //algoOutputParams.m_rType = m_rasterInfoPage->getWidget()->getType();

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
    if(algorithmInstance.initialize(algoInputParams))
    {
      if(algorithmInstance.execute(algoOutputParams))
      {
        for(std::size_t t = 0; t < algoOutputParams.m_sequencesInfo.size(); ++t)
        {
          std::string name = algoOutputParams.m_sequencesInfo[t].m_dataSetName;
          std::string path = m_rasterInfoPage->getWidget()->getPath();

          std::map<std::string, std::string> rinfo;
          rinfo["URI"] = path + name;

          te::map::AbstractLayerPtr outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), rinfo);

          m_outputLayerList.push_back(outputLayer);
        }

        algoOutputParams.reset();
      
        QMessageBox::information(this, tr("Mosaic"), tr("Mosaic ended sucessfully"));
      }
      else
      {
        QMessageBox::critical(this, tr("Mosaic"), tr("Mosaic execution error.") +
          ( " " + te::rp::Module::getLastLogStr() ).c_str());

        te::common::ProgressManager::getInstance().removeViewer(id);

        QApplication::restoreOverrideCursor();

        te::common::FreeContents(rasters);

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Mosaic"), tr("Mosaic initialization error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str() );

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      te::common::FreeContents(rasters);

      return false;
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Mosaic"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    te::common::FreeContents(rasters);

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Mosaic"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    te::common::FreeContents(rasters);

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  te::common::FreeContents(rasters);

  return true;
}
