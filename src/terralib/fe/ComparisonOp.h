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
  \file ComparisonOp.h
  
  \brief A comparison operator is used to form expressions that evaluate the mathematical comparison between two arguments.  
 */

#ifndef __TERRALIB_FE_INTERNAL_COMPARISONOP_H
#define __TERRALIB_FE_INTERNAL_COMPARISONOP_H

// TerraLib
#include "AbstractOp.h"

namespace te
{
  namespace fe
  {     
    /*!
      \class ComparisonOp
      
      \brief A comparison operator is used to form expressions that evaluate the mathematical comparison between two arguments.

      If the arguments satisfy the comparison
      then the expression evaluates to true.
      Otherwise the expression evaluates to false.

      \ingroup fe

      \sa AbstractOp, BinaryComparisonOp, PropertyIsLike, PropertyIsNull, PropertyIsBetween
     */
    class TEFEEXPORT ComparisonOp : public AbstractOp
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes the Comparison operator.

          \param opName The operator name.

          \note The ComparisonOp will not take the ownership of the given name.
         */
        ComparisonOp(const char* opName = 0);

        /*! \brief Virtual destructor. */
        virtual ~ComparisonOp();

        //@}
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_COMPARISONOP_H

