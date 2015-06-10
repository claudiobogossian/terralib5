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
  \file terralib/fe/AbstractOp.h
  
  \brief An abstract interface for operators.  
 */

#ifndef __TERRALIB_FE_INTERNAL_ABSTRACTOP_H
#define __TERRALIB_FE_INTERNAL_ABSTRACTOP_H

// TerraLib
#include "../common/BaseVisitable.h"
#include "Config.h"
#include "Visitor.h"

namespace te
{
  namespace fe
  {     
    /*!
      \class AbstractOp
      
      \brief An abstract interface for operators.

      \ingroup fe

      \sa Filter, SpatialOp, ComparisonOp, LogicOp      
     */
    class TEFEEXPORT AbstractOp : public te::common::BaseVisitable<Visitor> 
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes the operator.

          \param opName The operator name.

          \note The AbstractOp will not take the ownership of the given name.
         */
        AbstractOp(const char* opName = 0);

        /*! \brief Virtual destructor. */
        virtual ~AbstractOp();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the operator name.

          \return The operator name.
         */
        const char* getName() const { return m_name; }

        /*!
          \brief It sets the operator name.

          \param opName The operator name.
         */
        void setName(const char* opName) { m_name = opName; }

        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other instance.
         */
        AbstractOp(const AbstractOp& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other instance.

          \return A reference for this.
         */
        AbstractOp& operator=(const AbstractOp& rhs);

        //@}

      protected:

        const char* m_name;   //!< Operator name.
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_ABSTRACTOP_H
