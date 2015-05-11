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
  \file terralib/common/BaseVisitable.h

  \brief The root of all hierarchies that can be visited.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_BASEVISITABLE_H
#define __TERRALIB_COMMON_INTERNAL_BASEVISITABLE_H

namespace te
{
  namespace common
  {
    /*!
      \class BaseVisitable

      \brief The root of all hierarchies that can be visited.

      This class defines a pure virtual function that subclasses must implement
      in order to be visited.

      This function may return values (see the R type in template declaration).

      Requirements on types:
      <ul>
      <li>T: the type of visitor;</li>
      <li>R: the return type for the accept method.</li>
      </ul>

      \ingroup common

      \sa BaseVisitor, Visitor
    */
    template<class T, class R = void> class BaseVisitable
    {
      public:
        
        typedef T VisitorType;
        typedef R ReturnType;

        /*! 
          \brief It call the visit method from the guest object.

          \param guest The guest or visitor.

          \return Any valid value define by the template type R.
        */
        virtual ReturnType accept(VisitorType& guest) const = 0;

      protected:

        /*! \brief Destructor. */
       virtual ~BaseVisitable() {}
    };

#define TE_DEFINE_VISITABLE \
            virtual ReturnType accept(VisitorType& guest) const \
            { return guest.visit(*this); }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_BASEVISITABLE_H



