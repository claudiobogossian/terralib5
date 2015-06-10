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
  \file terralib/qt/widgets/rp/ContrastWizard.cpp

  \brief A Qt dialog that allows users to run a contrast operation defined by RP module.
*/

// TerraLib 
#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/Contrast.h"
#include "../../../rp/Module.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "ContrastWizard.h"
#include "ContrastWizardPage.h"
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QApplication>
#include <QMessageBox>


te::qt::widgets::ContrastWizard::ContrastWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Contrast"));
  //this->setFixedSize(640, 480);

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_contrast.html");

  addPages();
}

te::qt::widgets::ContrastWizard::~ContrastWizard()
{

}

bool te::qt::widgets::ContrastWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_contrastPage->set(l);
    }

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_contrastPage.get())
  {
     bool res = m_contrastPage->isComplete();

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

void te::qt::widgets::ContrastWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();
}

void te::qt::widgets::ContrastWizard::setLayer(te::map::AbstractLayerPtr layer)
{
  removePage(m_layerSearchId);

  m_contrastPage->set(layer);
}

te::map::AbstractLayerPtr te::qt::widgets::ContrastWizard::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::ContrastWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_contrastPage.reset(new te::qt::widgets::ContrastWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));

  m_layerSearchId = addPage(m_layerSearchPage.get());
  addPage(m_contrastPage.get());
  addPage(m_rasterInfoPage.get());

  //for contrast only one layer can be selected
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(false);
}

bool te::qt::widgets::ContrastWizard::execute()
{
  if(m_rasterInfoPage->getWidget()->fileExists())
  {
    QMessageBox::warning(this, tr("Contrast"), tr("File already exists."));
    return false;
  }

  //get layer
  te::map::AbstractLayerPtr l = m_contrastPage->get();
  std::auto_ptr<te::da::DataSet> ds = l->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  //run contrast
  te::rp::Contrast algorithmInstance;

  te::rp::Contrast::InputParameters algoInputParams = m_contrastPage->getInputParams();
  algoInputParams.m_inRasterPtr = inputRst.get();
  algoInputParams.m_enableProgress = true;

  te::rp::Contrast::OutputParameters algoOutputParams;
  algoOutputParams.m_createdOutRasterDSType = m_rasterInfoPage->getWidget()->getType();
  algoOutputParams.m_createdOutRasterInfo = m_rasterInfoPage->getWidget()->getInfo();
  algoOutputParams.m_outRasterBands = algoInputParams.m_inRasterBands;

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

        QMessageBox::information(this, tr("Contrast"), tr("Contrast enhencement ended sucessfully"));
      }
      else
      {
        QMessageBox::critical(this, tr("Contrast"), tr("Contrast enhencement execution error.") +
          ( " " + te::rp::Module::getLastLogStr() ).c_str());

        QApplication::restoreOverrideCursor();

        return false;
      }
    }
    else
    {
      QMessageBox::critical(this, tr("Contrast"), tr("Contrast enhencement initialization error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str() );

      QApplication::restoreOverrideCursor();

      return false;
    }
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Contrast"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Contrast"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  return true;
}
