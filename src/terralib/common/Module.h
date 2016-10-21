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
  \file terralib/common/Module.h
   
  \brief This singleton defines an entry in the Platform for the TerraLib Common Runtime module.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_MODULE_H
#define __TERRALIB_COMMON_INTERNAL_MODULE_H

// TerraLib
#include "Singleton.h"

namespace te
{
  namespace common
  {
    /*!
      \class Module

      \brief This singleton defines an entry in the Platform for the TerraLib Common Runtime module.

      This class is responsible for:
      <ul>
      <li>registering the initialization and cleanup routines for the Common Runtime module if it is required.</li>
      <li>start the logger configuration if TerraLib is configured to have it initialized automatically.</li>
      <li>registering the Common Runtime multilingual support.</li>
      </ul>

      \ingroup common
    */
    class Module : public te::common::Singleton<Module>
    {
      friend class te::common::Singleton<Module>;

      protected:

        /*!
          \brief The singleton constructor will make some initializations.

          The initialization includes:
          <ul>
          <li>registering the Common Runtime module in the list of initializable modules.</li>
          <li>registering the Common Runtime multilingual support.</li>
          <li>Singletons initialization</li>
          <li>Logger initialization when automatic static initialization is enabled.</li>
          </ul>
        */
        Module();

        /*
          \brief The singleton destructor will make some cleanup.

          The cleanup includes:
          <ul>
          <li>Logger finalization when automatic static initialization is enabled.</li>
          <li>un-registering the module from the TerraLib Platform singleton.</li>
          </ul>
        */
        ~Module();       

      private:

        /*!
          \brief This is the startup function for the TerraLib Common Runtime module.

          \note This method doesn't perform operations for this module.
        */
        static void initialize();

        /*!
          \brief This is the cleanup function for the TerraLib Common Runtime module.

          \note This method doesn't perform operations for this module.
        */
        static void finalize();

      private:

        static const Module& sm_module; //!< let's force C++ to make an static initialization.
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_MODULE_H

