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
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../gdal/Utils.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../../widgets/utils/DoubleListWidget.h"
#include "../../../widgets/utils/ScopedCursor.h"
#include "BuilderGatheringLayersWizardPage.h"
#include "BuilderInputLayersWizardPage.h"
#include "BuilderFormsWizardPage.h"
#include "BuilderOutputWizardPage.h"
#include "GeoPackageBuilderWizard.h"

// Qt
#include <QMessageBox>


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
  int srid = layer->getSRID();
  std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();

  //Checking if the layer contains a raster property
  if (dsType->hasRaster())
  {
    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());

    // Gets the connection info
    const std::string& id = dsLayer->getDataSourceId();
    te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(id);
    std::string connInfo = info->getConnInfoAsString();

    if (boost::filesystem::is_directory(connInfo))
      connInfo += ("/" + dsLayer->getDataSetName());

    te::gdal::copyToGeopackage(connInfo, outFileName);
  }
  else
  {
    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), dsGPKG->getCapabilities(), dsGPKG->getEncoding());

    te::da::DataSetType* dsTypeResult = converter->getResult();

    dsTypeResult->setName(dsType->getName());

    // Check dataset name
    if (!dsGPKG->isDataSetNameValid(dsTypeResult->getName()))
    {
      int r = QMessageBox::question(this, tr("GeoPackage Builder"), tr("Layer name invalid for output datasource. Would you like to normalize the name?"), QMessageBox::Yes, QMessageBox::No);

      if (r == QMessageBox::Yes)
      {
        bool aux;
        std::string newName = te::common::ReplaceSpecialChars(dsTypeResult->getName(), aux);
        dsTypeResult->setName(newName);
      }
      else
      {
        throw te::common::Exception(tr("Layer name invalid for output datasource!").toStdString());
      }
    }

    // Check properties names
    std::vector<te::dt::Property* > props = dsTypeResult->getProperties();
    std::map<std::size_t, std::string> invalidNames;
    for (std::size_t i = 0; i < props.size(); ++i)
    {
      if (!dsGPKG->isPropertyNameValid(props[i]->getName()))
      {
        invalidNames[i] = props[i]->getName();
      }
    }

    if (!invalidNames.empty())
    {
      int r = QMessageBox::question(this, tr("GeoPackage Builder"), tr("Some property name is invalid for output datasource. Would you like to normalize the name?"), QMessageBox::Yes, QMessageBox::No);

      if (r == QMessageBox::Yes)
      {
        std::map<std::size_t, std::string>::iterator it = invalidNames.begin();

        while (it != invalidNames.end())
        {
          bool aux;
          std::string newName = te::common::ReplaceSpecialChars(it->second, aux);

          props[it->first]->setName(newName);

          ++it;
        }
      }
      else
      {
        QString err(tr("Some property name is invalid for output datasource:\n\n"));

        std::map<std::size_t, std::string>::iterator it = invalidNames.begin();

        while (it != invalidNames.end())
        {
          err.append(" - ");
          err.append(it->second.c_str());

          ++it;
        }

        throw te::common::Exception(err.toStdString());
      }
    }

    te::qt::widgets::ScopedCursor c(Qt::WaitCursor);

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
  std::string gpkgName = m_outputPage->getGeoPackageFilePath();

  if (gpkgName.empty())
  {
    QMessageBox::warning(this, tr("GeoPackage Builder"), "The name for the output geopackage can not be empty.");
    return false;
  }

  //get forms
  std::map<std::string, Section*> sectionsMap = m_formsPage->getSections();
  //std::string jsonStr = te::qt::plugins::terramobile::Write(m_section);

  te::gdal::createGeopackage(gpkgName);

  //create data source
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = gpkgName;

  std::auto_ptr<te::da::DataSource> dsGPKG = te::da::DataSourceFactory::make("OGR");
  dsGPKG->setConnectionInfo(connInfo);
  dsGPKG->open();

  std::list<te::map::AbstractLayerPtr> inputLayers = getInputLayers();
  std::list<te::map::AbstractLayerPtr> gatheringLayers = getGatheringLayers();

  std::list<te::map::AbstractLayerPtr>::iterator it;

  for (it = inputLayers.begin(); it != inputLayers.end(); ++it)
  {
    exportToGPKG(*it, dsGPKG.get(), gpkgName);
  }

  for (it = gatheringLayers.begin(); it != gatheringLayers.end(); ++it)
  {
    exportToGPKG(*it, dsGPKG.get(), gpkgName);
  }

  bool visibleArea = m_outputPage->useVisibleArea();
  if (visibleArea)
  {
    std::string query = "create table if not exists VISIBLE_AREA (LLX real not null, LLY real not null, URX real not null, URY real not null);";
    te::gdal::queryGeopackage(gpkgName, query);
  }

  std::string query = "create table if not exists TM_STYLE (LAYER_NAME text primary key not null, SLD_XML text);";
  te::gdal::queryGeopackage(gpkgName, query);

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
