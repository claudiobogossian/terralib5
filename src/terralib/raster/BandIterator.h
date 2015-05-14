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
  \file terralib/raster/BandIterator.h

  \brief It implements an iterator to "navigate" over a single band (const or not const).
*/

#ifndef __TERRALIB_RASTER_INTERNAL_BANDITERATOR_H
#define __TERRALIB_RASTER_INTERNAL_BANDITERATOR_H

// Terralib
#include "Band.h"
#include "BandProperty.h"
#include "BlockUtils.h"

// Boost
#include <boost/dynamic_bitset.hpp>

namespace te
{
  namespace rst
  {
// Forward declaration.
    class Band;

    /*!
      \class AbstractBandIterator

      \brief This class implements an abstract iterator to "navigate" over a single band.

      We provide an efficient method to obtain all values in a raster, without regard
      to their locations. The implementation navigates through the blocks of the image.

      \ingroup rst

      \sa te::rst::Band
    */
    template<class T> class AbstractBandIterator
    {
      public:

        /*! \brief Constructor. */
        AbstractBandIterator();

        /*!
          \brief Constructor.

          \param b The band to iterate.
        */
        AbstractBandIterator(const te::rst::Band* b);

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
        unsigned int getColumn() const;

        /*! \brief Advances to the next position. */
        virtual void operator++();

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

        /*! \brief Replaces the current bufferized block values. */
        virtual void replaceBlock() = 0;

      protected:

        int m_blkw;                              //!< The internal block width.
        int m_blkh;                              //!< The internal block height.
        int m_npxlsblk;                          //!< The maximum number of pixels inside the block.
        int m_nblocksx;                          //!< The number of blocks in X direction.
        int m_nblocksy;                          //!< The number of blocks in Y direction.
        int m_i;                                 //!< The actual position inside the block.
        int m_blkx;                              //!< The position in X of the current block.
        int m_blky;                              //!< The position in Y of the current block.
        int m_lastblksize;                       //!< The number of pixels inside the last block.
        int m_blksize;                           //!< The block size of the band.
        T* m_blk;                                //!< Internal block.
        te::rst::GetBufferValueFPtr m_getBuff;   //!< A pointer to a function that helps to extract a double or complex value from a specific buffer data type (char, int16, int32, float, ...).
        te::rst::GetBufferValueFPtr m_getBuffI;  //!< A pointer to a function that helps to extract the imaginary part value from a specific buffer data type (cint16, cint32, cfloat, cdouble).
        te::rst::SetBufferValueFPtr m_setBuff;   //!< A pointer to a function that helps to insert a double or complex value into a specific buffer data type (char, int16, int32, float, ...).
        te::rst::SetBufferValueFPtr m_setBuffI;  //!< A pointer to a function that helps to insert the imaginary part value into a specific buffer data type (cint16, cint32, cfloat, cdouble).

    };

    /*!
      \class BandIterator

      \brief This class implements an iterator to "navigate" over a single band.

      We provide an efficient method to obtain all values in a raster, without regard
      to their locations. The implementation navigates through the blocks of the image.

      \ingroup rst

      \sa te::rst::Band
    */
    template<class T> class BandIterator : public te::rst::AbstractBandIterator<T>
    {
      public:

        /*! \brief Constructor. */
        BandIterator();

        /*!
          \brief Constructor.

          \param b The band to iterate.
        */
        BandIterator(Band* b);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        BandIterator(const BandIterator& rhs);

        /*! \brief Returns the value in current position (column, row) from iterator. */
        T operator*();

        /*! \brief Returns the value in current position (column, row) from iterator. */
        const T operator*() const;

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        BandIterator& operator=(const BandIterator& rhs);

        /*! \brief Returns an iterator referring to the first value of the band.*/
        static BandIterator begin(Band* b);

        /*! \brief Returns an iterator referring to after the end of the iterator. */
        static BandIterator end(Band* b);

        /*! \brief Replaces the current bufferized block values. */
        void replaceBlock();

      protected:

        Band* m_band;                            //!< The band from where to get the values.

    };

    /*!
      \class ConstBandIterator

      \brief This class implements an iterator to "navigate" over a single band (const).

      We provide an efficient method to obtain all values in a raster, without regard
      to their locations. The implementation navigates through the blocks of the image.

      \ingroup rst

      \sa te::rst::Band
    */
    template<class T> class ConstBandIterator : public te::rst::AbstractBandIterator<T>
    {
      public:

        /*! \brief Constructor. */
        ConstBandIterator();

        /*!
          \brief Constructor.

          \param b The band to iterate.
        */
        ConstBandIterator(const Band* b);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        ConstBandIterator(const ConstBandIterator& rhs);

        /*! \brief Returns the value in current position (column, row) from iterator. */
        const T operator*() const;

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        ConstBandIterator& operator=(const ConstBandIterator& rhs);

        /*! \brief Returns an iterator referring to the first value of the band.*/
        static ConstBandIterator begin(const Band* b);

        /*! \brief Returns an iterator referring to after the end of the iterator. */
        static ConstBandIterator end(const Band* b);

        /*! \brief Replaces the current bufferized block values. */
        void replaceBlock();

      protected:

        const Band* m_band;                      //!< The band from where to get the values.
    };

