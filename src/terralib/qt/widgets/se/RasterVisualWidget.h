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
  \file terralib/qt/widgets/se/RasterVisualWidget.h

  \brief A widget used to build
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_RASTERVISUALWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_RASTERVISUALWIDGET_H

// TerraLib
#include "../Config.h"
#include "../../../se.h"

// Qt
#include <QtGui/QWidget>
#include <QtGui/QComboBox>

// STL
#include <memory>
#include <string>
#include <vector>
#include <map>

// Forward declaraion
namespace Ui { class RasterVisualWidgetForm; }

namespace te
{
  // Forward declarations
  namespace se
  {
    class ContrastEnhancement;
    class RasterSymbolizer;
    class Symbolizer;
  }

  namespace rst
  {
    class BandProperty;
  }

// Forward declarations
  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class HorizontalSliderWidget;

      /*!
        \class RasterVisualWidget

        \brief A dialog used to define the raster visual
      */
      class TEQTWIDGETSEXPORT RasterVisualWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a RasterVisualWidget dialog which is a child of parent, with widget flags set to f. */
          RasterVisualWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~RasterVisualWidget();

          //@}

          void setRasterSymbolizer(te::se::RasterSymbolizer* rs);

          void setBandProperty(std::vector<te::rst::BandProperty*> bp);

        protected:

          /*! \brief Internal method to initialize the widget (e.g.: color, combos, icons, etc.) */
          void initialize();

          /*! \brief Updates the widget form based on internal mark element. */
          void updateUi();

          void setComboBoxText(QComboBox* cb, std::string value);


        protected slots:

          void onOpacityChanged(int value);

          void onMonoChannelSelectionClicked();
          void onRedChannelSelectionClicked();
          void onGreenChannelSelectionClicked();
          void onBlueChannelSelectionClicked();
          void onCompositionChannelSelectionClicked();

          void onMonoChannelNameChanged(QString s);
          void onRedChannelNameChanged(QString s);
          void onGreenChannelNameChanged(QString s);
          void onBlueChannelNameChanged(QString s);

          void onTypeConstratChanged(QString s);

          void onMonoGammaChanged(int v);
          void onRedGammaChanged(int v);
          void onGreenGammaChanged(int v);
          void onBlueGammaChanged(int v);

          void onIncreaseGain();
          void onDecreaseGain();
          void onIncreaseOffset();
          void onDecreaseOffset();

        private:

          std::auto_ptr<Ui::RasterVisualWidgetForm> m_ui;                 //!< Dialog form.
          te::qt::widgets::HorizontalSliderWidget* m_sliderWidget;        //!< Slider widget used for opacity information.

          te::se::RasterSymbolizer* m_symbolizer;                         //!< SE Raster Symbolizer element.

          te::se::ContrastEnhancement* m_contrastRed;                     //!< SE Contrast element for red band.
          te::se::ContrastEnhancement* m_contrastGreen;                     //!< SE Contrast element for green band.
          te::se::ContrastEnhancement* m_contrastBlue;                     //!< SE Contrast element for blue band.
          te::se::ContrastEnhancement* m_contrastMono;                     //!< SE Contrast element for mono band.

          te::se::SelectedChannel* m_scRed;                               //!< SE Selected Channel element for red band.
          te::se::SelectedChannel* m_scGreen;                             //!< SE Selected Channel element for red band.
          te::se::SelectedChannel* m_scBlue;                              //!< SE Selected Channel element for red band.
          te::se::SelectedChannel* m_scMono;                              //!< SE Selected Channel element for red band.

          te::se::ChannelSelection* m_cs;                                 //!< SE Channel Selection element.

          std::map<te::se::ContrastEnhancement::ContrastEnhancementType, QString> m_ceNames;   //!< Contrast Types Names

          double m_gainValue;                                             //!< Value used to define the gain value.
          double m_offsetValue;                                           //!< Value used to define the offset value.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_RASTERVISUALWIDGET_H
