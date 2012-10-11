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
  \file terralib/rp/PositionIterator.h

  \brief It implements several ways to retrieve positions inside a band with spatial restriction,
         e.g. through a line, inside a bounding box or polygon, etc.
*/

#ifndef __TERRALIB_RP_INTERNAL_POSITIONITERATOR_H
#define __TERRALIB_RP_INTERNAL_POSITIONITERATOR_H

// Terralib
#include "../geometry.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/BlockUtils.h"
#include "../raster/Grid.h"

// STL
#include <iostream>

namespace te
{
  namespace rp
  {
// Forward declaration.
    class Band;

    /*!
      \class AbstractPositionIterator

      \brief This class is the base for implementing ways to navigate over the band with spatial restriction,
             e.g. through a line, inside a bounding box or polygon, etc.
    */
    template<class T> class AbstractPositionIterator
    {
      public:

        /*! \brief Constructor. */
        AbstractPositionIterator();

        /*!
          \brief Constructor.

          \param b The band to iterate.
        */
        AbstractPositionIterator(const te::rst::Band* b);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        AbstractPositionIterator(const AbstractPositionIterator& rhs);

        /*! \brief Destructor. */
        ~AbstractPositionIterator();

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
        AbstractPositionIterator& operator=(const AbstractPositionIterator& rhs);

        /*!
          \brief Difference operator.

          \param rhs The right-hand side to compare.

          \return Returns true if the iterators are at different positions, or false otherwise.
        */
        virtual bool operator!=(const AbstractPositionIterator& rhs) const;

        /*!
          \brief Sets the iterator position to the end of the current band.

          \param b The band to retrieve the end information.
        */
        virtual void setEnd() = 0;

      protected:

        int m_blkx;                              //!< The position in X of the current block.
        int m_blky;                              //!< The position in Y of the current block.
        const te::rst::Band* m_band;             //!< The band from where to get the values.

    };

    /*!
      \class PolygonIterator

      \brief This class implements the strategy to iterate with spatial restriction,
             the iteration occurs inside a geometry (i.e. a polygon).
    */
    template<class T> class PolygonIterator: public AbstractPositionIterator<T>
    {
      public:

        PolygonIterator();

        /*!
          \brief Constructor.

          \param b The band to iterate.
          \param p The polygon from where the iteration will navigate.
        */
        PolygonIterator(const te::rst::Band* b, const te::gm::Polygon* p);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        PolygonIterator(const PolygonIterator& rhs);

        ~PolygonIterator();

        void setNextLine();

        const T operator*() const;

        unsigned int getRow() const;

        unsigned int getCol() const;

        void operator++();

        void operator--();

        PolygonIterator& operator=(const PolygonIterator& rhs);

        void setEnd();

        /*! \brief Returns an iterator referring to the first value of the band.*/
        static PolygonIterator begin(const te::rst::Band* b, const te::gm::Polygon* p);

        /*! \brief Returns an iterator referring to after the end of the iterator. */
        static PolygonIterator end(const te::rst::Band* b, const te::gm::Polygon* p);

      protected:

        const te::gm::Polygon* m_polygon;        //!< The spatial restriction to be applied in the iterator.
        te::gm::MultiLineString* m_intersection; //!< The line (or lines) of the intersection between the geometry and the current line.
        te::gm::Line* m_currline;                //!< The current line in the iterator.
        int m_column;                            //!< The current column of the iterator.
        int m_row;                               //!< The current row of the iterator.
        int m_startingcolumn;                    //!< The starting column (in current line) to initialize the iteration.
        int m_endingcolumn;                      //!< The column (in current line) to finalize the iteration.
        int m_startingrow;                       //!< The starting row of the iteration.
        int m_endingrow;                         //!< The ending row of the iteration.
        unsigned m_maxcolumns;                   //!< The number of columns in band.
        unsigned m_maxrows;                      //!< The number of rows in band.
        int m_nline;                             //!< The actual line of the iterator.
        int m_nintersections;                    //!< The number number of intersected lines in current line of the iterator.

    };

    template<class T> te::rp::AbstractPositionIterator<T>::AbstractPositionIterator()
      : m_blkx(-1),
        m_blky(-1),
        m_band(0)
    {
    }

    template<class T> te::rp::AbstractPositionIterator<T>::AbstractPositionIterator(const te::rst::Band* b)
      : m_blkx(0),
        m_blky(0),
        m_band(b)
    {
    }

    template<class T> te::rp::AbstractPositionIterator<T>::AbstractPositionIterator(const AbstractPositionIterator& rhs)
      : m_blkx(rhs.m_blkx),
        m_blky(rhs.m_blky),
        m_band(rhs.m_band)
    {
    }

    template<class T> te::rp::AbstractPositionIterator<T>::~AbstractPositionIterator()
    {
    }

