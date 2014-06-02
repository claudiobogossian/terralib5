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
  \file terralib/qt/widgets/se/SelectedChannelWidget.h

  \brief A widget used to build SelectedChannel element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_RASTERSYMBOLIZERDIALOG_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_RASTERSYMBOLIZERDIALOG_H

// TerraLib
#include "../Config.h"

// Qt
#include <QDialog>

// STL
#include <memory>
#include <string>
#include <vector>

// Forward declaraion
namespace Ui { class RasterSymbolizerDialogForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class RasterSymbolizer;
    class Symbolizer;
    class ColorMap;
  }

  namespace rst
  {
    class RasterProperty;
    class Raster;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class ChannelSelectionWidget;
      class ColorMapWidget;
      class OverlapBehaviorWidget;
      class ContrastEnhancementWidget;
      class ShadedReliefWidget;
      class ImageOutlineWidget;
      class HorizontalSliderWidget;
      class RasterSymbolizerWidget;


      /*!
        \class RasterSymbolizerDialog

        \brief A dialog used to build a RasterSymbolizerDialog element.
      */
      class TEQTWIDGETSEXPORT RasterSymbolizerDialog : public QDialog
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a RasterSymbolizerDialog dialog which is a child of parent, with widget flags set to f. */
          RasterSymbolizerDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~RasterSymbolizerDialog();

          //@}

        public:

          /*! 
            \brief Sets the raster information

            \param p Raster property

            \note The class will NOT take the ownership of the pointer.
          */
          void setRasterProperty(te::rst::Raster* r, te::rst::RasterProperty* p);

          void setRasterSymbolizer(const te::se::RasterSymbolizer* rs);

          te::se::Symbolizer* getRasterSymbolizer() const;

        protected:

          /*! \brief Updates the widget form based on internal mark element. */
          void updateUi();

        protected slots:

          void onOpacityClicked();
          void onChannelSelectionClicked();
          void onColorMapClicked();
          void onContrastEnhancementClicked();
          void onImageOutlineClicked();
          void onOverlapBehaviorClicked();
          void onShadedReliefClicked();
          void onVisualClicked();

        signals:

         

        private:

          std::auto_ptr<Ui::RasterSymbolizerDialogForm> m_ui;                   //!< Dialog form.
          te::qt::widgets::ChannelSelectionWidget* m_channelSelectionWidget;    //!< Channel Selection Widget form.
          te::qt::widgets::ColorMapWidget* m_colorMapWidget;                    //!< Color Map Widget form.
          te::qt::widgets::OverlapBehaviorWidget* m_overlapBehaviorWidget;      //!< Overlap Behavior Widget form.
          te::qt::widgets::ContrastEnhancementWidget* m_contrastWidget;         //!< Constrast Widget form.
          te::qt::widgets::ShadedReliefWidget* m_shadedReliefWidget;            //!< Shaded Relief Widget form.
          te::qt::widgets::ImageOutlineWidget* m_imageOutlineWidget;            //!< Image Outline Widget form.
          te::qt::widgets::HorizontalSliderWidget* m_sliderWidget;              //!< Slider widget used for opacity information.
          te::qt::widgets::RasterSymbolizerWidget* m_rasterWidget;              //!< Raster Symbolizer widget.

          te::se::RasterSymbolizer* m_symbolizer;                               //!< SE Raster Symbolizer element.
          te::se::ColorMap* m_colorMap;                                         //!< SE Color Map element.

          te::rst::RasterProperty* m_property;                                  //!< Raster Property object.
          te::rst::Raster* m_raster;                                            //!< Raster Object.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_RASTERSYMBOLIZERDIALOG_H