    /*!
      \class BandIteratorWithMask

      \brief This class implements an iterator to "navigate" over a single band, with a spatial restriction given by a mask.

      We provide an efficient method to obtain all values in a raster, without regard
      to their locations. The implementation navigates through the blocks of the image.
      The mask is a 1bit raster where the pixels with value 0 are not returned by the
      iterator, and pixels with value 1 are returned.

      \ingroup rst

      \sa te::rst::Band
    */
    template<class T> class BandIteratorWithMask : public te::rst::BandIterator<T>
    {
      public:

        /*! \brief Constructor. */
        BandIteratorWithMask();

        /*!
          \brief Constructor.

          \param b The band to iterate.
          \param m The raster with the mask.
        */
        BandIteratorWithMask(Band* b, Raster* m);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        BandIteratorWithMask(const BandIteratorWithMask& rhs);

        void operator++();

        void operator--();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        BandIteratorWithMask& operator=(const BandIteratorWithMask& rhs);

        /*! \brief Returns an iterator with the mask referring to the first value of the band.*/
        static BandIteratorWithMask begin(Band* b, Raster* m);

        /*! \brief Returns an iterator with the mask referring to after the end of the iterator. */
        static BandIteratorWithMask end(Band* b, Raster* m);

      protected:
        boost::dynamic_bitset<> m_mask;          //!< The internal mask of bits, one bit per pixel.
        unsigned int m_masksize;                 //!< The size of the mask (rows * columns of the mask raster).
        unsigned int m_currentpixelindex;        //!< The index of the current pixel location.

    };

// Abstract Band Iterator implementation
    template<class T> te::rst::AbstractBandIterator<T>::AbstractBandIterator()
      : m_blkw(-1),
        m_blkh(-1),
        m_npxlsblk(-1),
        m_nblocksx(-1),
        m_nblocksy(-1),
        m_i(-1),
        m_blkx(-1),
        m_blky(-1),
        m_lastblksize(-1),
        m_blksize(-1),
        m_blk(0),
        m_getBuff(0),
        m_getBuffI(0),
        m_setBuff(0),
        m_setBuffI(0)
    {
    }

    template<class T> te::rst::AbstractBandIterator<T>::AbstractBandIterator(const te::rst::Band* b)
      : m_blkw(b->getProperty()->m_blkw),
        m_blkh(b->getProperty()->m_blkh),
        m_npxlsblk(m_blkw * m_blkh),
        m_nblocksx(b->getProperty()->m_nblocksx),
        m_nblocksy(b->getProperty()->m_nblocksy),
        m_i(0),
        m_blkx(0),
        m_blky(0),
        m_blksize(b->getBlockSize()),
        m_getBuff(0),
        m_getBuffI(0),
        m_setBuff(0),
        m_setBuffI(0)
    {
      m_lastblksize = m_npxlsblk - ((m_npxlsblk * m_nblocksx * m_nblocksy) -
                                    (b->getRaster()->getNumberOfColumns() * b->getRaster()->getNumberOfRows()));

      m_blk = new T[m_blksize];

      te::rst::SetBlockFunctions(&m_getBuff, &m_getBuffI, &m_setBuff, &m_setBuffI, b->getProperty()->getType());
    }

    template<class T> te::rst::AbstractBandIterator<T>::AbstractBandIterator(const AbstractBandIterator<T>& rhs)
      : m_blkw(rhs.m_blkw),
        m_blkh(rhs.m_blkh),
        m_npxlsblk(rhs.m_npxlsblk),
        m_nblocksx(rhs.m_nblocksx),
        m_nblocksy(rhs.m_nblocksy),
        m_i(rhs.m_i),
        m_blkx(rhs.m_blkx),
        m_blky(rhs.m_blky),
        m_lastblksize(rhs.m_lastblksize),
        m_blksize(rhs.m_blksize),
        m_getBuff(rhs.m_getBuff),
        m_getBuffI(rhs.m_getBuffI),
        m_setBuff(rhs.m_setBuff),
        m_setBuffI(rhs.m_setBuffI)
    {
      if (m_blksize > 0)
        m_blk = new T[m_blksize];
    }

