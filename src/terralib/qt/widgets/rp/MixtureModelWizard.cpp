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
  \file terralib/qt/widgets/rp/MixtureModelWizard.cpp

  \brief A Qt dialog that allows users to run a mixture model operation defined by RP module.
*/

// TerraLib 
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/MixtureModel.h"
#include "../../../rp/Module.h"
#include "../../widgets/help/HelpPushButton.h"
#include "MixtureModelWizard.h"
#include "MixtureModelWizardPage.h"
#include "LayerSearchWidget.h"
#include "LayerSearchWizardPage.h"
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>

//STL
#include <memory>


te::qt::widgets::MixtureModelWizard::MixtureModelWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Mixture Model"));
  //this->setFixedSize(640, 480);

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_mixture.html");

  addPages();
}

te::qt::widgets::MixtureModelWizard::~MixtureModelWizard()
{

}

bool te::qt::widgets::MixtureModelWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_mixtureModelPage->set(l);
    }

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_mixtureModelPage.get())
  {
    return m_mixtureModelPage->isComplete();
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }

  return true;
}

te::map::AbstractLayerPtr te::qt::widgets::MixtureModelWizard::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::MixtureModelWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();
}

void te::qt::widgets::MixtureModelWizard::setLayer(te::map::AbstractLayerPtr layer)
{
  removePage(m_layerSearchId);

  m_mixtureModelPage->set(layer);
}

void te::qt::widgets::MixtureModelWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_mixtureModelPage.reset(new te::qt::widgets::MixtureModelWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));

  m_layerSearchId =  addPage(m_layerSearchPage.get());
  addPage(m_mixtureModelPage.get());
  addPage(m_rasterInfoPage.get());

  //for contrast only one layer can be selected
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(false);
}

bool te::qt::widgets::MixtureModelWizard::execute()
{
  if(m_rasterInfoPage->getWidget()->fileExists())
  {
    QMessageBox::warning(this, tr("Mixture Model"), tr("File already exists."));
    return false;
  }

  //get layer
  te::map::AbstractLayerPtr l = m_mixtureModelPage->get();
  std::auto_ptr<te::da::DataSet> ds(l->getData());
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  //run contrast
  te::rp::MixtureModel algorithmInstance;

  te::rp::MixtureModel::InputParameters algoInputParams = m_mixtureModelPage->getInputParams();
  
  if( algoInputParams.m_inputRasterBands.size() != algoInputParams.m_components.size() )
  {
    QMessageBox::critical(this, tr("Mixture Model"), tr("The number of components must be the same as the number of selected bands"));
    return false;
  }
  
  algoInputParams.m_inputRasterPtr = inputRst.get();

  te::rp::MixtureModel::OutputParameters algoOutputParams = m_mixtureModelPage->getOutputParams();
  algoOutputParams.m_rInfo = m_rasterInfoPage->getWidget()->getInfo();
  algoOutputParams.m_rType = m_rasterInfoPage->getWidget()->getType();

  QApplication::setOverrideCursor(Qt::WaitCursor);

  if(algorithmInstance.initialize(algoInputParams))
  {
    if(algorithmInstance.execute(algoOutputParams))
    {
      algoOutputParams.reset();
      
      //set output layer
      m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(), 
                                                   m_rasterInfoPage->getWidget()->getInfo());
      QMessageBox::information(this, tr("Mixture Model"), tr("Mixture Model ended sucessfully"));
    }
    else
    {
      QMessageBox::critical(this, tr("Mixture Model"), tr("Mixture Model execution error.") +
        ( " " + te::rp::Module::getLastLogStr() ).c_str() );

      QApplication::restoreOverrideCursor();

      return false;
    }
  }
  else
  {
    QMessageBox::critical(this, tr("Mixture Model"), tr( "Mixture Model initialization error." ) +
      ( " " + te::rp::Module::getLastLogStr() ).c_str() );

    QApplication::restoreOverrideCursor();

    return false;
  }

  QApplication::restoreOverrideCursor();

  return true;
}

