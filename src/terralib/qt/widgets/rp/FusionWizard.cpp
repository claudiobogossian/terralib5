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
  \file terralib/qt/widgets/rp/FusionWizard.cpp

  \brief A Qt dialog that allows users to run a fusion operation defined by RP module.
*/

// TerraLib 
#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/IHSFusion.h"
#include "../../../rp/Module.h"
#include "../../../rp/PCAFusion.h"
#include "../../widgets/progress/ProgressViewerDialog.h"
#include "FusionWizard.h"
#include "FusionWizardPage.h"
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


te::qt::widgets::FusionWizard::FusionWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Fusion"));
  //this->setFixedSize(640, 480);

  addPages();
}

te::qt::widgets::FusionWizard::~FusionWizard()
{

}

bool te::qt::widgets::FusionWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerLowerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerLowerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_fusionPage->setLower(l);
    }

    return m_layerLowerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_layerHigherSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerHigherSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_fusionPage->setHigher(l);
    }

    return m_layerHigherSearchPage->isComplete();
  }
  else if(currentPage() ==  m_fusionPage.get())
  {
    return m_fusionPage->isComplete();
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }

  return true;
}

void te::qt::widgets::FusionWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerLowerSearchPage->getSearchWidget()->setList(layerList);
  m_layerLowerSearchPage->getSearchWidget()->filterOnlyByRaster();

  m_layerHigherSearchPage->getSearchWidget()->setList(layerList);
  m_layerHigherSearchPage->getSearchWidget()->filterOnlyByRaster();
}

te::map::AbstractLayerPtr te::qt::widgets::FusionWizard::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::FusionWizard::addPages()
{
  m_layerLowerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_layerHigherSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_fusionPage.reset(new te::qt::widgets::FusionWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));

  addPage(m_layerLowerSearchPage.get());
  addPage(m_layerHigherSearchPage.get());
  addPage(m_fusionPage.get());
  addPage(m_rasterInfoPage.get());

  //for contrast only one layer can be selected
  m_layerLowerSearchPage->setSubTitle(tr("Allows selection of layers using filters for selection. Select the layer with a LOWER raster resolution."));
  m_layerLowerSearchPage->getSearchWidget()->enableMultiSelection(false);
  m_layerHigherSearchPage->setSubTitle(tr("Allows selection of layers using filters for selection. Select the layer with a HIGHER raster resolution."));
  m_layerHigherSearchPage->getSearchWidget()->enableMultiSelection(false);
}

bool te::qt::widgets::FusionWizard::execute()
{
  if(m_fusionPage->isIHSFusion())
    return executeIHS();

  if(m_fusionPage->isPCAFusion())
    return executePCA();

  return false;
}

bool te::qt::widgets::FusionWizard::executeIHS()
{
  //get layer lower
  std::list<te::map::AbstractLayerPtr> listLower = m_layerLowerSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr lLower = *listLower.begin();
  std::auto_ptr<te::da::DataSet> dsLower = lLower->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(dsLower.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> inputRstLower = dsLower->getRaster(rpos);

  //get layer higher
  std::list<te::map::AbstractLayerPtr> listHigher = m_layerHigherSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr lHigher = *listHigher.begin();
  std::auto_ptr<te::da::DataSet> dsHigher = lHigher->getData();

  rpos = te::da::GetFirstPropertyPos(dsHigher.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> inputRstHigher = dsHigher->getRaster(rpos);


  //run IHS Fusion
  te::rp::IHSFusion algorithmInstance;

  te::rp::IHSFusion::InputParameters algoInputParams = m_fusionPage->getInputIHSParams();
  algoInputParams.m_lowResRasterPtr = inputRstLower.get();
  algoInputParams.m_highResRasterPtr = inputRstHigher.get();

  te::rp::IHSFusion::OutputParameters algoOutputParams = m_fusionPage->getOutputIHSParams();
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
      
        QMessageBox::information(this, tr("Fusion"), tr("Fusion ended sucessfully"));
      }
      else
      {
        QMessageBox::critical(this, tr("Fusion"), tr("Fusion execution error.") +
          ( " " + te::rp::Module::getLastLogStr() ).c_str());

        te::common::ProgressManager::getInstance().removeViewer(id);

        QApplication::restoreOverrideCursor();

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Fusion"), tr("Fusion initialization error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str() );

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      return false;
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Fusion"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Fusion"), tr("An exception has occuried!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  return true;
}

bool te::qt::widgets::FusionWizard::executePCA()
{
//get layer lower
  std::list<te::map::AbstractLayerPtr> listLower = m_layerLowerSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr lLower = *listLower.begin();
  std::auto_ptr<te::da::DataSet> dsLower = lLower->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(dsLower.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> inputRstLower = dsLower->getRaster(rpos);

  //get layer higher
  std::list<te::map::AbstractLayerPtr> listHigher = m_layerHigherSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr lHigher = *listHigher.begin();
  std::auto_ptr<te::da::DataSet> dsHigher = lHigher->getData();

  rpos = te::da::GetFirstPropertyPos(dsHigher.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> inputRstHigher = dsHigher->getRaster(rpos);


  //run PCA Fusion
  te::rp::PCAFusion algorithmInstance;

  te::rp::PCAFusion::InputParameters algoInputParams = m_fusionPage->getInputPCAParams();
  algoInputParams.m_lowResRasterPtr = inputRstLower.get();
  algoInputParams.m_highResRasterPtr = inputRstHigher.get();

  te::rp::PCAFusion::OutputParameters algoOutputParams = m_fusionPage->getOutputPCAParams();
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
      
        QMessageBox::information(this, tr("Fusion"), tr("Fusion ended sucessfully"));
      }
      else
      {
        QMessageBox::critical(this, tr("Fusion"), tr("Fusion execution error.") +
          ( " " + te::rp::Module::getLastLogStr() ).c_str());

        te::common::ProgressManager::getInstance().removeViewer(id);

        QApplication::restoreOverrideCursor();

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Fusion"), tr("Fusion initialization error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str() );

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      return false;
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Fusion"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Fusion"), tr("An exception has occuried!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  return true;
}
