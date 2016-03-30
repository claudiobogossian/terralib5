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
  \file terralib/vp/qt/Utils.cpp
   
  \brief Utility functions for Vector Processing.
*/

// TerraLib
#include "../../dataaccess/dataset/DataSetAdapter.h"
#include "../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"

#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceManager.h"

#include "../../dataaccess/query/And.h"
#include "../../dataaccess/query/DataSetName.h"
#include "../../dataaccess/query/Expression.h"
#include "../../dataaccess/query/Field.h"
#include "../../dataaccess/query/Fields.h"
#include "../../dataaccess/query/From.h"
#include "../../dataaccess/query/FromItem.h"
#include "../../dataaccess/query/LiteralInt32.h"
#include "../../dataaccess/query/PropertyName.h"
#include "../../dataaccess/query/ST_SetSRID.h"
#include "../../dataaccess/query/ST_Transform.h"
#include "../../dataaccess/query/Where.h"

#include "../../dataaccess/utils/Utils.h"

#include "../../maptools/DataSetLayer.h"
#include "../../maptools/QueryLayer.h"

#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryCollection.h"
#include "../../geometry/GeometryProperty.h"

#include "../../memory/DataSet.h"

#include "Utils.h"

// Qt
#include <QMessageBox>
#include <QTreeView>

//STL
#include <memory>
#include <vector>

// Boost
#include <boost/algorithm/string.hpp>

std::list<te::map::AbstractLayerPtr> te::vp::GetFilteredLayers(std::string text, std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr> filteredList;

  for(std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin(); it != layers.end(); ++it)
  {
    std::string layName = (*it)->getTitle().substr(0, text.size());
    
    if(boost::to_upper_copy(layName) == boost::to_upper_copy(text))
      filteredList.push_back(*it);
  }

  return filteredList;
}

