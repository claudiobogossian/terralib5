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
  \file DataSetModel.h
  \brief Defines a subclass of QAbstractTableModel, for manipulate and present te::da::DataSet objects in Qt 4 framework.
  \details Its used to create models for te::da::DataSet objects. This way we can present te::da::DataSet objects as tables in Qt 4.
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
*/
#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATASETMODEL_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATASETMODEL_H

#include <terralib/qt/widgets/Config.h>

//! Qt4 include files
#include <QAbstractTableModel>
//#include <qnamespace.h>

namespace te 
{
  //Forward declarations.
  namespace da 
  {
    class DataSet;
  }

  namespace map 
  {
    class AbstractTable;
  }

  namespace qt 
  {
    namespace widgets 
    {

      /*!
        \class DataSetModel
        \copydoc DataSetModel.h
      */
      class TEQTWIDGETSEXPORT DataSetModel : public QAbstractTableModel
      {
      public:
        /*!
          \enum ItemDataRole
          \brief Defines the types of datas.
        */
        enum ItemDataRole
        {
          PKEY = Qt::UserRole +1, //!< Used to tell model that the data requested are the primary keys defined.
          GEOMETRY
        };


        /*!
          \name Initializer methods.
          \brief Methods related to instantiation and destruction.
        */
        //@{
        /*!
          \brief Constructor.    
          \param table Table pointer of the object to be presented.
          \param parent Used by Qt framework.
          \note This class gets the ownership of the \a table pointer.
        */
        DataSetModel(te::map::AbstractTable * table, QObject * parent = 0);

        /*!
          \brief Virtual destructor.    
        */
        virtual ~DataSetModel();
        //@}

        /*!
          \name QAbstractTableModel re-implementation methods.
          \brief Re-implementation of QAbstractTableModel methods.
        */
        //@{

        /*!
          \brief Returns the number of rows int the te::da::DataSet    
          \param parent Parent. (This is not used for this model).
          \return The number of rows in the te::da::DataSet.
        */
        int rowCount(const QModelIndex & parent) const;

        /*!
          \brief Returns the number of columns.    
          \param parent Parent index. (For this model it is not used)
          \return Number of columns in the te::da::DataSet.
        */
        int columnCount(const QModelIndex & parent) const;

        /*!
          \brief Returns data identified by \a index.    
          \param index Data index.
          \param role Data role.
          \return Data as QVariant object.
        */
        QVariant data(const QModelIndex & index, int role) const;

        /*!
          \brief Returns the data to be presented as header.    
          \param section Column / Row number.
          \param orientation Horizontal / Vertical orientation options.
          \param role Data role.
          \return Data to be presented as header.
        */
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;

        /*!
          \brief Returns the flags associated with a given item.    
          \param index Index of the cell.
          \return Flags related to the item.
        */
        Qt::ItemFlags flags(const QModelIndex & index) const;
        //@}


      protected:
        te::map::AbstractTable * m_table; //!< Table to be used.

        /** @name Copy methods.
        *  No copy allowed.
        */
        //@{

      private:
        /*!
          \brief Copy constructor.    
        */
        DataSetModel(const DataSetModel & source);

        /*!
          \brief Copy operator.    
        */
        DataSetModel & operator=(const DataSetModel & source);
        //@}
      };
    } 
  } 
} 

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_DATASETMODEL_H
