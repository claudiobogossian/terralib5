/* Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazi
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
  \file AbstractTable.h
  \brief Abstraction for a table object.
  \details The table also has code to execute the functionalities listed above:

  <ol>
    <li> Change the positions of columns.</li>
    <li> Hide / show columns.</li>
    <li> Define primary key.</li>
  </ol>
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
*/
#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTTABLE_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTTABLE_H

#include "Config.h"

//STL include files
#include <string>
#include <vector>
#include <set>
#include <map>

namespace te
{
  namespace map
  {
    /*!
      \class AbstractTable
      \copydoc AbstractTable.h
      \note The operations of hide, show and reordering consider the previously changes made to table.
    */
    class TEMAPEXPORT AbstractTable
    {

      /*!
        \name Initializer methods.
        \brief Methods related to instantiation and destruction.
      */
      //@{

    public:
      /*! 
        \brief Constructor.    
      */
      AbstractTable();

      /*! 
        \brief Copy constructor.  
        \param source To copy.
      */
      AbstractTable(const AbstractTable & source);

      /*! 
        \brief Virtual destructor.
      */
      virtual ~AbstractTable();

      //@}

      /*!
        \name Overloaded operators.
        \brief Overloaded operators.
      */
      //@{

      /*! 
        \brief Copy operator.    
        \param source To copy.
      */
      AbstractTable & operator=(const AbstractTable & source);

      //@}

      /*!
        \brief Updates separator character of primary keys.
        \details The separator is used in cocatenation of all fields that compose primary key.
        \param separator The character to be used as separator.
      */
      void setPKeySeparator(const char& separator);

      /*!
        \brief Returns the separator, of primary keys, that is being used.
        \return Character that is being used as separator of the primary keys.
      */
      char getPKeySeparator() const;

      /*! 
        \brief Resets the visibility and order of the tables columns.    
        \details Turns all columns to its original positions, and also turn all the columns visible, including geographic ones.
      */
      virtual void resetTable();


      /*!
        \name Accessor methods.
        \brief Methods used to access the internal members. Using these member functions will considere the hidden and the order of columns. 
        To clean up these configurations use the .
      */
      //@{

      /*! 
        \brief Returns the number of rows int the data set.    
        \return The number of rows in the data set.
      */
      virtual size_t numRows() = 0;

      /*! 
        \brief Returns the number of columns int the data set.    
        \return The number of columns in the data set.
      */
      virtual size_t numColumns() const;

      /*! 
        \brief Returns the contents of a cell as a string value.    
        \param row Row position.
        \param column Column postion.
        \return The contents of cell as a string value.
        \exception Row or column out of boundaries.
      */
      virtual std::string dataAsString(size_t row, size_t column) = 0;

      /*! 
        \brief Changes the position of a column.    
        \param fromPos Position of column to be changed.
        \param toPos New column position.
        \exception fromPos or toPos out of table boundaries.
      */
      void moveColumn(size_t fromPos, size_t toPos);

      /*! 
        \brief Returns the name of the i-th column in the Table.    
        \param column Defines the position of the column.
        \exception Value of column out of table boundaries raises te::common::Exception.
      */
      virtual std::string getColumnName(size_t column) const = 0;

      /*! 
        \brief Set column visibility state. True makes \a column visible, otherwise hide it.
        \param column Column number. Column must be a value must be between 0 and table size. 
        \param visible Visiblity of the column. 
        \exception Column number exceeds the number of columns of table.
      */
      void setColumnVisibility(size_t column, bool visible);

      /*! 
        \brief Sets the visibility of geographical columns.    
        \param visibile True to show geometries columns.
      */
      void setGeoColumnsVisible(bool visibile);

      /*! 
        \brief Defines the columns that will compose the primary key.    
        \details The columns to be used are the logical columns. It means that this method considere visibility and order of 
        the columns when this method is called.
        \param cols The position of columns to be used as primary keys.
        \exception If a column is out of boundaries an te::common::Exception will be raised.
        \note The columns used SHOULD UNIQUELY IDENTIFY each record. This is a rule and if that is not respected, there is no guarantee that will work properly.
      */
      void setPKeysColumns(const std::vector<size_t> & cols);

      /*! 
        \brief Returns a vector containing the logical positions of the primary keys.
        \return The positions of the primary keys.
      */
      std::vector<size_t> getPKeysColumns() const;

      /*! 
        \brief Stores the primary key as values composed, separated by \a separator.    
        \details By default the separator is the '#' character.
        \param separator The separator used to compose the value of primary key.
      */
      void preprocessPKeys(char separator = '#');

      /*! 
        \brief Cleans the values of prepocessed primary key.     
      */
      void cleanPreprocessPKeys();

      /*! 
        \brief Gets the primary of a given row.    
        \param row Row in data set.
        \return The primary key of the row.
        \exception If the row exceeds the size of table, an exception will be raised.
      */
      std::string map2PKey(size_t row);

      /*! 
        \brief Given a primary key locate its row.    
        \param pkey Primary key.
        \return The number of row if it was found, and -1 otherwise.
      */
      int map2Row(std::string pkey) const;
      //@}

    protected:

      /*! 
        \brief Given a column, map it considering visibility and position.    
        \param column Column posistion.
        \exception Column out of table boundaries, considering hidding columns.
      */
      size_t getLogicalColumn(size_t column) const;

      /*! 
        \brief Returns the set of logical positions of geometry columns.
        \return Positions of the geometry columns logically (considering visibility and postioning).
      */
      std::set<size_t> findLogicalGeoColumns() const;

    public:

      /*! 
        \brief Returns the positions of columns that contains geometry data.    
        \return Positions of the geometry columns.
      */
      virtual std::set<size_t> findGeoColsPositions() const = 0;

    protected:

      size_t m_currentRow;                          //!< The current row inspected in the data set.
      size_t m_nextRow;                             //!< The next row. Its use for performance pourposes.    
      std::vector<bool> m_colVisiblity;             //!< Visibility of the columns.
      std::vector<size_t> m_presentOrder;           //!< The order the columns must be presented.
      std::vector<size_t> m_absPKeys;               //!< Contains the primary keys columns in data set.
      std::map<std::string, size_t> m_pkeys_2rows;  //!< Pre processed primary key values. For search pourposes.
      std::set<size_t> m_geoColumns;                //!< Columns that contains geometry data.
      char m_separator;                             //!< Separator of primary keys to be used.
    };
  }
}

#endif //__TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTTABLE_H
