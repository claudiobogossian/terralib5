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
  \file STDataLoaderFromDSFactory.h

  \brief This is the concrete factory for a STDataSourceLoaderFromDS.  
 */

#ifndef __TERRALIB_ST_INTERNAL_STDATALOADERFROMDSFACTORY_H
#define __TERRALIB_ST_INTERNAL_STDATALOADERFROMDSFACTORY_H

// TerraLib
#include "STDataLoaderImplFactory.h"

namespace te
{
  namespace st
  {
    /*!
      \class STDataLoaderFromDSFactory

      \brief This is the concrete factory for a STDataSourceLoaderFromDS.

      \sa STDataSourceLoaderFromDS
     */
    class TESTEXPORT STDataLoaderFromDSFactory : public STDataLoaderImplFactory
    {
      public:
        
         /*! \brief Destructor that automatically unregister the factory from the dictionary. */
        ~STDataLoaderFromDSFactory();

        /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory CachedSTDataLoader. */
        static void initialize();

        /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory CachedSTDataLoader. */
        static void finalize();

        /*! \brief It returns the type of the factory.  */
        const std::string& getType() const;
              
      protected:        

        /*! \brief It returns the global object DataSourceLoader.  */
        STDataLoaderImpl* build();

        /*! \brief Constructor.  */
        STDataLoaderFromDSFactory();        

      private:

        static STDataLoaderFromDSFactory*    sm_factory; //!< A pointer to the global STDataLoaderFromDSFactory factory.
    };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_STDATALOADERFROMDSFACTORY_H


