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

#ifndef __TERRALIB_RASTER_INTERNAL_POSITIONITERATOR_H
#define __TERRALIB_RASTER_INTERNAL_POSITIONITERATOR_H

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
  namespace rst
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

        const te::rst::Band* m_band;             //!< The band from where to get the values.

    };

    /*!
      \class PolygonIterator

      \brief This class implements the strategy to iterate with spatial restriction,
             the iteration occurs inside a polygon.
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

        /*!
          \brief Decomponse one geometry collection in a vector of basic components (line, point).

          \param g The input geometry collection.

          \return A vector of geometries.
        */
        std::vector<te::gm::LineString*> decompose(te::gm::Geometry* g);

        void setNextLine(bool updatecurrline = true);

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

        bool operator!=(const PolygonIterator<T>& rhs) const;

      protected:

        const te::gm::Polygon* m_polygon;                  //!< The spatial restriction to be applied in the iterator.
        std::vector<te::gm::LineString*> m_intersections;  //!< The points or lines of the intersection between the geometry and the current line.
        te::gm::Line* m_currline;                          //!< The current line in the iterator.
        int m_column;                                      //!< The current column of the iterator.
        int m_row;                                         //!< The current row of the iterator.
        int m_startingcolumn;                              //!< The starting column (in current line) to initialize the iteration.
        int m_endingcolumn;                                //!< The column (in current line) to finalize the iteration.
        int m_startingrow;                                 //!< The starting row of the iteration.
        int m_endingrow;                                   //!< The ending row of the iteration.
        int m_maxcolumns;                                  //!< The number of columns in band.
        int m_maxrows;                                     //!< The number of rows in band.
        int m_actualintersection;                          //!< The actual line of the iterator.
        int m_nintersections;                              //!< The number number of intersected lines in current line of the iterator.

    };

    /*!
      \class LineIterator

      \brief This class implements the strategy to iterate with spatial restriction,
             the iteration occurs inside a line.
    */
    template<class T> class LineIterator: public AbstractPositionIterator<T>
    {
      public:

        LineIterator();

        /*!
          \brief Constructor.

          \param b The band to iterate.
          \param l The line from where the iteration will navigate.
        */
        LineIterator(const te::rst::Band* b, const te::gm::Line* l);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        LineIterator(const LineIterator& rhs);

        ~LineIterator();

        const T operator*() const;

        unsigned int getRow() const;

        unsigned int getCol() const;

        void operator++();

        void operator--();

        LineIterator& operator=(const LineIterator& rhs);

        void setEnd();

        /*! \brief Returns an iterator referring to the first value of the band.*/
        static LineIterator begin(const te::rst::Band* b, const te::gm::Line* l);

        /*! \brief Returns an iterator referring to after the end of the iterator. */
        static LineIterator end(const te::rst::Band* b, const te::gm::Line* l);

        bool operator!=(const LineIterator<T>& rhs) const;

      protected:

        const te::gm::Line* m_line;                   //!< The spatial restriction to be applied in the iterator.
        int m_currentpixelindex;                      //!< The index of the current pixel location.
        std::vector<te::gm::Point*> m_pixelsinline;   //!< A vector of pixel locations that intersects the line.

    };

    /*!
      \class PointSetIterator

      \brief This class implements the strategy to iterate with spatial restriction,
             the iteration occurs inside a vector of points.
    */
    template<class T> class PointSetIterator: public AbstractPositionIterator<T>
    {
      public:

        PointSetIterator();

        /*!
          \brief Constructor.

          \param b The band to iterate.
          \param p The vector of points where the iteration will navigate.
        */
        PointSetIterator(const te::rst::Band* b, const std::vector<te::gm::Point*> p);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        PointSetIterator(const PointSetIterator& rhs);

        ~PointSetIterator();

        const T operator*() const;

        unsigned int getRow() const;

        unsigned int getCol() const;

        void operator++();

        void operator--();

        PointSetIterator& operator=(const PointSetIterator& rhs);

        void setEnd();

        /*! \brief Returns an iterator referring to the first value of the band.*/
        static PointSetIterator begin(const te::rst::Band* b, const std::vector<te::gm::Point*> p);

        /*! \brief Returns an iterator referring to after the end of the iterator. */
        static PointSetIterator end(const te::rst::Band* b, const std::vector<te::gm::Point*> p);

        bool operator!=(const PointSetIterator<T>& rhs) const;

      protected:

        std::vector<te::gm::Point*> m_pixelsinpointset;         //!< The spatial restriction to be applied in the iterator.
        int m_currentpixelindex;                      //!< The index of the current pixel location.

    };
