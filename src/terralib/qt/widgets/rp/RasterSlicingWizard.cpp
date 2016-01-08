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

#include "RasterSlicingWizard.h"

//Terralib
#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../rp/Functions.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "RasterSlicingWizardPage.h"
#include "RasterInfoWizardPage.h"
#include "../raster/RasterInfoWidget.h"
#include "Utils.h"


// Qt
#include <QApplication>
#include <QMessageBox>

te::qt::widgets::RasterSlicingWizard::RasterSlicingWizard(QWidget* parent) :
QWizard(parent)
{
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Raster Slicing"));

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_rasterslicing.html");

  addPages();
}

te::qt::widgets::RasterSlicingWizard::~RasterSlicingWizard()
{
}

bool te::qt::widgets::RasterSlicingWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr layerPtr = *list.begin();
      
      if( layerPtr.get() != 0 )
      {
        //get input raster
        std::auto_ptr<te::da::DataSet> ds = layerPtr->getData();

        if(ds.get())
        {
          std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

          std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

          if(inputRst.get())
          {
            m_wizardPage->setBandsNumber( inputRst->getNumberOfBands() );
          }
        }
        
      }
    }    
    
    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_wizardPage.get())
  {
    return true;
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }  

  return true;
}

void te::qt::widgets::RasterSlicingWizard::setList(std::list<te::map::AbstractLayerPtr> &layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();
}

te::map::AbstractLayerPtr te::qt::widgets::RasterSlicingWizard::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::RasterSlicingWizard::addPages()
{
  m_layerSearchPage.reset(new LayerSearchWizardPage(this));
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(false);
  addPage(m_layerSearchPage.get());

  m_wizardPage.reset(new RasterSlicingWizardPage(this));
  addPage(m_wizardPage.get());
  
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));
  addPage(m_rasterInfoPage.get());
}

bool te::qt::widgets::RasterSlicingWizard::execute()
{
  //get layer
  std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr l = *list.begin();
  std::auto_ptr<te::da::DataSet> ds = l->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);
  
  unsigned int inputRasterBand = 0;
  bool createPaletteRaster = false;
  unsigned int slicesNumber = 0;
  bool eqHistogram = false;
  m_wizardPage->getParameters( inputRasterBand, createPaletteRaster, slicesNumber, eqHistogram );
  
  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);
  
  // Execute slicing

  QApplication::setOverrideCursor(Qt::WaitCursor);
  std::auto_ptr< te::rst::Raster > outRasterPtr;

  if( te::rp::RasterSlicing( *inputRst, inputRasterBand, createPaletteRaster,
    slicesNumber, eqHistogram, m_rasterInfoPage->getWidget()->getInfo(),
    m_rasterInfoPage->getWidget()->getType(), true, 0, outRasterPtr ) )
  {
    outRasterPtr.reset();
    //set output layer
    m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                 m_rasterInfoPage->getWidget()->getInfo());
    
    QMessageBox::information(this, tr("Raster slicing"), tr("Raster slicing ended sucessfully"));
  }
  else
  {
    QMessageBox::critical(this, tr("Raster slicing"), tr("Raster slicing execution error.") +
      ( " " + te::rp::Module::getLastLogStr() ).c_str());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  return true;
}


