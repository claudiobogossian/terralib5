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
  \file terralib/qt/widgets/rp/MixtureModelWizard.cpp

  \brief A Qt dialog that allows users to run a mixture model operation defined by RP module.
*/

// TerraLib 
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Raster.h"
#include "../../../rp/Functions.h"
#include "../../../rp/MixtureModel.h"
#include "../../../rp/Module.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "MixtureModelWizard.h"
#include "MixtureModelWizardPage.h"
#include "../raster/RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QApplication>
#include <QMessageBox>
#include <QActionGroup>

//STL
#include <memory>


te::qt::widgets::MixtureModelWizard::MixtureModelWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Mixture Model"));

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_mixture.html");

  addPages();
}

te::qt::widgets::MixtureModelWizard::~MixtureModelWizard()
{
  m_mixtureModelPage->clearCanvas();
}

bool te::qt::widgets::MixtureModelWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      //te::map::AbstractLayerPtr l = *list.begin();

      m_mixtureModelPage->set(list);
    }

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_mixtureModelPage.get())
  {
    m_mixtureModelPage->updateComponents();
    return m_mixtureModelPage->isComplete();
  }
  else if(currentPage() ==  m_rasterInfoPage.get())
  {
    return execute();
  }

  return true;
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::MixtureModelWizard::getOutputLayers()
{
  return m_outputLayerList;
}

void te::qt::widgets::MixtureModelWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();
}

void te::qt::widgets::MixtureModelWizard::setMapDisplay(te::qt::widgets::MapDisplay* mapDisplay)
{
  m_mixtureModelPage->setMapDisplay(mapDisplay);
}

void te::qt::widgets::MixtureModelWizard::setActionGroup(QActionGroup* actionGroup)
{
  m_mixtureModelPage->setActionGroup(actionGroup);
}

void te::qt::widgets::MixtureModelWizard::setLayer(std::list<te::map::AbstractLayerPtr> layers)
{
  removePage(m_layerSearchId);

  m_mixtureModelPage->set(layers);
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

bool te::qt::widgets::MixtureModelWizard::decompose(te::rp::MixtureModel::InputParameters &algoInputParams, te::rp::MixtureModel::OutputParameters &algoOutputParams)
{
  std::vector<unsigned int> inputRasterBands;
  unsigned int nbands = 0;

  //get output parameters
  std::string outputDataSetNamePath = m_rasterInfoPage->getWidget()->getPath();
  std::string outputDataSetNamePrefix = m_rasterInfoPage->getWidget()->getShortName();
  std::string outputDataSetNameExt = m_rasterInfoPage->getWidget()->getExtension();
  std::string type = m_rasterInfoPage->getWidget()->getType();
  std::vector< std::map<std::string, std::string> > outputRastersInfos;
  
  //set output names
  std::vector<std::string> outputDataSetNames;
  std::map<std::string, std::vector<double> >::iterator it;
  for (it = algoInputParams.m_components.begin(); it != algoInputParams.m_components.end(); it++)
  {
    std::string name = outputDataSetNamePrefix;
    name += "_";
    name += it->first;
    name += outputDataSetNameExt;

    outputDataSetNames.push_back(name);

    std::map<std::string, std::string> rinfo;

    std::string fileName =
      outputDataSetNamePath
      + std::string("/")
      + name;
    rinfo["URI"] = fileName;

    outputRastersInfos.push_back(rinfo);
    inputRasterBands.push_back(nbands++);
  }
  if (algoOutputParams.m_createErrorRaster)
  {
    for (it = algoInputParams.m_components.begin(); it != algoInputParams.m_components.end(); it++)
    {
      std::string name = outputDataSetNamePrefix;
      name += "_error_";
      name += it->first;
      name += outputDataSetNameExt;

      outputDataSetNames.push_back(name);
      std::map<std::string, std::string> rinfo;

      std::string fileName =
        outputDataSetNamePath
        + std::string("/")
        + name;
      rinfo["URI"] = fileName;

      outputRastersInfos.push_back(rinfo);
      inputRasterBands.push_back(nbands++);
    }
  }

  std::vector< boost::shared_ptr< te::rst::Raster > > outputRastersPtrs;

  bool res = false;

  try
  {
    res = te::rp::DecomposeBands(*algoOutputParams.m_outputRasterPtr, inputRasterBands, outputRastersInfos, type, outputRastersPtrs);
  }
  catch (const std::exception& e)
  {
    QMessageBox::warning(this, tr("Mixture Model - Decompose"), e.what());
    QApplication::restoreOverrideCursor();
    outputRastersPtrs.clear();
    return false;
  }
  catch (...)
  {
    QMessageBox::warning(this, tr("Mixture Model - Decompose"), tr("An exception has occurred!"));
    QApplication::restoreOverrideCursor();
    outputRastersPtrs.clear();
    return false;
  }

  QApplication::restoreOverrideCursor();

  //create layer
  if (res)
  {
    outputRastersPtrs.clear();
    std::string type = m_rasterInfoPage->getWidget()->getType();

    for (std::size_t t = 0; t < outputRastersInfos.size(); ++t)
    {
      std::map<std::string, std::string> rinfo = outputRastersInfos[t];
      te::map::AbstractLayerPtr layer = te::qt::widgets::createLayer(type, rinfo);
      m_outputLayerList.push_back(layer);
    }
  }
  return res;
}

bool te::qt::widgets::MixtureModelWizard::execute()
{
  if(m_rasterInfoPage->getWidget()->fileExists())
  {
    QMessageBox::warning(this, tr("Mixture Model"), tr("File already exists."));
    return false;
  }

  //get layer
  std::list<te::map::AbstractLayerPtr> l = m_mixtureModelPage->get();
  std::list<te::map::AbstractLayerPtr> ::iterator it_l;
  for (it_l = l.begin(); it_l != l.end(); it_l++)
  {
    std::auto_ptr<te::da::DataSet> ds(it_l->get()->getData());
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    //run mixture
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
        //set output layer
        //if (algoOutputParams.m_decomposeOutput)
        if (m_mixtureModelPage->isDecompose())
        { 
          decompose(algoInputParams, algoOutputParams);
        }
        else
        {
          algoOutputParams.reset();
          m_outputLayerList.push_back(te::qt::widgets::createLayer(m_rasterInfoPage->getWidget()->getType(),
            m_rasterInfoPage->getWidget()->getInfo()));
        }


        int addOutPutLayer = QMessageBox::question(this, tr("Mixture Model"), tr("Would you like to add layer on active project?"),
                                                   QMessageBox::Yes | QMessageBox::No);

        if (addOutPutLayer == QMessageBox::Yes)
        {
          std::list<te::map::AbstractLayerPtr>::iterator itl;
          for (itl = m_outputLayerList.begin(); itl != m_outputLayerList.end(); ++itl)
            emit addLayer(*itl);
        }

        QMessageBox::information(this, tr("Mixture Model"), tr("Mixture Model ended sucessfully."));
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
  }

  QApplication::restoreOverrideCursor();

  return true;
}

