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
  \file terralib/dataaccess/query/Select.h

  \brief A Select models a query to be used when retrieving data from a data source.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_SELECT_H
#define __TERRALIB_DATAACCESS_INTERNAL_SELECT_H

// TerraLib
#include "../Enums.h"
#include "Distinct.h"
#include "Fields.h"
#include "From.h"
#include "GroupBy.h"
#include "OrderBy.h"
#include "Query.h"

// STL
#include <memory>

namespace te
{
  namespace da
  {
// Forward declaration
    class DataSetName;
    class Field;
    class GroupByItem;
    class Having;
    class OrderByItem;
    class PropertyName;
    class Where;

    /*!
      \class Select
      
      \brief A Select models a query to be used when retrieving data from a DataSource.

      \ingroup dataaccess

      \sa Query, Fields, From, Expression, GroupBy, Expression, OrderBy, Distinct

      \todo Ainda falta incluir a possibilidade de construir consultas com operacoes de INTERSECT/UNION/EXCEPT e etc...
    */
    class TEDATAACCESSEXPORT Select : public Query
    {
      public:

        TE_DEFINE_VISITABLE

        /*! \brief Default constructor. */
        Select();

        Select(Fields* fds, From* f = 0, Where* w = 0, OrderBy* o = 0);

        Select(const Fields& fds);

        Select(const Fields& fds, const From& f);

        Select(const Fields& fds, const From& f, const Where& w);

        Select(const Fields& fds, const From& f, const Where& w, const OrderBy& o);

        Select(Fields* fds, From* f, OrderBy* o);

        Select(const Fields& fds, const From& f, const OrderBy& o);

        Select(Fields* fds, From* f, Where* w, GroupBy* gb, OrderBy* o = 0);

        Select(const Fields& fds, const From& f, const Where& w, const GroupBy& gb);

        Select(const Fields& fds, const From& f, const Where& w, const GroupBy& gb, const OrderBy& o);

        Select(Fields* fds, From* f, GroupBy* gb, OrderBy* o = 0);

        Select(const Fields& fds, const From& f, const GroupBy& gb);

        Select(const Fields& fds, const From& f, const GroupBy& gb, const OrderBy& o);

        Select(Fields* fds, From* f, Where* w, GroupBy* gb, Having* h, OrderBy* o = 0);

        Select(const Fields& fds, const From& f, const Where& w, const GroupBy& gb, const Having& h);

        Select(const Fields& fds, const From& f, const Where& w, const GroupBy& gb, const Having& h, const OrderBy& o);

        Select(Field* f);

        Select(const Field& f);

        Select(const std::string& propertyName);

        Select(const std::string& propertyName, const std::string& alias);

        Select(const Select& rhs);

        Select(const Select* rhs);

        /*! \brief Destructor. */
        ~Select();

        /*! Assignment operator.  */
        Select& operator=(const Select& rhs);

        /*! \brief It creates a new copy of this query. */
        Query* clone() const;

// fields
        Select& fields(const Fields& f);

        Select& fields(Fields* f);

        Fields& fields();

        const Fields& fields() const;

        Select& operator()(const Field& f);

        Select& operator()(Field* f);

        Select& operator()(const std::string& propertyName);

        Select& operator()(const std::string& propertyName, const std::string& alias);

// from / join
        Select& from(const FromItem& item);

        Select& from(FromItem* item);

        Select& from(const FromItem& i1, const FromItem& i2);

        Select& from(FromItem* i1, FromItem* i2);

        Select& from(const FromItem& i1, const FromItem& i2, const FromItem& i3);

        Select& from(FromItem* i1, FromItem* i2, FromItem* i3);

        Select& from(const std::string& datasetName);        

        Select& from(From* f);

        Select& from(const From& rhs);
        
        From& from();

        const From& from() const;

        Select& Join(FromItem* d2, JoinType t, JoinCondition* c);

        Select& Join(const FromItem& d2, JoinType t, const JoinCondition& c);

        Select& InnerJoin(FromItem* d2, JoinCondition* c);

        Select& InnerJoin(const FromItem& d2, const JoinCondition& c);

        Select& LeftJoin(FromItem* d2, JoinCondition* c);

        Select& LeftJoin(const FromItem& d2, const JoinCondition& c);

        Select& RightJoin(FromItem* d2, JoinCondition* c);

        Select& RightJoin(const FromItem& d2, const JoinCondition& c);

        Select& FullOuterJoin(FromItem* d2, JoinCondition* c);

        Select& FullOuterJoin(const FromItem& d2, const JoinCondition& c);

        Select& CrossJoin(FromItem* d2);

        Select& CrossJoin(const FromItem& d2);

        Select& NaturalJoin(FromItem* d2, JoinType t);

        Select& NaturalJoin(const FromItem& d2, JoinType t);

// where
        Select& where(Expression* e);

        Select& where(const Expression& e);

        Select& where(Where* w);

        Select& where(const Where& rhs);

        Where& where();

        const Where& where() const;

// group by
        Select& groupBy(const GroupByItem& item);

        Select& groupBy(GroupByItem* item);

        Select& groupBy(const Expression& e);

        Select& groupBy(const std::string& propertyName);

        Select& groupBy(GroupBy* gb);

        Select& groupBy(const GroupBy& rhs);        

