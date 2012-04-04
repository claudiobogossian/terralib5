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
  \file terralib/rp/AbstractParameters.h
  \brief Raster Processing algorithm parameters base interface.
 */

#ifndef __TERRALIB_RP_INTERNAL_ABSTRACT_PARAMETERS_H
#define __TERRALIB_RP_INTERNAL_ABSTRACT_PARAMETERS_H

#include "Config.h"
#include "Exception.h"

namespace te
{
  namespace rp
  {
    /*!
      \class AbstractParameters
      \brief Parameters base interface.
    */
    class TERPEXPORT AbstractParameters
    {
      public:
        
        virtual ~AbstractParameters();
        
        virtual const AbstractParameters& operator=( const AbstractParameters& params );
        
        /*!
          \brief Clear all internal allocated objects and reset the parameters instance to its initial state.
        */
        virtual void reset() throw( te::rp::Exception ) = 0;        

        /*!
          \brief Create a clone copy of this instance.
          \return A clone copy of this instance.
          \note The returned pointer must be deleted by the caller of this method when necessary.
        */        
        virtual AbstractParameters* clone() const= 0;

      protected:

        AbstractParameters();
        
        AbstractParameters( const AbstractParameters& ext );
    };
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_ABSTRACT_PARAMETERS_H

