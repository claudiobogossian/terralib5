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
  \file terralib/sa/core/KernelMapOperation.h

  \brief This file contains a class that represents the kernel map operation.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_KERNELMAPOPERATION_H
#define __TERRALIB_SA_INTERNAL_KERNELMAPOPERATION_H

// Terralib Includes
#include "../Config.h"
#include "KernelFunctions.h"
#include "KernelOperation.h"
#include "KernelParams.h"

// STL Includes
#include <map>
#include <memory>


namespace te
{
  namespace sa
  {
    /*!
      \class KernelMapOperation

      \brief Class used to calculate the kernel map of a dataset

      \sa KernelOperation
    */
    class TESAEXPORT KernelMapOperation : public te::sa::KernelOperation
    {
      public:

        /*! \brief Default constructor. */
        KernelMapOperation();

        /*! \brief Virtual destructor. */
        ~KernelMapOperation();

      public:

        /*! \brief Function to execute the kernel operation. */
        virtual void execute();

      protected:

        /*! Function used to build the tree with data set information */
        virtual void buildTree();

      protected:

        te::sa::KernelMap m_kMap;                                     //!< Kernel map with input data
    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_KERNELMAPOPERATION_H