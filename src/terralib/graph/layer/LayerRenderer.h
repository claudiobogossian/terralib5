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
  \file LayerRenderer.h

  \brief It renders the objects associated to a Layer.
 */

#ifndef __TERRALIB_GRAPH_INTERNAL_LAYERRENDERER_H
#define __TERRALIB_GRAPH_INTERNAL_LAYERRENDERER_H

// TerraLib
#include "../Config.h"
#include "../maptools/AbstractRenderer.h"

namespace te
{
// Forward declarations
  namespace gm { Envelope; }

  namespace map 
  { 
    class AbstractLayer; 
    class Canvas;
  }

  namespace graph
  {    
    /*!
      \class LayerRenderer

      \brief It renders the objects associated to a Layer.
    
      \sa Layer, AbstractRenderer, Canvas
     */
    class TEGRAPHEXPORT LayerRenderer : public te::map::AbstractRenderer
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! Constructor */
        LayerRenderer();

        /*! \brief Destructor. */
        ~LayerRenderer();

        //@}      

        /** @name Implementation of Pure Virtual Methods from AbstractLayer
         *  Every subclass must implent this interface.
         */
        //@{

        /*!
          \brief It draws the layer geographic objects in the given canvas using the informed SRS.

          The informed bounding box (bbox) is used to constraint the Layer objects to be drawn.
          The bbox coordinates must be in the Spatial Reference System given by srid.

          \param layer  The layer to be rendered.
          \param canvas The canvas were the layer objects will be drawn.
          \param bbox   The interest area to render the map.
          \param srid   The SRS to be used to draw the layer objects.
         */
        virtual void draw(te::map::AbstractLayer* layer, te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        //@}

      protected:

        /*! \brief Verify if the graph layer has a geometry to define the vertexs elements. */
        int checkVertexGeometryProperty(te::graph::Layer* l);

        /*! \brief Verify if the graph layer has a geometry to define the edges elements. */
        int checkEdgeGeometryProperty(te::graph::Layer* l);

        /*! \brief Configs the canvas with default values for line styles. */
        void configDefaultLine(te::map::Canvas* canvas);
        
        /*! \brief Configs the canvas with default values for point styles. */
        void configDefaultPoint(te::map::Canvas* canvas);

        /*! \brief Configs the canvas with default values for point styles. */
        void configLoopPoint(te::map::Canvas* canvas);

      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        LayerRenderer(const LayerRenderer& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
         */
        LayerRenderer& operator=(const LayerRenderer& rhs);

        //@}
    };

  } // end namespace graph
}   // end namespace te

#endif  // __TERRALIB_GRAPH_INTERNAL_LAYERRENDERER_H

