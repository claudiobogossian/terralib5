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
  \file terralib/qt/widgets/se/TextPropertyItem.h

  \brief A widget used to define the text se object.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_TEXTPROPERTYITEM_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_TEXTPROPERTYITEM_H

// TerraLib
#include "../Config.h"
#include "../propertybrowser/AbstractPropertyItem.h"

namespace te
{
// Forward declarations
  namespace se
  {
    class Font;
    class TextSymbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
      class BasicFillPropertyItem;

      /*!
        \class TextPropertyItem

        \brief A widget used to define the text se object.

      */

      class TEQTWIDGETSEXPORT TextPropertyItem : public te::qt::widgets::AbstractPropertyItem
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          TextPropertyItem(QtTreePropertyBrowser* pb, QColor c = QColor());

          /*! \brief Destructor. */
          ~TextPropertyItem();

          //@}

          /** @name Re-implementation of Pure Virtual Method
          *   AbstractFillWidget methods.
          */
          //@{

          void setTextSymbolizer(const te::se::TextSymbolizer* ts);

          void setLabels(std::vector<std::string> labels);

          te::se::TextSymbolizer* getTextSymbolizer() const;

          /*! \brief Updates the widget form based on internal fill element. */
          void updateUi();

        public slots:

          virtual void valueChanged(QtProperty* p, int value);

          virtual void valueChanged(QtProperty* p, const QFont &value);

          void onFillChanged();

        signals:

          /*! This signal is emitted when the internal fill element is changed. */
          void textChanged();

        protected:

          te::qt::widgets::BasicFillPropertyItem* m_bf;

          QtProperty* m_labelProperty;
          QtProperty* m_fontProperty;

          te::se::TextSymbolizer* m_textSymbolizer;         //!< Fill element that will be configured by this widget.

          std::vector<std::string> m_labels;

          bool m_update;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_TEXTPROPERTYITEM_H