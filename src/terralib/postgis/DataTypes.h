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
  \file terralib/postgis/DataTypes.h
   
  \brief Utility data types for PostgreSQL.  
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_DATATYPES_H
#define __TERRALIB_POSTGIS_INTERNAL_DATATYPES_H

// STL
#include <cstring>

namespace te
{
  namespace pgis
  {
    /*!
      \struct VersionInfo

      \brief A data structure for holding version information.
    */
    struct VersionInfo
    {
      int m_major;
      int m_minor;
      int m_release;
    };

    /*!
      \struct Numeric
      
      \brief Auxiliary structure used to decode Numeric data types.
    */
    struct Numeric
    { 
      int m_ndigits;            //!< number of digits in digits[] - can be 0! 
      int m_weight;             //!< weight of first digit.
      int m_rscale;             //!< result scale.
      int m_dscale;             //!< display scale
      int m_sign;               //!< NUMERIC_POS, NUMERIC_NEG, or NUMERIC_NAN.
      unsigned char* m_buf;     //!< start of alloc'd space for digits[].
      unsigned char* m_digits;  //!< decimal digits
    };

    /*!
      \struct Array
      
      \brief Auxiliary structure used to decode Array data types.

      After the dimensions information we have the following structure:
      <ul>
      <li>lower bnds: lower boundary of each dimension (C array of int)</li>
      <li>null bitmap: bitmap showing locations of nulls (OPTIONAL)</li>
      <li>actual data: whatever is the stored data.</li>
      <ul>

      \note Individual items in the array are aligned as specified by the array element type. They are
            stored in row-major order (last subscript varies most rapidly).
    */
    struct Array
    {
      int m_ndim;               //!< number of dimensions of the array.
      int m_dataoffset;         //!< offset to stored data, or 0 if no nulls bitmap.
      unsigned int m_elemtype;  //!< element type OID.
      int m_dimensions;         //!< length of each array axis (C array of int)      
    };

  } // end namespace pgis
}   // end namespace te

te::pgis::VersionInfo& operator<<(te::pgis::VersionInfo& vinfo, const char* v);

#endif  // __TERRALIB_POSTGIS_INTERNAL_DATATYPES_H

