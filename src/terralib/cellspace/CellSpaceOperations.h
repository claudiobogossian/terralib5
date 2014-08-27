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

  namespace cellspace
  {
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

        void createCellSpace(const std::string& name, te::map::AbstractLayerPtr layerBase,
                             double resX, double resY, bool useMask,
                             CellSpaceType type = CELLSPACE_POLYGONS);

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

      private:

        void addCell(te::mem::DataSet* ds, int col, int row, te::gm::Geometry* geom);

        te::sam::rtree::Index<size_t, 8>* getRtree(te::map::AbstractLayerPtr layerBase);

      private:

        te::da::DataSetType* m_outputDataSetType;
        te::da::DataSet* m_outputDataSet;
    };
  }
}


#endif // __TERRALIB_CELLSPACE_INTERNAL_CELLSPACEOPERATIONS_H