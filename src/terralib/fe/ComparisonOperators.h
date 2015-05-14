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
  \file ComparisonOperators.h
  
  \brief It is used to indicate what types of comparison operators are supported by a service.  
 */

#ifndef __TERRALIB_FE_INTERNAL_COMPARISONOPERATORS_H
#define __TERRALIB_FE_INTERNAL_COMPARISONOPERATORS_H

// TerraLib
#include "../common/Comparators.h"
#include "Config.h"

// STL 
#include <set>
#include <vector>

namespace te
{
  namespace fe
  {
    /*!
      \class ComparisonOperators
      
      \brief It is used to indicate what types of comparison operators are supported by a service.

      The operators name can be:
      <ul>
      <li>PropertyIsLessThan</li>
      <li>PropertyIsGreaterThan</li>
      <li>PropertyIsLessThanEqualTo</li>
      <li>PropertyIsGreaterThanEqualTo</li>
      <li>EqualTo</li>
      <li>PropertyIsNotEqualTo</li>
      <li>Like</li>
      <li>Between</li>
      <li>NullCheck</li>
      </ul>

      \sa ScalarCapabilities

      \todo Fazer um metodo estatico que verifica se um dado operador de comparaco encontra-se no set... e ja retirna o const char* dele ou NULL senao encontrar...
     */
    class TEFEEXPORT ComparisonOperators
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new ComparisonOperators. */
        ComparisonOperators();

        /*! \brief Destructor. */
        ~ComparisonOperators();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It adds the comparison operator to the supported list of operators.
         
          \param c The comparison operator to be added to the supported list of operators.
         */
        void push_back(const char* c);       

        /*!
          \brief It returns a specified comparison operator.

          \param i The index of desired comparison operator.

          \return A specified comparison operator.

          \note You must not clear the returned comparison operator.

          \note The method will not check the index range.
         */
        const char* operator[](size_t i) const;

        /*!
          \brief It checks if operator 'o' is a valid operator. It returns a pointer
                 to the found operator or NULL otherwise.

          \param o The operator name we are looking for.
         */
        static const char* findOperator(const char* o);

        /*! \brief It loads the valid comparison operator list. */
        static void loadValidOperatorList();

        /*! \brief It clears the valid comparison operator list. */
        static void clearValidOperatorList();

        //@}

      private:

        std::vector<const char*> m_comparisonOperators;  //!< Optional.
        static std::set<const char*, te::common::LessCmp<const char*> >  sm_validComparisonOperators; //!< The list of valid comparison operators.
    };    

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_COMPARISONOPERATORS_H
