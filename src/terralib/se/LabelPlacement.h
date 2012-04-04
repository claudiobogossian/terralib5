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
  \file LabelPlacement.h
  
  \brief The LabelPlacement specifies where and how a text label should be rendered relative to a geometry.
 */

#ifndef __TERRALIB_SE_INTERNAL_LABELPLACEMENT_H
#define __TERRALIB_SE_INTERNAL_LABELPLACEMENT_H

// TerraLib
#include "../common/BaseVisitable.h"
#include "Config.h"
#include "Visitor.h"

namespace te
{
  namespace se
  {
// Forward declarations
    class LinePlacement;
    class PointPlacement;

    /*!
      \class LabelPlacement
      
      \brief The LabelPlacement specifies where and how a text label should be rendered relative to a geometry.

      \sa TextSymbolizer, PointPlacement, LinePlacement
     */
    class TESEEXPORT LabelPlacement
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new LabelPlacement. */
        LabelPlacement();

        /*! \brief Destructor. */
        ~LabelPlacement();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setPointPlacement(PointPlacement* p);
        void setLinePlacement(LinePlacement* l);

        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other LabelPlacement.
         */
        LabelPlacement(const LabelPlacement& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other LabelPlacement.

          \return A reference for this.
         */
        LabelPlacement& operator=(const LabelPlacement& rhs);

        //@}

      private:

        PointPlacement* m_pointPlacement;     //!< It specifies how a text label should be rendered relative to a geometric point. (Mandatory)
        LinePlacement*  m_linePlacement;      //!< It specifies how a text label should be rendered relative to a linear geometry. (Mandatory)
    };    

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_LABELPLACEMENT_H

