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
  \file terralib/cellspace/CellularSpacesOperations.h

  \brief Class to provide Cellular Spaces operations.
*/

#ifndef __TERRALIB_CELLSPACE_INTERNAL_CELLSPACEOPERATIONS_H
#define __TERRALIB_CELLSPACE_INTERNAL_CELLSPACEOPERATIONS_H

// TerraLib
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../maptools/AbstractLayer.h"
#include "Config.h"

namespace te
{

  namespace ds
  {
    class DataSetType;
    class DataSet;
  }

  namespace gm
  {
    class Envelope;
    class Geometry;
  }

  namespace mem
  {
    class DataSet;
  }

  namespace rst
  {
    class Raster;
  }

  namespace cellspace
  {
    /*!
      \class CellularSpacesOperations

      \brief This class provide cellular spaces operations.
    */
    class TECELLSPACEEXPORT CellularSpacesOperations
    {
      public:

        /*! \enum  Cellular Space Type. */
        enum CellSpaceType
        {
          CELLSPACE_POLYGONS, /*!< Polygons Type. */
          CELLSPACE_POINTS    /*!< Points Type. */
        };

        CellularSpacesOperations();

        ~CellularSpacesOperations();

        /*!
          \brief It creates a Cellular Space.

          \param outputSource Information about the output datasource.
          \param name      Name of the output layer.
          \param resX      Cells resolution in X-dimension.
          \param resY      Cells resolution in X-dimension.
          \param env       The bouding box of the cell space.
          \param srid      The spatial reference for the bouding box.
          \param type      The type of cell space to be created.
        */
        void createCellSpace(te::da::DataSourceInfoPtr outputSource,
                             const std::string& name,
                             const double& resX,
                             const double& resY,
                             const te::gm::Envelope& env,
                             const int srid,
                             CellSpaceType type,
                             te::map::AbstractLayerPtr layerBase);
      private:

        /*!
          \brief Add a cell in the memory dataset

          \param ds   DataSet where the cell will be added.
          \param col  Column number.
          \param row  Row number.
          \param geom Geometry that will be added.
        */
        void addCell(te::mem::DataSet* ds, int col, int row, te::gm::Geometry* geom);

        /*!
          \brief Get a RTree with the geometries envelopes of layer.

          \param layerBase Layer base.

          \return The RTree created.
        */
        te::sam::rtree::Index<size_t, 8>* getRtree(te::map::AbstractLayerPtr layerBase);

        /*!
          \brief Create the DataSetType of the cellular space.

          \param name DataSetType name.
          \param srid DataSetType SRID.
          \param type Celluar space type (polygon, points or raster).

          \return The DataSetType created.
        */
        te::da::DataSetType* createCellularDataSetType(const std::string& name, int srid, CellSpaceType type);
    };
  }
}


#endif // __TERRALIB_CELLSPACE_INTERNAL_CELLSPACEOPERATIONS_H