te::da::Select* te::vp::GetSelectQueryFromLayer(te::map::AbstractLayerPtr layer, bool onlySelectedObjects, int srid)
{
  // Do a Cast from AbstractLayerPtr to DataSetLayer or QueryLayer.
  te::map::DataSetLayer* dataSetLayer = 0;
  te::map::QueryLayer* queryLayer = 0;

  if (layer->getType() == "DATASETLAYER")
  {
    dataSetLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());
    if (!dataSetLayer)
    {
      QMessageBox::information(0, "Error", "Can not execute this operation on this type of layer.");
      return 0;
    }
  }
  else if (layer->getType() == "QUERYLAYER")
  {
    queryLayer = dynamic_cast<te::map::QueryLayer*>(layer.get());
    if (!queryLayer)
    {
      QMessageBox::information(0, "Error", "Can not execute this operation on this type of layer.");
      return 0;
    }
  }
  else
  {
    QMessageBox::information(0, "Error", "Can not execute this operation on this type of layer.");
    return 0;
  }

  // Select query
  te::da::Select* select = new te::da::Select();

  // From dataSetLayer
  if (dataSetLayer)
  {
    te::da::Fields* fields = new te::da::Fields;

    if (srid == 0)
    {
      te::da::Field* f_all = new te::da::Field("*");
      fields->push_back(f_all);
    }
    else
    {
      std::auto_ptr<te::da::DataSetType> dsType = dataSetLayer->getSchema();
      std::vector<te::dt::Property*> properties = dsType.get()->getProperties();

      for (std::size_t i = 0; i < properties.size(); ++i)
      {
        if (properties[i]->getType() != te::dt::GEOMETRY_TYPE)
        {
          te::da::Field* currentField = new te::da::Field(properties[i]->getName());
          fields->push_back(currentField);
        }
        else
        {
          te::da::LiteralInt32* litSRID = new te::da::LiteralInt32(srid);

          te::da::Expression* eSetSRID = new te::da::ST_SetSRID(new te::da::PropertyName(properties[i]->getName()), litSRID);

          te::da::Expression* eTransform = new te::da::ST_Transform(eSetSRID, srid);

          te::da::Field* geomField = new te::da::Field(*eTransform, properties[i]->getName());
          fields->push_back(geomField);
        }
      }
    }

    select->setFields(fields);

    te::da::From* from = new te::da::From;
    te::da::FromItem* fromItem = new te::da::DataSetName(dataSetLayer->getDataSetName());
    from->push_back(fromItem);
    select->setFrom(from);

    if (onlySelectedObjects)
    {
      const te::da::ObjectIdSet* oidSet = layer->getSelected();
      if (!oidSet)
      {
        QMessageBox::information(0, "Difference", "Select the layer objects to perform the operation.");
        return 0;
      }

      te::da::Where* w_oid = new te::da::Where(oidSet->getExpression());
      select->setWhere(w_oid);
    }
  }
  // From queryLayer
  else
  {
    select = queryLayer->getQuery();

    if (srid != 0)
    {
      std::auto_ptr<te::da::DataSetType> dsType = queryLayer->getSchema();
      std::vector<te::dt::Property*> properties = dsType.get()->getProperties();

      te::da::Fields* fields = new te::da::Fields;

      for (std::size_t i = 0; i < properties.size(); ++i)
      {
        if (properties[i]->getType() != te::dt::GEOMETRY_TYPE)
        {
          te::da::Field* currentField = new te::da::Field(properties[i]->getName());
          fields->push_back(currentField);
        }
        else
        {
          te::da::LiteralInt32* litSRID = new te::da::LiteralInt32(srid);

          te::da::Expression* eSetSRID = new te::da::ST_SetSRID(new te::da::PropertyName(properties[i]->getName()), litSRID);

          te::da::Expression* eTransform = new te::da::ST_Transform(eSetSRID, srid);

          te::da::Field* geomField = new te::da::Field(*eTransform, properties[i]->getName());
          fields->push_back(geomField);
        }
      }

      select->setFields(fields);

    }

    if (onlySelectedObjects)
    {
      const te::da::ObjectIdSet* oidSet = layer->getSelected();
      if (!oidSet)
      {
        QMessageBox::information(0, "Difference", "Select the layer objects to perform the operation.");
        return 0;
      }

      te::da::Where* w = select->getWhere();

      te::da::Expression* e_where = w->getExp()->clone();
      te::da::Expression* e_oidWhere = oidSet->getExpression();

      te::da::And* andPtr = new te::da::And(e_where, e_oidWhere);

      te::da::Where* newWhere = new te::da::Where(andPtr);

      select->setWhere(newWhere);
    }
  }

  return select;
}

te::vp::DataStruct te::vp::GetDataStructFromLayer(te::map::AbstractLayerPtr layer, bool onlySelectedObjects, int srid)
{
  DataStruct data;

  std::string sourceId = layer->getDataSourceId();
  te::da::DataSourcePtr dataSource = te::da::DataSourceManager::getInstance().find(sourceId);

  std::auto_ptr<te::da::DataSetType> dataSetType = layer->getSchema();
  std::auto_ptr<te::da::DataSet> dataSet = dataSource->getDataSet(dataSetType->getName());

  if (dataSetType.get() && dataSet.get())
  {
    if (onlySelectedObjects)
    {
      const te::da::ObjectIdSet* oidSet = layer->getSelected();
      if (!oidSet)
      {
        QMessageBox::information(0, "Difference", "Select the layer objects to perform the operation.");
        return data;
      }

      dataSet = dataSource->getDataSet(dataSetType->getName(), oidSet);

      if (!dataSet.get())
        return data;
    }

    if (srid != 0)
    {
      std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(dataSetType.get(), dataSource->getCapabilities(), dataSource->getEncoding()));
      te::da::AssociateDataSetTypeConverterSRID(converter.get(), srid);

      dataSetType.reset(new te::da::DataSetType(*converter->getResult()));

      te::da::DataSetAdapter* dataSetAdapter = te::da::CreateAdapter(dataSet.release(), converter.get());

      if (!dataSetAdapter)
        return data;

      dataSet.reset(dataSetAdapter);
    }

    data.m_dataSetType = dataSetType.release();
    data.m_dataSet = dataSet.release();
  }

  return data;
}