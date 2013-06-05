/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/core/dataset/Sequence.h

  \brief It describes a sequence (a number generator).
*/

#ifndef __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_SEQUENCE_H
#define __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_SEQUENCE_H

// TerraLib
#include "../Config.h"

// STL
#include <string>

// Boost
#include <boost/cstdint.hpp>

namespace te
{
// Forward declaration
  namespace dt { class Property; }

  namespace da
  {
    namespace core
    {
// Forward declaration
      class DataSourceCatalog;

      /*!
        \class Sequence
      
        \brief It describes a sequence (a number generator).

        \sa DataSourceCatalog
      */
      class TEDATAACCESSEXPORT Sequence
      {
        public:

          /*! \brief It constructs a new sequence. */
          Sequence();

          /*!
            \brief It creates a new sequence.

            \param name       The sequence name.
            \param increment  The value to be added to the current sequence value to create the new value (default = 1).
            \param startValue The sequence starting value (default = 1).

            \post If catalog is provided, the sequence will belong to the given DataSourceCatalog.

            \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
          */
          Sequence(const std::string& name,
                   boost::int64_t increment = 1,
                   boost::int64_t startValue = 1);

          /*!
            \brief Copy constructor.

            The new object will not have an associated catalog.

            \param rhs Right-hand-side instance.
          */
          Sequence(const Sequence& rhs);

          /*! \brief Destructor. */
          ~Sequence() { }

          /*!
            \brief Assignment operator.

            The new object will not have an associated catalog.

            \param rhs Right-hand-side instance.

            \return A reference to this object.
          */
          Sequence& operator=(const Sequence& rhs);

          /*!
            \brief It returns the sequence name.

            \return The sequence name.
          */
          const std::string& getName() const { return m_name; }

          /*!
            \brief It sets the sequence name.

            \param name The sequence name.
          */
          void setName(const std::string& name) { m_name = name; }

          /*!
            \brief It returns the increment value.

            \return The increment value.
          */
          boost::int64_t getIncrement() const { return m_increment; }

          /*!
            \brief It sets the increment value.

            \param n The increment value.
          */
          void setIncrement(boost::int64_t n) { m_increment = n; }
        
          /*!
            \brief It returns the minimum value that the sequence can generate.

            \return The minimum value that the sequence can generate.
          */
          boost::int64_t getMinValue() const { return m_minValue; }

          /*!
            \brief It sets the minimum value that the sequence can generate.

            \param value The minimum value that the sequence can generate.
          */
          void setMinValue(boost::int64_t value) { m_minValue = value; }

          /*!
            \brief It returns the maximum value that the sequence can generate.

            \return The maximum value that the sequence can generate.
          */
          boost::int64_t getMaxValue() const { return m_maxValue; }

          /*!
            \brief It sets the maximum value that the sequence can generate.

            \param value The maximum value that the sequence can generate.
          */
          void setMaxValue(boost::int64_t value) { m_maxValue = value; }

          /*!
            \brief It returns the initial value of the sequence.

            \return the initial value of the sequence
           */
          boost::int64_t getStartValue() const { return m_startValue; }

          /*!
            \brief It sets the sequence starting value.

            \param value The sequence starting value.
          */
          void setStartValue(boost::int64_t value) { m_startValue = value; }

          /*!
            \brief It returns how many sequence numbers are preallocated.

            \return How many sequence numbers are preallocated.
          */
          boost::int64_t getCachedValues() const { return m_cachedValues; }

          /*!
            \brief It sets how many sequence numbers are to be preallocated.

            \param value How many sequence numbers are to be preallocated.
          */
          void setCachedValues(boost::int64_t value) { m_cachedValues = value; }

          /*!
            \brief It returns true if the sequence can wrap, otherwise it returns false.

            \return True if the sequence can wrap, otherwise it returns false.
          */
          bool isCycled() const { return m_cycled; }

          /*! \brief It sets the sequence as cycled (it can wrap). */
          void setAsCycle() { m_cycled = true; }

          /*! \brief It sets the sequence as not cycled (it can't wrap). */
          void setAsNoCycle() { m_cycled = false; }

          /*!
            \brief It returns the property type associated to the sequence.

            \return The property type associated to the sequence.
          */
          const std::string& getOwnerDataSet() const { return m_ownedByDataSet; }

          /*!
            \brief It causes the sequence to be associated with a specific property type.

            When the dataset type is dropped (or its whole catalog), the sequence
            will be automatically dropped as well.

            \param owner The property type owner of this sequence.
          */
          void setOwnerDataSet(const std::string& owner) { m_ownedByDataSet = owner; }

          /*!
            \brief It returns the property type associated to the sequence.

            \return The property type associated to the sequence.
          */
          const std::size_t getOwnerProperty() const { return m_ownedByPropertyPos; }

          /*!
            \brief It causes the sequence to be associated with a specific property type.

            When the dataset type is dropped (or its whole catalog), the sequence
            will be automatically dropped as well.

            \param owner The property type owner of this sequence.
          */
          void setOwnerProperty(const std::size_t propertyPos) { m_ownedByPropertyPos = propertyPos; }

          /*!
            \brief It returns the catalog associated to the sequence.

            \return The catalog associated to the sequence.
          */
          DataSourceCatalog* getCatalog() const { return m_catalog; }

          /*!
            \brief It sets the catalog associated to the sequence.

            \param catalog The data source catalog to which this sequence belongs.
          */
          void setCatalog(DataSourceCatalog* catalog) { m_catalog = catalog; }

        private:

          std::string m_name;                //!< The sequence name.
          std::string m_ownedByDataSet;      //!< The sequence may be associated with a specific property type (owned by a property).
          boost::int64_t m_increment;        //!< The value to be added to the current sequence value to create the new value.
          boost::int64_t m_minValue;         //!< The minimum value that the sequence can generate.
          boost::int64_t m_maxValue;         //!< The maximum value that the sequence can generate.
          boost::int64_t m_startValue;       //!< The sequence starting value.        
          boost::int64_t m_cachedValues;     //!< It specifies how many sequence numbers are to be preallocated for faster access.
          DataSourceCatalog* m_catalog;      //!< The DataSourceCatalog associated to this sequence.
          std::size_t m_ownedByPropertyPos;  //!< The sequence may be associated with a specific property type (owned by a property).
          bool m_cycled;                  //!< If it is true, the sequence can wrap when it reaches the maximum value in the case of an ascendent sequence or wrap when it reaches the minimum value in the case of a descendant sequence.
      };

    }  //end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_SEQUENCE_H
