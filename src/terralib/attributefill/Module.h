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
  \file terralib/attributefill/Module.h
   
  \brief This singleton defines the TerraLib Attribute Fill module entry.
*/

#ifndef __TERRALIB_ATTRIBUTEFILL_INTERNAL_MODULE_H
#define __TERRALIB_ATTRIBUTEFILL_INTERNAL_MODULE_H

// TerraLib
#include "../common/Singleton.h"

namespace te
{
  namespace attributefill
  {
    /*!
      \class Module

      \brief This singleton defines the TerraLib Attribute Fill module entry.
    */
    class Module : public te::common::Singleton<Module>
    {
      friend class te::common::Singleton<Module>;

      protected:

        /*! \brief The singleton constructor is not callable outside the class. */
        Module();

        /* \brief Destructor. */
        ~Module();

      private:

        /*!
          \brief This is the startup function for the TerraLib Attribute Fill module.

          \note This method doesn't perform operations for this module.
        */
        static void initialize();

        /*!
          \brief This is the cleanup function for the TerraLib Attribute Fill module.

          \note This method doesn't perform operations for this module.
        */
        static void finalize();

      private:

        static const Module& sm_module; //!< Just to make a static initialization.
    };

  } // end namespace attributefill
}   // end namespace te

#endif  // __TERRALIB_ATTRIBUTEFILL_INTERNAL_MODULE_H

