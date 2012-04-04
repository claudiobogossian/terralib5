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
  \file Globals.h
   
  \brief A singleton class with global definitions.
 */

#ifndef __TERRALIB_GDAL_INTERNAL_GLOBAL_H
#define __TERRALIB_GDAL_INTERNAL_GLOBAL_H

// TerraLib
#include "../common/Singleton.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace gdal
  {
    /*!
      \class Globals

      \brief A singleton class with global definitions.
     */
    class TEGDALEXPORT Globals : public te::common::Singleton<Globals>
    {
      friend class te::common::Singleton<Globals>;

      public:

        /*!
          \brief It returns the GDAL driver identifier.

          \return The GDAL driver identifier.
         */
        const std::string& getDriverID() const 
        {
          return m_driverIdentifier;
        }

        /* \brief Destructor. */
        ~Globals();

      protected:

        /*! \brief The singleton constructor is not callable outside the class. */
        Globals();

      private:

        const std::string m_driverIdentifier;  //!< The GDAL driver identifier.
    };

  } // end namespace gdal
}   // end namespace te

#endif  // __TERRALIB_GDAL_INTERNAL_GLOBAL_H

