/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/cellspace/CellSet.h
   
  \brief class that define a Cell Set.
*/

#ifndef __TERRALIB_CELLSPACE_INTERNAL_CELLSET_H
#define __TERRALIB_CELLSPACE_INTERNAL_CELLSET_H

// TerraLib
#include "Config.h"

// STL
#include <vector>

namespace te
{
  namespace cellspace
  {
    class Cell;

    /*!
      \class CellSet

      \brief This class define a cell set in the Cellular Spaces module.
    */
    class TECELLSPACEEXPORT CellSet
    {
      public:

        CellSet(double resX, double resY);

        ~CellSet();

        double getResX();

        double getResY();

        std::vector<Cell> getCells();

        void setResX(double resX);

        void setResY(double resY);

        void setCells(std::vector<Cell> cells);

        void addCell(Cell cell);

      private:

        int m_resX;
        int m_resY;

        std::vector<Cell> m_cells;
    };

  } // end namespace cellspace
}   // end namespace te

#endif  // __TERRALIB_CELLSPACE_INTERNAL_CELLSET_H

