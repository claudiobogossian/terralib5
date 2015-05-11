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
  \file terralib/raster/BandIteratorWindow.h

  \brief It implements and iterator to "navigate" over a single band,
         optimized by a window structure (e.g. 3x3, 5x5, etc). Examples
         include filters, moving windows, morphology, etc.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_BANDITERATORWINDOW_H
#define __TERRALIB_RASTER_INTERNAL_BANDITERATORWINDOW_H

// Terralib
#include "../common/STLUtils.h"
#include "Band.h"
#include "BandProperty.h"

namespace te
{
  namespace rst
  {
// Forward declaration.
    class Band;

    /*!
      \class BandIteratorWindow

      \brief It implements and iterator to "navigate" over a single band,
             optimized by a window structure (e.g. 3x3, 5x5, etc). Examples
             include filters, moving windows, morphology, etc.

      \ingroup rst

      \sa te::rst::Band
    */
    template<class T> class BandIteratorWindow
    {
      public:
        /*!
          \brief Constructor.

          \param b The band to iterate.
          \param w The width of the window to iterate.
          \param h The height of the window to iterate.
        */
        BandIteratorWindow(Band const * const b, std::size_t w, const std::size_t h);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        BandIteratorWindow(const BandIteratorWindow& rhs);

        /*! \brief Constructor. */
        BandIteratorWindow();

        /*! \brief Destructor. */
        ~BandIteratorWindow();

        /*! \brief Returns the current row in iterator. */
        unsigned int getRow() const;

        /*! \brief Returns the current column in iterator. */
        unsigned int getColumn() const;

        /*! \brief Returns the value in current position (column, row) from iterator. */
        T getValue() const;

        /*!
          \brief Returns the value in a relative position to the center pixel currently pointed by iterator.

          \param r The row relative to the center pixel.
          \param c The column relative to the center pixel.

          \warning The caller is responsible for providing correct values for the range [c x r].

          \note In a 5x5 window, the expected values for [c x r] in this method are in the interval [-2, +2].
        */
        T getValue(int c, int r) const;

        /*! \brief Advances to the next position. */
        void operator++();

        /*! \brief Returns to the previous position. */
        void operator--();

        BandIteratorWindow operator*();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        BandIteratorWindow& operator=(const BandIteratorWindow& rhs);

        /*!
          \brief Returns an iterator referring to the first value of the band.

          \param b The band to iterate.
          \param w The width of the window to iterate.
          \param h The height of the window to iterate.
        */
        static BandIteratorWindow begin(Band const * const b, std::size_t w, const std::size_t h);

        /*!
          \brief Returns an iterator referring to after the end of the iterator.

          \param b The band to iterate.
          \param w The width of the window to iterate.
          \param h The height of the window to iterate.
        */
        static BandIteratorWindow end(Band const * const b, std::size_t w, const std::size_t h);

        /*!
          \brief Differece operator.

          \param rhs The right-hand side to compare.

          \return Returns true if the iterators are at different positions, or false otherwise.
        */
        bool operator!=(const BandIteratorWindow& rhs) const;

        /*! \brief Replaces the current bufferized blocks. */
        void replaceBlocks();

      private:
        /*!
          \brief Returns the correct block where relative value is used.

          \param r The row relative to the center pixel.
          \param c The column relative to the center pixel.

          \return The correct block.
        */
        T* getBlock(int c, int r) const;

        /*!
          \brief Returns the correct index where the relative value is in the relative block.

          \param r The row relative to the center pixel.
          \param c The column relative to the center pixel.

          \return The correct index.
        */
        int getRelativeIndex(int c, int r) const;

      private:

        int m_w;                  //!< The width of the window to iterate.
        int m_h;                  //!< The height of the window to iterate.
        int m_blkw;               //!< The internal block width.
        int m_blkh;               //!< The internal block height.
        int m_npxlsblk;           //!< The maximum number of pixels inside the block.
        int m_nblocksx;           //!< The number of blocks in X direction.
        int m_nblocksy;           //!< The number of blocks in Y direction.
        int m_nvblocksx;          //!< The amount of blocks in direction X in vector of internal blocks.
        int m_nvblocksy;          //!< The amount of blocks in direction Y in vector of internal blocks.
        int m_cblock;             //!< The index of the central block;
        int m_i;                  //!< The actual position inside the block.
        int m_blkx;               //!< The position in X of the current block.
        int m_blky;               //!< The position in Y of the current block.
        int m_lastblksize;        //!< The number of pixels inside the last block.
        std::vector<T*> m_blocks; //!< A vector of internal blocks, to provide fast access to window elements.
        Band const * m_band;      //!< The band from where to get the values.
        te::rst::GetBufferValueFPtr m_getBuff;   //!< A pointer to a function that helps to extract a double or complex value from a specific buffer data type (char, int16, int32, float, ...).
        te::rst::GetBufferValueFPtr m_getBuffI;  //!< A pointer to a function that helps to extract the imaginary part value from a specific buffer data type (cint16, cint32, cfloat, cdouble).
        te::rst::SetBufferValueFPtr m_setBuff;   //!< A pointer to a function that helps to insert a double or complex value into a specific buffer data type (char, int16, int32, float, ...).
        te::rst::SetBufferValueFPtr m_setBuffI;  //!< A pointer to a function that helps to insert the imaginary part value into a specific buffer data type (cint16, cint32, cfloat, cdouble).
    };

