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
#include "../../../classification/ROISet.h"
#include "../../../geometry/Polygon.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../raster/Raster.h"
#include "../../../rp/Classifier.h"
#include "../../../rp/ClassifierMAPStrategy.h"
#include "../../../rp/ClassifierSAMStrategy.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizardPage>
#include <QTableWidget>

// Forward declaration
namespace Ui { class ClassifierWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
      class ROIManagerDialog;

      /*!
        \class ClassifierWizardPage

        \brief This class is GUI used to define the classifier parameters for the RP constast operation.
      */
      class TEQTWIDGETSEXPORT ClassifierWizardPage : public QWizardPage
      {
        Q_OBJECT

           enum ClassifierTypes
          {
            CLASSIFIER_ISOSEG,
            CLASSIFIER_KMEANS,
            CLASSIFIER_MAP,
            CLASSIFIER_EM,
            CLASSIFIER_SAM
          };

        public:

          ClassifierWizardPage(QWidget* parent = 0);

          ~ClassifierWizardPage();

          bool isComplete() const;

        public:

          /*!
            \brief This method is used to set the selected layer for classifier operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void set(te::map::AbstractLayerPtr layer);

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          te::rp::Classifier::InputParameters getInputParams();

          te::rp::Classifier::OutputParameters getOutputParams();

          te::cl::ROISet* getROISet();

        protected:

          void fillClassifierTypes();

          void listBands();

          te::rp::ClassifierMAPStrategy::Parameters::MClassesSamplesCTPtr getMAPSamples(te::cl::ROISet* rs, te::rst::Raster* raster);

          te::rp::ClassifierSAMStrategy::ClassesSamplesTPtr getSAMSamples(te::cl::ROISet* rs, te::rst::Raster* raster);

        public slots:

          void showROIManager(bool show);

          void onROIManagerClosed();

          void onRoiSetChanged(te::cl::ROISet* rs);

        private:

          std::auto_ptr<Ui::ClassifierWizardPageForm> m_ui;
          std::auto_ptr<te::qt::widgets::ROIManagerDialog> m_roiMngDlg;

          te::map::AbstractLayerPtr m_layer;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_CLASSIFIERWIZARDPAGE_H