        GroupBy& groupBy();

        const GroupBy& groupBy() const;

// having

        Select& having(Expression* e);

        Select& having(const Expression& e);

        Select& having(Having* h);

        Select& having(const Having& rhs);

        Having& having();

        const Having& having() const;

// order by
        Select& orderBy(const OrderByItem& item);

        Select& orderBy(OrderByItem* item);

        Select& orderBy(const Expression& e, SortOrder o = ASC);

        Select& orderBy(const std::string& propertyName, SortOrder o = ASC);

        Select& orderBy(OrderBy* o);

        Select& orderBy(const OrderBy& rhs);

        OrderBy& orderBy();

        const OrderBy& orderBy() const;

// distinct
        Select& distinct(Expression* e);

        Select& distinct(const Expression& e);

        Select& distinct(const std::string& propertyName);

        Select& distinct(Distinct* d);

        Select& distinct(const Distinct& rhs);

        Distinct& distinct();

        const Distinct& distinct() const;

// limit and offset
        Select& limit(std::size_t l);

        Select& offset(std::size_t i);

        /*!
          \brief It sets the list of output expressions used to form the result set.

          \param f The list of output expressions used to form the result set.

          \note Select will take the ownership of Fields.
        */
        void setFields(Fields* f);

        /*!
          \brief It returns the list of output expressions used to form the result set.

          \return The list of output expressions used to form the result set.
        */
        const Fields* getFields() const;

        /*!
          \brief It sets the list of source information.

          \param f The list of source information.

          \note Select will take the ownership of From.
        */
        void setFrom(From* f);

        /*!
          \brief It returns the list of source information to be used by the query.

          \return The list of source information to be used by the query.
        */
        const From* getFrom() const;

        /*!
          \brief It sets the filter codition.

          \param w The filter condition.

          \note Select will take the ownership of Where.
        */
        void setWhere(Where* w);

        /*!
          \brief It returns the filter condition.

          \return The filter condition.
        */
        Where* getWhere() const;

        /*!
          \brief It sets the list of expressions used to condense the result set.

          \param g The list of expressions used to condense the result set.

          \note Select will take the ownership of GroupBy.
        */
        void setGroupBy(GroupBy* g);

        /*!
          \brief It returns the list of expressions used to condense the result set.

          \return The list of expressions used to condense the result set.
        */
        const GroupBy* getGroupBy() const;

        /*!
          \brief It sets the list of expressions used to eliminate group row that doesn't satisfy the condition.

          \param h The list of expressions used to eliminate group row that doesn't satisfy the condition.

          \note Select will take the ownership of Having.
        */
        void setHaving(Having* h);

        /*!
          \brief It returns the list of expressions used to eliminate group row that doesn't satisfy the condition.

          \return The list of expressions used to eliminate group row that doesn't satisfy the condition.
        */
        const Having* getHaving() const;

        /*!
          \brief It sets the list of expressions used to sort the output result.

          \param o The list of expressions used to sort the output result.

          \note Select will take the ownership of OrderBy.
        */
        void setOrderBy(OrderBy* o);

        /*!
          \brief It returns the list of expressions used to sort the output result.

          \return The list of expressions used to sort the output result.
        */
        const OrderBy* getOrderBy() const;

        /*!
          \brief If Distinct is specified, all duplicate rows are removed from the result set (one row is kept from each group of duplicates).

          \param d The Distinct modifier.

          \note Select will take the ownership of Distinct.
        */
        void setDistinct(Distinct* d);

        /*!
          \brief It returns the Distinct modifier.

          \return The Distinct modifier.
        */
        const Distinct* getDistinct() const;

        /*!
          \brief It specifies the maximum number of rows to return.

          \param m The maximum number of rows to return.
        */
        void setLimit(std::size_t m);

        /*!
          \brief It tells the maximum number of rows to return.

          \return The maximum number of rows to return.
        */
        std::size_t getLimit() const;

        /*!
          \brief It specifies the number of rows to skip before starting to return rows.

          \param o The number of rows to skip before starting to return rows.
        */
        void setOffset(std::size_t o);

        /*!
          \brief It tells the number of rows to skip before starting to return rows.

          \return The number of rows to skip before starting to return rows.
        */
        std::size_t getOffset() const;

        Select& operator+(const te::da::From& f);

        Select& operator+(te::da::From* f);

        Select& operator+(const te::da::Where& w) ;

        Select& operator+(te::da::Where* w) ;

        Select& operator+(const te::da::GroupBy& g);

        Select& operator+(te::da::GroupBy* g);

        Select& operator+(const te::da::Having& h);

        Select& operator+(te::da::Having* h);

        Select& operator+(const te::da::OrderBy& o);

        Select& operator+(te::da::OrderBy* o);

      private:

        std::auto_ptr<Fields> m_fields;       //!< .
        std::auto_ptr<From> m_from;           //!< .
        std::auto_ptr<Where> m_where;         //!< .
        std::auto_ptr<GroupBy> m_groupBy;     //!< .
        std::auto_ptr<Having> m_having;       //!< .
        std::auto_ptr<OrderBy> m_orderBy;     //!< .
        std::auto_ptr<Distinct> m_distinct;   //!< .
        std::size_t m_limit;                  //!< .
        std::size_t m_offset;                 //!< .
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_SELECT_H

