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
  \file terralib/qt/widgets/rp/SegmenterWizardPage.h

  \brief This file defines a class for a Segmenter Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_SEGMENTERWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_SEGMENTERWIZARDPAGE_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../../rp/Segmenter.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWizardPage>

// Forward declaration
namespace Ui { class SegmenterWizardPageForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class RasterNavigatorWidget;

      /*!
        \class SegmenterWizardPage

        \brief This class is GUI used to define the segmenter parameters for the RP constast operation.
      */
      class TEQTWIDGETSEXPORT SegmenterWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          SegmenterWizardPage(QWidget* parent = 0);

          ~SegmenterWizardPage();

          bool isComplete() const;

        public:

          /*!
            \brief This method is used to set the selected layer for segmenter operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void set(te::map::AbstractLayerPtr layer);

          te::map::AbstractLayerPtr get();

          te::rp::Segmenter::InputParameters getInputParams();

          te::rp::Segmenter::OutputParameters getOutputParams();


        public slots:

          void onStrategyTypeComboBoxActivated(int index);

          void apply();

        protected:

          void fillSegmenterTypes();

          void listBands();

        private:

          std::auto_ptr<Ui::SegmenterWizardPageForm> m_ui;
          std::auto_ptr<te::qt::widgets::RasterNavigatorWidget> m_navigator;
          std::auto_ptr<te::rst::Raster> m_inputRst;
          te::map::AbstractLayerPtr m_layer;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_SEGMENTERWIZARDPAGE_H
