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
  \file Globals.h
   
  \brief An static class with global definitions.
 */

#ifndef __TERRALIB_OGR_INTERNAL_GLOBAL_H
#define __TERRALIB_OGR_INTERNAL_GLOBAL_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class Globals

        \brief An static class with global definitions.
       */
      class TEQTWIDGETSEXPORT Globals
      {
        private:

          /*! \brief Not instantiable. */
          Globals();

        public:

            static const char* sm_pngFmt;
            static const char* sm_jpegFmt;
            static const char* sm_gifFmt;
            static const char* sm_bmpFmt;
            static const char* sm_xpmFmt;
            static const char* sm_xbmFmt;
            static const char* sm_unknownFmt;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_OGR_INTERNAL_GLOBAL_H

