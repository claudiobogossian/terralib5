/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/raster/Grid.h

  \brief A rectified grid is the spatial support for raster data.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_GRID_H
#define __TERRALIB_RASTER_INTERNAL_GRID_H

// TerraLib
#include "../geometry/Coord2D.h"
#include "../srs/Config.h"
#include "Config.h"

// Boost
#include <boost/shared_ptr.hpp>

namespace te
{
// Forward declaration
  namespace gm { class Envelope; }

  namespace rst
  {
    /*!
      \class Grid

      \brief A rectified grid is the spatial support for raster data.
      
      \note The upper left corner of the upper left pixel is considered the grid extent top-left reference.
      
      \code
            Affine transformation:
            
            u = a.x + b.y + c
            v = d.x + e.y + f
            
            | x |   | a b c |   | column |
            | y | = | d e f | * | row    |
            | 1 |   | 0 0 1 |   | 1      |

            Georeference parameters order = [ a b c d e f ]
      \endcode

      \ingroup rst

      \sa Raster
    */
    class TERASTEREXPORT Grid
    {
      public:

        /*!
          \brief Constructor.

          \param nCols  The number of columns.
          \param nRows  The number of rows.
          \param srid   The SRS associated to the grid.
          \param mbr    The grid extent. The grid will take the ownership of the given envelope.

          \note If extent is not specified, no georeferencing is assumed.
          \note The grid resolution (x-axis and y-axis) will be computed based on the number of columns/rows and the provided extent.
        */
        Grid(unsigned int nCols = 0, unsigned int nRows = 0,
             te::gm::Envelope* mbr = 0, int srid = TE_UNKNOWN_SRS);

        /*!
          \brief Constructor.

          \param nCols  The number of columns.
          \param nRows  The number of rows.
          \param resX   The grid horizontal (x-axis) resolution.
          \param resY   The grid vertical (y-axis) resolution.
          \param ulc    The location of the upper-left corner of the upper-left pixel (or cell) of the grid assuming srid as the SRS.
          \param srid   The SRS associated to the grid.

          \note If ulc is not specified, no georeferencing is assumed.
        */
        Grid(unsigned int nCols, unsigned int nRows,
             double resX, double resY,
             const te::gm::Coord2D* ulc, int srid = TE_UNKNOWN_SRS);

        /*!
          \brief Constructor.

          \param nCols  The number of columns.
          \param nRows  The number of rows.
          \param resX   The grid horizontal (x-axis) resolution.
          \param resY   The grid vertical (y-axis) resolution.
          \param mbr    The grid extent. The grid will take the ownership of the given envelope.
          \param srid   The SRS associated to the grid.

          \note If extent is not specified, no georeferencing is assumed.
        */
        Grid(unsigned int nCols, unsigned int nRows,
             double resX, double resY,
             te::gm::Envelope* mbr, int srid = TE_UNKNOWN_SRS);

        /*!
          \brief Constructor.

          \param resX   The grid horizontal (x-axis) resolution.
          \param resY   The grid vertical (y-axis) resolution.
          \param mbr    The grid extent. The grid will take the ownership of the given envelope.
          \param srid   The SRS associated to the grid.

          \note The grid resolution (x-axis and y-axis) will be computed based on the number of columns/rows and the provided extent.
        */
        Grid(double resX, double resY,
             te::gm::Envelope* mbr, int srid = TE_UNKNOWN_SRS);
             
        /*!
          \brief Constructor.

          \param nCols  The number of columns.
          \param nRows  The number of rows.
          \param geoTrans[]  A list of 6 coefficients describing an affine transformation to georeference a grid.
          \param srid   The SRS associated to the grid.

          \note If extent is not specified, no georeferencing is assumed.
        */
        Grid(const double geoTrans[], unsigned int nCols, unsigned int nRows,
             int srid = TE_UNKNOWN_SRS);             

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side object to be used to copy from.
        */
        Grid(const Grid& rhs);

        /*! \brief Destructor. */
        ~Grid();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side object to be used to copy from.

          \return A reference to this.
        */
        Grid& operator=(const Grid& rhs);

        /*!
          \brief Sets the grid number of columns.

          \param nCols The number of columns.
        */
        void setNumberOfColumns(unsigned int nCols);

        /*! \brief Returns the grid number of columns. */
        unsigned int getNumberOfColumns() const;

        /*!
          \brief Sets the grid number of rows.

          \param nRows The number of rows.
        */
        void setNumberOfRows(unsigned int nRows);

        /*! \brief Returns the grid number of rows. */
        unsigned int getNumberOfRows() const;

        /*!
          \brief Sets the information needed to georeference the grid.

          Specifies a transformation that maps a grid point (col, row) to a spatial location (x, y).

          This method should be used when there is no translation associated to the mapping.

          \param ulLocation  The location of the upper-left corner of the upper-left pixel (or cell) of the grid.
          \param srid        The id of the SRS that the location refers to.
          \para resX         The grid horizontal (x-axis) resolution
          \para resY         The grid vertical (y-axis) resolution
        */
        void setGeoreference(const te::gm::Coord2D& ulLocation, int srid, double resX, double resY);

