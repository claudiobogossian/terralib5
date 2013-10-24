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
  \file terralib/qt/widgets/query/QueryDialog.h

  \brief This file defines a class for a Query Dialog Dialog.
*/

#ifndef __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYDIALOG_H
#define __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYDIALOG_H

// TerraLib
#include "../../../dataaccess/query/Where.h"
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QDialog>

namespace Ui { class QueryDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class WhereClauseWidget;

      /*!
        \class QueryDialog

        \brief This file defines a class for a Query Dialog Dialog.
      */
      class TEQTWIDGETSEXPORT QueryDialog : public QDialog
      {
        Q_OBJECT

        public:

          QueryDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~QueryDialog();

        public:

          te::qt::widgets::WhereClauseWidget* getWidget();

          /*!
            \brief This method is used to set the list of layers
            
          */
          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          te::da::Where* getWhere();


        protected slots:

          void onInputLayerActivated(QString value);

          void onOkPushButtonClicked();

        signals:

          /*! This signal is emitted when the layer selection changed. */
          void layerSelectedObjectsChanged(const te::map::AbstractLayerPtr& layer);

        private:

          std::auto_ptr<Ui::QueryDialogForm> m_ui;

          std::auto_ptr<te::qt::widgets::WhereClauseWidget> m_whereClauseWidget;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_QUERY_INTERNAL_QUERYDIALOG_H
