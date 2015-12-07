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
  \file terralib/raster/BandProperty.h
 
  \brief It describes one band (or dimension) of a raster.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_BANDPROPERTY_H
#define __TERRALIB_RASTER_INTERNAL_BANDPROPERTY_H

// TerraLib
#include "../common/UnitOfMeasure.h"
#include "Config.h"
#include "Enums.h"

// STL
#include <complex>
#include <string>
#include <vector>

// Boost
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace rst
  {
    /*!
      \class BandProperty

      \brief A raster band description.

      The band property describes how to interpret (and use) values that
      can be associated to each element of a raster, in one given dimension
      (e.g. a band of multi-hiperspectral remote sensing image). Each instance
      of this class refers to a particular dimension. This implementation defines
      a set of information commonly used to describe any raster.

      \ingroup rst

      \sa RasterProperty, Raster, Band
    */
    class TERASTEREXPORT BandProperty
    {
      public:

        /*!
          \struct ColorEntry

          \brief An structure to represent a color tuple.
        */
        struct ColorEntry
        {
          short c1;     //!< gray, red, cyan or hue.
          short c2;     //!< green, magenta, or lightness.
          short c3;     //!< blue, yellow, or saturation.
          short c4;     //!< alpha or blackband.
        };

        /*!
          \brief Constructor.

          \param idx    The band index in the set of bands.
          \param t      The data type of the band elements.
          \param desc   The band description.
        */
        BandProperty(std::size_t idx, int t, const std::string& desc = std::string(""));

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy that would be used to copy from.
        */
        BandProperty(const BandProperty& rhs);

        /*! \brief Destructor. */
        ~BandProperty();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
        */
        BandProperty& operator=(const BandProperty& rhs);

        /*!
          \brief It returns the data type of the elements in the band.

          \note The types are listed in terralib/datatype/DataTypes.h

          \return The data type of the elements in the band.
        */
        int getType() const { return m_type; }

        /*! \brief It returns the unit of measure of the values. */
        const te::common::UnitOfMeasurePtr& getUnitOfMeasure() const { return m_unit; }

        /*!
          \brief Sets the unit of measure of the values;

          \param u The new unit of measure.

          \note The object does not take the ownership of the unit.
        */
        void setUnitOfMeasure(te::common::UnitOfMeasurePtr u) { m_unit = u; }

        /* \brief Returns if the band is complex (true) or not (false). */
        bool isComplex() const;

    public:

      std::size_t m_idx;                                             //!< The band index.
      int m_type;                                                    //!< The data type of the elements in the band ( See te::common basic data types for reference ).
      std::string m_description;                                     //!< A description.
      std::vector<std::pair<std::string, std::string> > m_metadata;  //!< A list of metadata where each element consists of a (key, value).
      double m_noDataValue;                                          //!< Value to indicate elements where there is no data, default is std::numeric_limits<double>::max().
      std::complex<double> m_valuesOffset;                           //!< Offset is the values (real and imaginary) to add to grid values for this sample dimension, default is 0.
      std::complex<double> m_valuesScale;                            //!< Scale is the values (real and imaginary) which is multiplied to grid values for this sample dimension, default is 1.
      std::vector<std::string> m_categoryNames;                      //!< Set of categoric names for the values contained in a dimension.
      ColorInterp m_colorInterp;                                     //!< The color interpretation.
      PaletteInterpretation m_paletteInterp;                         //!< Pallete interpretation.
      std::vector<ColorEntry> m_palette;                             //!< The color palette.
      int m_blkw;                                                    //!< Block width (pixels).
      int m_blkh;                                                    //!< Block height (pixels).
      int m_nblocksx;                                                //!< The number of blocks in x.
      int m_nblocksy;                                                //!< The number of blocks in y.

    protected:

      te::common::UnitOfMeasurePtr m_unit;                       //!< The unit of values.
    };

    typedef boost::shared_ptr<BandProperty> BandPropertyPtr;
    
  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_BANDPROPERTY_H
