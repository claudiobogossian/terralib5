/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.
  
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

//Terralib
#include "../../../common/progress/ProgressManager.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype/SimpleProperty.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../memory/DataSet.h"
#include "../../../memory/DataSetItem.h"
#include "../../widgets/layer/utils/DataSet2Layer.h"
#include "../help/HelpPushButton.h"
#include "../layer/search/LayerSearchWidget.h"
#include "../layer/search/LayerSearchWizardPage.h"
#include "../progress/ProgressViewerDialog.h"
#include "VectorizationWizard.h"
#include "VectorizationWizardPage.h"

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Qt
#include <QApplication>
#include <QMessageBox>

te::qt::widgets::VectorizationWizard::VectorizationWizard(QWidget* parent) :
QWizard(parent)
{
  this->setWizardStyle(QWizard::ModernStyle);
  this->setWindowTitle(tr("Vectorization"));

  this->setOption(QWizard::HaveHelpButton, true);
  this->setOption(QWizard::HelpButtonOnRight, false);

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/rp/rp_vectorization.html");

  addPages();
}

te::qt::widgets::VectorizationWizard::~VectorizationWizard()
{
}

bool te::qt::widgets::VectorizationWizard::validateCurrentPage()
{
  if(currentPage() ==  m_layerSearchPage.get())
  {
    std::list<te::map::AbstractLayerPtr> list = m_layerSearchPage->getSearchWidget()->getSelecteds();

    if(list.empty() == false)
    {
      te::map::AbstractLayerPtr l = *list.begin();

      m_vectorPage->setLayer(l);
    }

    return m_layerSearchPage->isComplete();
  }
  else if(currentPage() ==  m_vectorPage.get())
  {
    return execute();
  }

  return true;
}

void te::qt::widgets::VectorizationWizard::setList(std::list<te::map::AbstractLayerPtr> &layerList)
{
  m_layerSearchPage->getSearchWidget()->setList(layerList);
  m_layerSearchPage->getSearchWidget()->filterOnlyByRaster();
}

te::map::AbstractLayerPtr te::qt::widgets::VectorizationWizard::getOutputLayer()
{
  return m_outputLayer;
}

void te::qt::widgets::VectorizationWizard::addPages()
{
  m_layerSearchPage.reset(new LayerSearchWizardPage(this));
  addPage(m_layerSearchPage.get());

  m_vectorPage.reset(new VectorizationWizardPage(this));
  addPage(m_vectorPage.get());
}

bool te::qt::widgets::VectorizationWizard::execute()
{
  //check output datasource parameters
  te::da::DataSourceInfoPtr outDSInfo;

  std::string outputdataset = m_vectorPage->getLayerName();

  bool toFile = m_vectorPage->outputDataSourceToFile();

  if(toFile)
  {
    boost::filesystem::path uri(m_vectorPage->getRepositoryName());

    std::size_t idx = outputdataset.find(".");
    if(idx != std::string::npos)
      outputdataset = outputdataset.substr(0,idx);

    std::map<std::string, std::string> dsinfo;
    dsinfo["URI"] = uri.string();

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
 
    te::da::DataSourceInfoManager::getInstance().add(ds);
    outDSInfo = ds;
  }
  else
  {
    outDSInfo = m_vectorPage->getDataSourceInfo();
  }

  //input parameters
  std::auto_ptr<te::rst::Raster> raster = m_vectorPage->getRaster();
  unsigned int band = m_vectorPage->getBand();
  unsigned int maxGeom = 0;
  m_vectorPage->hasMaxGeom(maxGeom);

  //output parameters
  std::vector<te::gm::Geometry*> geomVec;

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
    //run operation
    raster->vectorize(geomVec, band, maxGeom);
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Vectorizer"), e.what());

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Vectorizer"), tr("An exception has occurred!"));

    te::common::ProgressManager::getInstance().removeViewer(id);

    QApplication::restoreOverrideCursor();

    return false;
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();

  try
  {
    //save data
    std::auto_ptr<te::da::DataSetType> dsType = createDataSetType(outputdataset, raster->getSRID());

    std::auto_ptr<te::mem::DataSet> dsMem = createDataSet(dsType.get(), geomVec);

    te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().get(outDSInfo->getId(), outDSInfo->getType(), outDSInfo->getConnInfo());

    saveDataSet(dsMem.get(), dsType.get(), ds, outputdataset);

    //create output layer
    te::da::DataSourcePtr outDataSource = te::da::GetDataSource(outDSInfo->getId());
    
    te::qt::widgets::DataSet2Layer converter(outDSInfo->getId());
      
    te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());

    m_outputLayer = converter(dt);
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Vectorizer"), e.what());

    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Vectorizer"), tr("An exception has occurred saving geometries!"));

    return false;
  }

  return true;
}

std::auto_ptr<te::da::DataSetType> te::qt::widgets::VectorizationWizard::createDataSetType(std::string dataSetName, int srid)
{
  std::auto_ptr<te::da::DataSetType> dsType(new te::da::DataSetType(dataSetName));

  //create id property
  te::dt::SimpleProperty* idProperty = new te::dt::SimpleProperty("id", te::dt::INT32_TYPE);
  dsType->add(idProperty);

  //create geometry property
  te::gm::GeometryProperty* geomProperty = new te::gm::GeometryProperty("geom", srid, te::gm::PolygonType);
  dsType->add(geomProperty);

  //create primary key
  std::string pkName = "pk_id";
              pkName+= "_" + dataSetName;
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(pkName, dsType.get());
  pk->add(idProperty);

  return dsType;
}

std::auto_ptr<te::mem::DataSet> te::qt::widgets::VectorizationWizard::createDataSet(te::da::DataSetType* dsType, std::vector<te::gm::Geometry*>& geoms)
{
  std::auto_ptr<te::mem::DataSet> ds(new te::mem::DataSet(dsType));

  for(std::size_t t = 0; t < geoms.size(); ++t)
  {
    //create dataset item
    te::mem::DataSetItem* item = new te::mem::DataSetItem(ds.get());

    //set id
    item->setInt32("id", (int)t);

    //set geometry
    item->setGeometry("geom", geoms[t]);

    ds->add(item);
  }

  return ds;
}

void te::qt::widgets::VectorizationWizard::saveDataSet(te::mem::DataSet* dataSet, te::da::DataSetType* dsType, te::da::DataSourcePtr ds, std::string dataSetName)
{
  //save dataset
  dataSet->moveBeforeFirst();

  std::map<std::string, std::string> options;

  ds->createDataSet(dsType, options);

  ds->add(dataSetName, dataSet, options);
}

