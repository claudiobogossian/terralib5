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
  \file DWithin.h

  \brief DistanceBuffer operator: DWithin.
 */

#ifndef __TERRALIB_FE_INTERNAL_DWITHIN_H
#define __TERRALIB_FE_INTERNAL_DWITHIN_H

// TerraLib
#include "DistanceBuffer.h"
#include "Globals.h"

namespace te
{
  namespace fe
  {
    /*!
      \class DWithin
      
      \brief DistanceBuffer operator: DWithin.

      \ingroup fe

      \sa BinarySpatialOp
     */
    class TEFEEXPORT DWithin : public DistanceBuffer
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief Constructor.

          \param p The property name.
          \param g The geometry to check its distance.
          \param d The distance to check.

          \note The DWithin operator will take the ownership of the given arguments.
         */
        DWithin(PropertyName* p,
                te::gm::Geometry* g,
                te::common::Distance* d) : DistanceBuffer(Globals::sm_dWithin, p, g, d) {}

        /*! \brief Destructor. */
        ~DWithin() {}

        //@}
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_DWITHIN_H

