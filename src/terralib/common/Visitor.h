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
  \file terralib/common/Visitor.h

  \brief A generic class for visitors.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_VISITOR_H
#define __TERRALIB_COMMON_INTERNAL_VISITOR_H

namespace te
{
  namespace common
  {
    /*!
      \class Visitor

      \brief A generic class for visitors.

      This class defines a pure virtual function for visiting an object of type T.

      This function may return a value (see the R type in template declaration).

      Requirements on types:
      <ul>
      <li>T: the type of visitable;</li>
      <li>R: the return type for the visit method.</li>
      </ul>

      \ingroup common

      \sa BaseVisitor, BaseVisitable
    */
    template<class T, class R = void> class Visitor //: public BaseVisitor
    {
      public:
        
        typedef R ReturnType;

        /*!
          \brief Pure virtual function that visitors must implement in order to visit some other object of type T.

          \param visited The object to be visited.
        */
        virtual ReturnType visit(T& visited) = 0;
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_VISITOR_H



