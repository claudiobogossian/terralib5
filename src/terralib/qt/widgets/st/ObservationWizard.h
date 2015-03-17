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
  \file terralib/qt/widgets/st/ObservationWizard.h

  \brief  A wizard used to generate a new Observationlayer.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_OBSERVATIONWIZARD_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_OBSERVATIONWIZARD_H

//TerraLib
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess.h"
#include "../../../st/maptools/ObservationDataSetLayer.h"
#include "../Config.h"

// Qt
#include <QWizard>

// Forward declaration
namespace Ui { class ObservationWizardForm; }

namespace te
{
  namespace st { class ObservationDataSetLayer; }

  namespace qt
  {
    namespace widgets
    {

    //Forward declarations
    class DataSourceSelectorWizardPage;
    class DataSetSelectorWizardPage;
    class ObservationPropertiesWizardPage;
    
    /*!
        \class ObservationDialog

        \brief A Dialog used to generate a new ObservationLayer
      */
      class TEQTWIDGETSEXPORT ObservationWizard : public QWizard
      {
      Q_OBJECT

        public:

          ObservationWizard(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ObservationWizard();

          te::da::DataSourceInfoPtr getDataSource() const;

          std::list<te::st::ObservationDataSetLayerPtr> getObservationLayers();

    protected slots:

          void back();

          void next();

          void finish();

      private:

          enum
          {
            PAGE_DATASOURCE_SELECTION,
            PAGE_DATASET_SELECTION,
            PAGE_OBSERVATION_PROPERTIES_SELECTION
          };

        std::auto_ptr<Ui::ObservationWizardForm>          m_ui;                      //!< The wizard's form
        std::auto_ptr<DataSourceSelectorWizardPage>       m_datasourceSelectorPage;  //!< The wizard page used to select the datasource
        std::auto_ptr<DataSetSelectorWizardPage>          m_datasetSelectorPage;     //!< The wizard page used to select the dataset
        std::auto_ptr<ObservationPropertiesWizardPage>    m_PropWidgetPage;          //!< The widget used to configure the properties of the new ObservationLayer
        std::list<te::st::ObservationDataSetLayerPtr>     m_observationLayers;        //!< The new Observation Layer(s);

      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_OBSERVATIONWIZARD_H