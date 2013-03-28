/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/chartStyleWidget.h

  \brief A widget created to customize the style parameters of a chart
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLEWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLEWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>

//STL
#include <memory>

namespace Ui { class ChartStyleDialog; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

    /*!
        \class Chart Style

        \brief A wdiget used to customize a chart's style parameters.
      */
      class TEQTWIDGETSEXPORT ChartStyleDialog : public QDialog
      {

        Q_OBJECT

        public:

          ChartStyleDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ChartStyleDialog();

          Ui::ChartStyleDialog* getForm() const;

        protected slots:

          void onTitleStylePushButtonClicked();
          void onLabelStylePushButtonClicked();
          void onBackgroundPushButtonClicked();
          void onGridCheckBoxToggled();
          void onOkPushButtonClicked();
          void onCancelPushButtonClicked();
          void onHelpPushButtonClicked();

        private:

          std::auto_ptr<Ui::ChartStyleDialog> m_ui;
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLEWIDGET_H