    template<class T> te::rst::BandIteratorWindow<T>::BandIteratorWindow(te::rst::Band const * const b, std::size_t w, const std::size_t h)
      : m_w(w),
        m_h(h),
        m_blkw(b->getProperty()->m_blkw),
        m_blkh(b->getProperty()->m_blkh),
        m_npxlsblk(m_blkw * m_blkh),
        m_nblocksx(b->getProperty()->m_nblocksx),
        m_nblocksy(b->getProperty()->m_nblocksy),
        m_nvblocksx(0),
        m_nvblocksy(0),
        m_cblock(0),
        m_i(0),
        m_blkx(0),
        m_blky(0),
        m_band(b),
        m_getBuff(0),
        m_getBuffI(0),
        m_setBuff(0),
        m_setBuffI(0)
    {
      m_lastblksize = m_npxlsblk - ((m_npxlsblk * m_nblocksx * m_nblocksy) -
                                    (b->getRaster()->getNumberOfColumns() * b->getRaster()->getNumberOfRows()));

      if (m_blkw > m_w)
      {
        if (m_blkw == (int) b->getRaster()->getNumberOfColumns())
          m_nvblocksx = 1;
        else
          m_nvblocksx = 3;
      }
      else
        m_nvblocksx = 1 + (m_w + m_blkw - 2) / m_blkw;

      if (m_blkh > m_h)
      {
        if (m_blkh == (int) b->getRaster()->getNumberOfRows())
          m_nvblocksy = 1;
        else
          m_nvblocksy = 3;
      }
      else
      {
        m_nvblocksy = 1 + (m_h + m_blkh - 2) / m_blkh;
// apply a special case, create three blocks instead of two
        if (m_nvblocksx == 1 && m_nvblocksy == 2)
          m_nvblocksy++;
        else if (m_nvblocksx == 2 && m_nvblocksy == 1)
          m_nvblocksx++;
      }

      for (int i = 0; i < (m_nvblocksx * m_nvblocksy); i++)
        m_blocks.push_back(new T[m_band->getBlockSize()]);

      m_cblock = (int) m_blocks.size() / 2;

      te::rst::SetBlockFunctions(&m_getBuff, &m_getBuffI, &m_setBuff, &m_setBuffI, b->getProperty()->getType());

      replaceBlocks();
    }

    template<class T> te::rst::BandIteratorWindow<T>::BandIteratorWindow(const BandIteratorWindow<T>& rhs)
      : m_w(rhs.m_w),
        m_h(rhs.m_h),
        m_blkw(rhs.m_blkw),
        m_blkh(rhs.m_blkh),
        m_npxlsblk(rhs.m_npxlsblk),
        m_nblocksx(rhs.m_nblocksx),
        m_nblocksy(rhs.m_nblocksy),
        m_nvblocksx(rhs.m_nvblocksx),
        m_nvblocksy(rhs.m_nvblocksy),
        m_cblock(rhs.m_cblock),
        m_i(rhs.m_i),
        m_blkx(rhs.m_blkx),
        m_blky(rhs.m_blky),
        m_lastblksize(rhs.m_lastblksize),
        m_band(rhs.m_band),
        m_getBuff(rhs.m_getBuff),
        m_getBuffI(rhs.m_getBuffI),
        m_setBuff(rhs.m_setBuff),
        m_setBuffI(rhs.m_setBuffI)
    {
      if (m_band)
      {
        te::common::FreeContents(m_blocks);

        for (int i = 0; i < (m_nvblocksx * m_nvblocksy); i++)
          m_blocks.push_back(new T[m_band->getBlockSize()]);

        replaceBlocks();
      }
    }

    template<class T> te::rst::BandIteratorWindow<T>::BandIteratorWindow()
      : m_w(0),
        m_h(0),
        m_blkw(-1),
        m_blkh(-1),
        m_npxlsblk(-1),
        m_nblocksx(-1),
        m_nblocksy(-1),
        m_nvblocksx(0),
        m_nvblocksy(0),
        m_cblock(0),
        m_i(-1),
        m_blkx(-1),
        m_blky(-1),
        m_lastblksize(-1),
        m_blocks(0),
        m_band(0)
    {
    }

    template<class T> te::rst::BandIteratorWindow<T>::~BandIteratorWindow()
    {
      te::common::FreeContents(m_blocks);
    }

    template<class T> unsigned int te::rst::BandIteratorWindow<T>::getRow() const
    {
      return m_blky * m_blkh + (m_i - (m_i % m_blkw)) / m_blkw;
    }

