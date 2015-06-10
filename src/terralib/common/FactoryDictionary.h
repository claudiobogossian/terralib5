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
  \file terralib/common/FactoryDictionary.h

  \brief A dictionary for a Factory.
 */

#ifndef __TERRALIB_COMMON_INTERNAL_FACTORYDICTIONARY_H
#define __TERRALIB_COMMON_INTERNAL_FACTORYDICTIONARY_H

// TerraLib
#include "Exception.h"
#include "Translator.h"

// STL
#include <map>

namespace te
{
  namespace common
  {
    /*!
      \class FactoryDictionary
      
      \brief This class represents a dictionary of factories.

      An abstract factory will keep an internal dictionary for its
      concrete factories. A factory is identified by a key of type TFACTORYKEY
      that must have the less-than operator. The type TFACTORYKEY must also be copy
      constructible.

      The abstract factory to which the dictionary will belong to
      has the type TFACTORY.

      Note that TFACTORY is the factory type and TFACTORYKEY is the type
      used to identify the factory.

      At destruction time, it will release the memory pointed by the registered factories.

      If you are creating a plugin that registers a factory, see the Abstract
      and Parameterized abstract factory notes about memory management.

      You should not use this class in your code. It is for internal use only.

      \ingroup common

      \sa AbstractFactory, ParameterizedAbstractFactory
     */
    template<class TFACTORY, class TFACTORYKEY, class TKEYCOMPARE = std::less<TFACTORYKEY> >
    class FactoryDictionary
    {
      public:

        typedef typename std::map<TFACTORYKEY, TFACTORY*, TKEYCOMPARE>::const_iterator const_iterator;

        /*! \brief It creates a new factory dictionary. */
        FactoryDictionary();

        /*!
          \brief Destructor.

          It will release all factories registered in it.
          If you need to release the memory before, call remove method, it will just de-register
          the given factory.
         */
        ~FactoryDictionary();

        /*!
          \brief It inserts a pointer to a factory and makes it associated to the factory key.

          \param factoryKey The key that will be used to access the given factory.
          \param factory    A valid pointer to a factory.

          \exception Exception It throws an exception if it can not insert the factory.

          \note You must not release the memory pointed by factory while it is being used.
         */
        void insert(const TFACTORYKEY& factoryKey, TFACTORY* factory);

        /*!
          \brief It removes the factory from the dictionary.
          
          It doesn't release the memory, the caller must do this when appropriated.

          \exception Exception It throws an exception if it can not unregister the factory.

          \param factoryKey The key identifying the factory.
         */
        void remove(const TFACTORYKEY& factoryKey);

        /*!
          \brief It looks for a given factory identified by a key.

          \param factoryKey The key identifying the searched factory.

          \return It returns a pointer to the searched factory or NULL otherwise.

          \note You must not release the memory pointed by factory while it is being used.
         */
        TFACTORY* find(const TFACTORYKEY& factoryKey) const;

        /*!
          \brief It returns an iterator to the first stored factory.
 
          \return An iterator to the first stored factory.

          \note Don't free the pointers!
         */
        typename std::map<TFACTORYKEY, TFACTORY*, TKEYCOMPARE>::const_iterator begin() const;

        /*!
          \brief It returns an iterator to the end of the container.

          \return An iterator to the end of the container.
         */
        typename std::map<TFACTORYKEY, TFACTORY*, TKEYCOMPARE>::const_iterator end() const;

        /*!
          \brief It returns the number of registered factories in the dictionary.

          \return The number of registered factories in the dictionary.
         */
        std::size_t size() const;

      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        FactoryDictionary(const FactoryDictionary& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
         */
        FactoryDictionary& operator=(const FactoryDictionary& rhs);

        //@}

      private:
        
        std::map<TFACTORYKEY, TFACTORY*, TKEYCOMPARE> m_factoryMap;  //!< The internal dictionary map:  key => factory.
    };

    template<class TFACTORY, class TFACTORYKEY, class TKEYCOMPARE> inline
    FactoryDictionary<TFACTORY, TFACTORYKEY, TKEYCOMPARE>::FactoryDictionary()
    {
    }

    template<class TFACTORY, class TFACTORYKEY, class TKEYCOMPARE> inline
    FactoryDictionary<TFACTORY, TFACTORYKEY, TKEYCOMPARE>::~FactoryDictionary()
    {
      //typename std::map<TFACTORYKEY, TFACTORY*, TKEYCOMPARE>::const_iterator it = m_factoryMap.begin();

      //while(it != m_factoryMap.end())
      //{
      //  TFACTORY* factory = it->second;
      //  ++it;                           // we need to move forward before deleting the factory, because of cross-reference
      //  delete factory;
      //}

      m_factoryMap.clear();
    }

    template<class TFACTORY, class TFACTORYKEY, class TKEYCOMPARE> inline
    void FactoryDictionary<TFACTORY, TFACTORYKEY, TKEYCOMPARE>::insert(const TFACTORYKEY& factoryKey,
                                                                       TFACTORY* factory)
    {
      if(factory == 0)
        throw Exception(TE_TR("Could not insert the given factory into the dictionary. The factory is a NULL object!"));

      typename std::map<TFACTORYKEY, TFACTORY*, TKEYCOMPARE>::const_iterator it = m_factoryMap.find(factoryKey);

      if(it != m_factoryMap.end())
        throw Exception(TE_TR("Could not insert the given factory into the dictionary. There is a factory already registered with the given key."));

      m_factoryMap[factoryKey] = factory;
    }

    template<class TFACTORY, class TFACTORYKEY, class TKEYCOMPARE> inline
    void FactoryDictionary<TFACTORY, TFACTORYKEY, TKEYCOMPARE>::remove(const TFACTORYKEY& factoryKey)
    {
      typename std::map<TFACTORYKEY, TFACTORY*, TKEYCOMPARE>::iterator it = m_factoryMap.find(factoryKey);

      if(it != m_factoryMap.end())
        m_factoryMap.erase(it);
      else
        throw Exception(TE_TR("Could not unregister the informed factory. Object factory not found!"));
    }

    template<class TFACTORY, class TFACTORYKEY, class TKEYCOMPARE> inline
    TFACTORY* FactoryDictionary<TFACTORY, TFACTORYKEY, TKEYCOMPARE>::find(const TFACTORYKEY& factoryKey) const
    {
      typename std::map<TFACTORYKEY, TFACTORY*, TKEYCOMPARE>::const_iterator it = m_factoryMap.find(factoryKey);

      if(it == m_factoryMap.end())
        return 0;

      return it->second;    
    }

    template<class TFACTORY, class TFACTORYKEY, class TKEYCOMPARE> inline
    typename FactoryDictionary<TFACTORY, TFACTORYKEY, TKEYCOMPARE>::const_iterator FactoryDictionary<TFACTORY, TFACTORYKEY, TKEYCOMPARE>::begin() const
    {
      return m_factoryMap.begin();
    }

    template<class TFACTORY, class TFACTORYKEY, class TKEYCOMPARE> inline
    typename FactoryDictionary<TFACTORY, TFACTORYKEY, TKEYCOMPARE>::const_iterator FactoryDictionary<TFACTORY, TFACTORYKEY, TKEYCOMPARE>::end() const
    {
      return m_factoryMap.end();
    }

    template<class TFACTORY, class TFACTORYKEY, class TKEYCOMPARE> inline
    std::size_t FactoryDictionary<TFACTORY, TFACTORYKEY, TKEYCOMPARE>::size() const
    {
      return m_factoryMap.size();
    }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_FACTORYDICTIONARY_H

