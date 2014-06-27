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
  \file terralib/qt/widgets/se/AbstractPropertyItem.h

  \brief A widget used to define the main property items
         that can be used to describe a se object.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_ABSTRACTPROPERTYITEM_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_ABSTRACTPROPERTYITEM_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantPropertyManager>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class AbstractPropertyItem

        \brief A widget used to define the main property items
              that can be used to describe a se object.

      */

      class TEQTWIDGETSEXPORT AbstractPropertyItem : public QWidget
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          AbstractPropertyItem(QtTreePropertyBrowser* pb, QColor c = QColor());

          /*! \brief Destructor. */
          ~AbstractPropertyItem();

          //@}

        protected:

          void addProperty(QtProperty *property, const QString &id, QColor c);

          void setExpandedStatus(QtBrowserItem* item, bool status);

        public slots:

          virtual void valueChanged(QtProperty* p, int value);
          virtual void valueChanged(QtProperty* p, double value);
          virtual void valueChanged(QtProperty* p, const QString &value);
          virtual void valueChanged(QtProperty* p, const QColor &value);
          virtual void valueChanged(QtProperty* p, const QFont &value);
          virtual void valueChanged(QtProperty* p, const QSize &value);
          virtual void valueChanged(QtProperty* p, const QPointF &value);

        protected:

          QtTreePropertyBrowser*    m_propertyBrowser;

          QMap<QtProperty *, QString> m_propertyToId;
          QMap<QString, QtProperty *> m_idToProperty;

          QColor m_colorParent;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_ABSTRACTPROPERTYITEM_H