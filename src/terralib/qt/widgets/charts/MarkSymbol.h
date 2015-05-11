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
  \file MarkSymbol.h

  \brief This file contains a class that specialize the QwtSymbol to draw a Terralib Mark. 
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_MARKSYMBOL_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_MARKSYMBOL_H

//TerraLib
#include "../Config.h"

//QWT
#include <qwt_symbol.h>

namespace te
{
  // Forward declarations
  namespace se { class Mark; }
   
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class MarkSymbol

        \brief A class that specialize the QwtSymbol to draw a Terralib Mark.
      */
      class TEQTWIDGETSEXPORT MarkSymbol : public QwtSymbol
      {
        public:

          /*!
            \brief Constructor

            \note It will not take the ownership of the pointers.
          */
          MarkSymbol(te::se::Mark* mark, std::size_t size);
          
          /*!
            \brief Set the scatter data

            \note It will not take the ownership of the pointers.
          */
          void drawSymbols(QPainter* painter, const QPointF* point, int numPoints) const; 
                          
        protected:
          te::se::Mark*   m_mark; //!<  The mark.
          std::size_t     m_size; //!< The mark's size
                                 
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_MARKSYMBOL_H

