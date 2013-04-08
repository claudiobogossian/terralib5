/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/HistogramStyle.h

  \brief A class used to define the style of a histogram's chart
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMSTYLE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMSTYLE_H

//TerraLib
#include "../Config.h"

//QT
#include "qcolor.h"

namespace te
{
  namespace se 
  {
    //forward declarations
    class Fill;
    class Stroke;
  }

  namespace qt
  {

    namespace widgets
    {
      class TEQTWIDGETSEXPORT HistogramStyle
      {

        public:

          HistogramStyle(); 

          HistogramStyle (te::se::Fill* fill, te::se::Stroke* stroke);

          ~HistogramStyle();

          te::se::Fill* getFill();

          void setFill(te::se::Fill* newFill);

          te::se::Stroke* getStroke();

          void setStroke(te::se::Stroke* newStroke);

        private:

          void adjustSymbol();

        private:

          te::se::Fill*    m_fill;     //!< The bar's fill .
          te::se::Stroke*  m_stroke;   //!< The bar's stroke.

      };
    }; //end namespace widgets
  }    //end namespace qt
}      // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMSTYLE_H