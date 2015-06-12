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
    class Fill;
    class Graphic;
    class Mark;
    class Stroke;
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
          \brief It sets the he scatter's graphic

          \param newGraphic The new graphic.
          \note It will take ownership of the given pointer
        */
        void setGraphic(te::se::Graphic* newGraphic);

        /*!
          \brief Returns a  copy of the pointer to the scatter's Mark

          \return A Mark type pointer to the scatter's point style.
          \note The caller will not take ownership of the returned pointer. 
        */
        te::se::Mark* getMark();

        /*!
          \brief It sets the he scatter's mark

          \param newmark The new mark.
          \note It will take ownership of the given pointer
        */
        void setMark(te::se::Mark* newMark);

          /*!
            \brief Returns a copy of the pointer to the scatter point's fill

            \return A Fill type pointer to the scatter point's fill.
            \note The caller will take ownership of the returned pointer.
            \note Will return a null pointer if the scatter's point does not have a Fill (f.e. if it's using an external graphic)
          */
          te::se::Fill* getFill();

          /*!
            \brief It sets the scatter point's fill

            \param newFill The new fill.
            \note It will take ownership of the given pointer
            \note The given pointer will not be used if the scatter's point is not currently represented by a Mark. The given pointer will simply be deleted.
          */
          void setFill(te::se::Fill* newFill);

          /*!
            \brief Returns a copy of thepointer to the scatter point's stroke

            \return A Fill type pointer to the scatter point's stroke.
            \note The caller will take ownership of the returned pointer.
            \note Will return a null pointer if the scatter's point does not have a stroke (f.e. if it's using an external graphic)
          */
          te::se::Stroke* getStroke();

          /*!            
            \brief It sets the  scatter point's stroke

            \param newFill The new stroke.
            \note It will take ownership of the given pointer
            \note The given pointer will not be used if the scatter's point is not currently represented by a Mark. The given pointer will simply be deleted.
          */
          void setStroke(te::se::Stroke* newStroke);

        /*!
          \brief Returns a pointer to a QwtSymbol representing the current scatter's point style

          \return A QwtSymbol type pointer based on the current point's style.
          \note The caller will take ownership of the returned pointer. 
        */
        QwtSymbol* getSymbol();

        private:

          te::se::Graphic*  m_graphic;    //!< The graphic used to edit the symbol's style
      }; 
    }  //end namespace widgets
  }    //end namespace qt
}      // end namespace te
#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERSTYLE_H