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
 * \file LayerViewDelegate.h
 *
 * \brief A class for rendering icons for layer and folder itens.
 */
#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERVIEWDELEGATE_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERVIEWDELEGATE_H

#include "../../Config.h"

// TerraLib
#include "../../../../common/Decorator.h"

// Qt
#include <QIcon>
#include <QStyledItemDelegate>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
       * \class LayerViewDelegate
       *
       * \brief A class for rendering icons for layer and folder itens.
       *
       * This is the default delegate for LayerItemView. It's used for present icons for the known kind of items.
       * For unknown items, or if you want to change the known ones, you can create your own delegate and install
       * it on the LayerItemView. Note that we use a decorator instead the QStyledItemDelegate. This is useful for
       * making the inheritance dynamic.
       *
       * \ingroup widgets
       *
       * \sa te::common::Decorator
       */
      class TEQTWIDGETSEXPORT LayerViewDelegate: public te::common::Decorator<QStyledItemDelegate>
      {
        Q_OBJECT

      public:

        /*!
        * \name Instantiation Methods
        *
        * Methods related to creation and destruction of the object.
        */
        //@{

        /*!
        * \brief Constructor.
        *
        * \param decorated The delegate to be decorated.
        *
        * \param parent Used by Qt5.
        */
        LayerViewDelegate(QStyledItemDelegate* decorated, QObject* parent = 0);

        /*!
         * \brief Destructor.
        */
        ~LayerViewDelegate();

        /*!
        * \name Reimplementation of QStyledItemDelegate.
        *
        * Methods reimplemented of the QStyledItemDelegate class.
        */
        //@{
        void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
        //@}

      protected:

        /*!
         * \brief Returns the icon for a given index.
         *
         * \param index Index of the object being rendered.
         *
         * \return The icon for the \a index.
         */
        QIcon getIcon(const QModelIndex & index) const;

        QIcon m_layer;          //!< Icon for a layer not invalid.
        QIcon m_folder;         //!< Icon for a folder.
        QIcon m_colorMap;       //!< Icon for a ColorMap.
        QIcon m_chart;          //!< Icon for a chart.
        QIcon m_queryLayer;     //!< Icon for a query layer.
        QIcon m_tabularLayer;   //!< Icon for a tabular layer. 
        QIcon m_invalidLayer;   //!< Icon for an invalid layers.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERVIEWDELEGATE_H
