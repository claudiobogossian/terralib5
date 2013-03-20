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
  \file terralib/qt/widgets/charts/histogramCreatorDialog.h

  \brief A widget used to define the basic parameters of a new histogram.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMCREATORWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMCREATORWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>

#include <memory>

namespace Ui { class HistogramDialog; }

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
    class Histogram;
    class HistogramChart;

    /*!
        \class Histogram Creator

        \brief A wdiget used to customize a Histogram parameters.
      */
      class TEQTWIDGETSEXPORT HistogramCreatorDialog : public QDialog
      {

        Q_OBJECT

        public:

          HistogramCreatorDialog(te::da::DataSet* dataSet, QWidget* parent = 0,  Qt::WindowFlags f = 0);

          ~HistogramCreatorDialog();

          Ui::HistogramDialog* getForm() const;

        protected slots:

          void onStylePushButtonClicked();
          void onBarStylePushButtonClicked();
          void onOkPushButtonClicked();
          void onCancelPushButtonClicked();
          void onHelpPushButtonClicked();
          void onPropertyComboBoxIndexChanged(QString text);

        private:

          std::auto_ptr<Ui::HistogramDialog>  m_ui;
          te::da::DataSet*                    m_dataSet;
          te::da::DataSetType*                m_type;
          te::qt::widgets::Histogram*         m_histogram;
          te::qt::widgets::HistogramChart*    m_histogramChart;

      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMCREATORWIDGET_H