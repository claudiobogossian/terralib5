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

/*!
\file src/terraMobilePlugin/qt/GeoPackageBuilderWizard.h

\brief This interface is used to get the input parameters for GeoPackage Builder wizard operation.
*/

// TerraLib
#include "../../../../common/StringUtils.h"
#include "../../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../gdal/Utils.h"
#include "../../../../gdal/DataSetsManager.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../../widgets/utils/DoubleListWidget.h"
#include "../../../widgets/utils/ScopedCursor.h"
#include "../core/form/Serializer.h"
#include "BuilderGatheringLayersWizardPage.h"
#include "BuilderInputLayersWizardPage.h"
#include "BuilderFormsWizardPage.h"
#include "BuilderOutputWizardPage.h"
#include "GeoPackageBuilderWizard.h"

// Qt
#include <QMessageBox>

//Boost
#include <boost/lexical_cast.hpp>

te::qt::plugins::terramobile::GeoPackageBuilderWizard::GeoPackageBuilderWizard(QWidget* parent, Qt::WindowFlags f)
  : QWizard(parent, f)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setFixedSize(640, 480);
  this->setWindowTitle(tr("GeoPackage Builder Wizard"));

  addPages();
}

te::qt::plugins::terramobile::GeoPackageBuilderWizard::~GeoPackageBuilderWizard()
{

}

void te::qt::plugins::terramobile::GeoPackageBuilderWizard::exportToGPKG(te::map::AbstractLayerPtr layer, te::da::DataSource* dsGPKG, std::string outFileName)
{
  std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();

  //Checking if the layer contains a raster property
  if (dsType->hasRaster())
  {
    std::auto_ptr<te::da::DataSet> dataset = layer->getData();
    std::size_t rpos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> raster = dataset->getRaster(rpos);

    int inputSRID = raster->getSRID();
    int bandType = raster->getBandDataType(0);

    //Adjusting the output raster to tconform with mobile app needs
    if ((inputSRID != 4326) || (bandType != te::dt::UCHAR_TYPE))
    {
      te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());

      // Gets the URI
      const std::string& id = dsLayer->getDataSourceId();
      te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(id);
      std::map<std::string, std::string>  connInfo = info->getConnInfo();
      std::string uri = connInfo["SOURCE"];

      if (boost::filesystem::is_directory(uri))
        uri += ("/" + dsLayer->getDataSetName());

      std::map<std::string, std::string> rinfo;
      rinfo["SOURCE"] = uri;
      rinfo["MEM_RASTER_NROWS"] = boost::lexical_cast<std::string>(raster->getNumberOfRows());
      rinfo["MEM_RASTER_NCOLS"] = boost::lexical_cast<std::string>(raster->getNumberOfColumns());
      rinfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>(raster->getBandDataType(0));
      rinfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(raster->getNumberOfBands());
     
      if (bandType != te::dt::UCHAR_TYPE)
      {
        raster = te::gdal::NormalizeRaster(raster.get(), raster->getBand(0)->getMinValue().real(), raster->getBand(0)->getMaxValue().real(), 0, 255, rinfo, "GDAL");
        te::gdal::DataSetsManager::getInstance().decrementUseCounter(uri);
      }

      if (inputSRID != 4326)
      {
         raster.reset(raster->transform(4326, rinfo));
         te::gdal::DataSetsManager::getInstance().decrementUseCounter(uri);
      }
    }

    te::gdal::copyToGeopackage(raster.get(), outFileName);
  }
  else
  {
    //SRID adaptation
    int inputSRID = layer->getSRID();
    int outputSRID = 4326;

    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), dsGPKG->getCapabilities(), dsGPKG->getEncoding());
    te::da::AssociateDataSetTypeConverterSRID(converter, inputSRID, outputSRID);

    te::da::DataSetType* dsTypeResult = converter->getResult();

    dsTypeResult->setName(dsType->getName());

    //exporting
    std::map<std::string, std::string> nopt;

    std::auto_ptr<te::da::DataSet> dataset = layer->getData();

    std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), converter));

    if (dataset->moveBeforeFirst())
      te::da::Create(dsGPKG, dsTypeResult, dsAdapter.get());
  }
}

