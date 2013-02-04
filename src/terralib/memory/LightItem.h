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
  \file terralib/memory/LightItem.h

  \brief A thin and lean implementation of DatasetItem class.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_LIGHTITEM_H
#define __TERRALIB_MEMORY_INTERNAL_LIGHTITEM_H

// TerraLib
#include "../dataaccess/dataset/DataSetItem.h"
#include "Config.h"

namespace te
{
  namespace mem
  {
    /*!
      \todo Transformar os metodos em inline e remover o cpp!
    */
    class TEMEMORYEXPORT LightItem : public te::da::DataSetItem
    {
      public:

        LightItem(std::size_t n);

        ~LightItem();

        void resize(std::size_t n);

        te::dt::AbstractData* take(int i);

        void copy(te::da::DataSet* item);

        te::da::DataSetType* getType() { return 0; }

        const te::da::DataSetType* getType() const { return 0; }

        te::da::DataSet* getParent() const { return 0; }

        char getChar(int i) const;

        void setChar(int i, char value);

        unsigned char getUChar(int i) const;

        void setUChar(int i, unsigned char value);

        boost::int16_t getInt16(int i) const;

        void setInt16(int i, boost::int16_t value);

        boost::int32_t getInt32(int i) const;

        void setInt32(int i, boost::int32_t value);

        boost::int64_t getInt64(int i) const;

        void setInt64(int i, boost::int64_t value);

        bool getBool(int i) const;

        void setBool(int i, bool value);

        float getFloat(int i) const;

        void setFloat(int i, float value);

        double getDouble(int i) const;

        void setDouble(int i, double value);

        std::string getNumeric(int i) const;

        void setNumeric(int i, const std::string& value);

        std::string getString(int i) const;

        void setString(int i, const std::string& value);

        te::dt::ByteArray* getByteArray(int i) const;

        void setByteArray(int i, const te::dt::ByteArray& value);

        te::gm::Geometry* getGeometry(int i) const;

        void setGeometry(int i, const te::gm::Geometry& value);

        te::rst::Raster* getRaster(int i) const;

        void setRaster(int i, const te::rst::Raster& value);

        te::dt::DateTime* getDateTime(int i) const;

        void setDateTime(int i, const te::dt::DateTime& value);
        
        void getArray(int i, std::vector<boost::int16_t>& values) const;

        const unsigned char* getWKB(int i) const;

        te::da::DataSet* getDataSet(int i);

        void setDataSet(int i, const te::da::DataSet& value);

        te::dt::AbstractData* getValue(int i) const;

        void setValue(int i, te::dt::AbstractData* value);
        
        bool isNull(int i) const;

      private:

        std::vector<te::dt::AbstractData*> m_data;
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_LIGHTITEM_H
