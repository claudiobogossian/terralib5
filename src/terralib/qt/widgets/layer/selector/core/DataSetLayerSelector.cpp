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
  \file terralib/qt/widgets/layer/selector/core/DataSetLayerSelector.cpp

  \brief ....
*/

// TerraLib
#include "../../../../../maptools/AbstractLayer.h"
#include "../../../../../maptools/Layer.h"
#include "../../../dataset/selector/DataSetSelectorDialog.h"
#include "../../utils/DataSet2Layer.h"
#include "DataSetLayerSelector.h"

// STL
#include <algorithm>
#include <iterator>

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
    std::auto_ptr<DataSetSelectorDialog> ldialog(new DataSetSelectorDialog(static_cast<QWidget*>(parent())));

    ldialog->set(*it, true);

    int retval = ldialog->exec();

    if(retval == QDialog::Rejected)
      continue;

    std::list<te::da::DataSetTypePtr> datasets = ldialog->getCheckedDataSets();

    std::transform(datasets.begin(), datasets.end(), std::back_inserter(layers), DataSet2Layer((*it)->getId()));
  }

  return layers;
}

