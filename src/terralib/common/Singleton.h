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
  \file terralib/common/Singleton.h

  \brief Template support for singleton pattern.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_SINGLETON_H
#define __TERRALIB_COMMON_INTERNAL_SINGLETON_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace common
  {
    /*!
      \class Singleton

      \brief Template support for singleton pattern.
      
      In TerraLib, classes that manage resources like DataSourceManager and ProjectManager
      are singletons. The basic interface of a singleton is defined by
      this singleton class. If you want to get access to the
      singleton instance you can write a code like:
      
      \code
      DataSourceManager& dsm = DataSourceManager::getInstance();
      \endcode

      or

      \code
      std::vector<DataSource*> datasources = DataSourceManager::getInstance().getDataSourceList();
      \endcode
        
      If you want to use this template class in order
      to create your singleton class, the type T must
      satisfy the following requirements:
      <ul>
      <li>It must have an empty constructor;</li>
      </ul>
      You can create a singleton class as follow:
      \code
      class MySingletonClass : public te::common::Singleton<MySingletonClass>
      {
        friend class te::common::Singleton<MySingletonClass>;

        public:

          // Singleton methods (just as an example).
          int getId()
          {
            return id;
          }

          ...

        protected:
            
          // Empty constructor
          MySingletonClass()
          {
            ...
          }

        private:

          // My private data
          int id;

          ...
      };
      \endcode

      \ingroup common
     */
    template<class T> class Singleton : public boost::noncopyable
    {
      public:

        /*!
          \brief It returns a reference to the singleton instance.

          \return A reference to the singleton instance.
         */
        static T& getInstance();

      protected:

        /*! \brief The singleton constructor is protected. */
        Singleton();

        /*! \brief This will avoid clients trying to release pointers to base class. */
        ~Singleton();
    };

    template<class T> inline T& Singleton<T>::getInstance()
    {
      static T m_singleton;  // The singleton instance.

      return m_singleton;
    }

    template<class T> inline Singleton<T>::Singleton()
    {
    }

    template<class T> inline Singleton<T>::~Singleton()
    {
    }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_SINGLETON_H

