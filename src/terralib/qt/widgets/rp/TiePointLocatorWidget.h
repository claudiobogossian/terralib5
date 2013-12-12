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
  \file terralib/qt/widgets/rp/TiePointLocatorWidget.h

  \brief This file has the TiePointLocatorWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTLOCATORWIDGET_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTLOCATORWIDGET_H

// TerraLib
#include "../../../color/RGBAColor.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/GTParameters.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../rp/TiePointsLocator.h"
#include "../Config.h"

// STL
#include <map>
#include <memory>
#include <set>

// Qt
#include <QtGui/QWidget>

namespace Ui { class TiePointLocatorWidgetForm; }

namespace te
{

  namespace se { class Mark; }

  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
      class RasterNavigatorWidget;

        /*! \class TiePointData Tie Point data. */
        class TiePointData
        {
          public :
              
            /*! \enum TiePointAcquisitionType Tie point acquisition type. */
            enum TiePointAcquisitionType 
            { 
              InvalidAcquisitionT,  //!< Invalid acquisition type.
              ManualAcquisitionT,   //!< Manual acquisition type.
              AutomaticAcquisitionT //!< Automatic acquisition type.
            };              
              
            TiePointAcquisitionType m_acqType;          //!< Acquisition type.
            te::gm::GTParameters::TiePoint m_tiePoint;  //!< Tie point coordinates.
            bool m_selected;                            //!< Tie point selection status;
              
            TiePointData();

            TiePointData( const TiePointData& other );

            ~TiePointData();

            const TiePointData& operator=( const TiePointData& other );

            typedef std::map< unsigned int, TiePointData >  TPContainerT; //!< Tie-pints container type definition.

        };

      /*!
        \class TiePointLocatorWidget

        \brief This class is used to define a widget for tie point acquirement.
      */
      class TEQTWIDGETSEXPORT TiePointLocatorWidget : public QWidget
      {
        Q_OBJECT

        public:

          TiePointLocatorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~TiePointLocatorWidget();

          Ui::TiePointLocatorWidgetForm* getForm() const;

        public:

          /*! 
            \brief Get the current acquired tie-points. 

            \param tiePoints The current acquired tie-points.
          */
          void getTiePoints( std::vector< te::gm::GTParameters::TiePoint >& tiePoints ) const;

          /*! 
            \brief Get tie point data container.

            \return  The tie point data container.
          */
          const te::qt::widgets::TiePointData::TPContainerT& getTiePointContainer();

          /*! 
            \brief Get tie point reference coord that does not have an adjust coordenate

            \param coordRef  The tie point reference coord.

            \return The m_tiePointHasFirstCoord status.
          */
          bool getReferenceTiePointCoord(te::gm::Coord2D& coordRef);

          /*!
            \brief This method is used to set the selected layer used to be the reference layer
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void setReferenceLayer(te::map::AbstractLayerPtr layer);

          /*!
            \brief This method is used to set the selected layer used to be the adjust layer
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void setAdjustLayer(te::map::AbstractLayerPtr layer);

          void getOutputSRID(int& srid);
          
          void getOutputResolution(double& resX, double& resY);

        protected:

          void refCoordPicked(double x, double y);

          void adjCoordPicked(double x, double y);

          void setTiePointMarkLegend(QPixmap p);

          void setSelectedTiePointMarkLegend(QPixmap p);

          void setReferenceTiePointMarkLegend(QPixmap p);

          void createSelection(int initialId);

        protected slots:

          void onAutoAcquireTiePointsToolButtonClicked();

          void onSelectAllToolButtonClicked();

          void onUnselectAllToolButtonClicked();

          void onDeleteSelectedToolButtonClicked();

          void onAddToolButtonClicked();

          void onTiePointsTableWidgetItemSelectionChanged();

          void onRefreshToolButtonClicked();

          void onDoneToolButtonClicked();

          void onRefMapDisplayExtentChanged();

          void onAdjMapDisplayExtentChanged();

          void onRefPointPicked(double x, double y, te::qt::widgets::MapDisplay* map);

          void onAdjPointPicked(double x, double y, te::qt::widgets::MapDisplay* map);

          void onTiePointsUpdated();

          void onSRIDPushButtonClicked();

        protected:

          /*! \brief Uptate the tie-points table widget. */
          void tiePointsTableUpdate();
          
          /*! \brief Uptate the current transformation information widgets. */
          void transformationInfoUpdate();

          /*! \brief Fill interface widget with initial values. */
          void startAdvancedOptions();

          /*! \brief Update tie point values with  advanced options changed values. */
          void updateAdvancedOptions();

          void startUpNavigators();

          void drawTiePoints();

          QPixmap getPixmap(te::color::RGBAColor** rgba);

        signals:

          void tiePointsUpdated();

          void doneAcquiredTiePoints();

        private:

         std::auto_ptr<Ui::TiePointLocatorWidgetForm> m_ui;

          te::map::AbstractLayerPtr m_refLayer;                         //!< Layer with reference imagem
          te::map::AbstractLayerPtr m_adjLayer;                         //!< Layer with adjust imagem

          te::qt::widgets::TiePointData::TPContainerT m_tiePoints;       //!< Internal tie-points container.

          te::rp::TiePointsLocator::InputParameters m_inputParameters;   //!< The current parameters.

          te::gm::GTParameters::TiePoint m_currentTiePoint;             //!< The current tie-point.
          bool m_tiePointHasFirstCoord;                                 //!< true if the tie-point has the first part set;
          unsigned int m_tiePointIdCounter;                             //!< A ID counter for new tie pointes inserted into m_tiePoints;

          std::set<int> m_tiePointsSelected;                            //!< List of selected tie points.


          te::qt::widgets::RasterNavigatorWidget* m_refNavigator;     //!< Reference raster navigator
          te::qt::widgets::RasterNavigatorWidget* m_adjNavigator;     //!< Adjust raster navigator

          te::color::RGBAColor** m_rgbaMarkSelected;                  //!< Represents the pattern of a selected tie point
          te::color::RGBAColor** m_rgbaMarkUnselected;                //!< Represents the pattern of a unselected tie point
          te::color::RGBAColor** m_rgbaMarkRef;                       //!< Represents the pattern of reference tie point
          te::se::Mark* m_markSelected;                               //!< Represents the mark of a selected tie point
          te::se::Mark* m_markUnselected;                             //!< Represents the mark of a unselected tie point
          te::se::Mark* m_markRef;                                    //!< Represents the mark of a reference tie point
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTLOCATORWIDGET_H

