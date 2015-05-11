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
  \file terralib/dataaccess/query/JoinCondition.h

  \brief A condition to be used in a Join clause.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_JOINCONDITION_H
#define __TERRALIB_DATAACCESS_INTERNAL_JOINCONDITION_H

// TerraLib
#include "../../common/BaseVisitable.h"
#include "QueryVisitor.h"

namespace te
{
  namespace da
  {
    /*!
      \class JoinCondition
      
      \brief A condition to be used in a Join clause.

      \sa Join, JoinConditionNatural, JoinConditionOn, JoinConditionUsing
    */
    class TEDATAACCESSEXPORT JoinCondition : public te::common::BaseVisitable<QueryVisitor>
    {
      public:

        TE_DEFINE_VISITABLE

        /*! \brief Default constructor. */
        JoinCondition() {}

        /*! \brief Virtual destructor. */
        virtual ~JoinCondition() {}

        /*! \brief It creates a new copy of this JoinCondition. */
        virtual JoinCondition* clone() const = 0;
    };

    /*! \brief For use with boost conteiners. */
    inline JoinCondition* new_clone(const JoinCondition& a)
    {
      return a.clone();
    }

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_JOINCONDITION_H