        /*!
          \brief Sets the information needed to georeference the grid using an affine transformation.

          An affine transformation maps grid point (col,lin) to a spatial location (x,y) using to the
          following relationship:
          This method should be used mainly when there is a translation associated to the mapping.
          Otherwise use the method above.

          \param geoTrans[]  A list of 6 coefficients describing an affine transformation to georeference a grid.
          \param srid        The SRS id to which the location refers to.
        */
        void setGeoreference(const double geoTrans[], int srid);

        /*! \brief Returns a list of 6 coefficients describing an affine transformation to georeference a grid. */
        const double* getGeoreference() const;

        /*! \brief Returns the grid horizontal (x-axis) resolution. */
        double getResolutionX() const;

        /*! \brief Returns the grid vertical (y-axis) resolution. */
        double getResolutionY() const;

        /*! \brief Returns the grid spatial reference system identifier. */
        int getSRID() const;

        /*! \brief Just sets the grid spatial reference system identifier. */
        void setSRID(int srid);

        /*! \brief Returns the geographic extension of the grid. */
        te::gm::Envelope* getExtent();

        /*! \brief Returns the geographic extension of the grid. */
        const te::gm::Envelope* getExtent() const;

        /*! \brief Computes the geographic extension of the grid. */
        void computeExtent() const;

        /*!
          \brief Get the spatial location of a grid point.

          \param col The grid point column.
          \param row The grid point row.
          \param x   The spatial location (X axis).
          \param y   The spatial location (Y axis).
        */
        void gridToGeo(const double& col, const double& row, double& x, double& y) const;

        /*!
          \brief Get the spatial location of a grid point.

          \param col The grid point column.
          \param row The grid point row.

          \return The spatial location.
        */
        te::gm::Coord2D gridToGeo(const double& col, const double& row) const;

        /*!
          \brief Get the grid point associated to a spatial location.

          \param x   The spatial x-coordiante.
          \param y   The spatial y-coordiante.
          \param col The grid point column.
          \param row The grid point row.
        */
        void geoToGrid(const double& x, const double& y, double& col, double& row) const;

        /*!
          \brief Get the grid point associated to a spatial location.

          \param x   The spatial x-coordiante.
          \param y   The spatial y-coordiante.

          \return The grid location.
        */
        te::gm::Coord2D geoToGrid(const double& x, const double& y) const;

        /*!
          \brief Equal operator.

          \param rhs The source object to be compared.

          \return True if the grids have the same exact coordinates, number of cols and rows, srid, and 6 coefficients.
        */
        bool operator==(const Grid& rhs) const;

        /*
          \brief Returns if point is in the grid.

          \param col The column to test.
          \param row The row to test.

          \return True if point is in the grid, and false otherwise.
        */
        bool isPointInGrid(unsigned int col, unsigned int row) const;

      private:

        mutable te::gm::Envelope* m_extent; //!< The grid extent.
        unsigned int m_nCols;               //!< Number of columns.
        unsigned int m_nRows;               //!< Number of rows.
        int m_srid;                         //!< The associated SRS.
        double m_geoT[6];                   //!< A list of 6 coefficients describing an affine transformation to georeference a grid.
        double m_geoTInverse[6];            //!< A list of 6 coefficients describing an inverse affine transformation to georeference a grid.
        
        /*!
          \brief Compute affine parameters from the given input parameters.

          \param extentWidth Extent width.
          \param extentHeight Extent height.
          \param nColumns Number of columns.
          \param nRows Number of rows.
          \param extentULX Extent upper-left X coordinate.
          \param extentULY Extent upper-left Y coordinate.
          \param affineParamsPtr A pointer to a allocated vector of 6 elements where the parameters will be stored.
          \return true if ok, false on errors.
        */
        bool computeAffineParameters( const double extentWidth,
                                      const double extentHeight,
                                      const unsigned int nColumns,
                                      const unsigned int nRows,
                                      const double extentULX,
                                      const double extentULY,
                                      double* affineParamsPtr,
                                      double* inverseAffineParamsPtr ) const;        
                                      
        /*!
          \brief Compute inverse affine parameters
          \param affineParamsPtr A pointer to a allocated vector of 6 elements (affine parameters).
          \param inverseAffineParamsPtr A pointer to a allocated vector of 6 elements where the parameters will be stored.
          \return true if ok, false on errors
        */
        bool computeInverseParameters( double* const affineParamsPtr,
          double* inverseAffineParamsPtr ) const;                                          
    };

    typedef boost::shared_ptr<Grid> GridPtr;

    inline te::gm::Coord2D Grid::gridToGeo(const double& col, const double& row) const
    {
      te::gm::Coord2D c;
      gridToGeo(col, row, c.x, c.y);
      return c;
    }

    inline te::gm::Coord2D Grid::geoToGrid(const double& x, const double& y) const
    {
      te::gm::Coord2D c;
      geoToGrid(x, y, c.x, c.y);
      return c;
    }

    inline bool Grid::isPointInGrid(unsigned int col, unsigned int row) const
    {
      return (col < m_nCols) && (row < m_nRows);
    }

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_GRID_H
