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
  \file terralib/qt/widgets/rp/MixtureModelWizardPage.h

  \brief This file defines a class for a MixtureModel Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELWIZARDPAGE_H

// TerraLib
#include "../../../color/RGBAColor.h"
#include "../../../geometry/Coord2D.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../raster/Raster.h"
#include "../../../rp/MixtureModel.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWizardPage>
#include <QtGui/QTableWidget>

// Forward declaration
namespace Ui { class MixtureModelWizardPageForm; }

namespace te
{
  namespace se { class Mark; }

  namespace qt
  {
    namespace widgets
    {
      class RasterNavigatorDialog;

      /*!
        \class MixtureModelWizardPage

        \brief This class is GUI used to define the mixture model parameters for the RP constast operation.
      */
      class TEQTWIDGETSEXPORT MixtureModelWizardPage : public QWizardPage
      {
        Q_OBJECT

          enum MixModelTypes
          {
            MIXMODEL_LINEAR,
            MIXMODEL_PCA
          };

          struct MixModelComponent
          {
            std::string m_id;
            std::string m_name;
            te::gm::Coord2D m_coordGrid;
            te::gm::Coord2D m_coordGeo;
            std::vector<double> m_values;
          };

        public:

          MixtureModelWizardPage(QWidget* parent = 0);

          ~MixtureModelWizardPage();

         bool isComplete() const;

        public:

          /*!
            \brief This method is used to set the selected layer for mixture model operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void set(te::map::AbstractLayerPtr layer);

          te::map::AbstractLayerPtr get();

          te::rp::MixtureModel::InputParameters getInputParams();

          te::rp::MixtureModel::OutputParameters getOutputParams();

        public slots:

          void onMapDisplayExtentChanged();

          void onPointPicked(double x, double y);

          void onItemChanged(QTableWidgetItem* item);

          void onRemoveToolButtonClicked();

          void showNavigator(bool show);

          void onNavigatorClosed();

        protected:

          void fillMixtureModelTypes();

          void listBands();

          void drawMarks();

          void updateComponents();

        private:

          std::auto_ptr<Ui::MixtureModelWizardPageForm> m_ui;
          std::auto_ptr<te::qt::widgets::RasterNavigatorDialog> m_navigatorDlg;

          std::map<std::string, MixModelComponent > m_components;   //!< The map of selected components
          unsigned int m_countComponents;                           //!< The maximum number of components inserted.

          te::map::AbstractLayerPtr m_layer;
          te::color::RGBAColor** m_rgbaMark;
          te::se::Mark* m_mark;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELWIZARDPAGE_H
