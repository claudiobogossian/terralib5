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
  \file terralib/qt/widgets/rp/MosaicWizard.cpp

  \brief A Qt dialog that allows users to run a mosaic operation defined by RP module.
*/

// TerraLib 
#include "../../../common/progress/ProgressManager.h"
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../widgets/progress/ProgressViewerDialog.h"
#include "MosaicWizard.h"
#include "MosaicWizardPage.h"
#include "LayerSearchWidget.h"
#include "LayerSearchWizardPage.h"
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>


te::qt::widgets::MosaicWizard::MosaicWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Mosaic"));
  //this->setFixedSize(640, 480);

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

te::map::AbstractLayerPtr te::qt::widgets::MosaicWizard::getOutputLayer()
{
  return m_outputLayer;
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
        m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                     m_rasterInfoPage->getWidget()->getInfo());
      
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
    QMessageBox::warning(this, tr("Mosaic"), tr("An exception has occuried!"));

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
        m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                     m_rasterInfoPage->getWidget()->getInfo());
      
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
    QMessageBox::warning(this, tr("Mosaic"), tr("An exception has occuried!"));

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

  te::rp::SequenceMosaic::OutputParameters algoOutputParams = m_mosaicPage->getOutputSeqParams();
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
        algoOutputParams.reset();

        //set output layer
        m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                     m_rasterInfoPage->getWidget()->getInfo());
      
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
    QMessageBox::warning(this, tr("Mosaic"), tr("An exception has occuried!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  return true;
}
