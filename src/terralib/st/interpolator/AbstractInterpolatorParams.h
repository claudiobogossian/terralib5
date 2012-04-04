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
  \file AbstractInterpolatorParams.h

  \brief This file contains an abstract class for interpolation function parameters..
 */

#ifndef __TERRALIB_ST_INTERNAL_ABSTRACTINTERPOLATORPARAMS_H
#define __TERRALIB_ST_INTERNAL_ABSTRACTINTERPOLATORPARAMS_H

// ST
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace st
  {
    /*!
      \class AbstractInterpolatorParams

      \brief An abstract class to represent interpolation function parameters. 

      This is an abstract class for every class that implements
      interpolation function parameters. 
      An interpolation function is a procedure that, given a set of discrete 
      observations and some specific parameters, is able to estimate a value in any 
      non-observed position in time, in space or in space and time.  

      \sa AbstractInterpolator
    */
    class TESTEXPORT AbstractInterpolatorParams
    {
      public:
      
        /*!
          \brief It gets the parameters as a string.

          \return It returns a string from the parameters. 
        */
        virtual std::string getAsString() = 0;
        
        /*! \brief Virtual destructor. */
        virtual ~AbstractInterpolatorParams() {}; 

      protected:

        /*! \brief Constructor. */
        AbstractInterpolatorParams() { };     
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_ABSTRACTINTERPOLATORPARAMS_H

