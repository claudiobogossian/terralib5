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
          \brief It returns the layer schema.

          \return The Layer schema.

          \note The caller will take the ownership of the returned layer schema.
        */
        virtual std::auto_ptr<te::da::DataSetType> getSchema() const;

        /*!
          \brief It gets the dataset identified by the layer name.

          \param travType The traverse type associated to the returned dataset. 
          \param accessPolicy Access policy.

          \return The caller of this method will take the ownership of the returned dataset.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during the data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::DataSet> getData(te::common::TraverseType travType = te::common::FORWARDONLY,
                                                       const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        /*!
          \brief It gets the dataset identified by the layer name using a spatial filter over the specified property.

          \param propertyName  The name of the spatial property used to apply the spatial filter.
          \param e             A rectangle to be used as a spatial filter when retrieving datasets.
          \param r             The spatial relation to be used during the filter.
          \param travType      The traverse type associated to the returned dataset. 
          \param accessPolicy Access policy.

          \return The caller of this method will take the ownership of the returned DataSet.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Transactor will not take the ownership of the given envelope.

          \note The envelope coordinates should be in the same coordinate system as the dataset.

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::DataSet> getData(const std::string& propertyName,
                                                       const te::gm::Envelope* e,
                                                       te::gm::SpatialRelation r = te::gm::INTERSECTS,
                                                       te::common::TraverseType travType = te::common::FORWARDONLY,
                                                       const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        /*!
          \brief It gets the dataset identified by the layer name using a spatial filter over the given geometric property.

          \param propertyName  The name of the spatial property used to apply the spatial filter.
          \param g             A geometry to be used as a spatial filter when retrieving datasets.
          \param r             The spatial relation to be used during the filter.
          \param travType      The traverse type associated to the returned dataset. 
          \param accessPolicy Access policy.

          \return The caller of this method will take the ownership of the returned DataSet.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Transactor will not take the ownership of the given geometry.

          \note The geometry coordinates should be in the same coordinate system as the dataset.
          
          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::DataSet> getData(const std::string& propertyName,
                                                       const te::gm::Geometry* g,
                                                       te::gm::SpatialRelation r,
                                                       te::common::TraverseType travType = te::common::FORWARDONLY,
                                                       const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        /*!
          \brief It gets the dataset identified by the layer name using the given restriction.

          \param restriction The restriction expression that will be used.
          \param travType    The traverse type associated to the returned dataset. 
          \param accessPolicy Access policy.

          \return The caller of this method will take the ownership of the returned DataSet.

          \exception Exception It can throws an exception if:
                      <ul>
                      <li>something goes wrong during data retrieval</li>
                      <li>if the data source driver doesn't support the traversal type</li>
                      <li>if the data source driver doesn't support the access policy</li>
                      </ul>

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::DataSet> getData(te::da::Expression* restriction,
                                                       te::common::TraverseType travType = te::common::FORWARDONLY,
                                                       const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        /*!
          \brief It gets the dataset from the given set of objects identification.

          \param oids     The set of object ids.
          \param travType The traverse type associated to the returned dataset.
          \param accessPolicy Access policy.

          \return The caller of this method will take the ownership of the returned dataset.

          \exception Exception It can throws an exception if:
                     <ul>
                     <li>something goes wrong during the data retrieval</li>
                     <li>if the data source driver doesn't support the traversal type</li>
                     <li>if the data source driver doesn't support the access policy</li>
                     </ul>

          \note Not thread-safe!
        */
        virtual std::auto_ptr<te::da::DataSet> getData(const te::da::ObjectIdSet* oids,
                                                       te::common::TraverseType travType = te::common::FORWARDONLY,
                                                       const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

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

