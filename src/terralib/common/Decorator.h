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
  \file terralib/common/Decorator.h

  \brief Defines a decorator abstraction.
 */
#ifndef __TERRALIB_COMMON_INTERNAL_DECORATOR_H
#define __TERRALIB_COMMON_INTERNAL_DECORATOR_H

namespace te 
{
  namespace common 
  {

    /*!
      \class Decorator

      Decorators are classes that can change object behavioral in run-time.
      For more informations about decorator pattern, see:
      <A HREF="http://en.wikipedia.org/wiki/Decorator_pattern">Decorator pattern on Wikipedia.</A>

      \ingroup common
    */
    template<class T> class Decorator : public T 
    {
      public:

        /*! 
          \brief Constructor.

          The \a deleteDecorated argument tells if the decorator HAS or NOT the ownership of the decorated pointer.
          A \a true value gives the ownership to the decorator and the client do not need manage memory of the decorated pointer.
          Otherwise, the client needs manage the memory of the decorated pointer.

          \param decorated The object to be decorated.
          \param deleteDecorated Tells to decorator to also delete decorated pointer. 
        */
        Decorator(T* decorated, bool deleteDecorated = false);

        /*! \brief Virtual destructor. */
        virtual ~Decorator();

        /*! 
          \brief Copy constructor.

          \param rhs Object to be copied.
        */
        Decorator(const Decorator& rhs);

        /*! 
          \brief Copy operator.

          \param rhs Object to be copied.
        */
        Decorator & operator=(const Decorator& rhs);

        /*! 
          \brief Returns the pointer of decorated object.    
        */
        T* getDecorated();

        /*!
         * \brief Returns the Decorator that decorates this item.
         * \param decorated The decorated item that we are looking for.
         *
         * \return The Decorator we are looking for or this if there's no decorators or 0 if not found.
         */
        T* findDecorator(T* decorated);

        /*!
         * \brief Removes The decorator of the \a decorated.
         * \param decorated The item that we are searching for the decorator.
         *
         * \return The new item to be used as new Delegate or NULL.
         */
        T* removeDecorator(T* decorated);

      protected:

        T * m_decorated;      //!< The object decorated.
        bool m_delDecorated;  //!< If true, also delete decorated pointer.
    };

    template<class T>
    Decorator<T>::Decorator(T* decorated, bool deleteDecorated)
      : T(),
        m_decorated(decorated),
        m_delDecorated(deleteDecorated)
    {
    }

    template<class T>
    Decorator<T>::~Decorator() 
    {
      if(m_delDecorated)
        delete m_decorated;
    }

    template<class T>
    Decorator<T>::Decorator(const Decorator<T> & rhs)
      : m_decorated(rhs.m_decorated),
        m_delDecorated(rhs.m_delDecorated)
    {
    }

    template<class T>
    Decorator<T> & Decorator<T>::operator=(const Decorator<T> & rhs)
    {
      m_decorated = rhs.m_decorated;
      m_delDecorated = rhs.m_delDecorated;

      return *this;
    }

    template<class T>
    T* Decorator<T>::getDecorated() 
    {
      return m_decorated;
    }

    template<class T>
    T* Decorator<T>::findDecorator(T* decorated)
    {
      if(decorated == m_decorated)
        return this;

      Decorator<T>* d = dynamic_cast< Decorator<T>* >(m_decorated);

      if(d != 0)
        return findDecorator(d);

      return 0;
    }

    template<class T>
    T* Decorator<T>::removeDecorator(T* decorated)
    {
      if(decorated == this)
      {
        m_delDecorated = false;
        return m_decorated;
      }

      T* fd = findDecorator(decorated);

      if(fd == this)
      {
        m_delDecorated = false;
        return m_decorated;
      }

      Decorator<T>* aux = dynamic_cast< Decorator<T>* >(decorated);

      if(aux != 0)
      {
        aux->m_delDecorated = false;
        ((Decorator<T>*)fd)->m_decorated = aux->m_decorated;
      }

      return 0;
    }

  } // end namespace common
}   // end namespace te

#endif //__TERRALIB_COMMON_INTERNAL_DECORATOR_H
