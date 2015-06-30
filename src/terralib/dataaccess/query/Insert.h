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
  \file terralib/dataaccess/query/Insert.h

   \brief A Insert can be used to add information in a table.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_INSERT_H
#define __TERRALIB_DATAACCESS_INTERNAL_INSERT_H

// TerraLib
#include "terralib/dataaccess/query/DataSetName.h"
#include "Field.h"
#include "Fields.h"
#include "Select.h"
#include "Query.h"

// STL
#include <memory>

namespace te
{
  namespace da
  {
    /*!
      \class Insert
      
      \brief The Insert object can add the return of a select object.

      \sa Select, Field, Fields, DataSet, DataSetName
    */
    class TEDATAACCESSEXPORT Insert : public Query
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param d The name of the dataset to insert data.
          \param f Vector with the fields to set data by select.
          \param s The real SubSelect to be associated to this object.

          \note The Insert will take the ownership of the Select.
        */
        Insert(DataSetName* d, Fields* f, Select* s);

        /*!
          \brief Constructor.

          \param d The name of the dataset to insert data.
          \param f Vector with the fields to set data by select.
          \param s The real SubSelect to be associated to this object.
        */
        Insert(const DataSetName& d, const Fields& f, const Select& s);

        /*!
        \brief Constructor.

        \param d The name of the dataset to insert data.
        \param s The real SubSelect to be associated to this object.

        \note The Insert will take the ownership of the Select.
        */
        Insert(DataSetName* d, Select* s);

        /*!
        \brief Constructor.
        
        \param d The name of the dataset to insert data.
        \param s The real SubSelect to be associated to this object.
        */
        Insert(const DataSetName& d, const Select& s);

        /*! \brief Copy constructor. */
        Insert(const Insert& rhs);

        /*! \brief Destructor. */
        ~Insert();

        /*! Assignment operator.  */
        Insert& operator=(const Insert& rhs);

        /*! \brief It creates a new copy of this Insert. */
        Query* clone() const;

        /*!
        \brief It returns the associated DataSetName.

        \return The associated DataSetName.
        */
        DataSetName* getDataSetName() const;

        /*!
        \brief It sets the DataSetName associated to this object.

        \param d The Table to be associated to the Insert object.

        \note The Insert will take the ownership of the FromItem.
        */
        void setDataSetName(DataSetName* d);

        /*!
        \brief It returns the associated fields.

        \return The associated fields.
        */
        Fields* getFields() const;
        
        /*!
        \brief It sets the Fields associated to this object.

        \param f Fields to be associated to the Insert object.

        \note The Insert will take the ownership of the Fields.
        */
        void setFields(Fields* f);

        /*!
        \brief It returns the associated select.

        \return The associated select.
        */
        Select* getSelect() const;

        /*!
          \brief It sets the select associated to this object.

          \param s The Select to be associated to the Insert object.

          \note The Insert will take the ownership of the Select.
        */
        void setSelect(Select* s);

      private:

        std::auto_ptr<DataSetName> m_dsName;  //!< The associated DataSetName.
        std::auto_ptr<Fields> m_fields;  //!< The associated Fields.
        std::auto_ptr<Select> m_select;  //!< The associated Select.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_INSERT_H

