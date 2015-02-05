/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/AbstractLayerRendererFactory.h

  \brief This is the concrete factory for renderers of AbstractLayers. i.e. builds a generic renderer.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYERRENDERERFACTORY_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYERRENDERERFACTORY_H

// TerraLib
#include "RendererFactory.h"

namespace te
{
  namespace map
  {
    /*!
      \class AbstractLayerRendererFactory

      \brief This is the concrete factory for renderers of a AbstractLayer. i.e. builds a generic renderer.

      \ingroup map

      \sa AbstractRenderer, AbstractLayerRenderer
    */
    class AbstractLayerRendererFactory : public RendererFactory
    {
      public:

        static void initialize();

        static void finalize();

        ~AbstractLayerRendererFactory();

      protected:

        AbstractRenderer* build();

        AbstractLayerRendererFactory();

      private:

        static AbstractLayerRendererFactory* sm_factory; //!< A pointer to the global renderer factory.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTLAYERRENDERERFACTORY_H

