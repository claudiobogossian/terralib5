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
  \file terralib/dataaccess/dataset/DataSetTypeCapabilities.h

  \brief A class that informs what kind of constraint and index is supported by a given data source.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPECAPABILITIES_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPECAPABILITIES_H

// TerraLib
#include "../Config.h"

namespace te
{
  namespace da
  {
    /*!
      \class DataSetTypeCapabilities

      \brief A class that informs what kind of constraint and index is supported by a given data source.

      \ingroup dataaccess
    */
    class TEDATAACCESSEXPORT DataSetTypeCapabilities
    {
      public:

        /*! \brief Constructor. */
        DataSetTypeCapabilities();

        /*! \brief Destructor. */
        ~DataSetTypeCapabilities();

      public:

        bool supportsPrimaryKey() const;

        void setSupportPrimaryKey(const bool& support);

        bool supportsUniqueKey() const;

        void setSupportUniqueKey(const bool& support);

        bool supportsForeignKey() const;

        void setSupportForeingKey(const bool& support);

        bool supportsSequence() const;

        void setSupportSequence(const bool& support);

        bool supportsCheckConstraints() const;

        void setSupportCheckConstraints(const bool& support);

        bool supportsIndex() const;

        void setSupportIndex(const bool& support);

        bool supportsRTreeIndex() const;

        void setSupportRTreeIndex(const bool& support);

        bool supportsBTreeIndex() const;

        void setSupportBTreeIndex(const bool& support);

        bool supportsHashIndex() const;

        void setSupportHashIndex(const bool& support);

        bool supportsQuadTreeIndex() const;

        void setSupportQuadTreeIndex(const bool& support);

        void setSupportAll();

        bool supportsAddColumn() const;

        void setSupportAddColumn(const bool& support);

        bool supportsRemoveColumn() const;

        void setSupportRemoveColumn(const bool& support);

        bool supportsDataEdition() const;

        void setSupportDataEdition(const bool& support);

      private:

        bool m_supportPrimaryKey;       //!< A flag that indicates if the data source supports primary key constraint.
        bool m_supportUniqueKey;        //!< A flag that indicates if the data source supports unique key constraint.
        bool m_supportForeignKey;       //!< A flag that indicates if the data source supports foreign key constraint.
        bool m_supportSequence;         //!< A flag that indicates if the data source supports sequence constraint.
        bool m_supportCheckConstraints; //!< A flag that indicates if the data source supports check-constraints.
        bool m_supportIndex;            //!< A flag that indicates if the data source supports some type of index.
        bool m_supportRTreeIndex;       //!< A flag that indicates if the data source supports r-tree indexes.
        bool m_supportBTreeIndex;       //!< A flag that indicates if the data source supports b-tree indexes.
        bool m_supportHashIndex;        //!< A flag that indicates if the data source supports hash indexes.
        bool m_supportQuadTreeIndex;    //!< A flag that indicates if the data source supports quad-tree indexes.
        bool m_supportAddColumn;        //!< A flag that indicates if the data set supports adding columns.
        bool m_supportRemoveColumn;     //!< A flag that indicates if the data set supports removing columns.
        bool m_supportDataEdition;      //!< A flag that indicates if the data set supports data edition.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASETTYPECAPABILITIES_H
