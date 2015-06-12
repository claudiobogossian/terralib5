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
  \file terralib/raster/RasterIterator.h

  \brief It implements and iterator to "navigate" over a raster,
         with a predefined number of bands.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_RASTERITERATOR_H
#define __TERRALIB_RASTER_INTERNAL_RASTERITERATOR_H

// Terralib
#include "Band.h"
#include "BandIterator.h"
#include "BandProperty.h"

namespace te
{
  namespace rst
  {
// Forward declaration.
    class Raster;

    /*!
      \class RasterIterator

      \brief This class implements and iterator to "navigate" over a raster,
             with a predefined number of bands.

      \ingroup rst

      \sa te::rst::Raster
    */
    template<class T> class RasterIterator
    {
      public:

        /*! \brief Empty constructor. */
        RasterIterator();

        /*!
          \brief Constructor.

          \param r      The raster to iterate.
          \param bands  The vector of band indices that the iterator will use.
        */
        RasterIterator(Raster* r, const std::vector<unsigned int>& bands);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        RasterIterator(const RasterIterator& rhs);

        /*! \brief Destructor. */
        ~RasterIterator();

        /*! \brief Returns the current row in iterator. */
        unsigned int getRow() const;

        /*! \brief Returns the current column in iterator. */
        unsigned int getColumn() const;

        /*!
          \brief Returns the value in current position (column, row, band) from iterator.

          \param i The band index.

          \return The pixel value in current position.
        */
        T operator[](const unsigned int i) const;

        /*! \brief Returns a vector with pixel values from all bands in current position (column, row) from iterator. */
        std::vector<T> operator*() const;

        /*!
          \brief Gets the values from all bands in current position (column, row) from iterator.

          \param v A vector previously created to be filled with values.

          \warning The caller is responsible for creating the vector with enough space.
        */
        void getValues(std::vector<T>& v) const;

        /*! \brief Advances to the next position. */
        void operator++();

        /*! \brief Returns to the previous position. */
        void operator--();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        RasterIterator& operator=(const RasterIterator& rhs);

        /*!
          \brief Difference operator.

          \param rhs The right-hand side to compare.

          \return Returns true if the iterators are at different positions, or false otherwise.
        */
        bool operator!=(const RasterIterator& rhs) const;

        /*!
          \brief Returns an iterator referring to the first value.

          \param r      The raster to iterate.
          \param bands  The vector of band indices that the iterator will use.
        */
        static RasterIterator begin(Raster* r, const std::vector<unsigned int>& bands);

        /*!
          \brief Returns an iterator referring to after the end of the iterator.

          \param r      The raster to iterate.
          \param bands  The vector of band indices that the iterator will use.
        */
        static RasterIterator end(Raster* r, const std::vector<unsigned int>& bands);

      private:

        std::vector<unsigned int> m_b;           //!< The vector of bands to iterate.
        Raster* m_raster;                        //!< The raster from where to get the values.
        std::vector<BandIterator<T> > m_it;      //!< The vector of band iterators.
    };

    template<class T> te::rst::RasterIterator<T>::RasterIterator()
      : m_b(0),
        m_raster(0),
        m_it(0)
    {
    }

    template<class T> te::rst::RasterIterator<T>::RasterIterator(Raster* r, const std::vector<unsigned int>& bands)
      : m_b(bands),
        m_raster(r)
    {
      for(unsigned int i = 0; i < m_b.size(); i++)
        m_it.push_back(te::rst::BandIterator<T>::begin(r->getBand(m_b[i])));
    }

    template<class T> te::rst::RasterIterator<T>::RasterIterator(const RasterIterator<T>& rhs)
    {
      *this = rhs;
    }

    template<class T> te::rst::RasterIterator<T>::~RasterIterator()
    {
    }

    template<class T> unsigned int te::rst::RasterIterator<T>::getRow() const
    {
      return m_it[0].getRow();
    }

    template<class T> unsigned int te::rst::RasterIterator<T>::getColumn() const
    {
      return m_it[0].getColumn();
    }

    template<class T> T te::rst::RasterIterator<T>::operator[](const unsigned int i) const
    {
      return *m_it[i];
    }

    template<class T> std::vector<T> te::rst::RasterIterator<T>::operator*() const
    {
      std::vector<T> values(m_b.size());

      getValues(values);

      return values;
    }

    template<class T> void te::rst::RasterIterator<T>::getValues(std::vector<T>& v) const
    {
      for (unsigned int b = 0; b < m_b.size(); b++)
        v[b] = *m_it[b];
    }

    template<class T> void te::rst::RasterIterator<T>::operator++()
    {
      for (unsigned int b = 0; b < m_b.size(); b++)
        ++m_it[b];
    }

    template<class T> void te::rst::RasterIterator<T>::operator--()
    {
      for (unsigned int b = 0; b < m_b.size(); b++)
        --m_it[b];
    }

    template<class T> te::rst::RasterIterator<T>& te::rst::RasterIterator<T>::operator=(const te::rst::RasterIterator<T>& rhs)
    {
      if (this != &rhs)
      {
        m_b = rhs.m_b;

        m_raster = rhs.m_raster;

        m_it = rhs.m_it;
      }

      return *this;
    }

    template<class T> bool te::rst::RasterIterator<T>::operator!=(const RasterIterator& rhs) const
    {
      return (m_it[0] != rhs.m_it[0]);
    }

    template<class T> te::rst::RasterIterator<T> te::rst::RasterIterator<T>::begin(Raster* r, const std::vector<unsigned int>& bands)
    {
      return te::rst::RasterIterator<T>(r, bands);
    }

    template<class T> te::rst::RasterIterator<T> te::rst::RasterIterator<T>::end(Raster* r, const std::vector<unsigned int>& bands)
    {
      te::rst::RasterIterator<T> it;

      for(unsigned int i = 0; i < bands.size(); i++)
        it.m_it.push_back(te::rst::BandIterator<T>::end(r->getBand(bands[i])));

      return it;
    }

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_RASTERITERATOR_H
