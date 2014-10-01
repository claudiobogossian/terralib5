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
  \file terralib/qt/widgets/rp/ColorTransformWizard.cpp

  \brief A Qt dialog that allows users to run a colorTransform operation defined by RP module.
*/

// TerraLib 
#include "../../../common/progress/ProgressManager.h"
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/RasterFactory.h"
#include "../../../rp/Functions.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "ColorTransformWizard.h"
#include "ColorTransformWizardPage.h"
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QApplication>
#include <QMessageBox>

te::qt::widgets::ColorTransformWizard::ColorTransformWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Color Transform"));
  //this->setFixedSize(640, 480);

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_colorTransform.html");

  addPages();
}

te::qt::widgets::ColorTransformWizard::~ColorTransformWizard()
{

}

bool te::qt::widgets::ColorTransformWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    m_colorTransformPage->setList(list);

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_colorTransformPage.get())
  {
    return m_colorTransformPage->isComplete();
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }

  return true;
}

void te::qt::widgets::ColorTransformWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(false);
}

te::map::AbstractLayerPtr te::qt::widgets::ColorTransformWizard::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::ColorTransformWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_colorTransformPage.reset(new te::qt::widgets::ColorTransformWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));

  addPage(m_layerSearchPage.get());
  addPage(m_colorTransformPage.get());
  addPage(m_rasterInfoPage.get());
}

bool te::qt::widgets::ColorTransformWizard::execute()
{
    //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
    bool res = false;

    if(m_colorTransformPage->isRGB2IHS())
      res =  executeRGB2IHS();
    else if(m_colorTransformPage->isIHS2RGB())
      res = executeIHS2RGB();

     if(res)
      {
        //set output layer
        m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                     m_rasterInfoPage->getWidget()->getInfo());
      
        QMessageBox::information(this, tr("Color Transform"), tr("Color Transform ended sucessfully"));
      }
      else
      {
        QMessageBox::critical(this, tr("Color Transform"), tr("Color Transform execution error.") +  ( " " + te::rp::Module::getLastLogStr() ).c_str());

        te::common::ProgressManager::getInstance().removeViewer(id);

        QApplication::restoreOverrideCursor();

        return false;
      }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Color Transform"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Color Transform"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  return true;
}

bool te::qt::widgets::ColorTransformWizard::executeRGB2IHS()
{
  te::rst::Raster* inputRaster = m_colorTransformPage->getRGBRaster();
  te::rst::Raster* outputRaster = buildOutputRaster();

  unsigned int rBand = m_colorTransformPage->getRGBRBand();
  unsigned int gBand = m_colorTransformPage->getRGBGBand();
  unsigned int bBand = m_colorTransformPage->getRGBBBand();

  double minValue = m_colorTransformPage->getRGBMinValue();
  double maxValue = m_colorTransformPage->getRGBMaxValue();

  bool res = te::rp::ConvertRGB2IHS(*inputRaster, rBand, gBand, bBand, minValue, maxValue, *outputRaster);

  delete inputRaster;
  delete outputRaster;

  return res;
}

bool te::qt::widgets::ColorTransformWizard::executeIHS2RGB()
{
  te::rst::Raster* inputRaster = m_colorTransformPage->getIHSRaster();
  te::rst::Raster* outputRaster = buildOutputRaster();

  unsigned int rBand = m_colorTransformPage->getIHSIBand();
  unsigned int gBand = m_colorTransformPage->getIHSHBand();
  unsigned int bBand = m_colorTransformPage->getIHSSBand();

  double minValue = m_colorTransformPage->getIHSMinValue();
  double maxValue = m_colorTransformPage->getIHSMaxValue();

  bool res = te::rp::ConvertIHS2RGB(*inputRaster, rBand, gBand, bBand, minValue, maxValue, *outputRaster);

  delete inputRaster;
  delete outputRaster;

  return res;
}

te::rst::Raster* te::qt::widgets::ColorTransformWizard::buildOutputRaster()
{
  te::rst::Raster* rasterOut;

  //check output data type
  int dataType = te::dt::UNKNOWN_TYPE;

  if(m_colorTransformPage->isRGB2IHS())
    dataType = te::dt::FLOAT_TYPE;
  else if(m_colorTransformPage->isIHS2RGB())
    dataType = te::dt::UCHAR_TYPE;

  //get output raster info
  std::string type = m_rasterInfoPage->getWidget()->getType();
  std::map<std::string, std::string> info = m_rasterInfoPage->getWidget()->getInfo();

  //get input raster
  te::rst::Raster* inputRaster;

  if(m_colorTransformPage->isRGB2IHS())
    inputRaster = m_colorTransformPage->getRGBRaster();
  else if(m_colorTransformPage->isIHS2RGB())
    inputRaster = m_colorTransformPage->getIHSRaster();

  //get input bands info
  std::vector<te::rst::BandProperty*> bands;

  for(std::size_t t = 0; t < inputRaster->getNumberOfBands(); ++t)
  {
    te::rst::BandProperty* bProp = new te::rst::BandProperty(*inputRaster->getBand(t)->getProperty());
    
    bProp->m_type = dataType;

    bands.push_back(bProp);

    if(t >= 2)
      break;
  }

  //get input grid
  te::rst::Grid* grid = new te::rst::Grid(*inputRaster->getGrid());

  //build output raster
  rasterOut = te::rst::RasterFactory::make(type, grid, bands, info);

  return rasterOut;
}
