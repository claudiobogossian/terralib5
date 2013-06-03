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
#include <QtGui/QMessageBox>


te::qt::widgets::MixtureModelWizard::MixtureModelWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Mixture Model"));
  this->setFixedSize(640, 480);

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
}

void te::qt::widgets::MixtureModelWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_mixtureModelPage.reset(new te::qt::widgets::MixtureModelWizardPage(this));
  m_rasterInfoPage.reset(new te::qt::widgets::RasterInfoWizardPage(this));

  addPage(m_layerSearchPage.get());
  addPage(m_mixtureModelPage.get());
  addPage(m_rasterInfoPage.get());

  //for contrast only one layer can be selected
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(false);
}

bool te::qt::widgets::MixtureModelWizard::execute()
{
  //get layer
  std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();
  te::map::AbstractLayerPtr l = *list.begin();
  std::auto_ptr<te::da::DataSet> ds(l->getData());
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  te::rst::Raster* inputRst = ds->getRaster(rpos);

  //run contrast
  te::rp::MixtureModel algorithmInstance;

  te::rp::MixtureModel::InputParameters algoInputParams = m_mixtureModelPage->getInputParams();
  algoInputParams.m_inputRasterPtr = inputRst;

  te::rp::MixtureModel::OutputParameters algoOutputParams = m_mixtureModelPage->getOutputParams();
  algoOutputParams.m_rInfo = m_rasterInfoPage->getWidget()->getInfo();
  algoOutputParams.m_rType = m_rasterInfoPage->getWidget()->getType();

  if(algorithmInstance.initialize(algoInputParams))
  {
    if(algorithmInstance.execute(algoOutputParams))
    {
      //set output layer
      m_outputLayer = te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getName(), 
                                                   m_rasterInfoPage->getWidget()->getInfo());
      QMessageBox::information(this, tr("Mixture Model"), tr("Mixture Model ended sucessfully"));
    }
    else
    {
      QMessageBox::critical(this, tr("Mixture Model"), tr("Mixture Model execution error"));
      return false;
    }
  }
  else
  {
    QMessageBox::critical(this, tr("Mixture Model"), tr("Mixture Model initialization error"));
    return false;
  }

  return true;
}
