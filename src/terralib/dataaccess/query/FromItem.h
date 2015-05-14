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
  \file terralib/dataaccess/query/FromItem.h

  \brief An abstract class that models a source of data in a query.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_FROMITEM_H
#define __TERRALIB_DATAACCESS_INTERNAL_FROMITEM_H

// TerraLib
#include "../../common/BaseVisitable.h"
#include "QueryVisitor.h"

// STL
#include <string>

namespace te
{
  namespace da
  {
// Forward declarations
    class Expression;

    /*!
      \class FromItem
      
      \brief An abstract class that models a source of data in a query.

      \sa From, DataSetType, DataSetName, SubSelect, DataSet, FromFunctionCall, Join
    */
    class TEDATAACCESSEXPORT FromItem : public te::common::BaseVisitable<QueryVisitor>
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param alias An alias to the source data item.
        */
        FromItem(const std::string& alias) : m_alias(alias) {}

        /*! \brief Copy constructor. */
        FromItem(const FromItem& rhs);

        /*! \brief Virtual destructor. */
        virtual ~FromItem() {}

        /*! Assignment operator.  */
        FromItem& operator=(const FromItem& rhs);

        /*! \brief It creates a new copy of this FromItem. */
        virtual FromItem* clone() const = 0;

        /*!
          \brief It sets the alias for a from source item.

          \param alias The alias for a from source item.

          \note The FromItem will take the ownership of the given alias.
        */
        void setAlias(const std::string& alias);

        /*!
          \brief It returns the alias associated to the source item.

          \return The alias for the source item.
        */
        const std::string& getAlias() const;

      protected:

        std::string m_alias; //!< An alias for the output name.
    };

    /*! \brief For use with boost conteiners. */
    inline FromItem* new_clone(const FromItem& a)
    {
      return a.clone();
    }

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_FROMITEM_H

