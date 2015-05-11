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
  \file terralib/common/AbstractFactory.h

  \brief A class that defines the interface of an abstract factory.
 */

#ifndef __TERRALIB_COMMON_INTERNAL_ABSTRACTFACTORY_H
#define __TERRALIB_COMMON_INTERNAL_ABSTRACTFACTORY_H

// TerraLib
#include "FactoryDictionary.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace common
  {
    /*!
      \class AbstractFactory

      \brief This class defines the interface of abstract factories without initializing parameters.

      The abstract factory keeps an internal dictionary (a singleton) to
      access their concrete factories.

      A concrete factory is identified by a key of type TFACTORYKEY.
      TFACTORYKEY must be copy constructible.

      It will create objects of type TPRODUCT.
      Note that TPRODUCT is the object type that the factory knows
      how to create.

      In order to create an abstract factory, the concrete
      classes will have to extend the build method.
      When an object from a concrete factory is created, it is automatically
      registered in the abstract factory.

      If you need to initialize the objects created by the factories try to use
      a derived class from ParameterizedAbstractFactory instead.

      \ingroup common

      \sa ParameterizedAbstractFactory, FactoryDictionary

      \note This class is not thread-safe for multiple writers.
     */
    template<class TPRODUCT, class TFACTORYKEY, class TKEYCOMPARE = std::less<TFACTORYKEY> >
    class AbstractFactory : public boost::noncopyable
    {
      public:

        typedef FactoryDictionary<AbstractFactory<TPRODUCT, TFACTORYKEY, TKEYCOMPARE>, TFACTORYKEY, TKEYCOMPARE> dictionary_type;

        friend class FactoryDictionary<AbstractFactory<TPRODUCT, TFACTORYKEY, TKEYCOMPARE>, TFACTORYKEY, TKEYCOMPARE>;

        typedef AbstractFactory factory_type;

        /*!
          \brief It returns the factory key associated to the concreate factory.

          \return The factory key associated to the concreate factory.
         */
        const TFACTORYKEY& getKey() const;

        /*!
          \brief It creates an object with the appropriated factory.

          \param factoryKey A key that identifies the factory used to build the object.

          \return It returns a new object created by the given factory.

          \exception Exception If the concrete factory is not specified or the object can not be built for any reason this methiod may throws an exception.

          \note The caller of this method will take the ownership of the returned object.
         */
        static TPRODUCT* make(const TFACTORYKEY& factoryKey);

        /*!
          \brief It returns a reference to the internal dictionary of concrete factories.

          The dictionary is a singleton.

          \return A reference to the internal dictionary.
         */
        static dictionary_type& getDictionary();

        static const factory_type* find(const TFACTORYKEY& factoryKey);

      protected:


        /*!
          \brief Concrete factories (derived from this one) must implement this method in order to create objects.
        
          \return It returns an object created by the concrete factory.
         */
        virtual TPRODUCT* build() = 0;

        /*!
          \brief It creates the factory and automatically registers it in the dictionary.

          \param factoryKey The key that identifies the factory.
         */
        AbstractFactory(const TFACTORYKEY& factoryKey);

        /*!
          \brief Virtual destructor.

          \note It will automatically unregister the factory from the dictionary.
         */
        virtual ~AbstractFactory();

      protected:
        
        TFACTORYKEY m_factoryKey;  //!< The key that identifies the concrete factory: it will be used for unregistering the factory during destruction.
    };  

    template<class TPRODUCT, class TFACTORYKEY, class TKEYCOMPARE> inline
    const TFACTORYKEY& AbstractFactory<TPRODUCT, TFACTORYKEY, TKEYCOMPARE>::getKey() const
    {
      return m_factoryKey;
    }

    template<class TPRODUCT, class TFACTORYKEY, class TKEYCOMPARE> inline
    TPRODUCT* AbstractFactory<TPRODUCT, TFACTORYKEY, TKEYCOMPARE>::make(const TFACTORYKEY& factoryKey)
    {
      AbstractFactory<TPRODUCT, TFACTORYKEY, TKEYCOMPARE>* factory = getDictionary().find(factoryKey);

      if(factory)
        return factory->build();

      throw Exception(TE_TR("Concrete factory not found!"));
    }

    template<class TPRODUCT, class TFACTORYKEY, class TKEYCOMPARE> inline
    typename AbstractFactory<TPRODUCT, TFACTORYKEY, TKEYCOMPARE>::dictionary_type& AbstractFactory<TPRODUCT, TFACTORYKEY, TKEYCOMPARE>::getDictionary()
    {
      static dictionary_type factoryDictionary; // Internal factory dictionary.
      return factoryDictionary;
    }

    template<class TPRODUCT, class TFACTORYKEY, class TKEYCOMPARE> inline
    const typename AbstractFactory<TPRODUCT, TFACTORYKEY, TKEYCOMPARE>::factory_type* AbstractFactory<TPRODUCT, TFACTORYKEY, TKEYCOMPARE>::find(const TFACTORYKEY& factoryKey)
    {
      const factory_type* factory = getDictionary().find(factoryKey);

      return factory;
    }

    template<class TPRODUCT, class TFACTORYKEY, class TKEYCOMPARE> inline
    AbstractFactory<TPRODUCT, TFACTORYKEY, TKEYCOMPARE>::AbstractFactory(const TFACTORYKEY& factoryKey)
      : m_factoryKey(factoryKey)
    {
      getDictionary().insert(factoryKey, this);
    }

    template<class TPRODUCT, class TFACTORYKEY, class TKEYCOMPARE> inline
    AbstractFactory<TPRODUCT, TFACTORYKEY, TKEYCOMPARE>::~AbstractFactory()
    {
      getDictionary().remove(m_factoryKey);
    }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_ABSTRACTFACTORY_H

