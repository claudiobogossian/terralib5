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
  \file terralib/dataaccess/query/SubSelect.h

   \brief A Select can be used as a source of information in another query.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_SUBSELECT_H
#define __TERRALIB_DATAACCESS_INTERNAL_SUBSELECT_H

// TerraLib
#include "FromItem.h"

// STL
#include <memory>

namespace te
{
  namespace da
  {
// Forward declarations
    class Select;

    /*!
      \class SubSelect
      
      \brief A Select can be used as a source of information in another query.

      \sa FromItem, DataSetName, DataSetType, DataSet, FromFunctionCall, Join 
    */
    class TEDATAACCESSEXPORT SubSelect : public FromItem
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param s The real SubSelect to be associated to this object.

          \note The SubSelect will take the ownership of the Select.
        */
        SubSelect(Select* s, const std::string& alias);

        /*!
          \brief Constructor.

          \param s The real SubSelect to be associated to this object.
        */
        SubSelect(const Select& s, const std::string& alias);        

        /*! \brief Copy constructor. */
        SubSelect(const SubSelect& rhs);

        /*! \brief Destructor. */
        ~SubSelect();

        /*! Assignment operator.  */
        SubSelect& operator=(const SubSelect& rhs);

        /*! \brief It creates a new copy of this FromItem. */
        FromItem* clone() const;

        /*!
          \brief It returns the associated subselect.

          \return The associated subselect.
        */
        Select* getSelect() const;

        /*!
          \brief It sets the real SubSelect associated to this object.

          \param s The SubSelect to be associated to the FromItem object.

          \note The SubSelect will take the ownership of the Select.
        */
        void setSelect(Select* s);

      private:

        std::auto_ptr<Select> m_select;  //!< The associated SubSelect.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_SUBSELECT_H

