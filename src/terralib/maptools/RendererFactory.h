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
  \file terralib/maptools/RendererFactory.h

  \brief An abstract factory for layer renderers.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_RENDERERFACTORY_H
#define __TERRALIB_MAPTOOLS_INTERNAL_RENDERERFACTORY_H

// TerraLib
#include "../common/AbstractFactory.h"
#include "AbstractRenderer.h"

namespace te
{
  namespace map
  {   

    /*!
      \class RendererFactory

      \brief This is the abstract factory for layer renderers.

      \ingroup map

      \sa AbstractRenderer, DataSetLayerRendererFactory, FeatureTypeStyleRendererFactory, CoverageStyleRendererFactory
    */
    class TEMAPEXPORT RendererFactory : public te::common::AbstractFactory<AbstractRenderer, std::string>
    {
      public:

        /*!
          \brief Virtual destructor.

          \note It will automatically unregister the factory from the dictionary.
        */
        virtual ~RendererFactory();

      protected:

        /*!
          \brief It creates the factory.

          \param factoryKey The key that identifies the factory.
        */
        RendererFactory(const std::string& factoryKey);
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_RENDERERFACTORY_H

