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
  \file terralib/dataaccess/serialization/xml/DataSetType.h
 
  \brief Support for DataSetType serialization.
*/

#ifndef __TERRALIB_SERIALIZATION_DATAACCESS_INTERNAL_DATASETTYPE_H
#define __TERRALIB_SERIALIZATION_DATAACCESS_INTERNAL_DATASETTYPE_H

// TerraLib
#include "../../query/Distinct.h"
#include "../../query/Fields.h"
#include "../../query/From.h"
#include "../../query/GroupBy.h"
#include "../../query/OrderBy.h"
#include "../../Config.h"

// STL
#include <vector>

namespace te
{
  namespace da
  {
    class DataSetType;
    class DataSourceCapabilities;
    class DataSourceCatalog;
    class DataSourceInfo;
    class Field;
    class Function;
    class GroupByItem;
    class Having;
    class Literal;
    class OrderByItem;
    class PropertyName;
    class Select;
    class SQLDialect;
    class Where;
  }

  namespace xml
  {
    class AbstractWriter;
    class Reader;
  }

  namespace serialize
  {
    namespace xml
    {
      TEDATAACCESSEXPORT void ReadDataSourceInfo(const std::string& datasourcesFileName);

      TEDATAACCESSEXPORT te::da::DataSourceInfo* ReadDataSourceInfo(te::xml::Reader& reader);

      TEDATAACCESSEXPORT void Save(const std::string& fileName);

      TEDATAACCESSEXPORT void Save(te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT te::da::DataSourceCatalog* ReadDataSourceCatalog(te::xml::Reader& reader);

      TEDATAACCESSEXPORT void Save(const te::da::DataSourceCatalog* catalog, const std::string& fileName);

      TEDATAACCESSEXPORT void Save(const te::da::DataSourceCatalog* catalog, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT te::da::DataSetType* ReadDataSetType(te::xml::Reader& reader);

      TEDATAACCESSEXPORT void Save(const te::da::DataSetType* dt, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Read(const std::string& dialectFileName, te::da::DataSourceCapabilities& capabilities, te::da::SQLDialect& dialect);

      TEDATAACCESSEXPORT te::da::SQLDialect* ReadDialect(const std::string& dialectFileName);

      TEDATAACCESSEXPORT te::da::Distinct* ReadDistinct(te::xml::Reader& reader);

      TEDATAACCESSEXPORT te::da::Fields* ReadFields(te::xml::Reader& reader);

      TEDATAACCESSEXPORT te::da::From* ReadFrom(te::xml::Reader& reader);

      TEDATAACCESSEXPORT te::da::FromItem* ReadFromItem(te::xml::Reader& reader);

      TEDATAACCESSEXPORT te::da::Function* ReadFunction(te::xml::Reader& reader);

      TEDATAACCESSEXPORT te::da::GroupBy* ReadGroupBy(te::xml::Reader& reader);

      TEDATAACCESSEXPORT te::da::Having* ReadHaving(te::xml::Reader& reader);

      TEDATAACCESSEXPORT te::da::Literal* ReadLiteral(te::xml::Reader& reader);

      TEDATAACCESSEXPORT te::da::OrderBy* ReadOrderBy(te::xml::Reader& reader);

      TEDATAACCESSEXPORT te::da::PropertyName* ReadPropertyName(te::xml::Reader& reader);

      TEDATAACCESSEXPORT te::da::Select* ReadSelect(te::xml::Reader& reader);

      TEDATAACCESSEXPORT te::da::Where* ReadWhere(te::xml::Reader& reader);

      TEDATAACCESSEXPORT void Save(const te::da::Distinct* distinct, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::Expression* expression, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::Field* field, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::Fields* fields, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::From* from, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::FromItem* fromItem, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::Function* func, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::GroupByItem* groupByItem, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::GroupBy* groupBy, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::Having* having, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::Literal* lit, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::OrderByItem* orderByItem, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::OrderBy* orderBy, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::PropertyName* propertyName, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::Select* select, te::xml::AbstractWriter& writer);

      TEDATAACCESSEXPORT void Save(const te::da::Where* wh, te::xml::AbstractWriter& writer);

    } // end namespace xml
  }   // end namespace serialize
}     // end namespace te

#endif  // __TERRALIB_SERIALIZATION_DATAACCESS_INTERNAL_DATASETTYPE_H

