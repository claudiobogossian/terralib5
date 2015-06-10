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

          /*!
            \brief Constructor
          */
          HistogramStyle(); 

          /*!
            \brief Constructor

            \param fill The histogram bar's fill
            \param stroke The histogram bar's stroke
          */
          HistogramStyle (te::se::Fill* fill, te::se::Stroke* stroke);

          /*! 
            \brief Destructor.
          */
          ~HistogramStyle();

          /*!
            \brief Returns a pointer to a clone of this HistogramStyle.

            \return A HistogramStyle type clone of this HistogramStyle.
            \note The caller will take ownership of the returned pointer. 
          */
          HistogramStyle* clone();

          /*!
            \brief Returns a pointer to the histogram bar's fill

            \return A Fill type pointer to the histogram bar's fill.
            \note The caller will not take ownership of the returned pointer. 
          */
          te::se::Fill* getFill();

          /*!
            \brief It sets the  histogram bar's fill

            \param newFill The new fill.
            \note It will take ownership of the given pointer
          */
          void setFill(te::se::Fill* newFill);

          /*!
            \brief Returns a pointer to the histogram bar's stroke

            \return A Fill type pointer to the  histogram bar's stroke.
            \note The caller will not take ownership of the returned pointer. 
          */
          te::se::Stroke* getStroke();

          /*!            
            \brief It sets the  histogram bar's stroke

            \param newFill The new stroke.
            \note It will take ownership of the given pointer
          */
          void setStroke(te::se::Stroke* newStroke);

        private:

          void adjustSymbol();

        private:

          te::se::Fill*    m_fill;     //!< The bar's fill .
          te::se::Stroke*  m_stroke;   //!< The bar's stroke.

      };
    }  //end namespace widgets
  }    //end namespace qt
}      // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMSTYLE_H