    template<class T> unsigned int te::rst::BandIteratorWindow<T>::getColumn() const
    {
      return m_blkx * m_blkw + m_i % m_blkw;
    }

    template<class T> T te::rst::BandIteratorWindow<T>::getValue() const
    {
      //return ((T*)m_blocks[m_cblock])[m_i];
      double value;

      this->m_getBuff(this->m_i, this->m_blocks[m_cblock], &value);

      return (T) value;
    }

    template<class T> T te::rst::BandIteratorWindow<T>::getValue(int c, int r) const
    {
      //return ((T*)getBlock(c, r))[getRelativeIndex(c, r)];
      double value;

      this->m_getBuff(this->getRelativeIndex(c, r), this->getBlock(c, r), &value);

      return (T) value;
    }

    template<class T> void te::rst::BandIteratorWindow<T>::operator++()
    {
      m_i++;

      if(m_i < m_npxlsblk)
        return;

      m_i = 0;

      m_blkx++;

      if(m_blkx < m_nblocksx)
      {
        replaceBlocks();

        return;
      }

      m_blkx = 0;

      m_blky++;

      if(m_blky < m_nblocksy)
        replaceBlocks();

      if((m_blkx == m_nblocksx - 1) && (m_blky == m_nblocksy - 1))
        m_npxlsblk = m_lastblksize;
    }

    template<class T> void te::rst::BandIteratorWindow<T>::operator--()
    {
      m_i--;

      if(m_i >= 0)
        return;

      m_i = m_npxlsblk - 1;

      m_blkx--;

      if(m_blkx >= 0)
      {
        replaceBlocks();

        return;
      }

      m_blkx = m_nblocksx - 1;

      m_blky--;

      if(m_blky >= 0)
        replaceBlocks();
      else
        m_blky = m_nblocksy;
    }

    template<class T> te::rst::BandIteratorWindow<T> te::rst::BandIteratorWindow<T>::operator*()
    {
      return *this;
    }

    template<class T> te::rst::BandIteratorWindow<T>& te::rst::BandIteratorWindow<T>::operator=(const te::rst::BandIteratorWindow<T>& rhs)
    {
      if (this != &rhs)
      {
        m_blkw = rhs.m_blkw;
        m_blkh = rhs.m_blkh;
        m_npxlsblk = rhs.m_npxlsblk;
        m_nblocksx = rhs.m_nblocksx;
        m_nblocksy = rhs.m_nblocksy;
        m_nvblocksx = rhs.m_nvblocksx;
        m_nvblocksy = rhs.m_nvblocksy;
        m_i = rhs.m_i;
        m_blkx = rhs.m_blkx;
        m_blky = rhs.m_blky;
        m_lastblksize = rhs.m_lastblksize;
        m_band = rhs.m_band;

        te::common::FreeContents(m_blocks);
        for (int i = 0; i < (m_nvblocksx * m_nvblocksy); i++)
          m_blocks.push_back(new T[m_band->getBlockSize()]);

        replaceBlocks();
      }

      return *this;
    }

    template<class T> bool te::rst::BandIteratorWindow<T>::operator!=(const BandIteratorWindow& rhs) const
    {
      return (m_blky != rhs.m_blky);
    }

    template<class T> te::rst::BandIteratorWindow<T> te::rst::BandIteratorWindow<T>::begin(te::rst::Band const * const b, std::size_t w, const std::size_t h)
    {
      return te::rst::BandIteratorWindow<T>(b, w, h);
    }

    template<class T> te::rst::BandIteratorWindow<T> te::rst::BandIteratorWindow<T>::end(te::rst::Band const * const b, std::size_t w, const std::size_t h)
    {
      te::rst::BandIteratorWindow<T> it;

       it.m_blky = b->getProperty()->m_nblocksy;

      return it;
    }

    template<class T> void te::rst::BandIteratorWindow<T>::replaceBlocks()
    {
      int blk = 0;

      for (int x = (m_blkx - m_nvblocksx / 2); x <= (m_blkx + m_nvblocksx / 2); x++)
        for (int y = (m_blky - m_nvblocksy / 2); y <= (m_blky + m_nvblocksy / 2); y++)
        {
          if ((x >= 0) && (x < m_nblocksx))
            if ((y >= 0) && (y < m_nblocksy))
            {
              m_band->read(x, y, m_blocks[blk]);
              blk++;
            }

          // blk++;
        }
    }

    template<class T> T* te::rst::BandIteratorWindow<T>::getBlock(int c, int r) const
    {
      int bx = (getColumn() + c) / m_blkw;

      int by = (getRow() + r) / m_blkh;

      int blk = m_cblock + (bx - m_blkx) + (by - m_blky) * m_nvblocksx;

      return m_blocks[blk];
    }

    template<class T> int te::rst::BandIteratorWindow<T>::getRelativeIndex(int c, int r) const
    {
      int cc = (getColumn() + c) % m_blkw;

      int cr = (getRow() + r) % m_blkh;

      return (cc + cr * m_blkw);
    }

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_BANDITERATORWINDOW_H
