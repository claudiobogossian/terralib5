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
  \file terralib/qt/widgets/se/WellKnownMarkPropertyItem.h

  \brief A widget used to define the mark se properties.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNMARKPROPERTYITEM_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNMARKPROPERTYITEM_H

// TerraLib
#include "../Config.h"
#include "../propertybrowser/AbstractPropertyItem.h"

namespace te
{
// Forward declarations
  namespace se
  {
    class Mark;
  }

  namespace qt
  {
    namespace widgets
    {
      class BasicFillPropertyItem;
      class BasicStrokePropertyItem;
      class GlyphPropertyItem;

      /*!
        \class WellKnownMarkPropertyItem

        \brief A widget used to define the mark se properties.

      */

      class TEQTWIDGETSEXPORT WellKnownMarkPropertyItem : public te::qt::widgets::AbstractPropertyItem
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          WellKnownMarkPropertyItem(QtTreePropertyBrowser* pb, QColor c = QColor());

          /*! \brief Destructor. */
          ~WellKnownMarkPropertyItem();

          //@}

        public:

          /*!
            \brief Sets a mark element to this widget.

            \param mark A valid mark element.

            \note The widget will NOT take the ownership of the given mark.
            \note The widget form will be update based on given mark parameters.
          */
          void setMark(const te::se::Mark* mark);

          /*!
            \brief Gets the configured mark element.

            \return The configured mark element.
            
            \note The caller will take the ownership of the returned mark.
          */
          te::se::Mark* getMark() const;

        protected:

          /*! \brief Updates the widget form based on internal mark element. */
          void updateUi();

          QStringList getTypeNames();

        public slots:

          virtual void valueChanged(QtProperty* p, int value);

          void onStrokeChanged();

          void onFillChanged();

        signals:

          /*! This signal is emitted when the internal mark element is changed. */
          void markChanged();

        protected:

          QtProperty* m_typeProperty;

          te::qt::widgets::BasicFillPropertyItem* m_bf;
          te::qt::widgets::BasicStrokePropertyItem* m_bs;

          te::se::Mark* m_mark;                               //!< Mark element that will be configured by this widget.

          bool m_setLocalMark;
          
          std::vector<std::string> m_supportedMarks;          //!< Names of supported marks.
          std::map<int, std::string> m_supportedMarksMap;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNMARKPROPERTYITEM_H