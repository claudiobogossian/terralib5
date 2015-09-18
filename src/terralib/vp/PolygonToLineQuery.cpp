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
  \file PolygonToLineQuery.h

  \brief Polygon To Line Vector Processing functions.
*/

//Terralib
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Expression.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/Fields.h"
#include "../dataaccess/query/From.h"
#include "../dataaccess/query/FromItem.h"
#include "../dataaccess/query/GroupBy.h"
#include "../dataaccess/query/GroupByItem.h"
#include "../dataaccess/query/LiteralInt32.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/ST_Boundary.h"
#include "../dataaccess/query/ST_Collect.h"
#include "../dataaccess/query/ST_Dump.h"
#include "../dataaccess/query/ST_DumpRings.h"
#include "../dataaccess/query/ST_SetSRID.h"
#include "../dataaccess/query/SubSelect.h"
#include "../dataaccess/query/Where.h"
#include "../dataaccess/utils/Utils.h"

#include "../datatype/Property.h"
#include "../geometry/GeometryProperty.h"

#include "PolygonToLineQuery.h"
#include "Utils.h"

// STL
#include <memory>
#include <string>
#include <vector>

te::vp::PolygonToLineQuery::PolygonToLineQuery()
{}

te::vp::PolygonToLineQuery::~PolygonToLineQuery()
{}

bool te::vp::PolygonToLineQuery::run() throw(te::common::Exception)
{
// Set output DataSetType
  std::auto_ptr<te::da::DataSetType> outDsType = buildOutDataSetType();
  std::vector<te::dt::Property*> props = outDsType->getProperties();

// Get DataSetType from DataSource to compare geometry SRID with DataSetType of Layer.
  std::auto_ptr<te::da::DataSetType> sourceDSetType(m_inDsrc->getDataSetType(m_inDsetName));
  te::gm::GeometryProperty* geomPropSource = te::da::GetFirstGeomProperty(sourceDSetType.get());

  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(m_converter->getResult());

// Subselect that apply the ST_Dump function in geometric column to separate multi polygons.
  te::da::Fields* pol_fields = new te::da::Fields;
  for(std::size_t i = 0; i < props.size(); ++i)
  {
    if(props[i]->getType() != te::dt::GEOMETRY_TYPE)
    {
      te::da::PropertyName* pName = new te::da::PropertyName(props[i]->getName());
      te::da::Field* field = new te::da::Field(pName);
      pol_fields->push_back(field);
    }
    else
    {
      te::da::Expression* e_geometry;

      if (geomPropSource->getSRID() != geomProp->getSRID())
      {
        te::da::LiteralInt32* srid = new te::da::LiteralInt32(geomProp->getSRID());
        e_geometry = new te::da::ST_SetSRID(new te::da::PropertyName(props[i]->getName()), srid);
      }
      else
      {
        e_geometry = new te::da::PropertyName(props[i]->getName());
      }

      te::da::Expression* e_dump = new te::da::ST_Dump(e_geometry);
      te::da::Field* f_dump = new te::da::Field(*e_dump, " polygon");
      pol_fields->push_back(f_dump);

    }
  }

  te::da::FromItem* fromItemPol = new te::da::DataSetName(m_inDsetName);
  te::da::From* fromPol = new te::da::From;
  fromPol->push_back(fromItemPol);

  te::da::Where* w_oid = 0;

  if(m_oidSet)
  w_oid = new te::da::Where(m_oidSet->getExpression());

  te::da::Select select_Pol(pol_fields, fromPol, w_oid);
  te::da::SubSelect subSelect_Pol(select_Pol, "pol");

// Subselect that apply the ST_DumpRings function in geometric column to get polygon as linestring.
  te::da::Fields* line_fields = new te::da::Fields;
  for(std::size_t i = 0; i < props.size(); ++i)
  {
    if(props[i]->getType() != te::dt::GEOMETRY_TYPE)
    {
      te::da::PropertyName* pName = new te::da::PropertyName(props[i]->getName());
      te::da::Field* field = new te::da::Field(pName);
      line_fields->push_back(field);
    }
    else
    {
      te::da::PropertyName* polName = new te::da::PropertyName("polygon");
      te::da::Expression* e_dumpRings = new te::da::ST_DumpRings(polName);
      te::da::Expression* e_boundary = new te::da::ST_Boundary(e_dumpRings);
      te::da::Field* f_boundary = new te::da::Field(*e_boundary, " linestring");
      line_fields->push_back(f_boundary);
    }
  }

  te::da::FromItem* fromItemLine = new te::da::SubSelect(subSelect_Pol);
  te::da::From* fromLine = new te::da::From;
  fromLine->push_back(fromItemLine);

  te::da::Select select_Line(line_fields, fromLine);
  te::da::SubSelect subSelect_Line(select_Line, "line");

// Collect the lines by register
  te::da::Fields* union_fields = new te::da::Fields;
  for(std::size_t i = 0; i < props.size(); ++i)
  {
    if(props[i]->getType() != te::dt::GEOMETRY_TYPE)
    {
      te::da::PropertyName* pName = new te::da::PropertyName(props[i]->getName());
      te::da::Field* field = new te::da::Field(pName);
      union_fields->push_back(field);
    }
    else
    {
      te::da::PropertyName* lineName = new te::da::PropertyName("linestring");
      te::da::Expression* e_union = new te::da::ST_Collect(lineName);
      te::da::Field* f_union = new te::da::Field(*e_union, props[i]->getName());
      union_fields->push_back(f_union);
    }
  }

  te::da::FromItem* fromItem = new te::da::SubSelect(subSelect_Line);
  te::da::From* from = new te::da::From;
  from->push_back(fromItem);

  te::da::Select select(union_fields, from);

// Group by
  te::da::GroupBy* groupBy = new te::da::GroupBy();
  for(std::size_t i = 0; i < props.size(); ++i)
  {
    if(props[i]->getType() != te::dt::GEOMETRY_TYPE)
    {
      te::da::GroupByItem* e_groupBy = new te::da::GroupByItem(props[i]->getName());
      groupBy->push_back(e_groupBy);
    }
  }
  select.setGroupBy(groupBy);

  std::auto_ptr<te::da::DataSet> dsQuery(m_inDsrc->query(select));

  te::vp::Save(m_outDsrc.get(), dsQuery.get(), outDsType.get());
  return true;

}
