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

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERCREATORWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERCREATORWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>

//STL
#include <memory>

namespace Ui { class ScatterCreatorDialogForm; }

namespace te
{

  namespace da
  {
    class DataSet;
    class DataSetType;
  }

  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class Scatter;
    class ScatterChart;
    class Symbol;

    /*!
        \class Scatter creator

        \brief A wdiget used to customize a Scatter parameters.
      */
      class TEQTWIDGETSEXPORT ScatterCreatorDialog : public QDialog
      {

        Q_OBJECT

        public:

          ScatterCreatorDialog(te::da::DataSet* dataSet, QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ScatterCreatorDialog();

          Ui::ScatterCreatorDialogForm* getForm() const;

        protected slots:

          void onStylePushButtonClicked();
          void onPlotStylePushButtonClicked();
          void onOkPushButtonClicked();
          void onHelpPushButtonClicked();

        private:

          std::auto_ptr<Ui::ScatterCreatorDialogForm>  m_ui;
          te::da::DataSet*                  m_dataSet;        //!< The dataset that will be used to generate the Scatter graph.
          te::da::DataSetType*              m_type;           //!< The dataset's type.
          te::qt::widgets::Scatter*         m_scatter;        //!< The scatter that will be configured by this dialog.
          te::qt::widgets::ScatterChart*    m_scatterChart;   //!< The scatter's chart that will be configured by this dialog.
          te::qt::widgets::Symbol*          m_symbol;         //!< The symbol that will be used to plot the scatter's points.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERCREATORWIDGET_H