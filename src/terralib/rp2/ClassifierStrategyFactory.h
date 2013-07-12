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
  \file terralib/rp/ClassifierStrategyFactory.h

  \brief Raster classifier strategy factory base class.
 */

#ifndef __TERRALIB_RP_INTERNAL_CLASSIFIERSTRATEGYFACTORY_H
#define __TERRALIB_RP_INTERNAL_CLASSIFIERSTRATEGYFACTORY_H

// TerraLib
#include "../common/AbstractFactory.h"
#include "ClassifierStrategy.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace rp
  {
    /*!
      \class ClassifierStrategyFactory

      \brief Raster classifier strategy factory base class.
    */
    class TERPEXPORT ClassifierStrategyFactory : public te::common::AbstractFactory<te::rp::ClassifierStrategy, std::string, std::less<std::string> >
    {
      public:

        virtual ~ClassifierStrategyFactory();

      protected:

        ClassifierStrategyFactory(const std::string& factoryKey);
    };

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_CLASSIFIERSTRATEGYFACTORY_H