    template<class T> te::rst::AbstractBandIterator<T>::~AbstractBandIterator()
    {
      if (m_blksize > 0)
        delete [] m_blk;
    }

    template<class T> unsigned int te::rst::AbstractBandIterator<T>::getRow() const
    {
      return m_blky * m_blkh + (m_i - (m_i % m_blkw)) / m_blkw;
    }

    template<class T> unsigned int te::rst::AbstractBandIterator<T>::getColumn() const
    {
      return m_blkx * m_blkw + m_i % m_blkw;
    }

    template<class T> void te::rst::AbstractBandIterator<T>::operator++()
    {
      m_i++;

      if(m_i < m_npxlsblk)
        return;

      m_i = 0;

      m_blkx++;

      if(m_blkx < m_nblocksx)
      {
        replaceBlock();

        return;
      }

      m_blkx = 0;

      m_blky++;

      if(m_blky < m_nblocksy)
        replaceBlock();

      if((m_blkx == m_nblocksx - 1) && (m_blky == m_nblocksy - 1))
        m_npxlsblk = m_lastblksize;
    }

    template<class T> void te::rst::AbstractBandIterator<T>::operator--()
    {
      m_i--;

      if(m_i >= 0)
        return;

      m_i = m_npxlsblk - 1;

      m_blkx--;

      if(m_blkx >= 0)
      {
        replaceBlock();

        return;
      }

      m_blkx = m_nblocksx - 1;

      m_blky--;

      if(m_blky >= 0)
        replaceBlock();
      else
        m_blky = m_nblocksy;
    }

    template<class T> te::rst::AbstractBandIterator<T>& te::rst::AbstractBandIterator<T>::operator=(const te::rst::AbstractBandIterator<T>& rhs)
    {
      if (this != &rhs)
      {
        m_blkw = rhs.m_blkw;
        m_blkh = rhs.m_blkh;
        m_npxlsblk = rhs.m_npxlsblk;
        m_nblocksx = rhs.m_nblocksx;
        m_nblocksy = rhs.m_nblocksy;
        m_i = rhs.m_i;
        m_blkx = rhs.m_blkx;
        m_blky = rhs.m_blky;
        m_lastblksize = rhs.m_lastblksize;

        delete [] m_blk;
        m_blk = new T[rhs.m_blksize];
      }

      return *this;
    }

    template<class T> bool te::rst::AbstractBandIterator<T>::operator!=(const AbstractBandIterator& rhs) const
    {
      return (m_blky != rhs.m_blky);
    }

// Band Iterator implementation
    template<class T> te::rst::BandIterator<T>::BandIterator()
      : te::rst::AbstractBandIterator<T>(),
        m_band(0)
    {
    }

    template<class T> te::rst::BandIterator<T>::BandIterator(te::rst::Band* b)
      : te::rst::AbstractBandIterator<T>(b),
        m_band(b)
    {
      if (this->m_blksize > 0)
        this->replaceBlock();
    }

    template<class T> te::rst::BandIterator<T>::BandIterator(const BandIterator<T>& rhs)
      : te::rst::AbstractBandIterator<T>(rhs),
        m_band(rhs.m_band)
    {
      if (this->m_blksize > 0)
        this->replaceBlock();
    }

    template<class T> T te::rst::BandIterator<T>::operator*()
    {
      double value;

      this->m_getBuff(this->m_i, this->m_blk, &value);

      return (T) value;
    }

    template<class T> const T te::rst::BandIterator<T>::operator*() const
    {
      double value;

      this->m_getBuff(this->m_i, this->m_blk, &value);

      return (T) value;
    }

    template<class T> te::rst::BandIterator<T>& te::rst::BandIterator<T>::operator=(const te::rst::BandIterator<T>& rhs)
    {
      te::rst::AbstractBandIterator<T>::operator=(rhs);

      m_band = rhs.m_band;

      return *this;
    }

    template<class T> te::rst::BandIterator<T> te::rst::BandIterator<T>::begin(te::rst::Band* b)
    {
      return te::rst::BandIterator<T>(b);
    }

    template<class T> te::rst::BandIterator<T> te::rst::BandIterator<T>::end(te::rst::Band* b)
    {
      te::rst::BandIterator<T> it;

      it.m_blky = b->getProperty()->m_nblocksy;

      return it;
    }

    template<class T> void te::rst::BandIterator<T>::replaceBlock()
    {
      m_band->read(this->m_blkx, this->m_blky, this->m_blk);
    }

// Const Band Iterator implementation
    template<class T> te::rst::ConstBandIterator<T>::ConstBandIterator()
      : te::rst::AbstractBandIterator<T>(),
        m_band(0)
    {
    }

