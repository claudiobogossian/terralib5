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
  \file ProjectXmlRW.h

  \brief Application Project reader/writer
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_PROJECTXMLRW_H
#define __TERRALIB_QT_AF_INTERNAL_PROJECTXMLRW_H

// Terralib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace qt
  {
    namespace af
    {
      // Forward declarations
      class Project;

      /*!
        \class ProjectXmlRW

        \brief Application Project reader/writer

      */
      class TEQTAFEXPORT ProjectXmlRW
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructor. */
          ProjectXmlRW();

          /*! \brief Virtual destructor. */
          ~ProjectXmlRW();

          //@}

          te::qt::af::Project* read(std::string path);

          void write(te::qt::af::Project* project, std::string path);

        private:

       
      };
    } // af
  } // qt
} // te

#endif  // __TERRALIB_QT_AF_INTERNAL_PROJECTXMLRW_H
