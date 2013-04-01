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
  \file terralib/qt/widgets/layer/selector/core/HiddenDataSetLayerSelector.cpp

  \brief ....
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../Exception.h"
#include "../utils/DataSet2Layer.h"
#include "HiddenDataSetLayerSelector.h"

// STL
#include <algorithm>
#include <iterator>

te::qt::widgets::HiddenDataSetLayerSelector::HiddenDataSetLayerSelector(QWidget* parent, Qt::WindowFlags f)
  : AbstractLayerSelector(parent, f)
{
}

te::qt::widgets::HiddenDataSetLayerSelector::~HiddenDataSetLayerSelector()
{
}

void te::qt::widgets::HiddenDataSetLayerSelector::set(const std::list<te::da::DataSourceInfoPtr>& datasources)
{
  m_datasources = datasources;
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::HiddenDataSetLayerSelector::getLayers()
{
  std::list<te::map::AbstractLayerPtr> layers;

  for(std::list<te::da::DataSourceInfoPtr>::iterator it = m_datasources.begin(); it != m_datasources.end(); ++it)
  {
    te::da::DataSourcePtr datasource = te::da::DataSourceManager::getInstance().find((*it)->getId());

    if(datasource.get() == 0)
    {
      datasource = te::da::DataSourceManager::getInstance().get((*it)->getId(), (*it)->getAccessDriver(), (*it)->getConnInfo());

      if(datasource.get() == 0)
        throw Exception(TR_QT_WIDGETS("Could not retrieve the data source instance!"));
    }

    if(!datasource->isOpened())
      datasource->open();

    boost::ptr_vector<std::string> datasets;

    te::da::GetDataSets(datasets, datasource.get());

    std::auto_ptr<te::da::DataSourceTransactor> transactor(datasource->getTransactor());

    std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(transactor->getCatalogLoader());

    DataSet2Layer converter((*it)->getId());

    for(std::size_t i = 0; i < datasets.size(); ++i)
    {
      te::da::DataSetTypePtr dataset(cloader->getDataSetType(datasets[i], true));

      if(dataset->hasDefaultGeom() && (dataset->getDefaultGeomProperty()->getExtent() == 0))
      {
// load geometry extent
        te::gm::Envelope* mbr = cloader->getExtent(dataset->getDefaultGeomProperty());

        dataset->getDefaultGeomProperty()->setExtent(mbr);
      }

      te::map::DataSetLayerPtr layer = converter(dataset);

      layers.push_back(layer);
    }
  }

  return layers;
}

