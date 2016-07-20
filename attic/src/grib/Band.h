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
  \file terralib/grib/Band.h

  \brief Band implemntatin for GRIB.
*/

#ifndef __TERRALIB_GRIB_INTERNAL_BAND_H
#define __TERRALIB_GRIB_INTERNAL_BAND_H

// TerraLib
#include "../raster/Band.h"
#include "Config.h"

// Grib API
#include <grib_api.h>

namespace te
{
  namespace grib
  {
// Forward declaration
    class Raster;

    /*!
      \class Band

      \brief Band implemntatin for GRIB.
    */
    class TEGRIBEXPORT Band : public te::rst::Band
    {
      public:

        Band(Raster* r, std::size_t idx, grib_handle* handle);

        Band(const Band& rhs);

        ~Band();

        te::rst::Raster* getRaster() const;

        Band& operator=(const Band& rhs);

        void getValue(unsigned int c, unsigned int r, double& value) const;

        void setValue(unsigned int c, unsigned int r, const double value);

        void getIValue(unsigned int c, unsigned int r, double& value) const;

        void setIValue(unsigned int c, unsigned int r, const double value);

        void read(int x, int y, void* buffer) const;

        void* read(int x, int y);

        void write(int x, int y, void* buffer);

        grib_handle* getHandle() const;

        long getLong(const char* key) const;

        double getDouble(const char* key) const;

        std::string getString(const char* key) const;

        static std::string getErrMsg(int errCode);

      private:

        Raster* m_raster;       //!< The associated raster.
        grib_handle* m_handle;  //!< The grib handle.
        double* m_data;         //!< The matrix data.
    };

  } // end namespace grib
}   // end namespace te

#endif  // __TERRALIB_GRIB_INTERNAL_BAND_H

