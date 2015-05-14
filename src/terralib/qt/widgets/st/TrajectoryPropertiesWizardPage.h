/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil. 

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

  \brief  A wizardPage used to configure the general properties of a new trajectory layer.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TRAJECOTORYPROPERTIESWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TRAJECOTORYPROPERTIESWIZARDPAGE_H

// TerraLib
#include "../Config.h"
#include "terralib/dataaccess/datasource/DataSourceInfo.h"
#include "terralib/dataaccess/dataset/DataSetType.h"

// Qt
#include <QWizardPage>

// STL
#include <memory>

namespace te
{
  namespace st { class TrajectoryDataSetInfo; }

  namespace qt
  {
    namespace widgets
    {
    //Forward declarations
    class TemporalPropertiesWidget;
    class TrajectoryPropertiesWidget;

    /*!
        \class TrajectoryPropertiesWizardPage

        \brief A WizardPage used to configure the general properties of a new spatio-temporal layer.
      */
      class TEQTWIDGETSEXPORT TrajectoryPropertiesWizardPage : public QWizardPage
      {
      Q_OBJECT

        public:
          TrajectoryPropertiesWizardPage(QWidget* parent = 0);

          ~TrajectoryPropertiesWizardPage();

          std::list<te::st::TrajectoryDataSetInfo*> getInfo(const te::da::DataSourceInfoPtr dsInfo);

          bool isComplete() const;

          void set(const std::list<te::da::DataSetTypePtr> dataTypes);

        private:

          std::list<te::da::DataSetTypePtr>          m_dataTypes;       //!< The list of datasettypes used to configure the trajectory(ies)
          std::auto_ptr<TrajectoryPropertiesWidget>  m_propWidget;      //!< The widget used to configure the unique TrajectoryLayer's properties
          std::auto_ptr<TemporalPropertiesWidget>    m_tempPropWidget;  //!< The widget used to configure the general TrajectoryLayer's properties
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TRAJECOTORYPROPERTIESWIZARDPAGE_H
