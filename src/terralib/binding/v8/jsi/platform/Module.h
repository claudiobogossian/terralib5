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
  \file Module.h
   
  \brief This singleton defines the TerraLib JavaScript-API module entry.
 */

#ifndef __TERRALIB_BINDING_V8_JSI_INTERNAL_MODULE_H
#define __TERRALIB_BINDING_V8_JSI_INTERNAL_MODULE_H

// TerraLib
#include "../../../../common/Singleton.h"

namespace te
{
  namespace v8
  {
    namespace jsi
    {
      /*!
        \class Module

        \brief This is a singleton that defines the TerraLib JavaScript-API module entry.
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
            \brief This is the startup function for the TerraLib V8-API module.

            \note This method doesn't perform operations for this module.
           */
          static void initialize();

          /*!
            \brief This is the cleanup function for the TerraLib V8-API module.

            \note This method doesn't perform operations for this module.
           */
          static void finalize();

        private:

          static const Module& sm_module; //!< Just to make a static initialization.
      };

    } // end namespace jsi
  }   // end namespace v8
}     // end namespace te

#endif  // __TERRALIB_BINDING_V8_JSI_INTERNAL_MODULE_H

