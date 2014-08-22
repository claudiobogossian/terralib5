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
  \file terralib/sa/qt/GeostatisticalMethodsDialog.h

  \brief A dialog with geostatistical methods to measure the spatial variability of attribute of a dataset.
*/

#ifndef __TERRALIB_SA_INTERNAL_GEOSTATISTICALMETHODSDIALOG_H
#define __TERRALIB_SA_INTERNAL_GEOSTATISTICALMETHODSDIALOG_H

// TerraLib
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../maptools/AbstractLayer.h"
#include "../core/GeostatisticalMethod.h"
#include "../Config.h"

// Qt
#include <QDialog>

// Boost
#include <boost/numeric/ublas/matrix.hpp>

namespace Ui { class GeostatisticalMethodsDialogForm; }

// Forward declarations

namespace te
{
  namespace sa
  {
// Forward declarations

    class TESAEXPORT GeostatisticalMethodsDialog : public QDialog
    {
      Q_OBJECT

      public:

        GeostatisticalMethodsDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

        ~GeostatisticalMethodsDialog();

        /*!
          \brief Set the layer that can be used

          \param layers   List of AbstractLayerPtr
        */
        void setLayers(std::list<te::map::AbstractLayerPtr> layers);

      protected slots:

        void onInputLayerComboBoxActivated(int index);

        void onApplyPushButtonClicked();

        void onChangeAttrToolButtonClicked();

        void calculate();

      protected:

        void fillParameters();

        void resetAdjustParameters(double mean, double variance);

      private:

        std::auto_ptr<Ui::GeostatisticalMethodsDialogForm> m_ui;

        te::sa::GeostatisticalMethod* m_method;

        boost::numeric::ublas::matrix<double> m_methodMatrix;

        boost::numeric::ublas::matrix<double> m_modelMatrix;
    };
  }   // end namespace sa
}     // end namespace te

#endif  // __TERRALIB_SA_INTERNAL_GEOSTATISTICALMETHODSDIALOG_H
