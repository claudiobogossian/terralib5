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
  \file terralib/qt/widgets/charts/scatterStyle.h

  \brief A class used to define the style of a scatter's chart
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERSTYLE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERSTYLE_H

//TerraLib
#include "../Config.h"

class QwtSymbol;

namespace te
{
  // Forward declarations
  namespace se
  {
    class Graphic;
    class Mark;
  }

  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT ScatterStyle
      {
        public:


          /*!
            \brief Constructor
          */
        ScatterStyle (); 

          /*!
            \brief Constructor

            \param graphic The scatter's point graphic.
          */
        ScatterStyle (te::se::Graphic* graphic); 

          /*! 
            \brief Destructor.
          */
        ~ScatterStyle();

        /*!
          \brief Returns a pointer to a clone of this ScatterStyle.

          \return A ScatterStyle type clone of this ScatterStyle.
          \note The caller will take ownership of the returned pointer. 
        */
        ScatterStyle* clone();

        /*!
          \brief Returns a pointer to the scatter's graphic

          \return A graphic type pointer to the scatter's point style.
          \note The caller will not take ownership of the returned pointer. 
        */
        te::se::Graphic* getGraphic();

        /*!
          \brief It sets the  histogram bar's fill

          \param newFill The new fill.
          \note It will take ownership of the given pointer
        */
        void setGraphic(te::se::Graphic* newGraphic);

        /*!
          \brief Returns a pointer to a QwtSymbol representing the current scatter's point style

          \return A QwtSymbol type pointer based on the current point's style.
          \note The caller will take ownership of the returned pointer. 
        */
        QwtSymbol* getSymbol();

        private:

          te::se::Graphic*  m_graphic;    //!< The graphic used to edit the symbol's style
          te::se::Mark*     m_mark;       //!< The mark used to edit the symbol's style
      }; 
    }  //end namespace widgets
  }    //end namespace qt
}      // end namespace te
#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERSTYLE_H