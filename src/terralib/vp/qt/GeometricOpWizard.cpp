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
  \file terralib/vp/qt/GeometricOpWizard.cpp

  \brief A Qt dialog that allows users to run the basic geographic operations defined by VP module.
*/

// TerraLib 
#include "../Enums.h"
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

#include "GeometricOpWizard.h"
#include "GeometricOpWizardPage.h"
#include "GeometricOpOutputWizardPage.h"
#include "../GeometricOp.h"
#include "../GeometricOpMemory.h"
#include "../GeometricOpQuery.h"
//#include "Utils.h"

// STL
#include <cassert>

// Qt
#include <QMessageBox>

// Boost
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::vp::GeometricOpWizard::GeometricOpWizard(QWidget* parent)
  : QWizard(parent)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Geometric Operation"));
  this->setFixedSize(640, 420);

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  //helpButton->setPageReference("../../help.html");

  addPages();
}

te::vp::GeometricOpWizard::~GeometricOpWizard()
{

}

bool te::vp::GeometricOpWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();
      m_geomOpPage->setLayer(l);
      m_inLayer = l;

      std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();
      std::vector<te::dt::Property*> vecProp = dsType->getProperties();
      std::vector<std::string> vecPropName;
      
      for(std::size_t i = 0; i < vecProp.size(); ++i)
      {
        if(vecProp[i]->getType() != te::dt::GEOMETRY_TYPE)
          vecPropName.push_back(vecProp[i]->getName());
      }

      m_geomOpOutputPage->setAttributes(vecPropName);
    }

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_geomOpOutputPage.get())
  {
    if(!m_geomOpOutputPage->getToFile())
    {
      if(!m_geomOpOutputPage->getDsInfoPtr())
      {
        QMessageBox::information(this, 
                                "Basic Geographic Operation", 
                                "Set the output data source before execute the operation.");
        return false;
      }
      if(m_geomOpOutputPage->getOutDsName() == "")
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

void te::vp::GeometricOpWizard::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByGeom();
}

void te::vp::GeometricOpWizard::setLayer(te::map::AbstractLayerPtr layer)
{
  removePage(m_layerSearchId);
  m_geomOpPage->setLayer(layer);
}

te::map::AbstractLayerPtr te::vp::GeometricOpWizard::getInLayer()
{
  return m_inLayer;
}

te::map::AbstractLayerPtr te::vp::GeometricOpWizard::getOutLayer()
{
  return m_outLayer;
}

void te::vp::GeometricOpWizard::addPages()
{
  m_layerSearchPage.reset(new te::qt::widgets::LayerSearchWizardPage(this));
  m_geomOpPage.reset(new te::vp::GeometricOpWizardPage(this));
  m_geomOpOutputPage.reset(new te::vp::GeometricOpOutputWizardPage(this));

  m_layerSearchId = addPage(m_layerSearchPage.get());
  addPage(m_geomOpPage.get());
  addPage(m_geomOpOutputPage.get());

  //for contrast only one layer can be selected
  m_layerSearchPage->getSearchWidget()->enableMultiSelection(false);
}

bool te::vp::GeometricOpWizard::execute()
{
  bool result;

// progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  try
  {
    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inLayer.get());
    te::da::DataSourcePtr inDataSource = te::da::GetDataSource(dsLayer->getDataSourceId(), true);
    
    m_ops.clear();

    //Params
    if(m_geomOpOutputPage->hasConvexHull())
      m_ops.push_back(te::vp::CONVEX_HULL);

    if(m_geomOpOutputPage->hasCentroid())
      m_ops.push_back(te::vp::CENTROID);

    if(m_geomOpOutputPage->hasMBR())
      m_ops.push_back(te::vp::MBR);

    if(m_geomOpOutputPage->hasArea())
      m_ops.push_back(te::vp::AREA);

    if(m_geomOpOutputPage->hasLine())
      m_ops.push_back(te::vp::LINE);

    if(m_geomOpOutputPage->hasPerimeter())
      m_ops.push_back(te::vp::PERIMETER);

// get the selected properties of input layer
    std::vector<std::string> geoProps = m_geomOpPage->getSelectedProps();

    std::string outputdataset = m_geomOpOutputPage->getOutDsName();

//get the selected property if the operation is by attribute
    m_attribute = m_geomOpOutputPage->getAttribute();

// Verify output datasource
    if(m_geomOpOutputPage->getToFile())
    {
      boost::filesystem::path uri(m_geomOpOutputPage->getPath());
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

      this->setCursor(Qt::WaitCursor);

      // sera feito por algum tipo de factory
      te::vp::GeometricOp* geomOp = 0;

      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities dsCapabilities = inDataSource->getCapabilities();

      if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
      {
        geomOp = new te::vp::GeometricOpQuery();
      }
      else
      {
        geomOp = new te::vp::GeometricOpMemory();
      }

      geomOp->setInput(inDataSource, dsLayer->getDataSetName(), dsLayer->getSchema());
      geomOp->setOutput(dsOGR, outputdataset);
      geomOp->setParams(geoProps, 
                        m_ops, 
                        m_geomOpOutputPage->getObjectStrategy(), 
                        m_attribute, 
                        m_geomOpOutputPage->hasOutputLayer());

      if (!geomOp->paramsAreValid())
        result = false;
      else
        result = geomOp->run();

      if (!result)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, "Geometric Operation", "Error: could not generate the operation.");
        return false;
      }

      delete geomOp;

      // let's include the new datasource in the managers
      boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string id = boost::uuids::to_string(u);
      
      te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
      ds->setConnInfo(dsinfo);
      ds->setTitle(uri.stem().string());
      ds->setAccessDriver("OGR");
      ds->setType("OGR");
      ds->setDescription(uri.string());
      ds->setId(id);
      
      te::da::DataSourcePtr newds = te::da::DataSourceManager::getInstance().get(id, "OGR", ds->getConnInfo());
      newds->open();
      te::da::DataSourceInfoManager::getInstance().add(ds);
      m_outputDatasource = ds;
    }
    else
    {
      m_outputDatasource = m_geomOpOutputPage->getDsInfoPtr();
      std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make(m_outputDatasource->getType());
      trgDs->setConnectionInfo(m_outputDatasource->getConnInfo());
      trgDs->open();

      if (!trgDs.get())
      {
        QMessageBox::information(this, "Geometric Operation", "The selected output datasource can not be accessed.");
        return false;
      }
      
      if (trgDs->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, "Geometric Operation", "Dataset already exists. Remove it or select a new name and try again.");
        return false;
      }

      te::vp::GeometricOp* geomOp = 0;
      
      // select a strategy based on the capabilities of the input datasource
      const te::da::DataSourceCapabilities dsCapabilities = inDataSource->getCapabilities();

      if(dsCapabilities.supportsPreparedQueryAPI() && dsCapabilities.getQueryCapabilities().supportsSpatialSQLDialect())
      {
        geomOp = new te::vp::GeometricOpQuery();
      }
      else
      {
        geomOp = new te::vp::GeometricOpMemory();
      }

      geomOp->setInput(inDataSource, dsLayer->getDataSetName(), dsLayer->getSchema());
      geomOp->setOutput(trgDs, outputdataset);
      geomOp->setParams(geoProps,
                        m_ops,
                        m_geomOpOutputPage->getObjectStrategy(),
                        m_attribute,
                        m_geomOpOutputPage->hasOutputLayer());

      if (!geomOp->paramsAreValid())
        result = false;
      else
        result = geomOp->run();

      if (!result)
      {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::information(this, "Geometric Operation", "Error: could not generate the operation.");
        return false;
      }

      delete geomOp;
    }

// creating a layer for the result
    //te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());
    //te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());

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

  te::common::ProgressManager::getInstance().removeViewer(id);
  this->setCursor(Qt::ArrowCursor);

  return true;
}
