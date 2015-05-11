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
  \file AbstractRenderer.h

  \brief This is the base class for renderers.
 */

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTRENDERER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTRENDERER_H

// TerraLib
#include "Config.h"

namespace te
{
// Forward declarations
  namespace gm { class Envelope; }
  namespace se { class Style; }

  namespace map
  {
// Forward declarations
   class AbstractLayer;
   class Canvas;

    /*!
      \class AbstractRenderer

      \brief It renders the objects associated to a layer.

      The renderer will be responsible for:
      <ul>
      <li>locate/retrieve the objects associated to the layer</li>
      <li>set the canvas with the appropriated style rules to draw the objects</li>
      <li>make use of a canvas to draw the objects</li>
      </ul>

      \ingroup map

      \sa AbstractLayer, Canvas, RendererFactory, FeatureTypeStyleRenderer,
          CoverageStyleRenderer, DataSetLayerRenderer, QueryLayerRenderer
     */
    class TEMAPEXPORT AbstractRenderer
    {
      public:
    
        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{
    
        /*! \brief Default Constructor. */
        AbstractRenderer(){};

        /*! \brief Virtual Destructor. */
        virtual ~AbstractRenderer(){};

        //@}

        /** @name Pure Virtual Methods
         *  Methods that subclasses must implement.
         */
        //@{
    
        /*!
          \brief It draws the layer geographic objects in the given canvas using the SRS informed.

          The informed bounding box (bbox) is used to constraint the Layer objects to be drawn.
          The bbox coordinates must be in the Spatial Reference System given by srid.

          \param layer  The layer that will be drawn.
          \param canvas The canvas were the layer objects will be drawn.
          \param bbox   The interest area to render the map.
          \param srid   The SRS to be used to draw the layer objects.
         */
        virtual void draw(AbstractLayer* layer, Canvas* canvas, const te::gm::Envelope& bbox, int srid) = 0;

        //@}
    
      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The renderer that would be copied.
         */
        AbstractRenderer(const AbstractRenderer& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The renderer whose contents would be assigned to this renderer.

          \return A reference to this renderer.
         */
        AbstractRenderer& operator=(const AbstractRenderer& rhs);

        //@}
    
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTRENDERER_H

