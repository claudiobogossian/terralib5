/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.
 
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
 \file AddressGeocodingOp.h
 
 \brief Address Geocoding operation.
 
 \ingroup vp
 */

#ifndef __TERRALIB_ADDRESSGEOCODING_INTERNAL_ADDRESSGEOCODING_OP_H
#define __TERRALIB_ADDRESSGEOCODING_INTERNAL_ADDRESSGEOCODING_OP_H

//Terralib

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"

#include "../datatype/Property.h"

#include "../memory/DataSet.h"
#include "../statistics/core/Enums.h"

#include "Config.h"

// STL
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace te
{
  namespace addressgeocoding
  {
    class TEADDRESSGEOCODINGEXPORT AddressGeocodingOp
    {
    public:
      
      AddressGeocodingOp();
      
      ~AddressGeocodingOp() {}
      
      void setInput(te::da::DataSourcePtr inDsrc,
                    std::string inDsetName,
                    int inSRID,
                    te::da::DataSourcePtr inAddressDsrc,
                    std::string inAddressDsetName);

      void setParams( std::vector<std::string> associatedProps,
                      std::string streetNumber);

      void setNumAttributes(std::string initialLeft,
                            std::string finalLeft,
                            std::string initialRight,
                            std::string finalRight);

      void setOutput( te::da::DataSourcePtr outDsrc, 
                      std::string m_outDsetName);

      bool paramsAreValid();

      bool run();

    protected:
      
      bool save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsetName);

      std::auto_ptr<te::da::DataSetType> getOutputDataSetType();

      te::gm::LineString* getLineString(te::gm::Geometry* geom);
      
      void getLines(te::gm::Geometry* geom, std::vector<te::gm::LineString*>& lines);

      void getLines(te::gm::GeometryCollection* gc, std::vector<te::gm::LineString*>& lines);

      void getLines(te::gm::LineString* l, std::vector<te::gm::LineString*>& lines);

      te::da::DataSourcePtr m_inDsrc;
      std::string m_inDsetName;
      int m_inSRID;
      te::da::DataSourcePtr m_inAddressDsrc;
      std::string m_inAddressDsetName;

      std::vector<std::string> m_associatedProps;         //!< File attributes.
      std::string m_streetNumber;                         //!< File attribute that represents the address number.

      std::string m_initialLeft;
      std::string m_finalLeft;
      std::string m_initialRight;
      std::string m_finalRight;
      
      te::da::DataSourcePtr m_outDsrc;
      std::string m_outDsetName;
    };
  }
}
#endif // __TERRALIB_ADDRESSGEOCODING_INTERNAL_ADDRESSGEOCODING_OP_H