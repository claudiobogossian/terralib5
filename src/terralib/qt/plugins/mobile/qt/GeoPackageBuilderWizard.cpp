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
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../widgets/utils/DoubleListWidget.h"
#include "../../../widgets/utils/ScopedCursor.h"
#include "../core/form/Serializer.h"
#include "../utils/Utils.h"
#include "BuilderGatheringLayersWizardPage.h"
#include "BuilderInputLayersWizardPage.h"
#include "BuilderFormsWizardPage.h"
#include "BuilderOutputWizardPage.h"
#include "GeoPackageBuilderWizard.h"

// Qt
#include <QMessageBox>

//Boost
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::qt::plugins::terramobile::GeoPackageBuilderWizard::GeoPackageBuilderWizard(QWidget* parent, Qt::WindowFlags f)
: QWizard(parent, f)
{
  //configure the wizard
  this->setWizardStyle(QWizard::ModernStyle);
  this->setFixedSize(640, 480);
  this->setWindowTitle(tr("GeoPackage Builder Wizard"));

  addPages();
  m_extent.makeInvalid();
}

te::qt::plugins::terramobile::GeoPackageBuilderWizard::~GeoPackageBuilderWizard()
{

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

void te::qt::plugins::terramobile::GeoPackageBuilderWizard::setExtent(const te::gm::Envelope& extent)
{
  m_extent = extent;
}

void te::qt::plugins::terramobile::GeoPackageBuilderWizard::setSRID(int srid)
{
  m_srid = srid;
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

  std::auto_ptr<te::da::DataSource> dsGPKG = te::qt::plugins::terramobile::createGeopackage(gpkgName);

  std::list<te::map::AbstractLayerPtr> gatheringLayers = getGatheringLayers();
  std::list<te::map::AbstractLayerPtr> inputLayers = getInputLayers();

  std::list<te::map::AbstractLayerPtr>::iterator it;

  //Exporting the selected input and gathering layers
  size_t pos = 1;
  bool visible = false;

  for (it = gatheringLayers.begin(); it != gatheringLayers.end(); ++it)
  {
    std::string dataSourceId = (*it)->getDataSourceId();
    te::da::DataSourceInfoPtr dsInfo = te::da::DataSourceInfoManager::getInstance().get(dataSourceId);

    bool aux = false;

    if (!visible && !aux)
    {
      visible = (*it)->getVisibility();
      aux = (*it)->getVisibility();
    }

    te::qt::plugins::terramobile::exportToGPKG(*it, dsGPKG.get(), gpkgName, m_extent);

    std::auto_ptr<te::da::DataSetType> dsType = (*it)->getSchema();

    m_outputPage->appendLogMesssage("Exporting gathering layer " + dsType->getName());

    std::string insert = "INSERT INTO tm_layer_settings ('layer_name', 'enabled', 'position', 'uri') values('" + dsType->getName() + "', " +
      boost::lexical_cast<std::string>(aux)+", " + boost::lexical_cast<std::string>(pos)+", '" + dsInfo->getConnInfoAsString() + "'); ";
    te::qt::plugins::terramobile::queryGPKG(insert, dsGPKG.get());
    ++pos;

    m_outputPage->appendLogMesssage("Gathering layer " + dsType->getName() + " successfuly exported.");
  }

  for (it = inputLayers.begin(); it != inputLayers.end(); ++it)
  {
    std::string dataSourceId = (*it)->getDataSourceId();
    te::da::DataSourceInfoPtr dsInfo = te::da::DataSourceInfoManager::getInstance().get(dataSourceId);

    visible = (*it)->getVisibility();

    std::auto_ptr<te::da::DataSetType> dsType = (*it)->getSchema();
    std::string name = dsType->getName();

    //Checking if the layer contains a raster property to adjust it's name removing the extension
    if (dsType->hasRaster())
    {
      std::vector<std::string> values;
      te::common::Tokenize(name, values, ".");
      name = "";
      for (size_t i = 0; i < values.size() - 1; ++i)
        name += values[i];
    }

    m_outputPage->appendLogMesssage("Exporting input layer " + name);

    std::string insert = "INSERT INTO tm_layer_settings ('layer_name', 'enabled', 'position', 'uri') values('" + name + "', " + boost::lexical_cast<std::string>(visible)+", " +
      boost::lexical_cast<std::string>(pos)+", '" + dsInfo->getConnInfoAsString() + "'); ";
    te::qt::plugins::terramobile::exportToGPKG(*it, dsGPKG.get(), gpkgName, m_extent);
    te::qt::plugins::terramobile::queryGPKG("select * from sqlite_master", dsGPKG.get());
    te::qt::plugins::terramobile::queryGPKG(insert, dsGPKG.get());
    ++pos;

    m_outputPage->appendLogMesssage("Input layer " + name + " successfuly exported.");
  }

  //Exporting the forms used to collect data on the field
  std::map<std::string, Section*>::iterator itb = sectionsMap.begin();
  std::map<std::string, Section*>::iterator ite = sectionsMap.end();

  while (itb != ite)
  {
    std::string jsonStr = te::qt::plugins::terramobile::Write(itb->second);

    std::string insert = "INSERT INTO tm_layer_form ('gpkg_layer_identify', 'tm_form', 'tm_media_table', 'tm_state' )  values('" + itb->first + "', '" + jsonStr + "', '" + "" + "', '" + boost::lexical_cast<std::string>(0) + "');";
    te::qt::plugins::terramobile::queryGPKG(insert, dsGPKG.get());
    ++itb;
  }

  //Settings - Exporting the visible area if it is valid
  if (m_outputPage->useVisibleArea() && m_extent.isValid() && m_srid != 0)
  {
    m_extent.transform(m_srid, 4326);

    std::string minX, minY, maxX, maxY;
    minX = boost::lexical_cast<std::string>(m_extent.getLowerLeftX());
    minY = boost::lexical_cast<std::string>(m_extent.getLowerLeftY());
    maxX = boost::lexical_cast<std::string>(m_extent.getUpperRightX());
    maxY = boost::lexical_cast<std::string>(m_extent.getUpperRightY());

    std::string insert1 = "INSERT INTO tm_settings ('key', 'value') values ('default_xmin', '" + minX + "');";
    std::string insert2 = "INSERT INTO tm_settings ('key', 'value') values ('default_ymin', '" + minY + "');";
    std::string insert3 = "INSERT INTO tm_settings ('key', 'value') values ('default_xmax', '" + maxX + "');";
    std::string insert4 = "INSERT INTO tm_settings ('key', 'value') values ('default_ymax', '" + maxY + "');";

    te::qt::plugins::terramobile::queryGPKG(insert1, dsGPKG.get());
    te::qt::plugins::terramobile::queryGPKG(insert2, dsGPKG.get());
    te::qt::plugins::terramobile::queryGPKG(insert3, dsGPKG.get());
    te::qt::plugins::terramobile::queryGPKG(insert4, dsGPKG.get());
  }

  //Generating a unique id for the geopackage file
  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id_ds = boost::uuids::to_string(u);

  std::string insGPKGID = "INSERT INTO tm_settings ('key', 'value') values ('gpkg_id', '" + id_ds + "');";
  te::qt::plugins::terramobile::queryGPKG(insGPKGID, dsGPKG.get());

  std::string insDbVr = "INSERT INTO tm_settings ('key', 'value') values ('db_version', '0.1');";
  te::qt::plugins::terramobile::queryGPKG(insDbVr, dsGPKG.get());

  //Removing trigggers and tables that could generate problems on the mobile application
  std::vector<std::string> triggers = te::qt::plugins::terramobile::getItemNames("trigger", dsGPKG.get());

  for (size_t i = 0; i < triggers.size(); ++i)
  {
    std::string drop = "drop trigger " + triggers[i] + ";";
    te::qt::plugins::terramobile::queryGPKG(drop, dsGPKG.get());
  }

  std::vector<std::string> tables = te::qt::plugins::terramobile::getItemNames("table", dsGPKG.get());

  for (size_t i = 0; i < tables.size(); ++i)
  {
    std::string drop = "drop table " + tables[i] + ";";
    te::qt::plugins::terramobile::queryGPKG(drop, dsGPKG.get());
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
