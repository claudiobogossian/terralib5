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
  \file DataSourceGraphMetadata.h

  \brief Class used to define the graph metadata informations
         over a SGBD source
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_DATASOURCEGRAPHMETADATA_H
#define __TERRALIB_GRAPH_INTERNAL_DATASOURCEGRAPHMETADATA_H

// Terralib Includes
#include "../../core/GraphMetadata.h"
#include "../../Config.h"


// STL Includes
#include <string>

namespace te
{
  // Forward declarations
  namespace da 
  { 
    class DataSource; 
    class DataSetType;
  }

  namespace gm { class Envelope; }

  namespace graph
  {
    class GraphMetadata;
  
    /*!
      \class DataSourceGraphMetadata

      \brief Class used to define the graph metadata informations
         over a SGBD source

      \sa GraphMetadata, Enums
    */

    class TEGRAPHEXPORT DataSourceGraphMetadata : public GraphMetadata
    {
      public:

        /*! \brief Default constructor. */
        DataSourceGraphMetadata(te::da::DataSource* ds);

        /*! \brief Default destructor. */
        ~DataSourceGraphMetadata();

        /*!
          \brief Function used to load the graph information given a graph id

          \param id The Graph identifier

          \exception Exception It throws an exception if graph id equal -1
        */
        void load(int id);

        /*!
          \brief Function used to save the graph information

        */
        void save();

        /*!
          \brief Function used to update the graph information on a data source

          \exception Exception It throws an exception if graph id equal -1
        */
        void update();

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


      protected:

        /*!
          \brief Function used to create the graph metadata tables in a SGBD

          \exception Exception It throws an exception if execution fails
        */
        void createGraphMetadataTable();


        /*!
          \brief Function used to create the graph metadata attributes tables in a SGBD

          \exception Exception It throws an exception if execution fails
        */
        void createGraphAttributesTable();

        /*!
          \brief Function used to create the vertex model scheme of tables

          \note NOT IN USE

          \exception Exception It throws an exception if execution fails
        */
        void createGraphTableVertexModel();

        /*!
          \brief Function used to create the edge model scheme of tables

          \exception Exception It throws an exception if execution fails
        */
        void createGraphTableEdgeModel();

        /*!
          \brief Used when edge_model is selected and vertex objects has
              attributes associated.

          \exception Exception It throws an exception if execution fails
        */
        void createVertexAttrTable();

        /*!
          \brief Used when vertex_model is selected and edge objects has
              attributes associated.

          \exception Exception It throws an exception if execution fails
        */
        void createEdgeAttrTable();

        /*!
          \brief Function used to create a table inside the SGBD

          \param tableName  String with the table name

          \param dt Struct that defines each column of the table

          \exception Exception It throws an exception if execution fails

        */
        void createTable(std::string tableName, te::da::DataSetType* dt);

        /*!
          \brief Verify if a graph name is valid
        */
        bool isValidGraphName(std::string graphName);

        /*!
          \brief Add a new graph metadata information into the SGBD

          \exception Exception It throws an exception if execution fails
        */
        void addGraphTableNewEntry();

        /*!
          \brief Add a new graph attributes metadata information into the SGBD

          \note Call the function saveGraphAttrTableNewEntry.

          \exception Exception It throws an exception if execution fails
        */
        void addGraphAttrTableNewEntry();

        /*!
          \brief Save each metadata attributes of vertex and eges into SGBD

          \param graphId      The graph id associated with this attribute

          \param tableName    The table name that contains this attribute

          \param attrName     The attribute name

          \param linkColumn   The link column name used to associate this attribute to the element (vertex or edge)

          \param type         The attribute type ( is vertex or edge attribute)

          \exception Exception It throws an exception if execution fails
        */
        void saveGraphAttrTableNewEntry(int graphId, std::string tableName, std::string attrName, std::string linkColumn, te::graph::GraphAttrType type);

        /*!
          \brief Save a property into a table in SGBD

          \param tableName    The table name that will contains this property

          \param p            The property to be saved

          \exception Exception It throws an exception if execution fails
        */
        void saveProperty(std::string tableName, te::dt::Property* p);

        /*!
          \brief Remove a property of a table in SGBD

          \param tableName    The table name that contains this property

          \param propertyName The property name to be removed

          \exception Exception It throws an exception if execution fails
        */
        void removeProperty(std::string tableName, std::string propertyName);

        /*!
          \brief Function used to load the graph information given a graph id

          \param id    The graph id

        */
        void loadGraphInfo(int id);

        /*!
          \brief Function used to load the graph attributes information given a graph id

          \param id    The graph id

        */
        void loadGraphAttrInfo(int id);

        /*!
          \brief Load the vertex attributes

          \param tableName    The table name that contains the vertex attributes

          \param columnName   The column name that contins the attribute information

        */
        void loadVertexAttr(std::string tableName, std::string columnName);

        /*!
          \brief Load the edge attributes

          \param tableName    The table name that contains the edge attributes

          \param columnName   The column name that contins the attribute information

        */
        void loadEdgeAttr(std::string tableName, std::string columnName);

        /*!
          \brief 
        */
        void updateGraphId();
    };
  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_DATASOURCEGRAPHMETADATA_H