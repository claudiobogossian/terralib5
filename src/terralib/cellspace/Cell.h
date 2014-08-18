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
  \file terralib/cellspace/Cell.h
   
  \brief class that define a Cell.
*/

#ifndef __TERRALIB_CELLSPACE_INTERNAL_CELL_H
#define __TERRALIB_CELLSPACE_INTERNAL_CELL_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace cellspace
  {
    /*!
      \class Cell

      \brief This class define a cell in the Cellular Spaces module.
    */
    class TECELLSPACEEXPORT Cell
    {
      public:

        Cell(int column, int row);

        ~Cell();

        int getColumn();

        int getRow();

        void setColumn(int column);

        void setRow(int row);

      private:

        int m_column;
        int m_row;
    };

  } // end namespace cellspace
}   // end namespace te

#endif  // __TERRALIB_CELLSPACE_INTERNAL_CELL_H

