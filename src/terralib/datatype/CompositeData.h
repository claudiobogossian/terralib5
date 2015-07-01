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
  \file terralib/datatype/CompositeData.h

  \brief A base class for composite data values.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_COMPOSITEDATA_H
#define __TERRALIB_DATATYPE_INTERNAL_COMPOSITEDATA_H

// TerraLib
#include "AbstractData.h"
#include "Enums.h"

// STL
#include <vector>

namespace te
{
  namespace dt
  {
    /*!
      \class CompositeData

      \brief A base class for composite data values.

      \ingroup datatype

      \sa AbstractData, DataType, ByteArray, DateTime, SimpleData
    */
    class TEDATATYPEEXPORT CompositeData : public AbstractData
    {
      public:

        /*! \brief Constructor. */
        CompositeData() {}

        /*!
          \brief It prepares the internal container for at least nComponents.

          \param nComponents The number of expected components for the composite.
        */
        CompositeData(std::size_t nComponents);

        CompositeData(const std::string& name);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side composite.
        */
        CompositeData(const CompositeData& rhs);

        /*! \brief Destructor. */
        ~CompositeData();

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side composite.

          \return A reference to this composite.
        */
        CompositeData& operator=(const CompositeData& rhs);

        void setName(const std::string& name);

        void add(const std::string& name, AbstractData* value);

        /*!
          \brief It returns the i-th component value of the composite data.

          \param i The position index of the component.

          \return The i-th component value. The caller will not take the ownership of the returned pointer.

          \note The component index is 0-based!
        */
        AbstractData* getValue(std::size_t i) const;

        /*!
          \brief It sets the i-th component value of the composite data.

          \param i     The position index of the component.
          \param value The component value at the specified index. If there is another component at the specified index it will be released.

          \note The component index is 0-based!

          \warning Make sure you have prepared the composite for having at least i + 1 components.
        */
        void setValue(std::size_t i, AbstractData* value);

        /*!
          \brief It adds the value as the last component of the composite data.

          \param value The component value at the specified index. If there is another component at the specified index it will be released.

          \note The component index is 0-based!
        */
        void add(AbstractData* value);

        /*!
          \brief It creates a new clone of the composite.

          \return A new clone of the composite. The caller will take its ownership.
        */
        AbstractData* clone() const;

        /*!
          \brief It returns the associated data type code.

          \return The associated data type code.
        */
        int getTypeCode() const { return COMPOSITE_TYPE; }

        /*!
          \brief It returns the data value in a string representation.

          \return The data value in a string representation.
        */
        std::string toString() const;

      private:

        std::vector<AbstractData*> m_values;  //!< The component values.
        std::vector<std::string> m_names;     //!< The component names.
        std::string m_name;                   //!< The composite name.
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_COMPOSITEDATA_H