void te::qt::plugins::terramobile::GeoPackageBuilderWizard::setLayerList(std::list<te::map::AbstractLayerPtr> list)
{
  m_list = list;

  //clear lists
  m_inputLayersPage->getWidget()->clearInputValues();
  m_inputLayersPage->getWidget()->clearOutputValues();

  m_gatheringLayersPage->getWidget()->clearInputValues();
  m_gatheringLayersPage->getWidget()->clearOutputValues();

  std::vector<std::string> inputLayerNames;
  std::vector<std::string> gatheringLayerNames;


  //fill combos
  std::list<te::map::AbstractLayerPtr>::iterator it = list.begin();

  while (it != list.end())
  {
    te::map::AbstractLayerPtr l = *it;

    if (l->isValid())
    {
      std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

      if (dsType->hasGeom())
      {
        inputLayerNames.push_back(l->getTitle());

        //if this layer has point as geom type, add this layer to gatherer list
        te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dsType.get());

        if (geomProp && (geomProp->getGeometryType() == te::gm::PointType || geomProp->getGeometryType() == te::gm::MultiPointType))
        {
          gatheringLayerNames.push_back(l->getTitle());
        }
      }
      else if (dsType->hasRaster())
      {
        inputLayerNames.push_back(l->getTitle());
      }
    }
    ++it;
  }

  m_inputLayersPage->getWidget()->setInputValues(inputLayerNames);
  m_gatheringLayersPage->getWidget()->setInputValues(gatheringLayerNames);
}

bool te::qt::plugins::terramobile::GeoPackageBuilderWizard::validateCurrentPage()
{
  if (currentPage() == m_inputLayersPage.get())
  {
    return m_inputLayersPage->isComplete();
  }
  else if (currentPage() == m_gatheringLayersPage.get())
  {
    bool res = m_gatheringLayersPage->isComplete();

    if (res)
    {
      std::list<te::map::AbstractLayerPtr> layers = getGatheringLayers();

      m_formsPage->setLayerList(layers);
    }

    return res;
  }
  else if (currentPage() == m_formsPage.get())
  {
    return m_formsPage->isComplete();
  }
  else if (currentPage() == m_outputPage.get())
  {
    return execute();
  }

  return false;
}

void te::qt::plugins::terramobile::GeoPackageBuilderWizard::addPages()
{
  m_inputLayersPage.reset(new te::qt::plugins::terramobile::BuilderInputLayersWizardPage(this));
  m_gatheringLayersPage.reset(new te::qt::plugins::terramobile::BuilderGatheringLayersWizardPage(this));
  m_formsPage.reset(new te::qt::plugins::terramobile::BuilderFormsWizardPage(this));
  m_outputPage.reset(new te::qt::plugins::terramobile::BuilderOutputWizardPage(this));

  addPage(m_inputLayersPage.get());
  addPage(m_gatheringLayersPage.get());
  addPage(m_formsPage.get());
  addPage(m_outputPage.get());
}

