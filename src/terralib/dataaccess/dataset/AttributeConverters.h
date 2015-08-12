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
  \file terralib/dataaccess/dataset/AttributeConverters.h

  \brief Definition of attribute converter and a set of them.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ATTRIBUTECONVERTERS_H
#define __TERRALIB_DATAACCESS_INTERNAL_ATTRIBUTECONVERTERS_H

// TerraLib
#include "../../common/CharEncodingConv.h"
#include "../../srs/Config.h"
#include "../Config.h"

// Boost
#include <boost/function.hpp>

// STL
#include <vector>

namespace te
{
// Forward declarations
  namespace dt
  {
    class AbstractData;
  }

  namespace da
  {
// Forward declarations
    class DataSet;

    /*!
      \brief The type of attribute converter functions.
      
      The functions following the typedef signature have:
      <ul>
      <ol>return type: e::dt::AbstractData*</ol>
      <ol>first input parameter: input data set (DataSet*)</ol>
      <ol>second input parameter: the position list of adapted properties in the input dataset (const std::vector<std::size_t>&)</ol>
      <ol>third input parameter: destination data type (int)</ol>
      </ul>
    */
    typedef boost::function3<te::dt::AbstractData*, DataSet*, const std::vector<std::size_t>&, int> AttributeConverter;

    TEDATAACCESSEXPORT te::dt::AbstractData* GenericAttributeConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);

    TEDATAACCESSEXPORT te::dt::AbstractData* XYToPointConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);

    TEDATAACCESSEXPORT te::dt::AbstractData* XYZToPointConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);

    TEDATAACCESSEXPORT te::dt::AbstractData* XYMToPointConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);

    TEDATAACCESSEXPORT te::dt::AbstractData* XYZMToPointConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);

    TEDATAACCESSEXPORT te::dt::AbstractData* PointToXConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);

    TEDATAACCESSEXPORT te::dt::AbstractData* PointToYConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);
    
    TEDATAACCESSEXPORT te::dt::AbstractData* PointToZConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);

    TEDATAACCESSEXPORT te::dt::AbstractData* PointToMConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);

    TEDATAACCESSEXPORT te::dt::AbstractData* TupleToStringConverter(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);

    struct TEDATAACCESSEXPORT CharEncodingConverter
    {
      CharEncodingConverter(const te::common::CharEncoding& toCode)
        : m_toCode(toCode)
      {
      }

      ~CharEncodingConverter() {}

      te::dt::AbstractData* operator()(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);

      te::common::CharEncoding m_toCode;
    };

    struct TEDATAACCESSEXPORT SRIDAssociation
    {
      SRIDAssociation(const int& inputSRID, const int& outputSRID = TE_UNKNOWN_SRS) :
        m_inputSRID(inputSRID),
        m_outputSRID(outputSRID)
      {
      }

      ~SRIDAssociation() {}

      te::dt::AbstractData* operator()(DataSet* dataset, const std::vector<std::size_t>& indexes, int dstType);

      int m_inputSRID;
      int m_outputSRID;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ATTRIBUTECONVERTERS_H
