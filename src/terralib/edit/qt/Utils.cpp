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
  \file terralib/edit/qt/Utils.cpp
   
  \brief Utility Qt functions for TerraLib Edit module.
*/

// TerraLib
#include "../../core/translator/Translator.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/GeometryProperty.h"
#include "../../qt/widgets/canvas/Canvas.h"
#include "../../qt/widgets/Utils.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../srs/Config.h"
#include "../Utils.h"
#include "Utils.h"

// Qt
#include <QColor>
#include <QFileDialog>
#include <QMessageBox>

// STL
#include <cassert>

// Boost
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

QPointF te::edit::GetPosition(QMouseEvent* e)
{
#if QT_VERSION >= 0x050000
  return e->localPos();
#else
  return e->posF();
#endif
}

te::map::AbstractLayerPtr te::edit::CreateShapeFileLayer(const te::gm::GeomType& type, const int& srid)
{

  te::map::AbstractLayerPtr layer;

  //get file path
  QString fileName = QFileDialog::getSaveFileName(0, TE_TR("Create Draft Layer..."), QString(), TE_TR("Shapefile (*.shp *.SHP);;"));

  if (fileName.isEmpty())
    return layer;

  //create draft layer
  boost::filesystem::path outfile(fileName.toStdString());
  std::string layerName = outfile.stem().string();

  //create data source info
  std::string outputdataset = layerName;
  std::size_t idx = outputdataset.find(".");
  if (idx != std::string::npos)
    outputdataset = outputdataset.substr(0, idx);

  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = fileName.toStdString();

  // let's include the new datasource in the managers
  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  te::da::DataSourceInfoPtr dsInfo(new te::da::DataSourceInfo);
  dsInfo->setConnInfo(connInfo);
  dsInfo->setTitle(layerName);
  dsInfo->setAccessDriver("OGR");
  dsInfo->setType("OGR");
  dsInfo->setDescription(layerName);
  dsInfo->setId(id);

  if (te::da::DataSourceInfoManager::getInstance().add(dsInfo))
  {
    //create dataSetType
    std::auto_ptr<te::da::DataSetType> dsType(new te::da::DataSetType(layerName));

    te::dt::SimpleProperty* nameProperty = new te::dt::SimpleProperty("name", te::dt::STRING_TYPE);
    dsType->add(nameProperty);

    te::dt::SimpleProperty* descriptionProperty = new te::dt::SimpleProperty("descr", te::dt::STRING_TYPE);
    dsType->add(descriptionProperty);

    te::gm::GeometryProperty* geomProperty = new te::gm::GeometryProperty("geom", srid, type);
    dsType->add(geomProperty);

    {
      std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
      ds->setConnectionInfo(connInfo);
      ds->open();

      std::map<std::string, std::string> nopt;

      try
      {
        ds->createDataSet(dsType.get(), nopt);
      }
      catch (...)
      {
        return layer;
      }
    }

    te::da::DataSourcePtr dsPtr = te::da::GetDataSource(id);

    //create layer
    te::qt::widgets::DataSet2Layer converter(id);
    te::da::DataSetTypePtr dt(dsType.release());
    layer = converter(dt);
    layer->setVisibility(te::map::VISIBLE);

    return layer;
  }
  else
  {
    dsInfo = te::da::DataSourceInfoManager::getInstance().getByConnInfo(dsInfo->getConnInfoAsString());
    te::da::DataSourcePtr dsPtr = te::da::GetDataSource(dsInfo->getId());

    //Overwrite the selected draft layer
    dsPtr->dropDataSet(layerName);

    //create dataSetType
    std::auto_ptr<te::da::DataSetType> dsType(new te::da::DataSetType(layerName));

    te::dt::SimpleProperty* nameProperty = new te::dt::SimpleProperty("name", te::dt::STRING_TYPE);
    dsType->add(nameProperty);

    te::dt::SimpleProperty* descriptionProperty = new te::dt::SimpleProperty("descr", te::dt::STRING_TYPE);
    dsType->add(descriptionProperty);

    te::gm::GeometryProperty* geomProperty = new te::gm::GeometryProperty("geom", srid, type);
    dsType->add(geomProperty);

    {
      std::map<std::string, std::string> nopt;
      if (!dsPtr->isOpened())
        dsPtr->open();

      try
      {
        dsPtr->createDataSet(dsType.get(), nopt);
      }
      catch (...)
      {
        return layer;
      }
    }

    //create layer
    te::qt::widgets::DataSet2Layer converter(dsInfo->getId());
    te::da::DataSetTypePtr dt(dsType);
    layer = converter(dt);
    layer->setVisibility(te::map::VISIBLE);

    return layer;
  }
}
