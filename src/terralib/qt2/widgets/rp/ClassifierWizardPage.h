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
  \file terralib/qt/widgets/rp/ClassifierWizardPage.h

  \brief This file defines a class for a Classifier Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_CLASSIFIERWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_CLASSIFIERWIZARDPAGE_H

// TerraLib
#include "../../../geometry/Polygon.h"
#include "../../../maptools2/AbstractLayer.h"
#include "../../../rp2/Classifier.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWizardPage>
#include <QtGui/QTableWidget>

// Forward declaration
namespace Ui { class ClassifierWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;

      /*!
        \class ClassifierWizardPage

        \brief This class is GUI used to define the classifier parameters for the RP constast operation.
      */
      class TEQTWIDGETSEXPORT ClassifierWizardPage : public QWizardPage
      {
        Q_OBJECT

           enum ClassifierTypes
          {
            CLASSIFIER_ISOSEG
          };

          struct ClassifierSamples
          {
            std::string m_id;
            std::string m_name;
            te::gm::Polygon* m_poly;
          };

        public:

          ClassifierWizardPage(QWidget* parent = 0);

          ~ClassifierWizardPage();

        public:

          /*!
            \brief This method is used to set the selected layer for classifier operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void set(te::map::AbstractLayerPtr layer);

          te::rp::Classifier::InputParameters getInputParams();

          te::rp::Classifier::OutputParameters getOutputParams();

        protected:

          void fillClassifierTypes();

          void listBands();

          void drawSamples();

          void updateSamples();

        public slots:

          void onMapDisplayExtentChanged();

          void onGeomAquired(te::gm::Polygon* poly, te::qt::widgets::MapDisplay* map);

          void onItemChanged(QTableWidgetItem* item);

          void onRemoveToolButtonClicked();

        private:

          std::auto_ptr<Ui::ClassifierWizardPageForm> m_ui;

          std::map<std::string, ClassifierSamples > m_samples;   //!< The map of selected samples
          unsigned int m_countSamples;                           //!< The maximum number of samples inserted.

          te::map::AbstractLayerPtr m_layer;
          te::qt::widgets::MapDisplay* m_display;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_CLASSIFIERWIZARDPAGE_H
