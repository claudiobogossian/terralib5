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
  \file PromoTable.h
  \brief Controls the order of rows to be presented.
  \details It is a te::qt::map::Table decoration that implements a particular kind of sorting, we can put some random rows on top / bottom.
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
 */
#ifndef __TERRALIB_MAPTOOLS_INTERNAL_PROMOTEDTABLE_H
#define __TERRALIB_MAPTOOLS_INTERNAL_PROMOTEDTABLE_H

#include "AbstractTable.h"
#include <terralib/common/Decorator.h>

namespace te
{
  namespace map
  {
    /*!
      \class PromoTable
      \copydoc PromoTable.h
    */
    class TEMAPEXPORT PromoTable : public te::common::Decorator<AbstractTable>
    {
    public:

      /*!
        \brief Constructor.
        \param table The table to be decorated.
        \note The PromoTable gets the ownership of the \a table pointer. It means that the \a table WILL BE deleted when PromoTable is being deleted.
      */
      PromoTable(AbstractTable* table);

      /*!
        \brief Destructor.
      */
      ~PromoTable();

      /*!
        \brief Changes the positions of the rows, identified by the \a pkeys argument, to the top of the table.
        \param pkeys Identifiers of rows to be promoted.
        \note To use promotion the client MUST pre-process primary keys before. It can be done using the method te::map::AbstractTable::preprocessPKeys().      
      */
      void promoteRows(const std::vector<std::string>& pkeys);

      /*! 
        @name Implementation of te::map::AbstractTable interface.
      */
      //@{
      void resetTable();

      size_t numRows();

      std::string dataAsString(size_t row, size_t column);

      std::string getColumnName(size_t pos) const;

      te::gm::Geometry* getGeometry(const size_t& row) const;

    protected:

      std::set<size_t> findGeoColsPositions() const;
      //@}

      /*!
        \brief Resets the rows order to its original positions.
      */
      void resetRowsOrder();

      std::vector<size_t> m_logRows_pos; //!< Postions of the rows.
    };
  }
}
#endif //__TERRALIB_MAPTOOLS_INTERNAL_PROMOTEDTABLE_H
