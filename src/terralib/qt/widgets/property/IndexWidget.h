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
  \file terralib/qt/widgets/property/IndexWidget.h

  \brief This file has the IndexWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_INDEXWIDGET_H
#define __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_INDEXWIDGET_H

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../Config.h"

// Qt
#include <QWidget>

namespace Ui { class IndexWidgetForm; }

namespace te
{
  namespace da { class Index; }

  namespace qt
  {
    namespace widgets
    {
      class DoubleListWidget;

      /*!
        \class IndexWidget

        \brief This class is used to define a Index DataSet class object.
      */
      class TEQTWIDGETSEXPORT IndexWidget : public QWidget
      {
        Q_OBJECT

        public:

          IndexWidget(te::da::DataSetType* dsType, QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~IndexWidget();

          Ui::IndexWidgetForm* getForm() const;


        public:

          /*!
            \brief It returns the Index DataSet class object.

            \return The caller of this method will TAKE the ownership of the returned Index pointer.
          */
          te::da::Index* getIndex();

          /*!
            \brief Check the interface parameters

            \return True if the necessary parameters is ok and false in other case.
          */
          bool checkParameters();


        private:

         std::auto_ptr<Ui::IndexWidgetForm> m_ui;
         te::qt::widgets::DoubleListWidget* m_doubleListWidget;
         te::da::DataSetType* m_dsType;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROPERTY_INTERNAL_INDEXWIDGET_H

