/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/ParameterizedAbstractFactory.h

  \brief A class that defines the basic interface of an abstract factory that makes use of parameters to construct the objects.
 */

#ifndef __TERRALIB_COMMON_INTERNAL_PARAMETERIZEDABSTRACTFACTORY_H
#define __TERRALIB_COMMON_INTERNAL_PARAMETERIZEDABSTRACTFACTORY_H

// TerraLib
#include "FactoryDictionary.h"

namespace te
{
  namespace common
  {

    /*!
      \class ParameterizedAbstractFactory

      \brief This class defines the basic interface of an abstract factory that makes use of parameters to construct the objects.

      The parameterized abstract factory keeps an internal dictionary for
      accessing its concrete factories. A concrete factory is identified
      by a key of type TFACTORYKEY. This type must be copy constructible.
      
      It will create objects of type TPRODUCT and will pass
      parameters of type TPARAM to their constructors.
      
      Note that TPRODUCT is the object type that
      the factory knows how to create. The TPARAM type is the type
      of parameters accepted by the objects being constructed. In order
      to create your abstract factory, the concrete
      classes will have to extend the build method.
      When an object from a concrete factory is created, it is automatically
      registered in the abstract factory.

      If you do not need to initialize the objects created by the factories, try to use
      a derived class from AbstractFactory instead.

      \ingroup common

      \sa AbstractFactory, FactoryDictionary
     */
    template<class TPRODUCT, class TFACTORYKEY, class TPARAM, class TKEYCOMPARE=std::less<TFACTORYKEY> >
    class ParameterizedAbstractFactory
    {
      public:

        typedef FactoryDictionary<ParameterizedAbstractFactory<TPRODUCT, TFACTORYKEY, TPARAM, TKEYCOMPARE>, TFACTORYKEY, TKEYCOMPARE> dictionary_type;

        friend class FactoryDictionary<ParameterizedAbstractFactory<TPRODUCT, TFACTORYKEY, TPARAM, TKEYCOMPARE>, TFACTORYKEY, TKEYCOMPARE>;

        /*!
          \brief It returns the factory key associated to the concreate factory.

          \return The factory key associated to the concreate factory.
         */
        const TFACTORYKEY& getKey() const;

        /*!
          \brief It creates an object with the appropriated factory.

          \param factoryKey A key that identifies the factory used to build the object.
          \param p          The parameter to be passed to the object being created.

          \return It returns a new object created by the given factory.

          \exception Exception If the concrete factory is not specified or the object can not be built for any reason this methiod may throws an exception.

          \note The caller of this method must release the memory pointed by the returned pointer.
         */
        static TPRODUCT* make(const TFACTORYKEY& factoryKey, TPARAM p);

        /*!
          \brief It returns a reference to the internal dictionary.

          \return A reference to the internal dictionary.
         */
        static dictionary_type& getDictionary();

      protected:

        /*!
          \brief Concrete factories (derived from this one) must implement this method in order to create objects.

          \param p The parameter to be passed to the object being created.

          \return It returns an object created by the concrete factory.
         */
        virtual TPRODUCT* build(TPARAM p) = 0;

        /*!
          \brief It creates the factory.

          \param factoryKey The key that identifies the factory.
         */
        ParameterizedAbstractFactory(const TFACTORYKEY& factoryKey);

        /*!
          \brief Virtual destructor.

          \note It will automatically unregister the factory from the dictionary.
         */
        virtual ~ParameterizedAbstractFactory();

      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        ParameterizedAbstractFactory(const ParameterizedAbstractFactory& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        ParameterizedAbstractFactory& operator=(const ParameterizedAbstractFactory& rhs);

        //@}

      protected:
        
        TFACTORYKEY m_factoryKey;  //!< The key that identifies the concrete factory: it will be used for unregistering the factory during destruction.
    };

    template<class TPRODUCT, class TFACTORYKEY, class TPARAM, class TKEYCOMPARE> inline
    const TFACTORYKEY& ParameterizedAbstractFactory<TPRODUCT, TFACTORYKEY, TPARAM, TKEYCOMPARE>::getKey() const
    {
      return m_factoryKey;
    }

    template<class TPRODUCT, class TFACTORYKEY, class TPARAM, class TKEYCOMPARE> inline
    TPRODUCT* ParameterizedAbstractFactory<TPRODUCT, TFACTORYKEY, TPARAM, TKEYCOMPARE>::make(const TFACTORYKEY& factoryKey,
                                                                                             TPARAM p)
    {
      ParameterizedAbstractFactory<TPRODUCT, TFACTORYKEY, TPARAM, TKEYCOMPARE>* factory = getDictionary().find(factoryKey);

      if(factory)
        return factory->build(p);

      throw Exception(TE_TR("Concrete factory not found!"));
    }

    template<class TPRODUCT, class TFACTORYKEY, class TPARAM, class TKEYCOMPARE> inline
    typename ParameterizedAbstractFactory<TPRODUCT, TFACTORYKEY, TPARAM, TKEYCOMPARE>::dictionary_type& ParameterizedAbstractFactory<TPRODUCT, TFACTORYKEY, TPARAM, TKEYCOMPARE>::getDictionary()
    {
      static dictionary_type factoryDictionary; // Internal factory dictionary.
      return factoryDictionary;
    }

    template<class TPRODUCT, class TFACTORYKEY, class TPARAM, class TKEYCOMPARE> inline
    ParameterizedAbstractFactory<TPRODUCT, TFACTORYKEY, TPARAM, TKEYCOMPARE>::ParameterizedAbstractFactory(const TFACTORYKEY& factoryKey)
      : m_factoryKey(factoryKey)
    {
      getDictionary().insert(factoryKey, this);
    }

    template<class TPRODUCT, class TFACTORYKEY, class TPARAM, class TKEYCOMPARE> inline
    ParameterizedAbstractFactory<TPRODUCT, TFACTORYKEY, TPARAM, TKEYCOMPARE>::~ParameterizedAbstractFactory()
    {
      getDictionary().remove(m_factoryKey);
    }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_PARAMETERIZEDABSTRACTFACTORY_H
