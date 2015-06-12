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
  \file FIFOCachePolicy.h

  \brief This class is used to implement the FIFO cache policy.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_FIFOCACHEPOLICY_H
#define __TERRALIB_GRAPH_INTERNAL_FIFOCACHEPOLICY_H

// Terralib Includes
#include "../Config.h"
#include "AbstractCachePolicy.h"

// STL Includes
#include <vector>

namespace te
{
  namespace graph
  {
    /*!
      \class FIFOCachePolicy

      \brief This class is used to implement the FIFO cache policy.

      \sa AbstractCachePolicy
    */

    class TEGRAPHEXPORT FIFOCachePolicy : public AbstractCachePolicy
    {
      public:

        /*! \brief Default constructor. */
        FIFOCachePolicy();

        /*! \brief Virtual destructor. */
        virtual ~FIFOCachePolicy();

        
        /** @name Access Methods
         *  Method used to access the cache policy
         */
        //@{

        /*!
          \brief Function used to add a new index to be controlled.

          \param value  Object index attribute

         */
        virtual void added(int value);

        /*!
          \brief Function used to inform that an index must be updated

          \param value  Object index attribute

         */
        virtual void update(int value);

        /*!
          \brief Function used to check what index has to be removed from the cache

          \param value  Object index attribute

         */
        virtual void toRemove(int& value);

        /*!
          \brief Function used to inform that an index was accessed.

          \param value  Object index attribute

         */
        virtual void accessed(int value);

        //@}

      protected:

        std::vector<int> m_FIFO;  // This vector is used to keep the input order - FIFO policy
    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_FIFOCACHEPOLICY_H
