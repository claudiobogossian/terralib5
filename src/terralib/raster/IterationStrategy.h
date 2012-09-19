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
  \file terralib/raster/IterationStrategy.h

  \brief It implements several ways to navigate over the band with spatial restriction,
         e.g. through a line, inside a bounding box or polygon, etc.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_ITERATIONSTRATEGY_H
#define __TERRALIB_RASTER_INTERNAL_ITERATIONSTRATEGY_H

// Terralib
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "../geometry/Point.h"
#include "Band.h"
#include "BandProperty.h"
#include "BlockUtils.h"
#include "Grid.h"

#include "../geometry.h"
#include <iostream>

namespace te
{
  namespace rst
  {
// Forward declaration.
    class Band;

    /*!
      \class AbstractIterationStrategy

      \brief This class is the base for implementing ways to navigate over the band with spatial restriction,
             e.g. through a line, inside a bounding box or polygon, etc.
    */
    template<class T> class AbstractIterationStrategy
    {
      public:

        /*! \brief Constructor. */
        AbstractIterationStrategy();

        /*!
          \brief Constructor.

          \param b The band to iterate.
        */
        AbstractIterationStrategy(const Band* b);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        AbstractIterationStrategy(const AbstractIterationStrategy& rhs);

        /*! \brief Destructor. */
        ~AbstractIterationStrategy();

        /*! \brief Returns the value in current position (column, row) from iterator. */
        virtual const T operator*() const = 0;

        /*! \brief Returns the current row in iterator. */
        virtual unsigned int getRow() const = 0;

        /*! \brief Returns the current column in iterator. */
        virtual unsigned int getCol() const = 0;

        /*! \brief Advances to the next position. */
        virtual void operator++() = 0;

        /*! \brief Returns to the previous position. */
        virtual void operator--() = 0;

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        AbstractIterationStrategy& operator=(const AbstractIterationStrategy& rhs);

        /*!
          \brief Difference operator.

          \param rhs The right-hand side to compare.

          \return Returns true if the iterators are at different positions, or false otherwise.
        */
        virtual bool operator!=(const AbstractIterationStrategy& rhs) const;

        /*!
          \brief Sets the iterator position to the end of the current band.

          \param b The band to retrieve the end information.
        */
        virtual void setEnd() = 0;

      protected:

        int m_blkx;                              //!< The position in X of the current block.
        int m_blky;                              //!< The position in Y of the current block.
        const Band* m_band;                      //!< The band from where to get the values.

    };

    /*!
      \class BlockedIterationStrategy

      \brief This class implements the basic strategy to iterate over an entire band.
    */
    template<class T> class BlockedIterationStrategy: public AbstractIterationStrategy<T>
    {
      public:

        BlockedIterationStrategy();

        /*!
          \brief Constructor.

          \param b The band to iterate.
        */
        BlockedIterationStrategy(const Band* b);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        BlockedIterationStrategy(const BlockedIterationStrategy& rhs);

        ~BlockedIterationStrategy();

        const T operator*() const;

        unsigned int getRow() const;

        unsigned int getCol() const;

        void operator++();

        void operator--();

        BlockedIterationStrategy& operator=(const BlockedIterationStrategy& rhs);

        /*! \brief Replaces the current bufferized block values. */
        void replaceBlock();

        void setEnd();

      protected:

        int m_blkw;                              //!< The internal block width.
        int m_blkh;                              //!< The internal block height.
        int m_npxlsblk;                          //!< The maximum number of pixels inside the block.
        int m_nblocksx;                          //!< The number of blocks in X direction.
        int m_nblocksy;                          //!< The number of blocks in Y direction.
        int m_i;                                 //!< The actual position inside the block.
        int m_lastblksize;                       //!< The number of pixels inside the last block.
        int m_blksize;                           //!< The block size of the band.
        T* m_blk;                                //!< Internal block.
        te::rst::GetBufferValueFPtr m_getBuff;   //!< A pointer to a function that helps to extract a double or complex value from a specific buffer data type (char, int16, int32, float, ...).
        te::rst::GetBufferValueFPtr m_getBuffI;  //!< A pointer to a function that helps to extract the imaginary part value from a specific buffer data type (cint16, cint32, cfloat, cdouble).
        te::rst::SetBufferValueFPtr m_setBuff;   //!< A pointer to a function that helps to insert a double or complex value into a specific buffer data type (char, int16, int32, float, ...).
        te::rst::SetBufferValueFPtr m_setBuffI;  //!< A pointer to a function that helps to insert the imaginary part value into a specific buffer data type (cint16, cint32, cfloat, cdouble).

    };