    template<class T> te::rst::ConstBandIterator<T>::ConstBandIterator(const te::rst::Band* b)
      : te::rst::AbstractBandIterator<T>(b), // (te::rst::Band*)(b) ?
        m_band(b)
    {
      if (this->m_blksize > 0)
        this->replaceBlock();
    }

    template<class T> te::rst::ConstBandIterator<T>::ConstBandIterator(const ConstBandIterator<T>& rhs)
      : te::rst::AbstractBandIterator<T>(rhs),
        m_band(rhs.m_band)
    {
      if (this->m_blksize > 0)
        this->replaceBlock();
    }

    template<class T> const T te::rst::ConstBandIterator<T>::operator*() const
    {
      double value;

      this->m_getBuff(this->m_i, this->m_blk, &value);

      return (T) value;
    }

    template<class T> te::rst::ConstBandIterator<T>& te::rst::ConstBandIterator<T>::operator=(const te::rst::ConstBandIterator<T>& rhs)
    {
      te::rst::AbstractBandIterator<T>::operator=(rhs);

      m_band = rhs.m_band;

      return *this;
    }

    template<class T> te::rst::ConstBandIterator<T> te::rst::ConstBandIterator<T>::begin(const te::rst::Band* b)
    {
      return te::rst::ConstBandIterator<T>(b);
    }

    template<class T> te::rst::ConstBandIterator<T> te::rst::ConstBandIterator<T>::end(const te::rst::Band* b)
    {
      te::rst::ConstBandIterator<T> it;

      it.m_blky = b->getProperty()->m_nblocksy;

      return it;
    }

    template<class T> void te::rst::ConstBandIterator<T>::replaceBlock()
    {
      m_band->read(this->m_blkx, this->m_blky, this->m_blk);
    }

// Band Iterator With Mask implementation
    template<class T> te::rst::BandIteratorWithMask<T>::BandIteratorWithMask()
      : te::rst::BandIterator<T>(),
        m_currentpixelindex(0),
        m_masksize(0),
        m_mask(0)
    {
    }

    template<class T> te::rst::BandIteratorWithMask<T>::BandIteratorWithMask(te::rst::Band* b, te::rst::Raster* m)
      : te::rst::BandIterator<T>(b),
        m_currentpixelindex(0),
        m_mask(0)
    {
// fill bitset maks with raster values
      te::rst::BandIterator<unsigned char> it = te::rst::BandIterator<unsigned char>::begin(m->getBand(0));
      te::rst::BandIterator<unsigned char> itend = te::rst::BandIterator<unsigned char>::end(m->getBand(0));

      m_masksize = m->getNumberOfColumns() * m->getNumberOfRows();
      m_mask.resize(m_masksize);

      unsigned int i = 0;
      while(it != itend)
      {
        m_mask[i++] = *it;

        ++it;
      }

// to avoid the first position be outside the mask
      if (!m_mask[0])
        operator++();
    }

    template<class T> te::rst::BandIteratorWithMask<T>::BandIteratorWithMask(const BandIteratorWithMask<T>& rhs)
      : te::rst::BandIterator<T>(rhs),
        m_mask(rhs.m_mask)
    {
    }

    template<class T> void te::rst::BandIteratorWithMask<T>::operator++()
    {
      do
      {
        te::rst::AbstractBandIterator<T>::operator++();

        m_currentpixelindex++;

        if (m_currentpixelindex >= m_masksize)
          break;
      }
      while (m_mask[m_currentpixelindex] == 0);
    }

    template<class T> void te::rst::BandIteratorWithMask<T>::operator--()
    {
      do
      {
        te::rst::AbstractBandIterator<T>::operator--();

        m_currentpixelindex--;

        if (m_currentpixelindex < 0)
          break;
      }
      while (m_mask[m_currentpixelindex] == 0);
    }

    template<class T> te::rst::BandIteratorWithMask<T>& te::rst::BandIteratorWithMask<T>::operator=(const te::rst::BandIteratorWithMask<T>& rhs)
    {
      te::rst::BandIterator<T>::operator=(rhs);

      m_currentpixelindex = rhs.m_currentpixelindex;
      m_masksize = rhs.m_masksize;
      m_mask = rhs.m_mask;

      return *this;
    }

    template<class T> te::rst::BandIteratorWithMask<T> te::rst::BandIteratorWithMask<T>::begin(te::rst::Band* b, te::rst::Raster* m)
    {
      return te::rst::BandIteratorWithMask<T>(b, m);
    }

    template<class T> te::rst::BandIteratorWithMask<T> te::rst::BandIteratorWithMask<T>::end(te::rst::Band* b, te::rst::Raster* m)
    {
      te::rst::BandIteratorWithMask<T> it;

      it.m_blky = b->getProperty()->m_nblocksy;

      return it;
    }

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_BANDITERATOR_H
