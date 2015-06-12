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
  \file terralib/qt/widgets/charts/TimeSeriesStyle.h

  \brief A class used to define the style of a TimeSeries's chart
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESSTYLE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESSTYLE_H

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
      class TEQTWIDGETSEXPORT TimeSeriesStyle
      {

        public:

          /*!
            \brief Constructor
          */
          TimeSeriesStyle(); 

          /*!
            \brief Constructor

            \param stroke The line's stroke
          */
          TimeSeriesStyle (te::se::Stroke* stroke);

          /*! 
            \brief Destructor.
          */
          ~TimeSeriesStyle();

          /*!
            \brief Returns a pointer to a clone of this TimeSeriesStyle.

            \return A TimeSeriesStyle type clone of this TimeSeriesStyle.
            \note The caller will take ownership of the returned pointer. 
          */
          TimeSeriesStyle* clone();


          /*!
            \brief Returns a pointer to the line's stroke

            \return A Fill type pointer to the  histogram bar's stroke.
            \note The caller will not take ownership of the returned pointer. 
          */
          te::se::Stroke* getStroke();

          /*!            
            \brief It sets the  line's stroke

            \param newStroke The new stroke.
            \note It will take ownership of the given pointer
          */
          void setStroke(te::se::Stroke* newStroke);

        private:

          void adjustSymbol();

        private:

          te::se::Stroke*  m_stroke;   //!< The bar's stroke.

      };
    }  //end namespace widgets
  }    //end namespace qt
}      // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESSTYLE_H