    /*!
      \class BoundedIterationStrategy

      \brief This class implements the strategy to iterate with spatial restriction,
             the iteration occurs inside a geometry (i.e. a polygon).
    */
    template<class T> class BoundedIterationStrategy: public AbstractIterationStrategy<T>
    {
      public:

        BoundedIterationStrategy();

        /*!
          \brief Constructor.

          \param b The band to iterate.
          \param g The geometry (i.e. a polygon) from where the iteration will navigate.
        */
        BoundedIterationStrategy(const Band* b, const te::gm::Geometry* g);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        BoundedIterationStrategy(const BoundedIterationStrategy& rhs);

        ~BoundedIterationStrategy();

        void setNextLine();

        const T operator*() const;

        unsigned int getRow() const;

        unsigned int getCol() const;

        void operator++();

        void operator--();

        BoundedIterationStrategy& operator=(const BoundedIterationStrategy& rhs);

        void setEnd();

      protected:

        const te::gm::Geometry* m_geometry;      //!< The spatial restriction to be applied in the iterator.
        te::gm::MultiLineString* m_intersection; //!< The line (or lines) of the intersection between the geometry and the current line.
        te::gm::Line* m_currline;                //!< The current line in the iterator.
        int m_column;                            //!< The current column of the iterator.
        int m_row;                               //!< The current row of the iterator.
        int m_startingcolumn;
        int m_endingcolumn;
        int m_startingrow;
        int m_endingrow;
        unsigned m_maxcolumns;
        unsigned m_maxrows;
        int m_nline;
        unsigned m_nintersections;

    };

    template<class T> te::rst::AbstractIterationStrategy<T>::AbstractIterationStrategy()
      : m_blkx(-1),
        m_blky(-1),
        m_band(0)
    {
    }

    template<class T> te::rst::AbstractIterationStrategy<T>::AbstractIterationStrategy(const Band* b)
      : m_blkx(0),
        m_blky(0),
        m_band(b)
    {
    }

    template<class T> te::rst::AbstractIterationStrategy<T>::AbstractIterationStrategy(const AbstractIterationStrategy& rhs)
      : m_blkx(rhs.m_blkx),
        m_blky(rhs.m_blky),
        m_band(rhs.m_band)
    {
    }

    template<class T> te::rst::AbstractIterationStrategy<T>::~AbstractIterationStrategy()
    {
    }

    template<class T> te::rst::AbstractIterationStrategy<T>& te::rst::AbstractIterationStrategy<T>::operator=(const AbstractIterationStrategy& rhs)
    {
      if (this != &rhs)
      {
        m_blkx = rhs.m_blkx;
        m_blky = rhs.m_blky;
        m_band = rhs.m_band;
      }

      return *this;
    }

    template<class T> bool te::rst::AbstractIterationStrategy<T>::operator!=(const AbstractIterationStrategy& rhs) const
    {
      return (this->m_blky != rhs.m_blky);
    }

// implementation of Blocked Iteration Strategy
    template<class T> te::rst::BlockedIterationStrategy<T>::BlockedIterationStrategy()
      : AbstractIterationStrategy<T>(),
        m_blkw(-1),
        m_blkh(-1),
        m_npxlsblk(-1),
        m_nblocksx(-1),
        m_nblocksy(-1),
        m_i(-1),
        m_lastblksize(-1),
        m_blksize(-1),
        m_blk(0),
        m_getBuff(0),
        m_getBuffI(0),
        m_setBuff(0),
        m_setBuffI(0)
    {
    }

