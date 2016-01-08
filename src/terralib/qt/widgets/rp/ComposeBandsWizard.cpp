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
  \file terralib/qt/widgets/rp/ComposeBandsWizard.cpp

  \brief A Qt dialog that allows users to compose or decompose bands.
*/

// TerraLib 
#include "../../../common/StringUtils.h"
#include "../../../common/STLUtils.h"
#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/FeedersRaster.h"
#include "../../../rp/Functions.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "ComposeBandsWizard.h"
#include "ComposeBandsWizardPage.h"
#include "../raster/RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QMessageBox>
#include <QApplication>


te::qt::widgets::ComposeBandsWizard::ComposeBandsWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Compose / Decompose Bands"));

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_compose_bands.html");

  addPages();
}

te::qt::widgets::ComposeBandsWizard::~ComposeBandsWizard()
{

}

bool te::qt::widgets::ComposeBandsWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    m_composeBandsPage->setList(list);

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_composeBandsPage.get())
  {
    return m_composeBandsPage->isComplete();
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }

  return true;
}

void te::qt::widgets::ComposeBandsWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::ComposeBandsWizard::getOutputLayers()
{
  return m_outputLayerList;
}

void te::qt::widgets::ComposeBandsWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_composeBandsPage.reset(new te::qt::widgets::ComposeBandsWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));

  addPage(m_layerSearchPage.get());
  addPage(m_composeBandsPage.get());
  addPage(m_rasterInfoPage.get());

  //for contrast only one layer can be selected
  m_layerSearchPage->setSubTitle(tr("Allows selection of layers using filters for selection. Select one layer for DECOMPOSITION or multi layers for COMPOSITION."));
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(true);
}

bool te::qt::widgets::ComposeBandsWizard::execute()
{
  if(m_composeBandsPage->isCompose())
    return executeCompose();

  if(m_composeBandsPage->isDecompose())
    return executeDecompose();

  return false;
}

bool te::qt::widgets::ComposeBandsWizard::executeCompose()
{
  //get output parameters
  std::map<std::string, std::string> rinfo = m_rasterInfoPage->getWidget()->getInfo();
  std::string type = m_rasterInfoPage->getWidget()->getType();

  //get input parameters
  std::vector<const te::rst::Raster*> inputRasters;
  std::vector<unsigned int> inputRasterBands;
  te::rst::Interpolator::Method interpMethod;

  m_composeBandsPage->getComposeParameters(inputRasters, inputRasterBands, interpMethod);

  //set feeder
  te::rp::FeederConstRasterVector feeder(inputRasters);

  //execute
  std::auto_ptr<te::rst::Raster> outputRaster;

  QApplication::setOverrideCursor(Qt::WaitCursor);

  bool res = false;

  try
  {
    res = te::rp::ComposeBands(feeder, inputRasterBands, interpMethod, rinfo, type, outputRaster);
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Compose"), e.what());

    QApplication::restoreOverrideCursor();

    te::common::FreeContents(inputRasters);

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Compose"), tr("An exception has occurred!"));

    QApplication::restoreOverrideCursor();

    te::common::FreeContents(inputRasters);

    return false;
  }

  QApplication::restoreOverrideCursor();

  //create layer
  if(res)
  {
    outputRaster.reset();

    te::map::AbstractLayerPtr layer = te::qt::widgets::createLayer(type, rinfo);

    m_outputLayerList.push_back(layer);
  }

  //clear vector
  te::common::FreeContents(inputRasters);

  return res;
}

bool te::qt::widgets::ComposeBandsWizard::executeDecompose()
{
  //get output parameters
  std::string outputDataSetNamePath = m_rasterInfoPage->getWidget()->getPath();
  std::string outputDataSetNamePrefix = m_rasterInfoPage->getWidget()->getShortName();
  std::string outputDataSetNameExt = m_rasterInfoPage->getWidget()->getExtension();
  std::string type = m_rasterInfoPage->getWidget()->getType();
  std::vector< std::map<std::string, std::string> > outputRastersInfos;

  //get input parameters
  te::rst::Raster* inputRaster;
  std::vector<unsigned int> inputRasterBands;

  m_composeBandsPage->getDecomposeParameters(inputRaster, inputRasterBands);

  //set output names
  std::vector<std::string> outputDataSetNames;

  for(std::size_t t = 0; t < inputRasterBands.size(); ++t)
  {
    std::string name = outputDataSetNamePrefix;
                name += "_";
                name += te::common::Convert2String(inputRasterBands[t]);
                name += outputDataSetNameExt;

    outputDataSetNames.push_back(name);

    outputRastersInfos.push_back(m_rasterInfoPage->getWidget()->getInfo((int)t));
  }
  
  std::vector< boost::shared_ptr< te::rst::Raster > > outputRastersPtrs;

  //execute
  bool res = false;

  try
  {
    res = te::rp::DecomposeBands(*inputRaster, inputRasterBands, outputRastersInfos, type, outputRastersPtrs);
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Decompose"), e.what());

    QApplication::restoreOverrideCursor();

    outputRastersPtrs.clear();

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Decompose"), tr("An exception has occurred!"));

    QApplication::restoreOverrideCursor();

    outputRastersPtrs.clear();

    return false;
  }

  QApplication::restoreOverrideCursor();

  //create layer
  if(res)
  {
    outputRastersPtrs.clear();

    std::string type = m_rasterInfoPage->getWidget()->getType();

    for(std::size_t t = 0; t < outputDataSetNames.size(); ++t)
    {
      std::map<std::string, std::string> rinfo;
      rinfo["URI"] = outputDataSetNamePath + outputDataSetNames[t];

      te::map::AbstractLayerPtr layer = te::qt::widgets::createLayer(type, rinfo);

      m_outputLayerList.push_back(layer);
    }
  }

  return res;
}
