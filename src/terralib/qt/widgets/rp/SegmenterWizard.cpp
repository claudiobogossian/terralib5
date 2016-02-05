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
  \file terralib/qt/widgets/rp/SegmenterWizard.cpp

  \brief A Qt dialog that allows users to run a segmenter operation defined by RP module.
*/

// TerraLib 
#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/Segmenter.h"
#include "../../../rp/SegmenterRegionGrowingBaatzStrategy.h"
#include "../../../rp/SegmenterRegionGrowingMeanStrategy.h"
#include "../../../rp/Module.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "SegmenterWizard.h"
#include "SegmenterAdvancedOptionsWizardPage.h"
#include "SegmenterWizardPage.h"
#include "../raster/RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "ui_SegmenterAdvancedOptionsWizardPageForm.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QMessageBox>

// Boost
#include <boost/timer.hpp>
#include <boost/format.hpp>


te::qt::widgets::SegmenterWizard::SegmenterWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Segmenter"));
  //this->setFixedSize(640, 580);

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_segmenter.html");

  addPages();
}

te::qt::widgets::SegmenterWizard::~SegmenterWizard()
{

}

bool te::qt::widgets::SegmenterWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_segmenterPage->set(l);
    }

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_segmenterPage.get())
  {
    bool res = m_segmenterPage->isComplete();
    
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

void te::qt::widgets::SegmenterWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();
}

void te::qt::widgets::SegmenterWizard::setLayer(te::map::AbstractLayerPtr layer)
{
  removePage(m_layerSearchId);

  m_segmenterPage->set(layer);
}

te::map::AbstractLayerPtr te::qt::widgets::SegmenterWizard::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::SegmenterWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_segmenterPage.reset(new te::qt::widgets::SegmenterWizardPage(this));
  m_segmenterAdvOptPage.reset(new te::qt::widgets::SegmenterAdvancedOptionsWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));

  m_layerSearchId = addPage(m_layerSearchPage.get());
  addPage(m_segmenterPage.get());
  addPage(m_segmenterAdvOptPage.get());
  addPage(m_rasterInfoPage.get());

  //for contrast only one layer can be selected
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(false);
}

bool te::qt::widgets::SegmenterWizard::execute()
{
  if(m_rasterInfoPage->getWidget()->fileExists())
  {
    QMessageBox::warning(this, tr("Segmenter"), tr("File already exists."));
    return false;
  }

  //get layer
  te::map::AbstractLayerPtr l = m_segmenterPage->get();
  std::auto_ptr<te::da::DataSet> ds(l->getData());

  //run contrast
  te::rp::Segmenter algorithmInstance;

  te::rp::Segmenter::InputParameters algoInputParams = m_segmenterPage->getInputParams();

  algoInputParams.m_enableProgress = true;
  algoInputParams.m_enableThreadedProcessing = m_segmenterAdvOptPage->getForm()->m_enableThreadedProcessingcheckBox->isChecked();
  algoInputParams.m_maxSegThreads = m_segmenterAdvOptPage->getForm()->m_maximumThreadsNumberLineEdit->text().toUInt();
  algoInputParams.m_enableBlockProcessing = m_segmenterAdvOptPage->getForm()->m_enableBlockProcessingcheckBox->isChecked();
  algoInputParams.m_maxBlockSize = m_segmenterAdvOptPage->getForm()->m_maximumBlockSizeLineEdit->text().toUInt();
  algoInputParams.m_blocksOverlapPercent = m_segmenterAdvOptPage->getForm()->m_blockOverlapPercentSpinBox->value();

  te::rp::Segmenter::OutputParameters algoOutputParams;
  algoOutputParams.m_rType = m_rasterInfoPage->getWidget()->getType();
  algoOutputParams.m_rInfo = m_rasterInfoPage->getWidget()->getInfo();

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
    if(algorithmInstance.initialize(algoInputParams))
    {
      boost::timer timer;
      
      if(algorithmInstance.execute(algoOutputParams))
      {
        algoOutputParams.reset();
        
        //set output layer
        m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                     m_rasterInfoPage->getWidget()->getInfo());
        std::string elapsedTimeStr = boost::str( boost::format( "%.2f" ) % timer.elapsed() );
        QMessageBox::information(this, tr("Segmenter"), tr("Segmenter ended sucessfully") + ( " (" + elapsedTimeStr + " " ).c_str() + tr("seconds") + ")" );
      }
      else
      {
        QMessageBox::critical(this, tr("Segmenter"), tr("Segmenter execution error"));

        te::common::ProgressManager::getInstance().removeViewer(id);

        QApplication::restoreOverrideCursor();

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Segmenter"), tr("Segmenter initialization error") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str());

      te::common::ProgressManager::getInstance().removeViewer(id);

      QApplication::restoreOverrideCursor();

      return false;
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Segmenter"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Segmenter"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  return true;
}