    template<class T> te::rst::BlockedIterationStrategy<T>::BlockedIterationStrategy(const Band* b)
      : AbstractIterationStrategy<T>(b),
        m_blkw(b->getProperty()->m_blkw),
        m_blkh(b->getProperty()->m_blkh),
        m_npxlsblk(m_blkw * m_blkh),
        m_nblocksx(b->getProperty()->m_nblocksx),
        m_nblocksy(b->getProperty()->m_nblocksy),
        m_i(0),
        m_blksize(b->getBlockSize()),
        m_getBuff(0),
        m_getBuffI(0),
        m_setBuff(0),
        m_setBuffI(0)
    {
      m_lastblksize = m_npxlsblk - ((m_npxlsblk * m_nblocksx * m_nblocksy) -
                                    (this->m_band->getRaster()->getNumberOfColumns() * this->m_band->getRaster()->getNumberOfRows()));
      m_blk = new T[m_blksize];

      te::rst::SetBlockFunctions(&m_getBuff, &m_getBuffI, &m_setBuff, &m_setBuffI, this->m_band->getProperty()->getType());

      replaceBlock();
    }

    template<class T> te::rst::BlockedIterationStrategy<T>::BlockedIterationStrategy(const BlockedIterationStrategy<T>& rhs)
      : AbstractIterationStrategy<T>(rhs),
        m_blkw(rhs.m_blkw),
        m_blkh(rhs.m_blkh),
        m_npxlsblk(rhs.m_npxlsblk),
        m_nblocksx(rhs.m_nblocksx),
        m_nblocksy(rhs.m_nblocksy),
        m_i(rhs.m_i),
        m_lastblksize(rhs.m_lastblksize),
        m_blksize(rhs.m_blksize),
        m_getBuff(rhs.m_getBuff),
        m_getBuffI(rhs.m_getBuffI),
        m_setBuff(rhs.m_setBuff),
        m_setBuffI(rhs.m_setBuffI)
    {
      if (m_blksize > 0)
        m_blk = new T[m_blksize];

      replaceBlock();
    }

    template<class T> te::rst::BlockedIterationStrategy<T>::~BlockedIterationStrategy()
    {
      if (m_blksize > 0)
        delete [] m_blk;
    }

    template<class T> const T te::rst::BlockedIterationStrategy<T>::operator*() const
    {
      double value;

      this->m_getBuff(this->m_i, this->m_blk, &value);

      return (T) value;
    }

    template<class T> unsigned int te::rst::BlockedIterationStrategy<T>::getRow() const
    {
      return this->m_blky * m_blkh + (m_i - (m_i % m_blkw)) / m_blkw;
    }

    template<class T> unsigned int te::rst::BlockedIterationStrategy<T>::getCol() const
    {
      return this->m_blkx * m_blkw + m_i % m_blkw;
    }

    template<class T> void te::rst::BlockedIterationStrategy<T>::operator++()
    {
      m_i++;

      if(m_i < m_npxlsblk)
        return;

      m_i = 0;

      this->m_blkx++;

      if(this->m_blkx < m_nblocksx)
      {
        replaceBlock();

        return;
      }

      this->m_blkx = 0;

      this->m_blky++;

      if(this->m_blky < m_nblocksy)
        replaceBlock();

      if((this->m_blkx == m_nblocksx - 1) && (this->m_blky == m_nblocksy - 1))
        m_npxlsblk = m_lastblksize;
    }

    template<class T> void te::rst::BlockedIterationStrategy<T>::operator--()
    {
      m_i--;

      if(m_i >= 0)
        return;

      m_i = m_npxlsblk - 1;

      this->m_blkx--;

      if(this->m_blkx >= 0)
      {
        replaceBlock();

        return;
      }

      this->m_blkx = m_nblocksx - 1;

      this->m_blky--;

      if(this->m_blky >= 0)
        replaceBlock();
      else
        this->m_blky = m_nblocksy;
    }

