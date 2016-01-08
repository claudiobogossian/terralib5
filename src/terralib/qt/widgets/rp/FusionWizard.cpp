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
#include "../../../rp/WisperFusion.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "FusionWizard.h"
#include "FusionWizardPage.h"
#include "../raster/RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QMessageBox>
#include <QApplication>


te::qt::widgets::FusionWizard::FusionWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Fusion"));
  //this->setFixedSize(640, 480);

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_fusion.html");

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

  if(m_fusionPage->isWisperFusion())
    return executeWisper();

  return false;
}

bool te::qt::widgets::FusionWizard::executeIHS()
{
  //get layer lower
  std::list<te::map::AbstractLayerPtr> listLower = m_layerLowerSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr lLower = *listLower.begin();
  std::auto_ptr<te::da::DataSet> dsLower = lLower->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(dsLower.get(), te::dt::RASTER_TYPE);

  te::rst::Raster* inputRstLower = dsLower->getRaster(rpos).release();

  //get layer higher
  std::list<te::map::AbstractLayerPtr> listHigher = m_layerHigherSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr lHigher = *listHigher.begin();
  std::auto_ptr<te::da::DataSet> dsHigher = lHigher->getData();

  rpos = te::da::GetFirstPropertyPos(dsHigher.get(), te::dt::RASTER_TYPE);

  te::rst::Raster* inputRstHigher = dsHigher->getRaster(rpos).release();

  te::rst::Raster* rasterLower = 0;
  te::rst::Raster* rasterHigher = 0;

  adjustRasters(inputRstLower, inputRstHigher, rasterLower, rasterHigher);

  //run IHS Fusion
  te::rp::IHSFusion algorithmInstance;

  te::rp::IHSFusion::InputParameters algoInputParams = m_fusionPage->getInputIHSParams();
  algoInputParams.m_lowResRasterPtr = rasterLower;
  algoInputParams.m_highResRasterPtr = rasterHigher;

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

        delete rasterLower;
        delete rasterHigher;

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Fusion"), tr("Fusion initialization error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str() );

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      delete rasterLower;
      delete rasterHigher;

      return false;
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Fusion"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    delete rasterLower;
    delete rasterHigher;

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Fusion"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    delete rasterLower;
    delete rasterHigher;

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  delete rasterLower;
  delete rasterHigher;

  return true;
}

bool te::qt::widgets::FusionWizard::executePCA()
{
//get layer lower
  std::list<te::map::AbstractLayerPtr> listLower = m_layerLowerSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr lLower = *listLower.begin();
  std::auto_ptr<te::da::DataSet> dsLower = lLower->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(dsLower.get(), te::dt::RASTER_TYPE);

  te::rst::Raster* inputRstLower = dsLower->getRaster(rpos).release();

  //get layer higher
  std::list<te::map::AbstractLayerPtr> listHigher = m_layerHigherSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr lHigher = *listHigher.begin();
  std::auto_ptr<te::da::DataSet> dsHigher = lHigher->getData();

  rpos = te::da::GetFirstPropertyPos(dsHigher.get(), te::dt::RASTER_TYPE);

  te::rst::Raster* inputRstHigher = dsHigher->getRaster(rpos).release();

  te::rst::Raster* rasterLower = 0;
  te::rst::Raster* rasterHigher = 0;

  adjustRasters(inputRstLower, inputRstHigher, rasterLower, rasterHigher);


  //run PCA Fusion
  te::rp::PCAFusion algorithmInstance;

  te::rp::PCAFusion::InputParameters algoInputParams = m_fusionPage->getInputPCAParams();
  algoInputParams.m_lowResRasterPtr = rasterLower;
  algoInputParams.m_highResRasterPtr = rasterHigher;

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

        delete rasterLower;
        delete rasterHigher;

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Fusion"), tr("Fusion initialization error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str() );

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      delete rasterLower;
      delete rasterHigher;

      return false;
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Fusion"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    delete rasterLower;
    delete rasterHigher;

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Fusion"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    delete rasterLower;
    delete rasterHigher;

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  delete rasterLower;
  delete rasterHigher;

  return true;
}

