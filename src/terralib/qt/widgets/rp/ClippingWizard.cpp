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
  \file terralib/qt/widgets/rp/ClippingWizard.cpp

  \brief A Qt dialog that allows users to run a clipping operation defined by RP module.
*/

// TerraLib 
#include "../../../common/progress/ProgressManager.h"
#include "../../../geometry/Envelope.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryCollection.h"
#include "../../../raster/Grid.h"
#include "../../../raster/Interpolator.h"
#include "../../../raster/PositionIterator.h"
#include "../../../raster/Raster.h"
#include "../../../raster/RasterFactory.h"
#include "../../../raster/Utils.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "ClippingWizard.h"
#include "ClippingWizardPage.h"
#include "../raster/RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QApplication>
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

std::vector<te::map::AbstractLayerPtr> te::qt::widgets::ClippingWizard::getOutputLayers()
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

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  bool res = false;

  try
  {
    if(m_clippingPage->isExtentClipping())
      res = executeExtentClipping();
    else if(m_clippingPage->isDimensionClipping())
      res = executeDimensionClipping();
    else if(m_clippingPage->isLayerClipping())
      res = executeLayerClipping();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Clipping"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Clipping"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  return res;
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

  if(!env.intersects(*inputRst->getExtent()))
  {
    QMessageBox::warning(this, tr("Clipping"), tr("Selected area do not intersects the raster extent."));
    return false;
  }

  std::map<std::string, std::string> info = m_rasterInfoPage->getWidget()->getInfo();

  //execute clipping
  te::rst::Raster* outputRst = inputRst->trim(&env, info);

  if(outputRst)
  {
    delete outputRst;

    //set output layer
    m_outputLayer.push_back(te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                        m_rasterInfoPage->getWidget()->getInfo()));
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

  if(y + height > (int)inputRst->getNumberOfRows() ||
     x + width  > (int)inputRst->getNumberOfColumns())
  {
    QMessageBox::warning(this, tr("Clipping"), tr("Selected area beyond the raster boundaries."));
    return false;
  }
  
  //execute clipping
  te::rst::Raster* outputRst = inputRst->resample(te::rst::NearestNeighbor, y, x, height, width, height, width, info);

  if(outputRst)
  {
    delete outputRst;

    //set output layer
    m_outputLayer.push_back(te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                  m_rasterInfoPage->getWidget()->getInfo()));
  }

  return true;
}