// implementation of abstract position iterator
    template<class T> te::rst::AbstractPositionIterator<T>::AbstractPositionIterator()
      : m_band(0)
    {
    }

    template<class T> te::rst::AbstractPositionIterator<T>::AbstractPositionIterator(const te::rst::Band* b)
      : m_band(b)
    {
    }

    template<class T> te::rst::AbstractPositionIterator<T>::AbstractPositionIterator(const AbstractPositionIterator& rhs)
      : m_band(rhs.m_band)
    {
    }

    template<class T> te::rst::AbstractPositionIterator<T>::~AbstractPositionIterator()
    {
    }

    template<class T> te::rst::AbstractPositionIterator<T>& te::rst::AbstractPositionIterator<T>::operator=(const AbstractPositionIterator& rhs)
    {
      if (this != &rhs)
        m_band = rhs.m_band;

      return *this;
    }

    template<class T> bool te::rst::AbstractPositionIterator<T>::operator!=(const te::rst::AbstractPositionIterator<T>& rhs) const
    {
      return (m_band != rhs.m_band);
    }

// implementation of iteration strategy bounded by a polygon
    template<class T> te::rst::PolygonIterator<T>::PolygonIterator()
      : AbstractPositionIterator<T>(),
        m_polygon(0),
        m_intersections(0),
        m_currline(0),
        m_column(-1),
        m_row(-1),
        m_startingcolumn(0),
        m_endingcolumn(0),
        m_startingrow(0),
        m_endingrow(0),
        m_maxcolumns(0),
        m_maxrows(0),
        m_actualintersection(-1),
        m_nintersections(0)
    {
    }

    template<class T> te::rst::PolygonIterator<T>::PolygonIterator(const te::rst::Band* b, const te::gm::Polygon* p)
      : AbstractPositionIterator<T>(b),
        m_polygon(p),
        m_intersections(0),
        m_column(0),
        m_maxcolumns(b->getRaster()->getNumberOfColumns()),
        m_maxrows(b->getRaster()->getNumberOfRows()),
        m_actualintersection(-1),
        m_nintersections(0)
    {
      te::gm::Coord2D ll = m_polygon->getMBR()->getLowerLeft();
      te::gm::Coord2D ur = m_polygon->getMBR()->getUpperRight();

// defining starting/ending rows
      m_startingrow = (int) b->getRaster()->getGrid()->geoToGrid(ll.x, ur.y).y;
      m_endingrow = (int) b->getRaster()->getGrid()->geoToGrid(ll.x, ll.y).y;

      int tmp;
      if (m_startingrow > m_endingrow)
      {
        tmp = m_startingrow;
        m_startingrow = m_endingrow;
        m_endingrow = tmp;
      }

// avoiding bad access
      m_startingrow = m_startingrow < 0? 0: m_startingrow;
      m_startingrow = m_startingrow >= m_maxrows? m_maxrows - 1: m_startingrow;

      m_endingrow = m_endingrow < 0? 0: m_endingrow;
      m_endingrow = m_endingrow >= m_maxrows? m_maxrows - 1: m_endingrow;

      m_row = m_startingrow;

      m_currline = new te::gm::Line(te::gm::Point(ll.x, ur.y, m_polygon->getSRID()),
                                    te::gm::Point(ur.x, ur.y, m_polygon->getSRID()),
                                    te::gm::LineStringType, m_polygon->getSRID());

// in case of problems, we initialize the first line here
      m_startingcolumn = 0;
      m_endingcolumn = m_maxcolumns - 1;

// defining starting/ending columns
      setNextLine(false);

      m_column = m_startingcolumn;
    }

    template<class T> te::rst::PolygonIterator<T>::PolygonIterator(const PolygonIterator<T>& rhs)
      : AbstractPositionIterator<T>(rhs),
        m_polygon(rhs.m_polygon),
        m_intersections(rhs.m_intersections),
        m_currline(rhs.m_currline),
        m_column(rhs.m_column),
        m_row(rhs.m_row),
        m_startingcolumn(rhs.m_startingcolumn),
        m_endingcolumn(rhs.m_endingcolumn),
        m_startingrow(rhs.m_startingrow),
        m_endingrow(rhs.m_endingrow),
        m_maxcolumns(rhs.m_maxcolumns),
        m_maxrows(rhs.m_maxrows),
        m_actualintersection(rhs.m_actualintersection),
        m_nintersections(rhs.m_nintersections)
    {
    }

    template<class T> te::rst::PolygonIterator<T>::~PolygonIterator()
    {
      m_intersections.clear();
    }

    template<class T> std::vector<te::gm::LineString*> te::rst::PolygonIterator<T>::decompose(te::gm::Geometry* g)
    {
      std::vector<te::gm::LineString*> lines;

      te::gm::Geometry* ing = g;
      te::gm::GeometryCollection* gc = static_cast<te::gm::GeometryCollection*> (g);
      if (gc->getNumGeometries() == 1)
        ing = gc->getGeometryN(0);

// check if the geometry is a multi line string
      if (ing->getGeomTypeId() == te::gm::MultiLineStringType)
      {
        te::gm::MultiLineString* mls = static_cast<te::gm::MultiLineString*> (ing);

        for (unsigned int i = 0; i < mls->getNumGeometries(); i++)
        {
          te::gm::LineString* ls = static_cast<te::gm::LineString*> (mls->getGeometryN(i));

          lines.push_back(ls);
        }
      }
// check if the geometry is a line
      else if (ing->getGeomTypeId() == te::gm::LineStringType)
      {
        te::gm::LineString* ls = static_cast<te::gm::LineString*> (ing);

        lines.push_back(ls);
      }
// check if the geometry is a multi point
      else if (ing->getGeomTypeId() == te::gm::MultiPointType)
      {
        te::gm::MultiPoint* mp = static_cast<te::gm::MultiPoint*> (ing);

        for (unsigned int i = 0; i < mp->getNumGeometries(); i++)
        {
          te::gm::LineString* lineinter = new te::gm::LineString(2, te::gm::LineStringType, g->getSRID());
          te::gm::Point* pointinter = static_cast<te::gm::Point*> (mp->getGeometryN(i));

          lineinter->setX(0, pointinter->getX());
          lineinter->setY(0, pointinter->getY());
          lineinter->setX(1, pointinter->getX());
          lineinter->setY(1, pointinter->getY());

          lines.push_back(lineinter);
        }
      }
// check if the geometry is a point
      else if (ing->getGeomTypeId() == te::gm::PointType)
      {
        te::gm::Point* p = static_cast<te::gm::Point*> (ing);

        te::gm::LineString* lineinter = new te::gm::LineString(2, te::gm::LineStringType, g->getSRID());

        lineinter->setX(0, p->getX());
        lineinter->setY(0, p->getY());
        lineinter->setX(1, p->getX());
        lineinter->setY(1, p->getY());

        lines.push_back(lineinter);
      }
// check if the geometry is a geometry collection
      else if (ing->getGeomTypeId() == te::gm::GeometryCollectionType)
      {
        for (unsigned int i = 0; i < gc->getNumGeometries(); i++)
        {
          std::vector<te::gm::LineString*> vg = decompose(gc->getGeometryN(i));

          for (unsigned int j = 0; j < vg.size(); j++)
            lines.push_back(vg[j]);
        }
      }
// throw exception when other types?
      else
      {
        std::string message = "An exception has occurried in Polygon Iterator, with geometry " + g->toString();

        throw(message.c_str());
      }
      return lines;
    }

    template<class T> void te::rst::PolygonIterator<T>::setNextLine(bool updatecurrline)
    {
      if (m_actualintersection == -1 || m_actualintersection >= m_nintersections)
      {
        if (updatecurrline)
        {
          double nexty = this->m_band->getRaster()->getGrid()->gridToGeo(0, m_row).y;

          m_currline->setX(0, m_polygon->getMBR()->getLowerLeft().x);
          m_currline->setX(1, m_polygon->getMBR()->getUpperRight().x);
          m_currline->setY(0, nexty);
          m_currline->setY(1, nexty);
        }

        te::gm::Geometry* inter;
// in some cases the intersection presents an unhandled exception, in this case we do not paint the current line
        try
        {
          inter = m_polygon->intersection(m_currline);

          if (inter->isEmpty())
          {
            delete inter;

            m_row++;

            setNextLine();

            return;
          }
        }
        catch(const std::exception& e)
        {
          std::cout << "Unhandled exception, status:" << std::endl;
          std::cout << "  m_startingcolumn: " << m_startingcolumn << " m_endingcolumn: " << m_endingcolumn << std::endl;
          std::cout << "  m_startingrow: " << m_startingrow << " m_endingrow: " << m_endingrow<< std::endl;
          std::cout << "  m_column: " << m_column << " m_row: " << m_row << std::endl;
          std::cout << "  intersection line: " << m_currline->toString() << std::endl << std::endl;
          std::cout << "  current polygon: " << m_polygon->toString() << std::endl << std::endl;
          std::cout << "  exception message: " << e.what() << std::endl;
          std::cout << std::endl;

          operator++();

          return;
        }


        te::gm::GeometryCollection* intersections = new te::gm::GeometryCollection(0, inter->getGeomTypeId(), inter->getSRID());

        intersections->add(inter);

        m_actualintersection = 0;

        m_intersections.clear();

        m_intersections = decompose(intersections);

        m_nintersections = m_intersections.size();
      }

      te::gm::LineString* lineinter = m_intersections[m_actualintersection];

      m_startingcolumn = (int) this->m_band->getRaster()->getGrid()->geoToGrid(lineinter->getStartPoint()->getX(), lineinter->getStartPoint()->getY()).x;

      m_endingcolumn = (int) this->m_band->getRaster()->getGrid()->geoToGrid(lineinter->getEndPoint()->getX(), lineinter->getEndPoint()->getY()).x;

      int tmp;
      if (m_startingcolumn > m_endingcolumn)
      {
        tmp = m_startingcolumn;
        m_startingcolumn = m_endingcolumn;
        m_endingcolumn = tmp;
      }

// avoiding bad access
      m_startingcolumn = m_startingcolumn < 0? 0: m_startingcolumn;
      m_startingcolumn = m_startingcolumn >= m_maxcolumns? m_maxcolumns - 1: m_startingcolumn;

      m_endingcolumn = m_endingcolumn < 0? 0: m_endingcolumn;
      m_endingcolumn = m_endingcolumn >= m_maxcolumns? m_maxcolumns - 1: m_endingcolumn;
    }

    template<class T> const T te::rst::PolygonIterator<T>::operator*() const
    {
      double value;

      this->m_band->getValue(m_column, m_row, value);

      return (T) value;
    }

    template<class T> unsigned int te::rst::PolygonIterator<T>::getRow() const
    {
      return m_row;
    }

    template<class T> unsigned int te::rst::PolygonIterator<T>::getCol() const
    {
      return m_column;
    }

    template<class T> void te::rst::PolygonIterator<T>::operator++()
    {
      m_column++;

      if (m_column > m_endingcolumn)
      {
        m_actualintersection++;

        if (m_actualintersection >= m_nintersections)
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

    template<class T> void te::rst::PolygonIterator<T>::operator--()
    {
      m_column--;

      if (m_column < m_startingcolumn)
      {
        m_actualintersection--;

        if (m_actualintersection < 0)
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

    template<class T> te::rst::PolygonIterator<T>& te::rst::PolygonIterator<T>::operator=(const te::rst::PolygonIterator<T>& rhs)
    {
      if (this != &rhs)
      {
        te::rst::AbstractPositionIterator<T>::operator=(rhs);

        m_polygon = rhs.m_polygon;
      }

      return *this;
    }

    template<class T> void te::rst::PolygonIterator<T>::setEnd()
    {
      this->m_column = -1;
      this->m_row = -1;
    }

    template<class T> te::rst::PolygonIterator<T> te::rst::PolygonIterator<T>::begin(const te::rst::Band* b, const te::gm::Polygon* p)
    {
      return te::rst::PolygonIterator<T>(b, p);
    }

    template<class T> te::rst::PolygonIterator<T> te::rst::PolygonIterator<T>::end(const te::rst::Band* b, const te::gm::Polygon* p)
    {
      te::rst::PolygonIterator<T> it(b, p);

      it.setEnd();

      return it;
    }

    template<class T> bool te::rst::PolygonIterator<T>::operator!=(const te::rst::PolygonIterator<T>& rhs) const
    {
      return ( (this->m_row != rhs.m_row) && (this->m_column != rhs.m_column));
    }

// implementation of iteration strategy bounded by a line
    template<class T> te::rst::LineIterator<T>::LineIterator()
      : AbstractPositionIterator<T>(),
        m_line(0),
        m_currentpixelindex(0),
        m_pixelsinline(0)
    {
    }

    template<class T> te::rst::LineIterator<T>::LineIterator(const te::rst::Band* b, const te::gm::Line* l)
      : AbstractPositionIterator<T>(b),
        m_line(l),
        m_currentpixelindex(0),
        m_pixelsinline(0)
    {
      int srid = this->m_band->getRaster()->getSRID();

// make intersection between line and band's envelope
      te::gm::Geometry* bandEnvelope = te::gm::GetGeomFromEnvelope(this->m_band->getRaster()->getExtent(), srid);
      te::gm::Geometry* inter = bandEnvelope->intersection(m_line);

      if (inter->isEmpty())
      {
        setEnd();

        return;
      }

// create line that intersects only band's envelope
      te::gm::Line* inrasterline = (te::gm::Line*) inter;

// find starting and ending points
      double startingcolumn;
      double startingrow;
      te::gm::Point* startpoint = inrasterline->getStartPoint();
      this->m_band->getRaster()->getGrid()->geoToGrid(startpoint->getX(), startpoint->getY(),
                                                      startingcolumn, startingrow);

      double endingcolumn;
      double endingrow;
      te::gm::Point* endpoint = inrasterline->getEndPoint();
      this->m_band->getRaster()->getGrid()->geoToGrid(endpoint->getX(), endpoint->getY(),
                                                      endingcolumn, endingrow);

// creating one envelope per pixel, and intersects with line
      const double resXdiv2 = this->m_band->getRaster()->getResolutionX() / 2;
      const double resYdiv2 = this->m_band->getRaster()->getResolutionY() / 2;
      double x1, x2, y1, y2, geoX, geoY;
      for(int r = startingrow; r <= endingrow; r++)
        for(int c = startingcolumn; c <= endingcolumn; c++)
        {
// define envelope of pixel
          this->m_band->getRaster()->getGrid()->gridToGeo(c, r, geoX, geoY);
          x1 = geoX - resXdiv2; y1 = geoY - resYdiv2;
          x2 = geoX + resXdiv2; y2 = geoY + resYdiv2;

          te::gm::Envelope* pixelbox = new te::gm::Envelope(x1, y1, x2, y2);
          te::gm::Geometry* pixelboxgeometry = GetGeomFromEnvelope(pixelbox, srid);

          if (te::gm::SatisfySpatialRelation(inrasterline, pixelboxgeometry, te::gm::INTERSECTS))
            m_pixelsinline.push_back(new te::gm::Point(c, r, srid));
        }
    }

    template<class T> te::rst::LineIterator<T>::LineIterator(const LineIterator<T>& rhs)
      : AbstractPositionIterator<T>(rhs),
        m_currentpixelindex(rhs.m_currentpixelindex),
        m_pixelsinline(rhs.m_pixelsinline)
    {
    }

    template<class T> te::rst::LineIterator<T>::~LineIterator()
    {
      m_pixelsinline.clear();
    }

    template<class T> const T te::rst::LineIterator<T>::operator*() const
    {
      double value;

      this->m_band->getValue(getCol(), getRow(), value);

      return (T) value;
    }

    template<class T> unsigned int te::rst::LineIterator<T>::getRow() const
    {
      return m_pixelsinline[m_currentpixelindex]->getY();
    }

    template<class T> unsigned int te::rst::LineIterator<T>::getCol() const
    {
      return m_pixelsinline[m_currentpixelindex]->getX();
    }

    template<class T> void te::rst::LineIterator<T>::operator++()
    {
      m_currentpixelindex++;

      if (m_currentpixelindex >= m_pixelsinline.size())
        setEnd();
    }

    template<class T> void te::rst::LineIterator<T>::operator--()
    {
      m_currentpixelindex--;

      if (m_currentpixelindex < 0)
        setEnd();
    }

    template<class T> te::rst::LineIterator<T>& te::rst::LineIterator<T>::operator=(const te::rst::LineIterator<T>& rhs)
    {
      if (this != &rhs)
      {
        te::rst::AbstractPositionIterator<T>::operator=(rhs);

        m_line = rhs.m_line;
        m_currentpixelindex = rhs.m_currentpixelindex;
        m_pixelsinline = rhs.m_pixelsinline;
      }

      return *this;
    }

    template<class T> void te::rst::LineIterator<T>::setEnd()
    {
      this->m_currentpixelindex = -1;
    }

    template<class T> te::rst::LineIterator<T> te::rst::LineIterator<T>::begin(const te::rst::Band* b, const te::gm::Line* l)
    {
      return te::rst::LineIterator<T>(b, l);
    }

    template<class T> te::rst::LineIterator<T> te::rst::LineIterator<T>::end(const te::rst::Band* b, const te::gm::Line* l)
    {
      te::rst::LineIterator<T> it(b, l);

      it.setEnd();

      return it;
    }

    template<class T> bool te::rst::LineIterator<T>::operator!=(const te::rst::LineIterator<T>& rhs) const
    {
      return ( (this->m_currentpixelindex != rhs.m_currentpixelindex) );
    }

// implementation of iteration strategy bounded by a vector of points
    template<class T> te::rst::PointSetIterator<T>::PointSetIterator()
      : AbstractPositionIterator<T>(),
        m_pixelsinpointset(0),
        m_currentpixelindex(0)
    {
    }

    template<class T> te::rst::PointSetIterator<T>::PointSetIterator(const te::rst::Band* b, const std::vector<te::gm::Point*> p)
      : AbstractPositionIterator<T>(b),
        m_pixelsinpointset(p),
        m_currentpixelindex(0)
    {
      int srid = this->m_band->getRaster()->getSRID();

      te::gm::Envelope* bandbox = this->m_band->getRaster()->getExtent();
      te::gm::Geometry* bandboxgeometry = GetGeomFromEnvelope(bandbox, srid);

// remove points that are not inside the band's envelope
      std::vector<te::gm::Point*> inside_points;
      double column;
      double row;
      for (unsigned int i = 0; i < m_pixelsinpointset.size(); i++)
        if (te::gm::SatisfySpatialRelation(m_pixelsinpointset[i], bandboxgeometry, te::gm::INTERSECTS))
        {
          this->m_band->getRaster()->getGrid()->geoToGrid(m_pixelsinpointset[i]->getX(), m_pixelsinpointset[i]->getY(), column, row);

          inside_points.push_back(new te::gm::Point(column, row));
        }

      m_pixelsinpointset.clear();
      m_pixelsinpointset = inside_points;

      if (m_pixelsinpointset.empty())
        setEnd();
    }

    template<class T> te::rst::PointSetIterator<T>::PointSetIterator(const PointSetIterator<T>& rhs)
      : AbstractPositionIterator<T>(rhs),
        m_pixelsinpointset(rhs.m_pixelsinpointset),
        m_currentpixelindex(rhs.m_currentpixelindex)
    {
    }

    template<class T> te::rst::PointSetIterator<T>::~PointSetIterator()
    {
      m_pixelsinpointset.clear();
    }

    template<class T> const T te::rst::PointSetIterator<T>::operator*() const
    {
      double value;

      this->m_band->getValue(getCol(), getRow(), value);

      return (T) value;
    }

    template<class T> unsigned int te::rst::PointSetIterator<T>::getRow() const
    {
      return m_pixelsinpointset[m_currentpixelindex]->getY();
    }

    template<class T> unsigned int te::rst::PointSetIterator<T>::getCol() const
    {
      return m_pixelsinpointset[m_currentpixelindex]->getX();
    }

    template<class T> void te::rst::PointSetIterator<T>::operator++()
    {
      m_currentpixelindex++;

      if (m_currentpixelindex >= m_pixelsinpointset.size())
        setEnd();
    }

    template<class T> void te::rst::PointSetIterator<T>::operator--()
    {
      m_currentpixelindex--;

      if (m_currentpixelindex < 0)
        setEnd();
    }

    template<class T> te::rst::PointSetIterator<T>& te::rst::PointSetIterator<T>::operator=(const te::rst::PointSetIterator<T>& rhs)
    {
      if (this != &rhs)
      {
        te::rst::AbstractPositionIterator<T>::operator=(rhs);

        m_pixelsinpointset = rhs.m_pixelsinpointset;
        m_currentpixelindex = rhs.m_currentpixelindex;
      }

      return *this;
    }

    template<class T> void te::rst::PointSetIterator<T>::setEnd()
    {
      this->m_currentpixelindex = -1;
    }

    template<class T> te::rst::PointSetIterator<T> te::rst::PointSetIterator<T>::begin(const te::rst::Band* b, const std::vector<te::gm::Point*> p)
    {
      return te::rst::PointSetIterator<T>(b, p);
    }

    template<class T> te::rst::PointSetIterator<T> te::rst::PointSetIterator<T>::end(const te::rst::Band* b, const std::vector<te::gm::Point*> p)
    {
      te::rst::PointSetIterator<T> it(b, p);

      it.setEnd();

      return it;
    }

    template<class T> bool te::rst::PointSetIterator<T>::operator!=(const te::rst::PointSetIterator<T>& rhs) const
    {
      return ( (this->m_currentpixelindex != rhs.m_currentpixelindex) );
    }
  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_POSITIONITERATOR_H
