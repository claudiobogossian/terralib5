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

        ScatterStyle (); 

        ScatterStyle (te::se::Graphic* graphic); 

        ~ScatterStyle();

        ScatterStyle* clone();

        te::se::Graphic* getGraphic();

        void setGraphic(te::se::Graphic* newGraphic);

        QwtSymbol* getSymbol();

        private:

          te::se::Graphic*  m_graphic;    //!< The graphic used to edit the symbol's style
          te::se::Mark*     m_mark;       //!< The mark used to edit the symbol's style
      }; 
    }  //end namespace widgets
  }    //end namespace qt
}      // end namespace te
#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERSTYLE_H