    template<class T> te::rst::BlockedIterationStrategy<T>& te::rst::BlockedIterationStrategy<T>::operator=(const te::rst::BlockedIterationStrategy<T>& rhs)
    {
      if (this != &rhs)
      {
        te::rst::AbstractIterationStrategy<T>::operator=(rhs);

        m_blkw = rhs.m_blkw;
        m_blkh = rhs.m_blkh;
        m_npxlsblk = rhs.m_npxlsblk;
        m_nblocksx = rhs.m_nblocksx;
        m_nblocksy = rhs.m_nblocksy;
        m_i = rhs.m_i;
        m_lastblksize = rhs.m_lastblksize;

        delete [] m_blk;
        m_blk = new T[rhs.m_blksize];
      }

      return *this;
    }

    template<class T> void te::rst::BlockedIterationStrategy<T>::replaceBlock()
    {
      this->m_band->read(this->m_blkx, this->m_blky, this->m_blk);
    }

    template<class T> void te::rst::BlockedIterationStrategy<T>::setEnd()
    {
      this->m_blky = this->m_band->getProperty()->m_nblocksy;
    }

// implementation of Bounded Iteration Strategy
    template<class T> te::rst::BoundedIterationStrategy<T>::BoundedIterationStrategy()
      : AbstractIterationStrategy<T>(),
        m_geometry(0),
        m_intersection(0),
        m_currline(0),
        m_column(-1),
        m_row(-1),
        m_startingcolumn(0),
        m_endingcolumn(0),
        m_startingrow(0),
        m_endingrow(0),
        m_maxcolumns(0),
        m_maxrows(0),
        m_nline(0),
        m_nintersections(0)
    {
    }

    template<class T> te::rst::BoundedIterationStrategy<T>::BoundedIterationStrategy(const Band* b, const te::gm::Geometry* g)
      : AbstractIterationStrategy<T>(b),
        m_geometry(g),
        m_maxcolumns(b->getRaster()->getNumberOfColumns()),
        m_maxrows(b->getRaster()->getNumberOfRows()),
        m_nline(0),
        m_nintersections(0)
    {
      m_intersection = new te::gm::MultiLineString(0, te::gm::MultiLineStringType, m_geometry->getSRID());

      te::gm::Coord2D ll = g->getMBR()->getLowerLeft();
      te::gm::Coord2D ur = g->getMBR()->getUpperRight();

      m_startingrow = b->getRaster()->getGrid()->geoToGrid(ll.x, ur.y).y;
      m_endingrow = b->getRaster()->getGrid()->geoToGrid(ll.x, ll.y).y;

      m_currline = new te::gm::Line(te::gm::Point(ll.x, ll.y, m_geometry->getSRID()),
                                    te::gm::Point(ur.x, ll.y, m_geometry->getSRID()),
                                    te::gm::LineStringType, m_geometry->getSRID());

      int tmp;
      if (m_startingrow > m_endingrow)
      {
        tmp = m_startingrow;
        m_startingrow = m_endingrow;
        m_endingrow = tmp;
      }

      m_endingrow--;

      m_row = m_startingrow;

      setNextLine();

      m_column = m_startingcolumn;
    }

    template<class T> te::rst::BoundedIterationStrategy<T>::BoundedIterationStrategy(const BoundedIterationStrategy<T>& rhs)
      : AbstractIterationStrategy<T>(rhs),
        m_geometry(rhs.m_geometry),
        m_intersection(rhs.m_intersection),
        m_currline(rhs.m_currline),
        m_column(rhs.m_column),
        m_row(rhs.m_row),
        m_startingcolumn(rhs.m_startingcolumn),
        m_endingcolumn(rhs.m_endingcolumn),
        m_startingrow(rhs.m_startingrow),
        m_endingrow(rhs.m_endingrow),
        m_maxcolumns(rhs.m_maxcolumns),
        m_maxrows(rhs.m_maxrows),
        m_nline(rhs.m_nline),
        m_nintersections(rhs.m_nintersections)
    {
    }

