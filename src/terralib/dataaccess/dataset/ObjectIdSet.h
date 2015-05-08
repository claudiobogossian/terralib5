/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/dataset/ObjectIdSet.h

  \brief This class represents a set of unique ids created in the same context. i.e. from the same data set.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_OBJECTIDSET_H
#define __TERRALIB_DATAACCESS_INTERNAL_OBJECTIDSET_H

// TerraLib
#include "../../common/Comparators.h"
#include "../Config.h"

// STL
#include <set>
#include <vector>
#include <string>

namespace te
{
  namespace gm { class Envelope; }

  namespace da
  {
// Forward declaration
    class ObjectId;
    class Expression;

    /*!
      \class ObjectIdSet

      \brief This class represents a set of unique ids created in the same context. i.e. from the same data set.

      \sa DataSet, DataSetType, ObjectId
    */
    class TEDATAACCESSEXPORT ObjectIdSet
    {
      public:

        /*! \brief Constructor. */
        ObjectIdSet();

        /*! \brief Copy Constructor. */
        ObjectIdSet(const ObjectIdSet& rhs, bool copyOids = true);

        /*! \brief Destructor. */
        ~ObjectIdSet();

        /*!
          \brief It adds a property that will be used to generate the unique ids.

          \param name A property name that will be used to generate the unique ids.
          \param name The property position.
          \param type The property data type.
        */
        void addProperty(const std::string& name, std::size_t pos, int type);

        /*!
          \brief It adds an object id to this object id set.

          \param The object id that will be added.

          \note This class will take the ownership of the given pointer.
        */
        void add(ObjectId* oid);


        /*!
          \brief It set the expression that can be used to retrieve the data set that contains the all indentified elements.

          \param expression The expression that can be used to retrieve the data set that contains the all indentified elements.

          \note This method will take the ownership of the given pointer.
        */
        void setExpression(te::da::Expression* expression, bool isClauseIn);

        void setExpressionByIntersection(std::string geomAttrName, te::gm::Envelope env, int srid);

        void setExpressionByInClause(const std::string source = "");

        /*!
          \brief It returns the expression that can be used to retrieve the data set that contains the all indentified elements.

          \return The expression that can be used to retrieve the data set that contains the all indentified elements.

          \note The caller will take the ownership of the given pointer.
        */
        Expression* getExpression() const;

        Expression* getExpressionByInClause(const std::string source = "") const;

        /*!
          \brief It clears this object id set.
        */
        void clear();

        /*!
          \brief It returns the object id set size.

          \return The object id set size.
        */
        std::size_t size() const;

        /*!
          \brief It returns the property names used to generated the oids.

          \return The property names used to generated the oids.
        */
        const std::vector<std::string>& getPropertyNames() const;

        /*!
          \brief It returns the property positions used to generated the oids.

          \return The property positions used to generated the oids.
        */
        const std::vector<std::size_t>& getPropertyPos() const;

        /*!
          \brief It returns the property types used to generated the oids.

          \return The property types used to generated the oids.
        */
        const std::vector<int>& getPropertyTypes() const;

        /*!
          \brief It returns if the object id set contains the given oid.

          \param oid The object id that will be searched.

          \return True if the object id set contains the given oid. False, otherwise.

          \note The ObjectIdSet will NOT take the ownership of the given pointer.
        */
        bool contains(ObjectId* oid) const;

        /*!
          \brief Removes the object id from set.

          \param oid Object id to be removed.

          \note This WILL NOT take the ownership of \a oid.
        */
        void remove(ObjectId* oid);

        /*!
          \brief It performs the union operation between this ObjectIdSet and the given ObjectIdSet.

          \param rhs A valid ObjectIdSet that will be used on union operation. Do not pass NULL!

          \note The ObjectIdSet will take the ownership of the given pointer.

          \note The given pointer will be invalidated at end of union operation.
        */
        void Union(ObjectIdSet* rhs);

        /*!
          \brief It performs the difference operation between this ObjectIdSet and the given ObjectIdSet.

          \param rhs A valid ObjectIdSet that will be used on difference operation. Do not pass NULL!

          \note The ObjectIdSet will NOT take the ownership of the given pointer.
        */
        void difference(const ObjectIdSet* rhs);

        /*!
          \brief It performs the symmetric difference operation between this ObjectIdSet and the given ObjectIdSet.

          \param rhs A valid ObjectIdSet that will be used on difference operation. Do not pass NULL!

          \note The ObjectIdSet will NOT take the ownership of the given pointer.
        */
        void symDifference(const ObjectIdSet* rhs);

        /*!
          \brief Returns an iterator for the object ids in container
          
          \return Iterator for the begin element of the container.

          \note When using the iterators it WILL NOT give the ownership of the pointers. 
          This means that the caller MUST NOT delete the pointers or it will lead to an inconsistent state.
        */
        std::set<ObjectId*, te::common::LessCmp<ObjectId*> >::const_iterator begin() const;

        /*!
          \brief Returns an iterator for the object ids in container
          
          \return Iterator for the ending element of the container.
        */
        std::set<ObjectId*, te::common::LessCmp<ObjectId*> >::const_iterator end() const;

        ObjectIdSet* clone() const;

        bool isExpressionClauseIn() const;

      private:

        std::vector<std::string> m_pnames;                            //!< The list of property names used to generate the unique ids.
        std::vector<std::size_t> m_ppos;                              //!< The list of property positions used to generate the unique ids.
        std::vector<int> m_ptypes;                                    //!< The list of property types used to generate the unique ids.
        std::set<ObjectId*, te::common::LessCmp<ObjectId*> > m_oids;  //!< The set of unique ids.
        te::da::Expression* m_expression;                             //!< The expression that can be used to retrieve the data set that contains the all indentified elements.

        bool m_expByClauseIn;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_OBJECTIDSET_H
