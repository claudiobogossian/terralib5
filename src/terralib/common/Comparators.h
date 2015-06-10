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
  \file terralib/common/Comparators.h

  \brief Several functor types for comparing objects.

  \ingroup common
 */

#ifndef __TERRALIB_COMMON_INTERNAL_COMPARATORS_H
#define __TERRALIB_COMMON_INTERNAL_COMPARATORS_H

// STL 
#include <cstring>

namespace te
{
  namespace common
  {
    /*!
      \struct LessCmp

      \brief It implements several compare functions.
    */
    template<class T> struct LessCmp
    {
      /*!
        \brief It compares two NULL terminated C-strings.

        \param f The first string.
        \param s The second string.

        \return True if the first string comes first in lexicographic order.        
      */
      bool operator()(const T& f, const T& s) const
      {
        return f < s;
      }
    };

    /*!
      \struct LessCmp

      \brief It implements several compare functions.
    */
    template<class T> struct LessCmp<T*>
    {
      /*!
        \brief It compares two NULL terminated C-strings.

        \param f The first string.
        \param s The second string.

        \return True if the first string comes first in lexicographic order.        
      */
      bool operator()(const T* f, const T* s) const
      {
        return *f < *s;
      }
    };
    
    /*!
      \struct LessCmp

      \brief It implements several compare functions.
    */
    template<> struct LessCmp<const char*>
    {
      /*!
        \brief It compares two NULL terminated C-strings.

        \param f The first string.
        \param s The second string.

        \return True if the first string comes first in lexicographic order.        
       */
      bool operator()(const char* f, const char* s) const
      {
        return (strcmp(f, s) < 0 ? true : false);
      }
    };
  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_COMPARATORS_H

