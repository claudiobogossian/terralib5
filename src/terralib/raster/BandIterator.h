/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/raster/BandIterator.h

  \brief It implements an iterator to "navigate" over a single band (const or not const).
*/

#ifndef __TERRALIB_RASTER_INTERNAL_BANDITERATOR_H
#define __TERRALIB_RASTER_INTERNAL_BANDITERATOR_H

// Terralib
#include "Band.h"
#include "BandProperty.h"
#include "BlockUtils.h"
#include "IterationStrategy.h"

namespace te
{
// Forward declaration.
  namespace gm
  {
    class Geometry;
  }

  namespace rst
  {
// Forward declaration.
    class Band;

    /*!
      \class AbstractBandIterator

      \brief This class implements an abstract iterator to "navigate" over a single band.

      We provide an efficient method to obtain all values in a raster, without regard
      to their locations. The implementation navigates through the blocks of the image.

      \sa te::rst::Band
    */
    template<class T> class AbstractBandIterator
    {
      public:

        /*! \brief Constructor. */
        AbstractBandIterator();

        /*!
          \brief Constructor to iterate over an entire image.

          \param b The band to iterate.
        */
        AbstractBandIterator(const Band* b);

        /*!
          \brief Constructor with spatial restriction of a geometry.

          \param b The band to iterate.
          \param g The geometry (i.e. a polygon) from where the iteration will navigate.
        */
        AbstractBandIterator(const Band* b, const te::gm::Geometry* g);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        AbstractBandIterator(const AbstractBandIterator& rhs);

        /*! \brief Destructor. */
        ~AbstractBandIterator();

        /*! \brief Returns the value in current position (column, row) from iterator. */
        virtual const T operator*() const = 0;

        /*! \brief Returns the current row in iterator. */
        unsigned int getRow() const;

        /*! \brief Returns the current column in iterator. */
        unsigned int getCol() const;

        /*! \brief Advances to the next position. */
        void operator++();

        /*! \brief Returns to the previous position. */
        void operator--();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        AbstractBandIterator& operator=(const AbstractBandIterator& rhs);

        /*!
          \brief Difference operator.

          \param rhs The right-hand side to compare.

          \return Returns true if the iterators are at different positions, or false otherwise.
        */
        bool operator!=(const AbstractBandIterator& rhs) const;

      protected:

        AbstractIterationStrategy<T>* m_itstrategy;   //!< The strategy employed to navigate over the band, e.g. through a line, inside a bounding box or polygon, etc.

    };

    /*!
      \class BandIterator

      \brief This class implements an iterator to "navigate" over a single band.

      We provide an efficient method to obtain all values in a raster, without regard
      to their locations. The implementation navigates through the blocks of the image.

      \sa te::rst::Band
    */
    template<class T> class BandIterator : public te::rst::AbstractBandIterator<T>
    {
      public:

        BandIterator();

        BandIterator(Band* b);

        BandIterator(Band* b, te::gm::Geometry* g);

        BandIterator(const BandIterator& rhs);

        T operator*();

        const T operator*() const;

        BandIterator& operator=(const BandIterator& rhs);

        static BandIterator begin(Band* b);

        static BandIterator begin(Band* b, te::gm::Geometry* g);

        static BandIterator end(Band* b);

        static BandIterator end(Band* b, te::gm::Geometry* g);

    };

    /*!
      \class ConstBandIterator

      \brief This class implements an iterator to "navigate" over a single band (const).

      We provide an efficient method to obtain all values in a raster, without regard
      to their locations. The implementation navigates through the blocks of the image.

      \sa te::rst::Band
    */
    template<class T> class ConstBandIterator : public te::rst::AbstractBandIterator<T>
    {
      public:

        ConstBandIterator();

        ConstBandIterator(const Band* b);

        ConstBandIterator(const Band* b, const te::gm::Geometry* g);

        ConstBandIterator(const ConstBandIterator& rhs);

        const T operator*() const;

        ConstBandIterator& operator=(const ConstBandIterator& rhs);

        static ConstBandIterator begin(const Band* b);

        static ConstBandIterator begin(const Band* b, const te::gm::Geometry* g);

        static ConstBandIterator end(const Band* b);

        static ConstBandIterator end(const Band* b, const te::gm::Geometry* g);

    };

    template<class T> te::rst::AbstractBandIterator<T>::AbstractBandIterator()
      : m_itstrategy()
    {
    }

    template<class T> te::rst::AbstractBandIterator<T>::AbstractBandIterator(const Band* b)
    {
      m_itstrategy = new BlockedIterationStrategy<T>(b);
    }

    template<class T> te::rst::AbstractBandIterator<T>::AbstractBandIterator(const Band* b, const te::gm::Geometry* g)
    {
      m_itstrategy = new BoundedIterationStrategy<T>(b, g);
    }

    template<class T> te::rst::AbstractBandIterator<T>::AbstractBandIterator(const AbstractBandIterator<T>& rhs)
      : m_itstrategy(rhs.m_itstrategy)
    {
    }

    template<class T> te::rst::AbstractBandIterator<T>::~AbstractBandIterator()
    {
    }

