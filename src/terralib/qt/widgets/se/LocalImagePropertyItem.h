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
  \file terralib/qt/widgets/se/LocalImagePropertyItem.h

  \brief A widget used to define a local image graphic for a se symbolizer.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_LOCALIMAGEPROPERTYITEM_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_LOCALIMAGEPROPERTYITEM_H

// TerraLib
#include "../Config.h"
#include "../propertybrowser/AbstractPropertyItem.h"

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
        \class LocalImagePropertyItem

        \brief A widget used to define a local image graphic for a se symbolizer.

      */

      class TEQTWIDGETSEXPORT LocalImagePropertyItem : public te::qt::widgets::AbstractPropertyItem
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          LocalImagePropertyItem(QtTreePropertyBrowser* pb, QColor c = QColor());

          /*! \brief Destructor. */
          ~LocalImagePropertyItem();

          //@}

        public:

          /*!
            \brief Sets a mark element to this widget.

            \param mark A valid mark element.

            \note The widget will NOT take the ownership of the given mark.
            \note The widget form will be update based on given mark parameters.
          */
          void setExternalGraphic(const te::se::ExternalGraphic* eg);

          /*!
            \brief Gets the configured mark element.

            \return The configured mark element.
            
            \note The caller will take the ownership of the returned mark.
          */
          te::se::ExternalGraphic* getExternalGraphic() const;

        protected:

          /*! \brief Updates the widget form based on internal mark element. */
          void updateUi();

        public slots:

          virtual void valueChanged(QtProperty* p, const QString &value);

          void setDlg(QWidget *parent, QtProperty * prop);

          void showImgDlg();

        signals:

          /*!
            \brief This signal is emitted when the internal external graphic element is changed.

            \param size The size of selected image.
          */
          void externalGraphicChanged();

        protected:

          QtProperty* m_imgProperty;

          QWidget* m_dlg;

          te::se::ExternalGraphic* m_eg;                                 //!< Mark element that will be configured by this widget.
    
          bool m_update;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_LOCALIMAGEPROPERTYITEM_H
