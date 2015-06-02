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
 
 \brief This singleton defines the TerraLib Spatial Reference System module entry.
 */

#ifndef __TERRALIB_SRS_INTERNAL_MODULE_H
#define __TERRALIB_SRS_INTERNAL_MODULE_H

// TerraLib
#include "../common/Singleton.h"

// STL
#include <string>

// Boost
#include <boost/thread/mutex.hpp>

namespace te
{
  namespace srs
  {
    /*!
     \class Module
     
     \brief This singleton defines the TerraLib Spatial Reference System module entry.
     */
    class Module : public te::common::Singleton<Module>
    {
      friend class te::common::Singleton<Module>;
      
    protected:
      
      //! Constructor. A singleton constructor is not callable outside the class.
      Module();
      
      //! Destructor.
      ~Module();
      
    private:
      
      /*!
       \brief This is the startup function for the TerraLib SRS module.
       
       \note This method doesn't perform operations for this module.
       */
      static void initialize();
      
      /*!
       \brief This is the cleanup function for the TerraLib SRS module.
       
       \note This method doesn't perform operations for this module.
       */
      static void finalize();
      
      static const Module& sm_module; //!< Just to make a static initialization.
      
    };

    /*!
    \brief Returns a reference to a static mutex initialized when this module is initialized.
    \return Returns a reference to a static mutex initialized when this module is initialized.
    */
    TESRSEXPORT boost::mutex& getStaticMutex();
  } // end namespace srs
}   // end namespace te

#endif  // __TERRALIB_SRS_INTERNAL_MODULE_H

