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
  \file terralib/qt/widgets/se/BasicStrokePropertyItem.h

  \brief A widget used to define the basic fill se object.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEPROPERTYITEM_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEPROPERTYITEM_H

// TerraLib
#include "../Config.h"
#include "../propertybrowser/AbstractPropertyItem.h"

namespace te
{
// Forward declarations
  namespace se
  {
    class Stroke;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class BasicFillPropertyItem

        \brief A widget used to define the basic fill se object.

      */

      class TEQTWIDGETSEXPORT BasicStrokePropertyItem : public te::qt::widgets::AbstractPropertyItem
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          BasicStrokePropertyItem(QtTreePropertyBrowser* pb, QColor c = QColor());

          /*! \brief Destructor. */
          ~BasicStrokePropertyItem();

          //@}

        public:

          /*!
            \brief Sets a stroke element to this widget.

            \param stroke A valid stroke element.

            \note The widget will NOT take the ownership of the given stroke.
            \note The widget form will be update based on given stroke parameters.
          */
          void setStroke(const te::se::Stroke* stroke);

          /*!
            \brief Gets the configured stroke element.

            \return The configured stroke element.
            
            \note The caller will take the ownership of the returned stroke.
          */
          te::se::Stroke* getStroke() const;

        protected:

          /*! \brief Updates the widget form based on internal stroke element. */
          void updateUi();

          /*! \brief Updates the widget form element used to visualize the stroke color. */
          void updateUiStrokeColor();

          QMap<int, QIcon> getDashIcons();
          QMap<int, QIcon> getJoinIcons();
          QMap<int, QIcon> getCapIcons();

          QStringList getDashIconsNames();

          QStringList getJoinNames();

          QStringList getCapNames();

        public slots:

          virtual void valueChanged(QtProperty* p, int value);
          virtual void valueChanged(QtProperty* p, double value);
          virtual void valueChanged(QtProperty* p, const QColor &value);

        signals:

          /*! This signal is emitted when the internal stroke element is changed. */
          void strokeChanged();

        protected:

          QtProperty* m_colorProperty;
          QtProperty* m_opacityProperty;
          QtProperty* m_widthProperty;
          QtProperty* m_dashProperty;
          QtProperty* m_joinProperty;
          QtProperty* m_capProperty;

          QColor m_color;                                //!< Auxiliary color attribute.

          te::se::Stroke* m_stroke;                      //!< Stroke element that will be configured by this widget.

          std::vector<std::string> m_dashes;             //!< Predefined dash styles. TODO: it can be filled out, based on a file, etc (?)
          std::map<int, std::string> m_joinMap;
          std::map<int, std::string> m_capMap;

      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_BASICSTROKEPROPERTYITEM_H
