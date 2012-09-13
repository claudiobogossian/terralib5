/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file DataSetTable.h
  \brief A specialization of te::map::AbstractTable that uses te::da::DataSet objects.
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
 */

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_DATASETTABLE_H
#define __TERRALIB_MAPTOOLS_INTERNAL_DATASETTABLE_H

#include "AbstractTable.h"

namespace te
{
  namespace da
  {
    //Forward declarations
    class DataSet;
  }

  namespace map
  {
    /*!
      \class DataSetTable
      \copydoc DataSetTable.h
    */
    class TEMAPEXPORT DataSetTable : public AbstractTable
    {
    public:

      /*!
        \brief Constructor
        \param dset The data set object to be used.
        \note The object does NOT get the owner of te::da::DataSet pointer.
      */
      DataSetTable(te::da::DataSet* dset);

      /*!
        \brief Virtual destructor.
      */
      virtual ~DataSetTable();

      /*! 
        @name Implementation of te::map::AbstractTable interface.
      */
      //@{
      void resetTable();

      size_t numRows();

      std::string dataAsString(size_t row, size_t column);

      std::string getColumnName(size_t pos) const;

    protected:

      std::set<size_t> findGeoColsPositions() const;
      //@}

      te::da::DataSet * m_dataSet; //!< Data set to be used.
    };
  }
}
#endif // __TERRALIB_MAPTOOLS_INTERNAL_DATASETTABLE_H