    template<class T> te::rp::AbstractPositionIterator<T>& te::rp::AbstractPositionIterator<T>::operator=(const AbstractPositionIterator& rhs)
    {
      if (this != &rhs)
      {
        m_blkx = rhs.m_blkx;
        m_blky = rhs.m_blky;
        m_band = rhs.m_band;
      }

      return *this;
    }

    template<class T> bool te::rp::AbstractPositionIterator<T>::operator!=(const AbstractPositionIterator& rhs) const
    {
      return (this->m_blky != rhs.m_blky);
    }

// implementation of Bounded Iteration Strategy
    template<class T> te::rp::PolygonIterator<T>::PolygonIterator()
      : AbstractPositionIterator<T>(),
        m_polygon(0),
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

    template<class T> te::rp::PolygonIterator<T>::PolygonIterator(const te::rst::Band* b, const te::gm::Polygon* p)
      : AbstractPositionIterator<T>(b),
        m_polygon(p),
        m_maxcolumns(b->getRaster()->getNumberOfColumns()),
        m_maxrows(b->getRaster()->getNumberOfRows()),
        m_nline(0),
        m_nintersections(0)
    {
      m_intersection = new te::gm::MultiLineString(0, te::gm::MultiLineStringType, m_polygon->getSRID());

      te::gm::Coord2D ll = m_polygon->getMBR()->getLowerLeft();
      te::gm::Coord2D ur = m_polygon->getMBR()->getUpperRight();

      m_startingrow = (int) b->getRaster()->getGrid()->geoToGrid(ll.x, ur.y).y;
      m_endingrow = (int) b->getRaster()->getGrid()->geoToGrid(ll.x, ll.y).y;

      m_currline = new te::gm::Line(te::gm::Point(ll.x, ll.y, m_polygon->getSRID()),
                                    te::gm::Point(ur.x, ll.y, m_polygon->getSRID()),
                                    te::gm::LineStringType, m_polygon->getSRID());

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

    template<class T> te::rp::PolygonIterator<T>::PolygonIterator(const PolygonIterator<T>& rhs)
      : AbstractPositionIterator<T>(rhs),
        m_polygon(rhs.m_polygon),
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

    template<class T> te::rp::PolygonIterator<T>::~PolygonIterator()
    {
      m_intersection->clear();
    }

    template<class T> void te::rp::PolygonIterator<T>::setNextLine()
    {
      double nexty = this->m_band->getRaster()->getGrid()->gridToGeo(0, m_row).y;

      if (m_nline == -1 || m_nline >= m_nintersections)
      {
        m_nline = 0;

        m_currline->setX(0, m_polygon->getMBR()->getLowerLeft().x);
        m_currline->setX(1, m_polygon->getMBR()->getUpperRight().x);
        m_currline->setY(0, nexty);
        m_currline->setY(1, nexty);

        m_intersection->clear();
        te::gm::Geometry* inter = m_polygon->intersection(m_currline);
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

      m_startingcolumn = (int) this->m_band->getRaster()->getGrid()->geoToGrid(lineinter->getStartPoint()->getX(), lineinter->getStartPoint()->getY()).x;

      m_endingcolumn = (int) this->m_band->getRaster()->getGrid()->geoToGrid(lineinter->getEndPoint()->getX(), lineinter->getEndPoint()->getY()).x;

      int tmp;

      if (m_startingcolumn > m_endingcolumn)
      {
        tmp = m_startingcolumn;

        m_startingcolumn = m_endingcolumn;

        m_endingcolumn = tmp;
      }

      m_endingcolumn--;
    }

    template<class T> const T te::rp::PolygonIterator<T>::operator*() const
    {
      double value;

      this->m_band->getValue(m_column, m_row, value);

      return (T) value;
    }

    template<class T> unsigned int te::rp::PolygonIterator<T>::getRow() const
    {
      return m_row;
    }

    template<class T> unsigned int te::rp::PolygonIterator<T>::getCol() const
    {
      return m_column;
    }

    template<class T> void te::rp::PolygonIterator<T>::operator++()
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

    template<class T> void te::rp::PolygonIterator<T>::operator--()
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

    template<class T> te::rp::PolygonIterator<T>& te::rp::PolygonIterator<T>::operator=(const te::rp::PolygonIterator<T>& rhs)
    {
      if (this != &rhs)
      {
        te::rp::AbstractPositionIterator<T>::operator=(rhs);

        m_polygon = rhs.m_polygon;
      }

      return *this;
    }

    template<class T> void te::rp::PolygonIterator<T>::setEnd()
    {
      this->m_blky = 1;
    }

    template<class T> te::rp::PolygonIterator<T> te::rp::PolygonIterator<T>::begin(const te::rst::Band* b, const te::gm::Polygon* p)
    {
      return te::rp::PolygonIterator<T>(b, p);
    }

    template<class T> te::rp::PolygonIterator<T> te::rp::PolygonIterator<T>::end(const te::rst::Band* b, const te::gm::Polygon* p)
    {
      te::rp::PolygonIterator<T> it(b, p);

      it.setEnd();

      return it;
    }


  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_POSITIONITERATOR_H
