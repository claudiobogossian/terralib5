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
  \file terralib/qt/widgets/rp/MixtureModelWizardPage.h

  \brief This file defines a class for a MixtureModel Wizard page.

  /brief JSON File Structure used to save the mix model components information

  {"MixModel_Components":
    [
      {"Component":
        {"name" : "******"},
        {"geomGrid":
          [
          {"x0" : ******},
          {"y0" : ******},
          .
          .
          .
          {"xn" : ******},
          {"yn" : ******}
          ]
        }
        {"geomGeo":
          [
          {"x0" : ******},
          {"y0" : ******},
          .
          .
          .
          {"xn" : ******},
          {"yn" : ******}
          ]
        }
        {"values":
          [
            {"pixelValue" : *****},
            {"pixelValue" : *****},
            {"pixelValue" : *****}
          ]
        }
        {"Color":"#******"
        }
      }
    ]
  }

  struct MixModelComponent
  {
    std::string m_name;
    te::gm::Geometry* m_geomGrid;
    te::gm::Geometry* m_geomGeo;
    std::vector<double> m_values;
    te::color::RGBAColor m_color;
  };
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELWIZARDPAGE_H

// TerraLib
#include "../../../color/RGBAColor.h"
#include "../../../geometry/Geometry.h"
#ifndef Q_MOC_RUN
#include "../../../maptools/AbstractLayer.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/charts/ChartDisplay.h"
#include "../../../qt/widgets/charts/ChartDisplayWidget.h"
#include "../../../qt/widgets/charts/ChartStyle.h"
#include "../../../raster/Raster.h"
#include "../../../rp/MixtureModel.h"
#endif
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWizardPage>
#include <QTableWidget>
#include <QActionGroup>
#include <qgridlayout.h>
#include <QTreeWidget>
#include <QTreeWidgetItem>

//QWT
#include <qwt_plot_curve.h>

// Forward declaration
namespace Ui { class MixtureModelWizardPageForm; }

namespace te
{
  namespace se { class Mark; }

  namespace qt
  {
    namespace widgets
    {
      class RpToolsWidget;

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
            std::string m_name;
            te::gm::Geometry *m_geomGrid;
            te::gm::Geometry *m_geomGeo;
            std::vector<double> m_values;
            te::color::RGBAColor m_color;
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
          void set(std::list<te::map::AbstractLayerPtr> layers);

          void setMapDisplay(te::qt::widgets::MapDisplay* mapDisplay);

          void setActionGroup(QActionGroup* actionGroup);

          std::list<te::map::AbstractLayerPtr> get();

          te::rp::MixtureModel::InputParameters getInputParams();

          te::rp::MixtureModel::OutputParameters getOutputParams();

          void saveMixtureModelComponents(std::string &fileName);

          void loadMixtureModelComponents(std::string &fileName);

          void updateComponents();

        public slots:

          void onMixturetabChanged(int);

          void onSaveToolButtonClicked();

          void onLoadToolButtonClicked();

          void onDisplayPaintEvent(QPainter* painter);

          void onMapDisplayExtentChanged();

          void onPointPicked(double x, double y);

          void onEnvelopeAcquired(te::gm::Envelope env);

          void onGeomAquired(te::gm::Polygon* poly);

          void onRemoveToolButtonClicked();

          void clearCanvas();
          void onComponentItemClicked(QTreeWidgetItem * item, int column);
          void oncomponentChanged();
          void oncolorToolButtonClicked();
          void onselectedEnabled(bool);
          void onallEnabled(bool);

        protected:

          void fillMixtureModelTypes();

          void listBands();

          void drawMarks();

          void PlotSpectralSignature();

          double GetMediumWavelength(std::string sensor);

          void addGeometryComponent();

      private:

          std::auto_ptr<Ui::MixtureModelWizardPageForm> m_ui;
          std::auto_ptr<te::qt::widgets::RpToolsWidget> m_navigator;

          std::map<std::string, MixModelComponent > m_components;   //!< The map of selected components
          unsigned int m_countComponents;                           //!< The maximum number of components inserted.

          std::list<te::map::AbstractLayerPtr>  m_layers;
          te::color::RGBAColor** m_rgbaMark;
          te::se::Mark* m_mark;

          te::qt::widgets::MapDisplay* m_mapDisplay;

          te::qt::widgets::ChartDisplayWidget *m_displayWidget;
          QGridLayout* m_layoutg;
          QColor m_color;

          //te::cl::ROISet* m_rs;
          te::gm::Geometry* m_geom;

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_MIXTUREMODELWIZARDPAGE_H