bool te::qt::widgets::FusionWizard::executeWisper()
{
  //get layer lower
  std::list<te::map::AbstractLayerPtr> listLower = m_layerLowerSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr lLower = *listLower.begin();
  std::auto_ptr<te::da::DataSet> dsLower = lLower->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(dsLower.get(), te::dt::RASTER_TYPE);

  te::rst::Raster* inputRstLower = dsLower->getRaster(rpos).release();

  //get layer higher
  std::list<te::map::AbstractLayerPtr> listHigher = m_layerHigherSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr lHigher = *listHigher.begin();
  std::auto_ptr<te::da::DataSet> dsHigher = lHigher->getData();

  rpos = te::da::GetFirstPropertyPos(dsHigher.get(), te::dt::RASTER_TYPE);

  te::rst::Raster* inputRstHigher = dsHigher->getRaster(rpos).release();

  te::rst::Raster* rasterLower = 0;
  te::rst::Raster* rasterHigher = 0;

  adjustRasters(inputRstLower, inputRstHigher, rasterLower, rasterHigher);

  //run Wisper Fusion
  te::rp::WisperFusion algorithmInstance;

  te::rp::WisperFusion::InputParameters algoInputParams = m_fusionPage->getInputWisperParams();
  algoInputParams.m_lowResRasterPtr = rasterLower;
  algoInputParams.m_highResRasterPtr = rasterHigher;

  te::rp::WisperFusion::OutputParameters algoOutputParams = m_fusionPage->getOutputWisperParams();
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

        delete rasterLower;
        delete rasterHigher;

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Fusion"), tr("Fusion initialization error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str() );

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      delete rasterLower;
      delete rasterHigher;

      return false;
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Fusion"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    delete rasterLower;
    delete rasterHigher;

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Fusion"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    delete rasterLower;
    delete rasterHigher;

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  delete rasterLower;
  delete rasterHigher;

  return true;
}

void te::qt::widgets::FusionWizard::adjustRasters(te::rst::Raster* rInLower, te::rst::Raster* rInHigher, te::rst::Raster*& rOutLower, te::rst::Raster*& rOutHigher)
{
  assert(rInLower);
  assert(rInHigher);

  if(!m_fusionPage->cropRasters())
  {
    rOutLower = rInLower;
    rOutHigher = rInHigher;

    return;
  }

  //get box
  te::gm::Envelope boxLower = *rInLower->getExtent();
  te::gm::Envelope boxHigher = *rInHigher->getExtent();

  //check if the box is equal
  if(boxLower.equals(boxHigher))
  {
    rOutLower = rInLower;
    rOutHigher = rInHigher;

    return;
  }

  //check if a reprojection is necessary
  bool reproject = rInLower->getSRID() != rInHigher->getSRID();

  if(reproject)
    boxHigher.transform(rInHigher->getSRID(), rInLower->getSRID());

  //check the intersection box
  if(!boxLower.intersects(boxHigher))
    return;

  te::gm::Envelope interBox = boxLower.intersection(boxHigher);

  //generate lower raster
  std::map<std::string, std::string> rLowerInfo;
  rLowerInfo["FORCE_MEM_DRIVER"] = "TRUE";
//   rLowerInfo["MEM_RASTER_NROWS"] = boost::lexical_cast<std::string>(interBox.getHeight() / rInLower->getResolutionX());
//   rLowerInfo["MEM_RASTER_NCOLS"] = boost::lexical_cast<std::string>(interBox.getWidth() / rInLower->getResolutionY());
//   rLowerInfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>(rInLower->getBandDataType(0));
//   rLowerInfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(rInLower->getNumberOfBands());

  rOutLower = rInLower->trim(&interBox, rLowerInfo);

  //generate higher raster
  std::map<std::string, std::string> rHigherInfo;
  rHigherInfo["FORCE_MEM_DRIVER"] = "TRUE";
//   rHigherInfo["MEM_RASTER_NROWS"] = boost::lexical_cast<std::string>(interBox.getHeight() / rInHigher->getResolutionX());
//   rHigherInfo["MEM_RASTER_NCOLS"] = boost::lexical_cast<std::string>(interBox.getWidth() / rInHigher->getResolutionY());
//   rHigherInfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>(rInHigher->getBandDataType(0));
//   rHigherInfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(rInHigher->getNumberOfBands());

  if(reproject)
  {
    boxHigher.transform(rInLower->getSRID(), rInHigher->getSRID());

    rOutHigher = rInHigher->transform(rInLower->getSRID(), boxHigher.getLowerLeftX(), boxHigher.getLowerLeftY(), boxHigher.getUpperRightX(), boxHigher.getUpperRightY(), rHigherInfo);
  }
  else
    rOutHigher = rInHigher->trim(&interBox, rHigherInfo);
}
