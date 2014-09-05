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
          CELLSPACE_POINTS,   /*!< Points Type. */
          CELLSPACE_RASTER    /*!< Raster Type. */
        };

        CellularSpacesOperations();

        ~CellularSpacesOperations();

        /*!
          \brief It create a Cellular Space.

          \param name      Name of the DataSet.
          \param layerbase Layer base for cellular space creation.
          \param resX      Resolution X.
          \param resY      Resolution Y.
          \param useMask   If will use poligons as mask.
          \param type      Celluar space type (polygon, points or raster).
        */
        void createCellSpace(te::da::DataSourceInfoPtr outputSource,
                             const std::string& name,
                             te::map::AbstractLayerPtr layerBase,
                             double resX,
                             double resY,
                             bool useMask,
                             CellSpaceType type = CELLSPACE_POLYGONS);

        /*!
          \brief It create a Cellular Space.

          \param name      Name of the DataSet.
          \param resX      Resolution X.
          \param resY      Resolution Y.
          \param env       Envelope base to creation.
          \param srid      SRID of the envelope.
          \param type      Celluar space type (polygon, points or raster).
        */
        void createCellSpace(te::da::DataSourceInfoPtr outputSource,
                             const std::string& name,
                             const double resX,
                             const double resY,
                             const te::gm::Envelope& env,
                             const int srid,
                             const CellSpaceType type = CELLSPACE_POLYGONS);

        /*!
          \brief It return the created DataSetType.

          \return The created DataSetType.

          \note The caller of this method will take the ownership of the returned pointer.
        */
        te::da::DataSetType* getDataSetType();

        /*!
          \brief It return the created DataSet.

          \return The created DataSet.

          \note The caller of this method will take the ownership of the returned pointer.
        */
        te::da::DataSet* getDataSet();

        /*!
          \brief It return the created Raster.

          \return The created Raster.

          \note The caller of this method will take the ownership of the returned pointer.
        */
        std::auto_ptr<te::rst::Raster> getRaster();

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

        void createRasteCellSpace(te::da::DataSourceInfoPtr outputSource,
                                  const std::string& name,
                                  const double resX,
                                  const double resY,
                                  const te::gm::Envelope& env,
                                  const int srid);

        void createRasteCellSpace(te::da::DataSourceInfoPtr outputSource,
                                  const std::string& name,
                                  te::map::AbstractLayerPtr layerBase,
                                  double resX,
                                  double resY,
                                  bool useMask);

        void save(te::da::DataSourceInfoPtr sourceInfo);

      private:

        te::da::DataSetType* m_outputDataSetType; /*!< Output DataSetType. */
        te::da::DataSet*     m_outputDataSet;     /*!< Output DataSet. */
        std::auto_ptr<te::rst::Raster>     m_outputRaster;      /*!< Output Raster. */
    };
  }
}


#endif // __TERRALIB_CELLSPACE_INTERNAL_CELLSPACEOPERATIONS_H