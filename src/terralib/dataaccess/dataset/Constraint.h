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
  \file terralib/dataaccess/dataset/Constraint.h

  \brief A class that describes a constraint.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_CONSTRAINT_H
#define __TERRALIB_DATAACCESS_INTERNAL_CONSTRAINT_H

// TerraLib
#include "../Config.h"
#include "../Enums.h"

// STL
#include <string>

namespace te
{
  namespace da
  {
// Forward declaration
    class DataSetType;    

    /*!
      \class DataSetConstraint
      
      \brief A class that describes a constraint.

      \sa DataSetType, PrimaryKey, ForeignKey, UniqueKey, CheckConstraint
    */
    class TEDATAACCESSEXPORT Constraint
    {
      public:

        /*!
          \brief Constructor.

          \param id   The constraint identifier.

          \note The new constraint will belong to the given DataSetType.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        Constraint(unsigned int id = 0);

        /*!
          \brief Constructor.

          \param name The constraint name.
          \param id   The constraint identifier.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        Constraint(const std::string& name, unsigned int id = 0);

        /*!
          \brief Copy constructor not allowed.

          The new object will not have an associated DataSetType.

          \param rhs Right-hand-side instance.
        */
        Constraint(const Constraint& rhs);

        /*! \brief Virtual destructor. */
        virtual ~Constraint() {}

        /*!
          \brief Assignment operator not allowed.

          \param rhs Right-hand-side instance.

          \return A reference to this object.
        */
        Constraint& operator=(const Constraint& rhs);

        /*!
          \brief It returns the constraint identifier.

          \return A number that identifies the constraint.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        virtual unsigned int getId() const { return m_id; }

        /*!
          \brief It sets the constraint identifier.

          \param id A number that identifies the constraint.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        virtual void setId(unsigned int id) { m_id = id; }

        /*!
          \brief It returns the constraint name.

          \return The constraint name.
        */
        virtual const std::string& getName() const { return m_name; }

        /*!
          \brief It sets the constraint name.

          \param name The constraint name.
        */
        virtual void setName(const std::string& name) { m_name = name; }
        
        /*!
          \brief It returns the DataSetType associated to the constraint.

          \return The DataSetType associated to the constraint.
        */
        virtual DataSetType* getDataSetType() const { return m_dt; }

        /*!
          \brief It sets the DataSetType associated to the constraint.

          \param dt The DataSetType associated to this constraint.

          \warning Take care when calling this method. If the constraint belongs to a DataSetType,
                   remember to detach it from the DataSetType before calling this method.
        */
        virtual void setDataSetType(DataSetType* dt) { m_dt = dt; }

        /*!
          \brief It returns the constraint type.
          
          \return The constraint type.

          \note Each child has to implement this method and return its specialized type.
        */
        virtual ConstraintType getType() const = 0;

        /*!
          \brief It returns a clone of the object.

          The new object will not have an associated DataSetType.

          \return A clone of the object.
        */
        virtual Constraint* clone() = 0;

      private:

        unsigned int m_id;    //!< An identification number for the constraint.
        DataSetType* m_dt;    //!< The associated DataSetType.
        std::string m_name;   //!< The constraint name.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_CONSTRAINT_H