    template<class T> unsigned int te::rst::AbstractBandIterator<T>::getRow() const
    {
      return m_itstrategy->getRow();
    }

    template<class T> unsigned int te::rst::AbstractBandIterator<T>::getCol() const
    {
      return m_itstrategy->getCol();
    }

    template<class T> void te::rst::AbstractBandIterator<T>::operator++()
    {
      m_itstrategy->operator++();
    }

    template<class T> void te::rst::AbstractBandIterator<T>::operator--()
    {
      m_itstrategy->operator--();
    }

    template<class T> te::rst::AbstractBandIterator<T>& te::rst::AbstractBandIterator<T>::operator=(const te::rst::AbstractBandIterator<T>& rhs)
    {
      if (this != &rhs)
        m_itstrategy = rhs.m_itstrategy;

      return *this;
    }

    template<class T> bool te::rst::AbstractBandIterator<T>::operator!=(const AbstractBandIterator<T>& rhs) const
    {
      return (m_itstrategy->operator!=(*rhs.m_itstrategy));
    }

    template<class T> te::rst::BandIterator<T>::BandIterator()
      : te::rst::AbstractBandIterator<T>()
    {
    }

    template<class T> te::rst::BandIterator<T>::BandIterator(Band* b)
      : te::rst::AbstractBandIterator<T>(b)
    {
    }

    template<class T> te::rst::BandIterator<T>::BandIterator(Band* b, te::gm::Geometry* g)
      : te::rst::AbstractBandIterator<T>(b, g)
    {
    }

    template<class T> te::rst::BandIterator<T>::BandIterator(const BandIterator<T>& rhs)
      : te::rst::AbstractBandIterator<T>(rhs)
    {
    }

    template<class T> T te::rst::BandIterator<T>::operator*()
    {
      return (T) this->m_itstrategy->operator*();
    }

    template<class T> const T te::rst::BandIterator<T>::operator*() const
    {
      return (T) this->m_itstrategy->operator*();
    }

    template<class T> te::rst::BandIterator<T>& te::rst::BandIterator<T>::operator=(const te::rst::BandIterator<T>& rhs)
    {
      te::rst::AbstractBandIterator<T>::operator=(rhs);

      return *this;
    }

    template<class T> te::rst::BandIterator<T> te::rst::BandIterator<T>::begin(Band* b)
    {
      return te::rst::BandIterator<T>(b);
    }

    template<class T> te::rst::BandIterator<T> te::rst::BandIterator<T>::begin(Band* b, te::gm::Geometry* g)
    {
      return te::rst::BandIterator<T>(b, g);
    }

    template<class T> te::rst::BandIterator<T> te::rst::BandIterator<T>::end(Band* b)
    {
      te::rst::BandIterator<T> it(b);

      it.m_itstrategy->setEnd();

      return it;
    }

    template<class T> te::rst::BandIterator<T> te::rst::BandIterator<T>::end(Band* b, te::gm::Geometry* g)
    {
      te::rst::BandIterator<T> it(b, g);

      it.m_itstrategy->setEnd();

      return it;
    }

    template<class T> te::rst::ConstBandIterator<T>::ConstBandIterator()
      : te::rst::AbstractBandIterator<T>()
    {
    }

    template<class T> te::rst::ConstBandIterator<T>::ConstBandIterator(const Band* b)
      : te::rst::AbstractBandIterator<T>(b)
    {
    }

    template<class T> te::rst::ConstBandIterator<T>::ConstBandIterator(const ConstBandIterator<T>& rhs)
      : te::rst::AbstractBandIterator<T>(rhs)
    {
    }

    template<class T> te::rst::ConstBandIterator<T>::ConstBandIterator(const Band* b, const te::gm::Geometry* g)
      : te::rst::AbstractBandIterator<T>(b, g)
    {
    }

    template<class T> const T te::rst::ConstBandIterator<T>::operator*() const
    {
      return (T) this->m_itstrategy->operator*();
    }

    template<class T> te::rst::ConstBandIterator<T>& te::rst::ConstBandIterator<T>::operator=(const te::rst::ConstBandIterator<T>& rhs)
    {
      te::rst::AbstractBandIterator<T>::operator=(rhs);

      return *this;
    }

    template<class T> te::rst::ConstBandIterator<T> te::rst::ConstBandIterator<T>::begin(const Band* b)
    {
      return te::rst::ConstBandIterator<T>(b);
    }

    template<class T> te::rst::ConstBandIterator<T> te::rst::ConstBandIterator<T>::begin(const Band* b, const te::gm::Geometry* g)
    {
      return te::rst::ConstBandIterator<T>(b, g);
    }

    template<class T> te::rst::ConstBandIterator<T> te::rst::ConstBandIterator<T>::end(const Band* b)
    {
      te::rst::ConstBandIterator<T> it(b);

      it.m_itstrategy->setEnd();

      return it;
    }

    template<class T> te::rst::ConstBandIterator<T> te::rst::ConstBandIterator<T>::end(const Band* b, const te::gm::Geometry* g)
    {
      te::rst::ConstBandIterator<T> it(b, g);

      it.m_itstrategy->setEnd();

      return it;
    }

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_BANDITERATOR_H
