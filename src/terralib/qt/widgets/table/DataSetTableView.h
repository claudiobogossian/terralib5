/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file DataSetTableView.h

  \brief A table view for a dataset.
*/


#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATASETTABLEVIEW_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATASETTABLEVIEW_H

#include "../Config.h"

// Qt
#include <QtGui/QTableView>

namespace te
{
  // Forward declarations
  namespace da
  {
    class DataSet;
  }

  namespace qt
  {
    namespace widgets
    {
      // Forward declaration
      class DataSetTableModel;

      /*!
        \class

        \brief A customized table view for te::map::AbstractLayer objects. Uses a te::qt::widgets::DataSetModel as its model.

        \note We assume that the layer can return ALWAYS return a te::da::DataSet object with random access of it's values. 
      */
      class TEQTWIDGETSEXPORT DataSetTableView : public QTableView
      {
        public:
          /*!
            \brief Constructor.

            \param parent Qt widget parent.
          */
          DataSetTableView(QWidget* parent=0);

          /*!
            \brief Virtual destructor.
          */
          virtual ~DataSetTableView();

          /*!
            \brief Updates the data set being visualized.

            Note that this DataSet MUST HAVE random access. The view DOES NOT get the ownership of the pointer.

            \param dset The new data set to be visualized.
          */
          void setDataSet(te::da::DataSet* dset);

        protected:

          DataSetTableModel* m_model;   //!< The model to be used.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_DATASETTABLEVIEW_H
