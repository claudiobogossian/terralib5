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
  \file terralib/qt/widgets/se/LocalImageWidget.h

  \brief A widget used to build an external graphic element that references a local image. e.g. a SVG file, a PNG file, etc.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_LOCALIMAGEWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_LOCALIMAGEWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

// STL
#include <memory>
#include <string>

// Forward declaraion
namespace Ui { class LocalImageWidgetForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class ExternalGraphic;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class LocalImageWidget

        \brief A widget used to build an external graphic element that references a local image. e.g. a SVG file, a PNG file, etc.
      */
      class TEQTWIDGETSEXPORT LocalImageWidget : public QWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a local image widget which is a child of parent, with widget flags set to f. */
          LocalImageWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~LocalImageWidget();

          //@}

        public:

          /*!
            \brief Sets an external graphic element to this widget.

            \param eg A valid external graphic element.

            \note The widget will NOT take the ownership of the given external graphic.
            \note The widget form will be update based on given external graphic parameters.
          */
          void setExternalGraphic(const te::se::ExternalGraphic* eg);

          /*!
            \brief Gets the configured external graphic element.

            \return The configured external graphic element.
            
            \note The caller will take the ownership of the returned external graphic.
          */
          te::se::ExternalGraphic* getExternalGraphic() const;

        protected:

          /*! \brief Updates the widget form based on internal external graphic element. */
          void updateUi();

        protected slots:

          void onBrowsePushButtonPressed();

        signals:

          /*!
            \brief This signal is emitted when the internal external graphic element is changed.

            \param size The size of selected image.
          */
          void externalGraphicChanged(const QSize& size);

        private:

          std::auto_ptr<Ui::LocalImageWidgetForm> m_ui;    //!< Widget form.
          te::se::ExternalGraphic* m_eg;                   //!< External graphic element that will be configured by this widget.
          QString m_filter;                                //!< String to filter the supported image formats by Qt.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_LOCALIMAGEWIDGET_H