bool te::qt::widgets::ClippingWizard::executeLayerClipping()
{
  //get raster
  te::map::AbstractLayerPtr layer = m_clippingPage->get();
  std::auto_ptr<te::da::DataSet> ds = layer->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  //get parameters
  std::auto_ptr< te::gm::GeometryCollection > geomColl;
  m_clippingPage->getLayerClipping(geomColl);

  if(geomColl->isEmpty())
    return false;
  
  if( geomColl->getSRID() != inputRst->getSRID() )
  {
    geomColl->transform( inputRst->getSRID() );
  }

  if(m_clippingPage->isSingleRasterResult())
  {
    //output raster
    std::vector<te::rst::BandProperty*> bands;
  
    for(std::size_t t = 0; t < inputRst->getNumberOfBands(); ++t)
    {
      te::rst::BandProperty* b = new te::rst::BandProperty(t, inputRst->getBand(t)->getProperty()->getType());
      bands.push_back(b);
    }

    te::gm::Envelope* env = new te::gm::Envelope(*geomColl->getMBR());
    te::rst::Grid* grid = new te::rst::Grid(inputRst->getGrid()->getResolutionX(), inputRst->getGrid()->getResolutionY(), env, inputRst->getSRID());

    std::string type = m_rasterInfoPage->getWidget()->getType();

    std::map<std::string, std::string> rinfo =  m_rasterInfoPage->getWidget()->getInfo();
    std::auto_ptr<te::rst::Raster> outputRst( te::rst::RasterFactory::make(type, grid, bands, rinfo) );
    te::rst::FillRaster(outputRst.get(), outputRst->getBand( 0 )->getProperty()->m_noDataValue );
    
    te::gm::Polygon* polygon = 0;
//     te::rst::PolygonIterator<double> it;
//     te::rst::PolygonIterator<double> itend;
    std::vector<double> doubleVec;
    te::gm::Coord2D inputCoord;
    te::gm::Coord2D outputCoord;

    for(std::size_t i = 0; i < geomColl->getNumGeometries(); ++i)
    {
      polygon = static_cast<te::gm::Polygon*> (geomColl->getGeometryN(i));

      te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(inputRst.get(), polygon);
      te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(inputRst.get(), polygon);

      while (it != itend)
      {
        inputRst->getValues(it.getColumn(), it.getRow(), doubleVec);

        inputCoord = inputRst->getGrid()->gridToGeo(it.getColumn(), it.getRow());
        outputCoord = outputRst->getGrid()->geoToGrid(inputCoord.x, inputCoord.y);
        outputCoord.x = te::rst::Round(outputCoord.x);
        outputCoord.y = te::rst::Round(outputCoord.y);

        if( 
            (
              ( outputCoord.x >= 0 )
              && 
              ( outputCoord.x < (int)outputRst->getNumberOfColumns() )
            ) 
            &&
            (
              ( outputCoord.y >= 0 )
              && 
              ( outputCoord.y < (int)outputRst->getNumberOfRows() )
            )
          )
        {
          outputRst->setValues(outputCoord.x, outputCoord.y, doubleVec);
        }

        ++it;
      }
    }

    outputRst.reset();

    //set output layer
    m_outputLayer.push_back(te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                        m_rasterInfoPage->getWidget()->getInfo()));
  }
  else
  {
    for(std::size_t i = 0; i < geomColl->getNumGeometries(); ++i)
    {
      //output raster
      std::vector<te::rst::BandProperty*> bands;
  
      for(std::size_t t = 0; t < inputRst->getNumberOfBands(); ++t)
      {
        te::rst::BandProperty* b = new te::rst::BandProperty(t, inputRst->getBand(t)->getProperty()->getType());
        bands.push_back(b);
      }

      te::gm::Envelope* env = new te::gm::Envelope(*geomColl->getMBR());
      te::rst::Grid* grid = new te::rst::Grid(inputRst->getGrid()->getResolutionX(), inputRst->getGrid()->getResolutionY(), env, inputRst->getSRID());

      std::string type = m_rasterInfoPage->getWidget()->getType();

      std::map<std::string, std::string> rinfo =  m_rasterInfoPage->getWidget()->getInfo(i);
      std::auto_ptr<te::rst::Raster> outputRst( te::rst::RasterFactory::make(type, grid, bands, rinfo) );
      
      te::rst::FillRaster(outputRst.get(), 255);

      te::gm::Polygon* polygon = static_cast<te::gm::Polygon*> (geomColl->getGeometryN(i));

      te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(inputRst.get(), polygon);
      te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(inputRst.get(), polygon);

      while (it != itend)
      {
        std::vector<double> doubleVec;
        inputRst->getValues(it.getColumn(), it.getRow(), doubleVec);

        te::gm::Coord2D inputCoord = inputRst->getGrid()->gridToGeo(it.getColumn(), it.getRow());
        te::gm::Coord2D outputCoord = outputRst->getGrid()->geoToGrid(inputCoord.x, inputCoord.y);

        if( (te::rst::Round(inputCoord.x) >= 0 && te::rst::Round(outputCoord.x) < (int)inputRst->getNumberOfColumns()) &&
            (te::rst::Round(inputCoord.y) >= 0 && te::rst::Round(outputCoord.y) < (int)inputRst->getNumberOfRows()))
          outputRst->setValues(te::rst::Round(outputCoord.x), te::rst::Round(outputCoord.y), doubleVec);

        ++it;
      }

      outputRst.reset();

      //set output layer
      m_outputLayer.push_back(te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                          m_rasterInfoPage->getWidget()->getInfo(i)));
    }
  }

  return true;
}

