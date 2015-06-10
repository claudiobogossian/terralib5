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
  \file STDataLoaderImplFactory.h

  \brief This is the abstract factory for STDataLoaderImpl.
*/

#ifndef __TERRALIB_ST_INTERNAL_STDATALOADERIMPLFACTORY_H
#define __TERRALIB_ST_INTERNAL_STDATALOADERIMPLFACTORY_H

// TerraLib
#include "../../common/AbstractFactory.h"
#include "../Config.h"
#include "STDataLoaderImpl.h"

namespace te
{
  namespace st
  {   
    //Forward declaration
    class STDataLoaderImpl;

    /*!
      \class STDataLoaderImplFactory

      \brief This is the abstract factory for STDataLoaderImpl.

      This is an abstract factory, so you will need to call the static method make
      as follow:

      \code
      STDataLoaderImpl* ds = STDataLoaderImplFactory::make("POSTGIS");
      
      \sa STDataLoaderImpl
    */
    class TESTEXPORT STDataLoaderImplFactory : public te::common::AbstractFactory<STDataLoaderImpl, std::string>
    {
      public:

        /*!
          \brief It verifies the type of the DataSource (dsType) and gets the suitable factory.
        */
        static STDataLoaderImpl* make(const std::string& dsType);
       
        /*!
          \brief Virtual destructor.

          \note It will automatically unregister the factory from the dictionary.
        */
        virtual ~STDataLoaderImplFactory();

        /*!
          \brief It returns the type of STDataLoaderImpl that the factory knows how to create.

          \return The type of STDataLoaderImpl that the factory knows how to create.
        */
        virtual const std::string& getType() const = 0;

     protected:

        /*!
          \brief It creates the factory.

          The key of a STDataLoaderFactory is a string.

          \param factoryKey The key that identifies the factory.
        */
        STDataLoaderImplFactory(const std::string& factoryKey);
    };    

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_STDATALOADERFACTORYIMPL_H

