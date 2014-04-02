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
  \file terralib/vp/qt/BasicGeographicOpWizard.cpp

  \brief A Qt dialog that allows users to run the basic geographic operations defined by VP module.
*/

// TerraLib 
#include "../../common/progress/ProgressManager.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"

#include "../../maptools/AbstractLayer.h"

#include "../../qt/widgets/help/HelpPushButton.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/layer/search/LayerSearchWidget.h"
#include "../../qt/widgets/layer/search/LayerSearchWizardPage.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"

#include "../Config.h"
#include "../Exception.h"

#include "BasicGeographicOpWizard.h"
#include "BasicGeographicOpWizardPage.h"
#include "BasicOpOutputWizardPage.h"
#include "BasicGeoOp.h"
#include "BasicGeoOpMemory.h"
#include "BasicGeoOpQuery.h"
//#include "Utils.h"




// STL
#include <cassert>

// Qt
#include <QtGui/QMessageBox>

// Boost
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::vp::BasicGeographicOpWizard::BasicGeographicOpWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Basic Geographic Operation"));
  this->setFixedSize(640, 510);

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  //helpButton->setPageReference("../../help.html");

  addPages();
}

te::vp::BasicGeographicOpWizard::~BasicGeographicOpWizard()
{

}

bool te::vp::BasicGeographicOpWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_basicGeoOpPage->setLayer(l);
      m_inLayer = l;
    }

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_basicOpOutputPage.get())
  {
    if(!m_basicOpOutputPage->getToFile())
    {
      if(!m_basicOpOutputPage->getDsInfoPtr())
      {
        QMessageBox::information(this, 
                                "Basic Geographic Operation", 
                                "Set the output data source before execute the operation.");
        return false;
      }
      if(m_basicOpOutputPage->getOutDsName() == "")
      {
        QMessageBox::information(this, 
                                "Basic Geographic Operation", 
                                "Set a Name for the output dataset before execute the operation.");
        return false;
      }
    }

    return execute();
  }

  return true;
}

void te::vp::BasicGeographicOpWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByGeom();
}

void te::vp::BasicGeographicOpWizard::setLayer(te::map::AbstractLayerPtr layer)
{
  removePage(m_layerSearchId);
  m_basicGeoOpPage->setLayer(layer);
}

te::map::AbstractLayerPtr te::vp::BasicGeographicOpWizard::getInLayer()
{
  return m_inLayer;
}

void te::vp::BasicGeographicOpWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_basicGeoOpPage.reset(new te::vp::BasicGeographicOpWizardPage(this));
  m_basicOpOutputPage.reset(new te::vp::BasicOpOutputWizardPage(this));

  m_layerSearchId = addPage(m_layerSearchPage.get());
  addPage(m_basicGeoOpPage.get());
  addPage(m_basicOpOutputPage.get());

  //for contrast only one layer can be selected
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(false);
}

bool te::vp::BasicGeographicOpWizard::execute()
{
  bool result;

  try
  {
    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inLayer.get());
    te::da::DataSourcePtr inDataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);

    //Params
    if(m_basicOpOutputPage->hasConvexHull())
      m_ops.push_back(te::vp::GeographicOperation::CONVEX_HULL);

    if(m_basicOpOutputPage->hasCentroid())
      m_ops.push_back(te::vp::GeographicOperation::CENTROID);

    if(m_basicOpOutputPage->hasMBR())
      m_ops.push_back(te::vp::GeographicOperation::MBR);

    if(m_basicOpOutputPage->hasArea())
      m_ops.push_back(te::vp::GeographicOperation::AREA);

    if(m_basicOpOutputPage->hasLine())
      m_ops.push_back(te::vp::GeographicOperation::LINE);

    if(m_basicOpOutputPage->hasPerimeter())
      m_ops.push_back(te::vp::GeographicOperation::PERIMETER);

// get the selected properties of input layer
    std::vector<std::string> geoProps = m_basicGeoOpPage->getSelectedProps();

    std::string outputdataset = m_basicOpOutputPage->getOutDsName();

// Verify output datasource
    if(m_basicOpOutputPage->getToFile())
    {
      boost::filesystem::path uri(m_basicOpOutputPage->getPath());
      if (boost::filesystem::exists(uri))
      {
        QMessageBox::information(this, "Basic Geographic Operation", "Output file already exists. Remove it or select a new name and try again.");
        return false;
      }

      std::size_t idx = outputdataset.find(".");

      if (idx != std::string::npos)
        outputdataset=outputdataset.substr(0,idx);
      
      std::map<std::string, std::string> dsinfo;
      dsinfo["URI"] = uri.string();

      std::auto_ptr<te::da::DataSource> dsOGR = te::da::DataSourceFactory::make("OGR");
      dsOGR->setConnectionInfo(dsinfo);
      dsOGR->open();
      if (dsOGR->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Basic Geographic Operation", "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.");
        return false;
      }

      // sera feito por algum tipo de factory
      //te::vp::BasicGeoOp* basicGeoOp = new te::vp::BasicGeoOpMemory();
      
      te::vp::BasicGeoOp* basicGeoOp = new te::vp::BasicGeoOpQuery();

      basicGeoOp->setInput(inDataSource, dsLayer->getData(), dsLayer->getSchema());
      basicGeoOp->setOutput(dsOGR, outputdataset);
      basicGeoOp->setParams(geoProps, m_ops);

      if (!basicGeoOp->paramsAreValid())
        result = false;
      else
        result = basicGeoOp->run();

      delete basicGeoOp;
    }
    else
    {
      m_outputDatasource = m_basicOpOutputPage->getDsInfoPtr();
      te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
      if (!aux)
      {
        QMessageBox::information(this, "Basic Geographic Operation", "The selected output datasource can not be accessed.");
        return false;
      }
      
      if (aux->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Basic Geographic Operation", "Dataset already exists. Remove it or select a new name and try again.");
        return false;
      }

      //Call Functions
      bool result;
      if (!result)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, "Basic Geographic Operation", "Error: could not generate the operation.");
        reject();
      }
    }

    // creating a layer for the result
    //te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());
    //
    //te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());
    //  
    //te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
    //m_outLayer = converter(dt);
  }
  catch(const std::exception& e)
  {
    this->setCursor(Qt::ArrowCursor);
    QMessageBox::information(this, "Basic Greographic Operation", e.what());

    //te::common::Logger::logDebug("vp", e.what());
    //te::common::ProgressManager::getInstance().removeViewer(id);
    return false;
  }

  return true;
}
