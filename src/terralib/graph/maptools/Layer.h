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
  \file Layer.h

  \brief A Layer is a reference to a virtual dataset with/without geometric attributes.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_LAYER_H
#define __TERRALIB_GRAPH_INTERNAL_LAYER_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../Config.h"


// STL
#include <memory>

namespace te
{
  namespace gm { class Envelope; }

  namespace map 
  { 
    class AbstractLayer; 
    class Canvas;
  }

  namespace se { class Style; }

  namespace graph
  {
// Forward declarations
    class AbstractGraph;
    class LayerRenderer;

    /*!
      \class Layer

      \brief A Layer is a reference to a virtual dataset with/without geometric attributes.

      A Layer is the result of a query in a given DataSource.
    
      \sa AbstractLayer
    */
    class TEGRAPHEXPORT Layer : public te::map::AbstractLayer
    {
      public:

        /*!
          \brief It initializes a new Layer.

          \param id     The layer id.
          \param title  The title is a brief description about the layer, that can be used by an application to show a meaninfull label to the user.
          \param parent The parent Layer (or NULL if it has no parent).

          \note If parent is informed, it will take the ownership of the new layer.
        */
        Layer(const std::string& id, const std::string& title, AbstractLayer* parent = 0);

        /*! \brief Destructor. */
        ~Layer();

        /*!
          \brief It returns the layer type.

          \return The layer type.
        */
        virtual const std::string& getType() const;

        /*!
          \brief It returns true if the layer can be drawn, otherwise, it returns false.

          This method can be used to check if the data referenced by the layer is available (accessible), or not.

          \return True, if the layer can be drawn, otherwise, it returns false.
        */
        virtual bool isValid() const;

        /*!
          \brief It draws the layer geographic objects in the given canvas using the informed SRS.

          The informed bounding box can be used to constraint the layer objects to be drawn.

          The bbox coordinates must be in the same Spatial Reference System given by srid.

          \param canvas The canvas were the layer objects will be drawn.
          \param bbox   The interest area to render the map.
          \param srid   The SRS to be used to draw the layer objects.
        */
        virtual void draw(te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid) ;

        /*!
          \brief It returns the graph associated to the layer.

          \return the graph associated to the layer.
        */
        te::graph::AbstractGraph* getGraph() const;

        /*!
          \brief It sets the graph associated to the layer.

          \param ds The graph associated to the layer.
        */
        void setGraph(te::graph::AbstractGraph* ds);

        /*!
          \brief It returns the renderer used to paint this layer.

          \return The renderer used to paint this layer.
        */
        LayerRenderer* getRenderer() const;

        /*!
          \brief It sets the renderer used to paint this layer.

          \param renderer The renderer to be used to paint this layer.

          \note The Layer will take the ownership of the given renderer.
        */
        void setRenderer(LayerRenderer* renderer);

      public:
         
        static const std::string sm_type;  //!< A static data member used in the implementation of getType method.

      private:

//        int m_srid;                                 //!< The layer SRS.
        std::auto_ptr<te::gm::Envelope> m_mbr;      //!< The Layer bounding box.
        std::auto_ptr<te::se::Style> m_style;       //!< The style to be applied to the geographic objects in the layer.
        std::auto_ptr<LayerRenderer> m_renderer;    //!< A pointer to the internal renderer used to paint this layer.
        te::graph::AbstractGraph* m_graph;          //!< The graph associated to this Layer.
    };

  } // end namespace graph
}   // end namespace te

#endif  // __TERRALIB_GRAPH_INTERNAL_LAYER_H

