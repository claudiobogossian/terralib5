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
  \file terralib/qt/widgets/charts/ScatterCreatorDialog.h

  \brief A widget used to define the basic parameters of a new Scatter chart.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERDIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERDIALOG_H

//TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>

//STL
#include <memory>

namespace Ui { class ScatterDialogForm; }

namespace te
{

  namespace da
  {
    //forward declarations
    class DataSet;
  }

  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class ScatterDataWidget;

    /*!
        \class Scatter creator

        \brief A wdiget used to customize a Scatter parameters.
      */
      class TEQTWIDGETSEXPORT ScatterDialog : public QDialog
      {

        Q_OBJECT

        public:

          ScatterDialog(te::da::DataSet* dataSet, QWidget* parent = 0,  Qt::WindowFlags f = 0);

          ~ScatterDialog();

        protected slots:

          void onOkPushButtonClicked();
          void onHelpPushButtonClicked();

        private:

          std::auto_ptr<Ui::ScatterDialogForm>  m_ui;                 //!< The dialog form.
          te::qt::widgets::ScatterDataWidget*   m_scatterDataWidget;  //!< The scatter's data widget used to configure the basic parameters of the new scatter.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERDIALOG_H