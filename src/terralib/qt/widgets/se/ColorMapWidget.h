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
  \file terralib/qt/widgets/se/ColorMapWidget.h

  \brief A widget used to build
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_COLORMAPWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_COLORMAPWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QTableWidgetItem>
#include <QWidget>

// STL
#include <memory>
#include <string>
#include <vector>

// Forward declaraion
namespace Ui { class ColorMapWidgetForm; }

namespace te
{
// Forward declarations
  namespace color
  {
    class ColorBar;
  }

  namespace se
  {
    class ColorMap;
  }

  namespace rst
  {
    class Raster;
  }

  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class ColorCatalogWidget;

      /*!
        \class ColorMapWidget

        \brief A dialog used to build a ColorMap element.
      */
      class TEQTWIDGETSEXPORT ColorMapWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a ShadedReliefWidget dialog which is a child of parent, with widget flags set to f. */
          ColorMapWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~ColorMapWidget();

          //@}

        public:

          void setRaster(te::rst::Raster* r);

          void setColorMap(te::se::ColorMap* cm);

          te::se::ColorMap* getColorMap();

          std::string getCurrentBand();

        protected:

          /*! \brief Internal method to initialize the widget (e.g.: color, combos, icons, etc.) */
          void initialize();

          /*! \brief Updates the widget form based on internal fill element. */
          void updateUi();

          void buildCategorizationMap();

          void buildInterpolationMap();

          void buildRecodingMap();

        protected slots:

          void onApplyPushButtonClicked();

          void onBandSelected(QString value);

          void onTableWidgetItemDoubleClicked(QTableWidgetItem* item);

        signals:

          void applyPushButtonClicked();

        private:

          std::auto_ptr<Ui::ColorMapWidgetForm> m_ui;                 //!< Dialog form.
          te::qt::widgets::ColorCatalogWidget* m_colorBar;            //!< Widget used to pick a color.
          te::se::ColorMap* m_cm;                                     //!< SE Color Map element.
          te::color::ColorBar* m_cb;                                  //!< Terralib color bar objetc

          te::rst::Raster* m_raster;                                  //!< TerraLib raster object

      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_COLORMAPWIDGET_H
