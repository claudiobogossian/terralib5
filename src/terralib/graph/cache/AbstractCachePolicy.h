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
  \file AbstractCachePolicy.h

  \brief This class is used to set the main functions of a cache policy.
         
         This is a generic interface to control the access frequency to 
         any type of objects. Only the index of each object is controlled
         by this class.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_ABSTRACTCACHEPOLICY_H
#define __TERRALIB_GRAPH_INTERNAL_ABSTRACTCACHEPOLICY_H

// Terralib Includes
#include "../Config.h"

// STL Includes


namespace te
{
  namespace graph
  {
    /*!
      \class AbstractCachePolicy

      \brief This class is used to set the main functions of a cache policy.

            This is a generic interface to control the access frequency to 
            any type of objects. Only the index of each object is controlled
            by this class.

    */

    class TEGRAPHEXPORT AbstractCachePolicy
    {
      public:

        /*! \brief Default constructor. */
        AbstractCachePolicy();

        /*! \brief Virtual destructor. */
        virtual ~AbstractCachePolicy();

        
        /** @name Access Methods
         *  Method used to access the cache policy
         */
        //@{

        /*!
          \brief Function used to add a new index to be controlled.

          \param value  Object index attribute

         */
        virtual void added(int value) = 0;

        /*!
          \brief Function used to inform that an index must be updated

          \param value  Object index attribute

         */
        virtual void update(int value) = 0;

        /*!
          \brief Function used to check what index has to be removed from the cache

          \param value  Object index attribute

         */
        virtual void toRemove(int& value) = 0;
        
        /*!
          \brief Function used to inform that an index was accessed.

          \param value  Object index attribute

         */
        virtual void accessed(int value) = 0;

        //@}
    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_ABSTRACTCACHEPOLICY_H
