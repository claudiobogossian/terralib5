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
  \file terralib/qt/widgets/rp/TiePointLocatorWizardPage.h

  \brief This file defines a class for a tie point locator Wizard page.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTLOCATORWIZARDPAGE_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTLOCATORWIZARDPAGE_H

// TerraLib
#include "../../../color/RGBAColor.h"
#include "../../../maptools2/AbstractLayer.h"
#include "../Config.h"

// STL
#include <map>
#include <memory>

// Qt
#include <QtGui/QDialog>
#include <QtGui/QWizardPage>

namespace te
{
  namespace se { class Mark; }

  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
      class RasterNavigatorDialog;
      class TiePointLocatorWidget;

      /*!
        \class TiePointLocatorWizardPage

        \brief This class is GUI used to define a wizard page for the tiepointlocator widget.
      */
      class TEQTWIDGETSEXPORT TiePointLocatorWizardPage : public QWizardPage
      {
        Q_OBJECT

        public:

          TiePointLocatorWizardPage(QWidget* parent = 0);

          ~TiePointLocatorWizardPage();

          bool isComplete() const;

        public:

          te::qt::widgets::TiePointLocatorWidget* getWidget();

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

        public slots:

          void showReferenceNavigator(bool show);

          void showAdjustNavigator(bool show);

          void onReferenceNavigatorClosed();

          void onAdjustNavigatorClosed();

          void onRefMapDisplayExtentChanged();

          void onAdjMapDisplayExtentChanged();

          void onRefPointPicked(double x, double y, te::qt::widgets::MapDisplay* map);

          void onAdjPointPicked(double x, double y, te::qt::widgets::MapDisplay* map);

          void onTiePointsUpdated();

        protected:

          void startUpNavigators();

          void drawTiePoints();

          QPixmap getPixmap(te::color::RGBAColor** rgba);

        private:

          std::auto_ptr<te::qt::widgets::TiePointLocatorWidget> m_tiePointWidget;

          te::qt::widgets::RasterNavigatorDialog* m_refNavigator;     //!< Reference raster navigator
          te::qt::widgets::RasterNavigatorDialog* m_adjNavigator;     //!< Adjust raster navigator

          te::map::AbstractLayerPtr m_refLayer;                       //!< Layer with reference imagem
          te::map::AbstractLayerPtr m_adjLayer;                       //!< Layer with adjust imagem

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

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_TIEPOINTLOCATORWIZARDPAGE_H
