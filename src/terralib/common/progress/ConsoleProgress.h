/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/ConsoleProgress.h
 
  \brief The ConsoleProgress is class used to define the main progress functions
         in console applications.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_CONSOLEPROGRESS_H
#define __TERRALIB_COMMON_INTERNAL_CONSOLEPROGRESS_H

// TerraLib
#include "../Config.h"
#include "AbstractProgress.h"


namespace te
{
  namespace common
  {
    /*!
    \class ConsoleProgress

    \brief The ConsoleProgress is a progress bar for console applications.

	        
    \sa ProgressManager, AbstractProgress, ProgressParams

    \todo 

    */
    class TECOMMONEXPORT ConsoleProgress : public AbstractProgress
    {
      public:

        /** @name Initializer Methods
        *  Methods related to instantiation and destruction.
        */
        //@{

        /*! \brief It initializes a new AbstractProgress.

        */
        ConsoleProgress();

        /*! \brief Destructor */
        ~ConsoleProgress();

        //@}

        /*!
        \brief Used to set the current step 

        \param step Integer value

        \note This function call the abstract setCurrentStep implementation and after
              shows a console message with the progress information
        */
        virtual void setCurrentStep(const int& step);
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_CONSOLEPROGRESS_H