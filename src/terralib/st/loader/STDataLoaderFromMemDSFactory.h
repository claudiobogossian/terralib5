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
  \file STDataLoaderFromMemDSFactory.h

  \brief This is the concrete factory for a STDataLoaderFromMemDS.  
*/

#ifndef __TERRALIB_ST_INTERNAL_STDATALOADERFROMMEMDSFACTORY_H
#define __TERRALIB_ST_INTERNAL_STDATALOADERFROMMEMDSFACTORY_H

// TerraLib
#include "STDataLoaderImplFactory.h"

namespace te
{
  namespace st
  {
    /*!
      \class STDataLoaderFromMemDSFactory

      \brief This is the concrete factory for a STDataLoaderFromMemDS.

      \sa STDataLoaderFromMemDS
     */
    class TESTEXPORT STDataLoaderFromMemDSFactory : public STDataLoaderImplFactory
    {
      public:
        
         /*! \brief Destructor that automatically unregister the factory from the dictionary. */
        ~STDataLoaderFromMemDSFactory();

        /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory CachedSTDataLoader. */
        static void initialize();

        /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory CachedSTDataLoader. */
        static void finalize();

        /*! \brief It returns the type of the factory.  */
        const std::string& getType() const;
              
      protected:        

        /*! \brief It returns the global object STDataSourceLoader.  */
        STDataLoaderImpl* build();

        /*! \brief Constructor.  */
        STDataLoaderFromMemDSFactory();        

      private:

        static STDataLoaderFromMemDSFactory*      sm_factory; //!< A pointer to the global STDataLoaderFromMemDSFactory factory.
      };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_STDATALOADERFROMMEMDSFACTORY_H


