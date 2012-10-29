/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file LexerFactory.h

  \brief A factory method for language lexers.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_LEXERFACTORY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_LEXERFACTORY_H

// TerraLib
#include "../../../common/Static.h"
#include "../Config.h"

// STL
#include <string>

// Forward declaration
class QObject;
class QString;

class QsciLexer;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class LexerFactory

        \brief A factory method for language lexers.
      */
      class TEQTWIDGETSEXPORT LexerFactory : public te::common::Static
      {
        public:

          static QsciLexer* make(const QString& lang, QObject* parent = 0);
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_LEXERFACTORY_H

