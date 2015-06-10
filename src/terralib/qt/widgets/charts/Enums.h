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
  \file terralib/.../Enums.h

  \brief ....
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ENUMS_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ENUMS_H

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \enum DateTimeType

        \brief The subtype of date and time type.
      */
      enum ChartType
      {
        HISTOGRAM_CHART = 1001,  /*!< Histogram.  */
        SCATTER_CHART = 1002     /*!< Scatter.    */
      };
    }
  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATETYPE_INTERNAL_ENUMS_H

