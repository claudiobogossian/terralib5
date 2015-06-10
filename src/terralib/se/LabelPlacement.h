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
  \file terralib/se/LabelPlacement.h
  
  \brief The LabelPlacement specifies where and how a text label should be rendered relative to a geometry.
*/

#ifndef __TERRALIB_SE_INTERNAL_LABELPLACEMENT_H
#define __TERRALIB_SE_INTERNAL_LABELPLACEMENT_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

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
    class TESEEXPORT LabelPlacement : public boost::noncopyable
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

        const PointPlacement* getPointPlacement() const { return m_pointPlacement; }

        void setLinePlacement(LinePlacement* l);

        const LinePlacement* getLinePlacement() const { return m_linePlacement; }

        //@}

        /*! \brief It creates a new copy of this object. */
        LabelPlacement* clone() const;

      private:

        PointPlacement* m_pointPlacement;     //!< It specifies how a text label should be rendered relative to a geometric point. (Mandatory)
        LinePlacement*  m_linePlacement;      //!< It specifies how a text label should be rendered relative to a linear geometry. (Mandatory)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_LABELPLACEMENT_H
