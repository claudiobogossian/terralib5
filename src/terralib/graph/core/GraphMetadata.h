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
  \file GraphMetadata.h

  \brief Class used to define the graph metadata informations.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_GRAPHMETADATA_H
#define __TERRALIB_GRAPH_INTERNAL_GRAPHMETADATA_H

// Terralib Includes
#include "../Config.h"
#include "../Enums.h"

// STL Includes
#include <string>

namespace te
{
  // Forward declarations
  namespace da { class DataSource; }
  namespace dt { class Property; }
  namespace gm { class Envelope; }

  namespace graph
  {
    class VertexProperty;
    class EdgeProperty;

    /*!
      \class GraphMetadata

      \brief Class used to define the graph metadata informations.

      \sa Enums, AbstractGraph
    */

    class TEGRAPHEXPORT GraphMetadata
    {
      public:

        /*! \brief Default constructor. */
        GraphMetadata(te::da::DataSource* ds);

        /*! \brief Default destructor. */
        virtual ~GraphMetadata();

        /*!
          \brief Function used to load the graph information given a graph id

          \param id The Graph identifier

          \exception Exception It throws an exception if graph id equal -1
        */
        virtual void load(int id) { };

        /*!
          \brief Function used to save the graph information

        */
        virtual void save() { };

        /*!
          \brief Function used to update the graph information on a data source

          \exception Exception It throws an exception if graph id equal -1
        */
        virtual void update() { };

        /*!
          \brief It returns the graph id

          \return Integer value with the graph id
        */
        int getId();

        /*!
          \brief Set the graph name 

          \param name String with the graph name

        */
        void setName(std::string name);

        /*!
          \brief It returns the graph name

          \return String with the graph name
        */
        std::string getName();

        /*!
          \brief Set the graph description 

          \param desc String with the graph description

        */
        void setDescription(std::string desc);

        /*!
          \brief It returns the the graph description 

          \return String with the graph description
        */
        std::string getDescription();

        /*!
          \brief Set the graph type (defined in Enums file)

          \param value Enumerator value that defines the graph type

        */
        void setType(std::string graphType);

        /*!
          \brief It returns the graph type (defined in Enums file)

          \return Enumerator value that defines the graph type
        */
        std::string getType();

        /*!
          \brief Set the graph class (defined in Enums file)

          \param value Enumerator value that defines the graph class

        */
        void setClass(GraphClass value);

        /*!
          \brief It returns the the graph class (defined in Enums file)

          \return Enumerator value that defines the graph class
        */
        GraphClass getClass();

        /*!
          \brief Set the graph storage mode (defined in Enums file)

          \param value Enumerator value that defines the graph storage mode

        */
        void setStorageMode(GraphStorageMode value);

        /*!
          \brief It returns the the graph storage mode (defined in Enums file)

          \return  Enumerator value that defines the graph storage mode
        */
        GraphStorageMode getStorageMode();

        /*!
          \brief It returns the data source associated with this graph

          \return A pointer to a data source
        */
        te::da::DataSource* getDataSource();

        /*!
          \brief It returns the vertex table name that contains the vertex elements in data source

          \return String with the table name
        */
        std::string getVertexTableName();

        /*!
          \brief It returns the edge table name that contains the vertex elements in data source

          \return String with the table name
        */
        std::string getEdgeTableName();

         /*!
          \brief Add a new property associated to the vertex element

          param p  New property to be associated with vertex elements.

          \note It's important before using this function call the flush() function, its necessary
                to force the memory clear and the elements will be loaded with the right size of
                properties.
        */
        virtual void addVertexProperty(te::dt::Property* p);

        /*!
          \brief Remove a property associated to the vertex element

          \param idx Index of the property 
        */
        virtual void removeVertexProperty(int idx);

        /*!
          \brief Get a vertex property given a index

          \param idx Index of the property 

          \return A property associated to the vertex element if the index is right and a null pointer in other case.

        */
        virtual te::dt::Property* getVertexProperty(int idx);

        /*!
          \brief Used to verify the number of properties associated to vertex elements

          \return  Integer value with the number of properties.

        */
        virtual int getVertexPropertySize();

         /*!
          \brief Add a new property associated to the edge element

          param p  New property to be associated with edge elements.

          \note It's important before using this function call the flush() function, its necessary
                to force the memory clear and the elements will be loaded with the right size of
                properties.
        */
        virtual void addEdgeProperty(te::dt::Property* p);

        /*!
          \brief Remove a property associated to the edge element

          \param idx Index of the property 
        */
        virtual void removeEdgeProperty(int idx);

        /*!
          \brief Get a edge property given a index

          \param idx Index of the property 

          \return A property associated to the edge element if the index is right and a null pointer in other case.

        */
        virtual te::dt::Property* getEdgeProperty(int idx);

        /*!
          \brief Used to verify the number of properties associated to edge elements

          \return  Integer value with the number of properties.

        */
        virtual int getEdgePropertySize();

        /*!
          \brief Used to get the SRID of the geometry elements associated with this graph

          \return Integer value that defines the SRID
        */
        virtual int getSRID();

        /*!
          \brief Used to set the SRID of the geometry elements associated with this graph

          \param Integer value that defines the SRID
        */
        virtual void setSRID(int srid);

        /*!
          \brief Used to get the bounding box of the geometry elements associated with this graph

          \return The bounding box information
        */
        virtual te::gm::Envelope* getEnvelope();

        /*!
          \brief Used to set the bounding box of the geometry elements associated with this graph

          \param The bounding box information
        */
        virtual void setEnvelope(te::gm::Envelope& extent);


      protected:

        int m_id;                                 //!< Attribute graph unique identifier.

        std::string m_name;                       //!< Attribute name.

        std::string m_description;                //!< Attribute used to describe a graph.

        int m_srid;                               //!< Attribute used to define the graph projection.

        te::gm::Envelope* m_extent;               //!< Attribute used to define the graph extent.

        std::string  m_type;                      //!< Enum attribute used to defines the graph type.

        te::graph::GraphClass m_class;            //!< Enum attribute used to defines the graph class.

        te::graph::GraphStorageMode m_mode;       //!< Enum attribute used to defines the storage mode.

        te::da::DataSource* m_ds;                 //!< Data source attribute, has to be database information

        VertexProperty* m_vertexProp;             //!< Attribute that defines the vertex properties.

        EdgeProperty*   m_edgeProp;                //!< Attribute that defines the edge properties.


      public:

        size_t m_maxCacheSize;          //!< Attribute used to set the max cache size
        size_t m_maxVecCacheSize;       //!< Attribute used to set the max vector cache size
        double m_boxPercentSize;        //!< Attribute used to box percent size used in loader strategy

        bool m_memoryGraph;             //!< Flag used to indicate if the graph is a memory graph
    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_GRAPHMETADATA_H