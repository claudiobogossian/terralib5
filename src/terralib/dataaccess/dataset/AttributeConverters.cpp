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
  \file terralib/dataaccess/dataset/AttributeConverters.cpp

  \brief Definition of attribute converter and a set of them.
*/

// TerraLib
#include "../../datatype/DataConverterManager.h"
#include "../../datatype/SimpleData.h"
#include "../../geometry/Point.h"
#include "../../geometry/PointM.h"
#include "../../geometry/PointZ.h"
#include "../../geometry/PointZM.h"
#include "AttributeConverters.h"
#include "DataSet.h"

// STL
#include <cassert>
#include <memory>

te::dt::AbstractData* te::da::GenericAttributeConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType)
{
  assert(indexes.size() == 1);

  if(dataset->isNull(indexes[0]))
    return 0;

  // Gets the data from input data set
  std::auto_ptr<te::dt::AbstractData> data(dataset->getValue(indexes[0]));
  assert(data.get());

  // Source and Destination Types
  int srcType = data->getTypeCode();
  if(srcType == dstType) // Need conversion?
    return data.release();

  // Try get a data type converter
  const te::dt::DataTypeConverter& converter = te::dt::DataConverterManager::getInstance().get(srcType, dstType);

  // Converts the original data
  te::dt::AbstractData* convertedData = converter(data.get());
  assert(convertedData);

  return convertedData;
}

te::dt::AbstractData* te::da::XYToPointConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int /*dstType*/)
{
  assert(dataset);
  assert(indexes.size() == 2);

  double x = dataset->getDouble(indexes[0]);
  double y = dataset->getDouble(indexes[1]);
  
  return new te::gm::Point(x, y);
}

te::dt::AbstractData* te::da::XYZToPointConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int /*dstType*/)
{
  assert(dataset);
  assert(indexes.size() == 3);

  double x = dataset->getDouble(indexes[0]);
  double y = dataset->getDouble(indexes[1]);
  double z = dataset->getDouble(indexes[2]);
  
  return new te::gm::PointZ(x, y, z);
}

te::dt::AbstractData* te::da::XYMToPointConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int /*dstType*/)
{
  assert(dataset);
  assert(indexes.size() == 3);

  double x = dataset->getDouble(indexes[0]);
  double y = dataset->getDouble(indexes[1]);
  double m = dataset->getDouble(indexes[2]);
  
  return new te::gm::PointM(x, y, m);
}

te::dt::AbstractData* te::da::XYZMToPointConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int /*dstType*/)
{
  assert(dataset);
  assert(indexes.size() == 4);

  double x = dataset->getDouble(indexes[0]);
  double y = dataset->getDouble(indexes[1]);
  double z = dataset->getDouble(indexes[2]);
  double m = dataset->getDouble(indexes[3]);
  
  return new te::gm::PointZM(x, y, z, m);
}

te::dt::AbstractData* te::da::PointToXConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int /*dstType*/)
{
  assert(dataset);
  assert(indexes.size() == 1);

  te::gm::Point* pt = static_cast<te::gm::Point*>(dataset->getGeometry(indexes[0]).get());

  return new te::dt::Double(pt->getX());
}

te::dt::AbstractData* te::da::PointToYConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int /*dstType*/)
{
  assert(dataset);
  assert(indexes.size() == 1);

  te::gm::Point* pt = static_cast<te::gm::Point*>(dataset->getGeometry(indexes[0]).get());

  return new te::dt::Double(pt->getY());
}

te::dt::AbstractData* te::da::PointToZConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int /*dstType*/)
{
  assert(dataset);
  assert(indexes.size() == 1);

  te::gm::Point* pt = static_cast<te::gm::Point*>(dataset->getGeometry(indexes[0]).get());

  return new te::dt::Double(pt->getZ());
}

te::dt::AbstractData* te::da::PointToMConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int /*dstType*/)
{
  assert(dataset);
  assert(indexes.size() == 1);

  te::gm::Point* pt = static_cast<te::gm::Point*>(dataset->getGeometry(indexes[0]).get());

  return new te::dt::Double(pt->getM());
}

te::dt::AbstractData* te::da::TupleToStringConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int /*dstType*/)
{
  assert(dataset);

  std::string result = "[ ";

  std::vector<std::size_t>::const_iterator it;
  for(it = indexes.begin(); it != indexes.end(); ++it)
    result += dataset->getAsString(*it) + " ";
  
  result += "]";

  return new te::dt::String(result);
}

te::dt::AbstractData* te::da::CharEncodingConverter::operator()(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType)
{
  assert(dataset);
  assert(indexes.size() == 1);

  std::size_t pos = indexes[0];

  assert(dataset->getPropertyDataType(pos) == te::dt::STRING_TYPE);
  assert(dstType == te::dt::STRING_TYPE);

  std::string value = dataset->getString(pos);
  te::common::CharEncoding fromCode = dataset->getPropertyCharEncoding(pos);

  if(fromCode == m_toCode)
    return new te::dt::String(value);

  if(fromCode == te::common::UNKNOWN_CHAR_ENCODING || m_toCode == te::common::UNKNOWN_CHAR_ENCODING)
    return new te::dt::String(value);

#ifdef TERRALIB_CHARENCODING_ENABLED
  try
  {
    std::string result = te::common::CharEncodingConv::convert(value, fromCode, m_toCode);
    return new te::dt::String(result);
  }
  catch(...)
  {
    return new te::dt::String(value);
  }
#else
    return new te::dt::String(value);
#endif
}
