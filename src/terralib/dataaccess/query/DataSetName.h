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
  \file terralib/dataaccess/query/DataSetName.h

  \brief A class that models the name of a dataset used in a From clause.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASETNAME_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASETNAME_H

// TerraLib
#include "FromItem.h"

namespace te
{
  namespace da
  {
    /*!
      \class DataSetName
      
      \brief A class that models the name of a dataset used in a From clause.

      \sa FromItem, SubSelect, FromFunctionCall, Join 
    */
    class TEDATAACCESSEXPORT DataSetName : public FromItem
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param name  The dataset name.
          \param alias An alias to the dataset.
        */
        DataSetName(const std::string& name, const std::string& alias = "");

        /*! \brief Copy constructor. */
        DataSetName(const DataSetName& rhs);

        /*! \brief Destructor. */
        ~DataSetName() {}

        /*! Assignment operator.  */
        DataSetName& operator=(const DataSetName& rhs);

        /*! \brief It creates a new copy of this FromItem. */
        FromItem* clone() const;

        /*!
          \brief It returns the dataset name.

          \return The dataset name.
        */
        const std::string& getName() const;

        /*!
          \brief It sets the dataset name.

          \param name The dataset name.
        */
        void setName(const std::string& name);

      private:

        std::string m_name;  //!< The dataset name.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASETNAME_H

