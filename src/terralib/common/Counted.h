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
  \file terralib/common/Counted.h

  \brief A counted class keeps track of how many abstract instances are pointing to it.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_COUNTED_H
#define __TERRALIB_COMMON_INTERNAL_COUNTED_H

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace common
  {
    /*!
      \class Counted

      \brief A counted class keeps track of how many abstract instances are pointing to it.

      \ingroup common
    */
    class Counted : public boost::noncopyable
    {
      public:

        /*! \brief Constructor. */
        Counted();

        /*! \brief Increases the number of references to this object. */
        void attach();

        /*! \brief Decreases the number of references to this object. Destroy it if there are no more references to it. */
        void detach();

        /*! \brief Returns the number of references to this object. */
        int refCount();

      protected:

        /*! \brief Destructor. */
        virtual ~Counted();

      private:

        int m_refCount; //!< The number of references to this object.
    };

    inline void intrusive_ptr_add_ref(Counted* p)
    {
      p->attach();
    }

    inline void intrusive_ptr_release(Counted* p)
    {
      p->detach();
    }

    inline Counted::Counted()
      : m_refCount(0)
    {
    }

    inline Counted::~Counted()
    {
    }

    inline void Counted::attach()
    {
      ++m_refCount;
    }

    inline void Counted::detach()
    {
      if(--m_refCount == 0)
        delete this;
    }

    inline int Counted::refCount()
    {
      return m_refCount;
    }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_COUNTED_H

