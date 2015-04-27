/*  Copyright (C) 2010-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/st/TrajectoryPropertiesWIzardPage.h

  \brief  A wizardPage used to configure the generaç properties of a new trajectory layer.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESPROPERTIESWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESPROPERTIESWIZARDPAGE_H

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../Config.h"

// Qt
#include <QWizardPage>

// STL
#include <memory>

namespace te
{

  namespace st { class TimeSeriesDataSetInfo; }

  namespace qt
  {
    namespace widgets
    {
    //Forward declarations
    class TemporalPropertiesWidget;
    class TimeSeriesPropertiesWidget;

    /*!
        \class TimeSeriesPropertiesWizardPage

        \brief A WizardPage used to configure the general properties of a new spatio-temporal layer.
      */
      class TEQTWIDGETSEXPORT TimeSeriesPropertiesWizardPage : public QWizardPage
      {
      Q_OBJECT

        public:
          TimeSeriesPropertiesWizardPage(QWidget* parent = 0);

          ~TimeSeriesPropertiesWizardPage();

          te::st::TimeSeriesDataSetInfo* getInfo(const te::da::DataSourceInfoPtr dsInfo);

          void set(const te::da::DataSetTypePtr dataType);

        private:

          te::da::DataSetTypePtr                     m_dataType;        //!< The list of datasettypes used to configure the time series
          std::auto_ptr<TimeSeriesPropertiesWidget>  m_propWidget;      //!< The widget used to configure the general spatio-temporal layer's properties
          std::auto_ptr<TemporalPropertiesWidget>    m_tempPropWidget;  //!< The widget used to configure the general TrajectoryLayer's properties
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESPROPERTIESWIZARDPAGE_H