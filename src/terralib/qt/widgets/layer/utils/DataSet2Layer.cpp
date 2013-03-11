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
  \file terralib/qt/widgets/layer/utils/DataSet2Layer.cpp

  \brief ....
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../Exception.h"
#include "DataSet2Layer.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::qt::widgets::DataSet2Layer::DataSet2Layer(const std::string& datasourceId)
  : m_datasourceId(datasourceId)
{
}

te::map::DataSetLayerPtr te::qt::widgets::DataSet2Layer::operator()(const te::da::DataSetTypePtr& dataset) const
{
  static boost::uuids::basic_random_generator<boost::mt19937> gen;

  if(dataset.get() == 0)
    throw Exception(TR_QT_WIDGETS("Can not convert a NULL dataset to a layer!"));

  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  te::map::DataSetLayerPtr layer(new te::map::DataSetLayer(id, dataset->getTitle().empty() ? dataset->getName() : dataset->getTitle()));
  layer->setDataSetName(dataset->getName());
  layer->setDataSourceId(m_datasourceId);
  layer->setVisibility(te::map::NOT_VISIBLE);

  if(dataset->hasGeom())
  {
    te::gm::GeometryProperty* gp = dataset->getDefaultGeomProperty();
    layer->setSRID(gp->getSRID());
    layer->setExtent(gp->getExtent() ? te::gm::Envelope(*(gp->getExtent())) : te::gm::Envelope());
  }
  else
  {
    layer->setSRID(0);
    layer->setExtent(te::gm::Envelope());
  }

  return layer;
}