    template<class T> te::rst::BoundedIterationStrategy<T>::~BoundedIterationStrategy()
    {
      delete m_intersection;
      delete m_currline;
    }

    template<class T> void te::rst::BoundedIterationStrategy<T>::setNextLine()
    {
      double nexty = this->m_band->getRaster()->getGrid()->gridToGeo(0, m_row).y;

      if (m_nline == -1 || m_nline >= m_nintersections)
      {
        m_nline = 0;

        m_currline->setX(0, m_geometry->getMBR()->getLowerLeft().x);
        m_currline->setX(1, m_geometry->getMBR()->getUpperRight().x);
        m_currline->setY(0, nexty);
        m_currline->setY(1, nexty);

        m_intersection->clear();
        te::gm::Geometry* inter = m_geometry->intersection(m_currline);
        if (inter->isEmpty())
        {
          delete inter;

          m_row++;

          setNextLine();

          return;
        }

        m_intersection->add(inter);
      }

      te::gm::LineString* lineinter;

      if (m_intersection->getGeometryN(0)->getGeomTypeId() == te::gm::LineStringType)
      {
        lineinter = static_cast<te::gm::LineString*> (m_intersection->getGeometryN(0));

        m_nintersections = 1;
      }
      else
      {
        te::gm::MultiLineString* mls = static_cast<te::gm::MultiLineString*> (m_intersection->getGeometryN(0));

        lineinter = static_cast<te::gm::LineString*> (mls->getGeometryN(m_nline));

        m_nintersections = mls->getNumGeometries();
      }

      m_startingcolumn = this->m_band->getRaster()->getGrid()->geoToGrid(lineinter->getStartPoint()->getX(), lineinter->getStartPoint()->getY()).x;

      m_endingcolumn = this->m_band->getRaster()->getGrid()->geoToGrid(lineinter->getEndPoint()->getX(), lineinter->getEndPoint()->getY()).x;

      int tmp;

      if (m_startingcolumn > m_endingcolumn)
      {
        tmp = m_startingcolumn;

        m_startingcolumn = m_endingcolumn;

        m_endingcolumn = tmp;
      }

      m_endingcolumn--;
    }

    template<class T> const T te::rst::BoundedIterationStrategy<T>::operator*() const
    {
      double value;

      this->m_band->getValue(m_column, m_row, value);

      return (T) value;
    }

    template<class T> unsigned int te::rst::BoundedIterationStrategy<T>::getRow() const
    {
      return m_row;
    }

    template<class T> unsigned int te::rst::BoundedIterationStrategy<T>::getCol() const
    {
      return m_column;
    }

    template<class T> void te::rst::BoundedIterationStrategy<T>::operator++()
    {
      m_column++;

      if (m_column > m_endingcolumn)
      {
        m_nline++;

        if (m_nline >= m_nintersections)
          m_row++;

        if (m_row > m_endingrow)
        {
          setEnd();

          return;
        }

        setNextLine();

        m_column = m_startingcolumn;
      }
    }

    template<class T> void te::rst::BoundedIterationStrategy<T>::operator--()
    {
      m_column--;

      if (m_column < m_startingcolumn)
      {
        m_nline--;

        if (m_nline < 0)
          m_row--;

        if (m_row < m_startingrow)
        {
          setEnd();

          return;
        }

        setNextLine();

        m_column = m_endingcolumn;
      }
    }

    template<class T> te::rst::BoundedIterationStrategy<T>& te::rst::BoundedIterationStrategy<T>::operator=(const te::rst::BoundedIterationStrategy<T>& rhs)
    {
      if (this != &rhs)
      {
        te::rst::AbstractIterationStrategy<T>::operator=(rhs);

        m_geometry = rhs.m_geometry;
      }

      return *this;
    }

    template<class T> void te::rst::BoundedIterationStrategy<T>::setEnd()
    {
      this->m_blky = 1;
    }

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_ITERATIONSTRATEGY_H
