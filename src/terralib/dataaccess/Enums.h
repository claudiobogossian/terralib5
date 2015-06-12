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
  \file terralib/dataaccess/Enums.h

  \brief General enumerations for the Data Access module.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ENUMS_H
#define __TERRALIB_DATAACCESS_INTERNAL_ENUMS_H

namespace te
{
  namespace da
  {
    /*!
      \enum SortOrder

      \brief Sort order type: asc or desc.
    */
    enum SortOrder
    {
      DESC, /*!< Descendent order type. */
      ASC   /*!< Ascendent order type.  */
    };

    /*!
      \enum JoinType

      \brief The type of join in a query.
    */
    enum JoinType
    {
      JOIN,             /*!< ... */
      INNER_JOIN,       /*!< ... */
      LEFT_JOIN,        /*!< left-outer-join */
      RIGHT_JOIN,       /*!< right-outer-jin... */
      FULL_OUTER_JOIN,  /*!< full-outer-jon... */
      CROSS_JOIN,       /*!< ... */
      NATURAL_JOIN      /*!< ... */
    };

    /*!
      \enum ConstraintType
          
      \brief A ConstraintType can have one of the following types:
    */
    enum ConstraintType
    {
      UNKNOWN,         /*!< Used when the constraint type is unknown.  */
      PRIMARY_KEY,     /*!< Primary key constraint.                    */
      FOREIGN_KEY,     /*!< Foreign key constraint.                    */
      UNIQUE_KEY,      /*!< Unique key constraint.                     */
      CHECK            /*!< Check constraint.                          */
    };

    /*! \brief An anonymous enum with the type of datasets retrieved from data sources. */
    enum
    {
      UNKNOWN_DATASET_TYPE,
      TABLE_TYPE,
      SYSTEM_TABLE_TYPE,
      VIEW_TYPE,
      QUERY_TYPE,
      INDEX_TYPE,
      SEQUENCE_TYPE,
      TRIGGER_TYPE,
      REGULAR_FILE_TYPE
    };

    /*!
      \enum FKActionType
          
      \brief Type of action performed on the foreign key data.
    */
    enum FKActionType
    {
      NO_ACTION,      /*!< Indicates that the deletion or update would create a foreign key constraint violation. */
      RESTRICT,       /*!< Indicates that the deletion or update would create a foreign key constraint violation. */
      CASCADE,        /*!< Delete any rows referencing the deleted row, or update the value of the referencing column to the new value of the referenced column, respectively. */
      SET_NULL,       /*!< Set the referencing column(s) to null. */
      SET_DEFAULT     /*!< Set the referencing column(s) to their default values. */
    };

    /*!
      \enum IndexType

      \brief Index type.
    */
    enum IndexType
    {
      B_TREE_TYPE,        /*!< Btree index type.    */
      R_TREE_TYPE,        /*!< RTree index type.    */
      QUAD_TREE_TYPE,     /*!< QuadTree index type. */
      HASH_TYPE           /*!< Hash index type.     */
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ENUMS_H

