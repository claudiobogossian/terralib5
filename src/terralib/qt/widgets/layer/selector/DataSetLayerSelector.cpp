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
  \file terralib/qt/widgets/layer/selector/DataSetLayerSelector.cpp

  \brief A simple widget that allows the selection of datasets from a given data source.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../dataset/selector/DataSetSelectorDialog.h"
#include "../../Exception.h"
#include "../utils/DataSet2Layer.h"
#include "DataSetLayerSelector.h"

// STL
#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>

te::qt::widgets::DataSetLayerSelector::DataSetLayerSelector(QWidget* parent, Qt::WindowFlags f)
  : AbstractLayerSelector(parent, f)
{
}

te::qt::widgets::DataSetLayerSelector::~DataSetLayerSelector()
{
}

void te::qt::widgets::DataSetLayerSelector::set(const std::list<te::da::DataSourceInfoPtr>& datasources)
{
  m_datasources = datasources;
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::DataSetLayerSelector::getLayers()
{
  std::list<te::map::AbstractLayerPtr> layers;

  for(std::list<te::da::DataSourceInfoPtr>::iterator it = m_datasources.begin(); it != m_datasources.end(); ++it)
  {
    te::da::DataSourcePtr datasource = te::da::DataSourceManager::getInstance().find((*it)->getId());

    if(datasource.get() == 0)
    {
      datasource = te::da::DataSourceManager::getInstance().get((*it)->getId(), (*it)->getAccessDriver(), (*it)->getConnInfo());

      if(datasource.get() == 0)
        throw Exception(TE_TR("Could not retrieve the data source instance!"));
    }

    if(!datasource->isOpened())
      datasource->open();

    std::vector<std::string> datasetNames = datasource->getDataSetNames();

    if(datasetNames.size() == 1) // In this case not show the DataSetSelectorDialog!
    {
      DataSet2Layer converter((*it)->getId());
      std::auto_ptr<te::da::DataSetType> dt = datasource->getDataSetType(datasetNames[0]);
      te::da::DataSetTypePtr dtpt(dt.release());
      te::map::DataSetLayerPtr layer = converter(dtpt);
      layers.push_back(layer);
    }
    else
    {
      std::auto_ptr<DataSetSelectorDialog> ldialog(new DataSetSelectorDialog(static_cast<QWidget*>(parent())));

      ldialog->set(*it, true);

      int retval = ldialog->exec();

      if(retval == QDialog::Rejected)
        continue;

      std::list<te::da::DataSetTypePtr> datasets = ldialog->getCheckedDataSets();
      std::list<std::string> geomProps = ldialog->getCheckedGeomProperties();

      assert(datasets.size() == geomProps.size());

      std::transform(datasets.begin(), datasets.end(), geomProps.begin(), std::back_inserter(layers), DataSet2Layer((*it)->getId()));
    }
  }

  return layers;
}