bool te::qt::plugins::terramobile::GeoPackageBuilderWizard::execute()
{
  te::qt::widgets::ScopedCursor c(Qt::WaitCursor);
  std::string gpkgName = m_outputPage->getGeoPackageFilePath();

  if (gpkgName.empty())
  {
    QMessageBox::warning(this, tr("GeoPackage Builder"), "The name for the output geopackage can not be empty.");
    return false;
  }

  //get forms
  std::map<std::string, Section*> sectionsMap = m_formsPage->getSections();

  te::gdal::createGeopackage(gpkgName);

  //create data source
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = gpkgName;

  std::auto_ptr<te::da::DataSource> dsGPKG = te::da::DataSourceFactory::make("OGR");
  dsGPKG->setConnectionInfo(connInfo);
  dsGPKG->open();

  std::string sql1 = "CREATE TABLE IF NOT EXISTS tm_style(layer_name TEXT PRIMARY KEY NOT NULL, sld_xml TEXT); ";
  std::string sql2 = "CREATE TABLE IF NOT EXISTS tm_layer_form ";
  sql2 += "(tm_conf_id INTEGER PRIMARY KEY AUTOINCREMENT, gpkg_layer_identify TEXT NOT NULL, tm_form TEXT, ";
  sql2 += "CONSTRAINT fk_layer_identify_id FOREIGN KEY (gpkg_layer_identify) REFERENCES gpkg_contents(table_name));";
  
  std::auto_ptr<te::da::DataSourceTransactor> transactor = dsGPKG->getTransactor();

  try
  {
    transactor->execute(sql1);
    transactor->execute(sql2);
  }
  catch (...)
  {
    throw;
  }

  std::list<te::map::AbstractLayerPtr> inputLayers = getInputLayers();
  std::list<te::map::AbstractLayerPtr> gatheringLayers = getGatheringLayers();

  std::list<te::map::AbstractLayerPtr>::iterator it;

  for (it = inputLayers.begin(); it != inputLayers.end(); ++it)
  {
    exportToGPKG(*it, dsGPKG.get(), gpkgName);
    std::string name = (*it)->getSchema()->getName();
    std::string sldString = te::qt::plugins::terramobile::Write((*it)->getStyle(), (gpkgName + "-temp-style.xml"));
    std::string insert = "INSERT INTO tm_style ('layer_name', 'sld_xml' )  values('" + name + "', '" + sldString + "');";  
    try
    {
      transactor->execute(insert);
    }
    catch (...)
    {
      transactor->rollBack();
      throw;
    }
  }

  for (it = gatheringLayers.begin(); it != gatheringLayers.end(); ++it)
  {
    exportToGPKG(*it, dsGPKG.get(), gpkgName);
    std::string name = (*it)->getSchema()->getName();
    std::string sldString = te::qt::plugins::terramobile::Write((*it)->getStyle(), (gpkgName + "-temp-style.xml"));
    std::string insert = "INSERT INTO tm_style ('layer_name', 'sld_xml' )  values('" + name + "', '" + sldString + "');";
    try
    {
      transactor->execute(insert);
    }
    catch (...)
    {
      transactor->rollBack();
      throw;
    }
  }

  std::map<std::string, Section*>::iterator itb = sectionsMap.begin();
  std::map<std::string, Section*>::iterator ite = sectionsMap.end();

  while (itb != ite)
  {
    std::string jsonStr = te::qt::plugins::terramobile::Write(itb->second);

    std::string insert = "INSERT INTO tm_layer_form ('gpkg_layer_identify', 'tm_form' )  values('" + itb->first + "', '" + jsonStr + "');";

    try
    {
      transactor->execute(insert);
    }
    catch (...)
    {
      transactor->rollBack();
      throw;
    }
    ++itb;
  }

  return true;
}

std::list<te::map::AbstractLayerPtr> te::qt::plugins::terramobile::GeoPackageBuilderWizard::getGatheringLayers()
{
  std::list<te::map::AbstractLayerPtr> list;

  //get gathering layers names
  std::vector<std::string> names = m_gatheringLayersPage->getWidget()->getOutputValues();

  for (std::size_t t = 0; t < names.size(); ++t)
  {
    std::list<te::map::AbstractLayerPtr>::iterator it;

    for (it = m_list.begin(); it != m_list.end(); ++it)
    {
      if (names[t] == it->get()->getTitle())
      {
        list.push_back(it->get());
      }
    }
  }

  return list;
}

std::list<te::map::AbstractLayerPtr> te::qt::plugins::terramobile::GeoPackageBuilderWizard::getInputLayers()
{
  std::list<te::map::AbstractLayerPtr> list;

  //get gathering layers names
  std::vector<std::string> names = m_inputLayersPage->getWidget()->getOutputValues();

  for (std::size_t t = 0; t < names.size(); ++t)
  {
    std::list<te::map::AbstractLayerPtr>::iterator it;

    for (it = m_list.begin(); it != m_list.end(); ++it)
    {
      if (names[t] == it->get()->getTitle())
      {
        list.push_back(it->get());
      }
    }
  }

  return list;
}
