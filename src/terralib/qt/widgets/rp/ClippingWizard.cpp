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
#include <QActionGroup>


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
  m_clippingPage->clearCanvas();
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

void te::qt::widgets::ClippingWizard::setMapDisplay(te::qt::widgets::MapDisplay* mapDisplay)
{
  m_clippingPage->setMapDisplay(mapDisplay);
}

void te::qt::widgets::ClippingWizard::setActionGroup(QActionGroup* actionGroup)
{
  m_clippingPage->setActionGroup(actionGroup);
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
  //te::gm::Polygon* poly;

  m_clippingPage->getExtentClipping(env);

  if (!env.intersects(*inputRst->getExtent()))
  {
    QMessageBox::warning(this, tr("Clipping"), tr("Selected area do not intersects the raster extent."));
    return false;
  }

  std::map<std::string, std::string> info = m_rasterInfoPage->getWidget()->getInfo();

  //execute clipping
  te::rst::Raster* outputRst = inputRst->trim(&env, info);

  if (outputRst)
  {
    delete outputRst;

    //set output layer
    te::map::AbstractLayerPtr outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), m_rasterInfoPage->getWidget()->getInfo());

    m_outputLayer.push_back(outputLayer);

    emit addLayer(outputLayer);
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

  if (y + height > (int)inputRst->getNumberOfRows() ||
    x + width > (int)inputRst->getNumberOfColumns())
  {
    QMessageBox::warning(this, tr("Clipping"), tr("Selected area beyond the raster boundaries."));
    return false;
  }

  //execute clipping
  te::rst::Raster* outputRst = inputRst->resample(te::rst::NearestNeighbor, y, x, height, width, height, width, info);

  if (outputRst)
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

  if (inputRst->getSRID() != layer->getSRID())
    inputRst->getGrid()->setSRID(layer->getSRID());

  //get parameters
  std::auto_ptr< te::gm::GeometryCollection > geomColl;
  m_clippingPage->getLayerClipping(geomColl);

  if (geomColl->isEmpty())
    return false;

  if (geomColl->getSRID() != inputRst->getSRID())
  {
    try
    {
      geomColl->transform(inputRst->getSRID());
    }
    catch(...)
    {
      QMessageBox::critical(this, tr("Clipping"), tr("Layer reprojection failed."));
      return false;
    }
  }

  // Creating the output raster (single raster case)

  std::auto_ptr<te::rst::Raster> outputRst;

  if (m_clippingPage->isSingleRasterResult())
  {
    std::vector<te::rst::BandProperty*> bands;

    for (std::size_t t = 0; t < inputRst->getNumberOfBands(); ++t)
    {
      te::rst::BandProperty* b = new te::rst::BandProperty(t, inputRst->getBand(t)->getProperty()->getType());
      bands.push_back(b);
    }

    te::gm::Envelope* env = new te::gm::Envelope(*geomColl->getMBR());
    te::rst::Grid* grid = new te::rst::Grid(inputRst->getGrid()->getResolutionX(),
      inputRst->getGrid()->getResolutionY(), env, inputRst->getSRID());

    std::string type = m_rasterInfoPage->getWidget()->getType();

    std::map<std::string, std::string> rinfo =
      m_rasterInfoPage->getWidget()->getInfo();

    try
    {
      outputRst.reset(te::rst::RasterFactory::make(type, grid, bands, rinfo));
    }
    catch(...)
    {
      outputRst.reset();
    }
    if( outputRst.get() == 0 )
    {
      QMessageBox::critical(this, tr("Clipping"), tr("Output raster creation error."));
      return false;
    }

    te::rst::FillRaster(outputRst.get(), outputRst->getBand(0)->getProperty()->m_noDataValue);
  }

  // Breaking the geometry collection into single geometries

  std::vector< te::gm::Geometry* > singleGeometriesPtrs;
  te::gm::Multi2Single(geomColl.get(), singleGeometriesPtrs);

  // Iterating over each geometry

  te::gm::Polygon* polygon = 0;
  std::vector< std::complex< double > > doubleVec;
  unsigned int outRow = 0;
  unsigned int outCol = 0;
  te::gm::Coord2D inputCoord;
  te::gm::Coord2D outputCoord;

  for (std::size_t singleGeometriesPtrsIdx = 0; singleGeometriesPtrsIdx <
    singleGeometriesPtrs.size(); ++singleGeometriesPtrsIdx)
  {
    te::gm::Geometry* currGeomPtr = singleGeometriesPtrs[singleGeometriesPtrsIdx];

    if (currGeomPtr->getGeomTypeId() == te::gm::PolygonType)
    {
      // creating the output raster : for the case where multiple rasters
      // should be generated (one for each geometry )

      if (!m_clippingPage->isSingleRasterResult())
      {
        std::vector<te::rst::BandProperty*> bands;

        for (std::size_t t = 0; t < inputRst->getNumberOfBands(); ++t)
        {
          te::rst::BandProperty* b = new te::rst::BandProperty(t, inputRst->getBand(t)->getProperty()->getType());
          bands.push_back(b);
        }

        te::gm::Envelope* env = new te::gm::Envelope(*currGeomPtr->getMBR());
        te::rst::Grid* grid = new te::rst::Grid(inputRst->getGrid()->getResolutionX(),
          inputRst->getGrid()->getResolutionY(), env, inputRst->getSRID());

        std::string type = m_rasterInfoPage->getWidget()->getType();

        std::map<std::string, std::string> rinfo =
          m_rasterInfoPage->getWidget()->getInfo(singleGeometriesPtrsIdx);

        try
        {
          outputRst.reset(te::rst::RasterFactory::make(type, grid, bands, rinfo));
        }
        catch(...)
        {
          outputRst.reset();
        }
        if( outputRst.get() == 0 )
        {
          QMessageBox::critical(this, tr("Clipping"), tr("Output raster creation error."));
          return false;
        }
        
        te::rst::FillRaster(outputRst.get(), outputRst->getBand(0)->getProperty()->m_noDataValue);
      }

      // Iterating over the current geometry

      polygon = (te::gm::Polygon*)currGeomPtr;
      te::rst::PolygonIterator<double> it =
        te::rst::PolygonIterator<double>::begin(outputRst.get(), polygon);
      te::rst::PolygonIterator<double> itend =
        te::rst::PolygonIterator<double>::end(outputRst.get(), polygon);
      te::rst::Interpolator interp(inputRst.get(), te::rst::NearestNeighbor);
      const te::rst::Grid& inputGrid = *inputRst->getGrid();
      const te::rst::Grid& outputGrid = *outputRst->getGrid();

      while (it != itend)
      {
        outRow = it.getRow();
        outCol = it.getColumn();

        outputGrid.gridToGeo((double)outCol, (double)outRow, outputCoord.x,
          outputCoord.y);
        inputGrid.geoToGrid(outputCoord.x, outputCoord.y, inputCoord.x,
          inputCoord.y);

        interp.getValues(inputCoord.x, inputCoord.y, doubleVec);

        outputRst->setValues(outCol, outRow, doubleVec);

        ++it;
      }

      if (!m_clippingPage->isSingleRasterResult())
      {
        outputRst.reset();
        
        //set output layer
        m_outputLayer.push_back(te::qt::widgets::createLayer(
          m_rasterInfoPage->getWidget()->getType(),
          m_rasterInfoPage->getWidget()->getInfo(singleGeometriesPtrsIdx)));
      }
    }
  }

  //set output layer ( for single output raster mode )

  if (m_clippingPage->isSingleRasterResult())
  {
    outputRst.reset();
    
    m_outputLayer.push_back(te::qt::widgets::createLayer(
      m_rasterInfoPage->getWidget()->getType(),
      m_rasterInfoPage->getWidget()->getInfo()));
  }

  return true;
}
