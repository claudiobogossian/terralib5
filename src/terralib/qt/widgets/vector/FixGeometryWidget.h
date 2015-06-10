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
  \file terralib/qt/widgets/vector/FixGeometryWidget.h

  \brief A widget to fix geometries.
*/

#ifndef __TERRALIB_QT_WIDGETS_VECTOR_INTERNAL_FIXGEOMETRYWIDGET_H
#define __TERRALIB_QT_WIDGETS_VECTOR_INTERNAL_FIXGEOMETRYWIDGET_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWidget>

namespace Ui { class FixGeometryWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class FixGeometryWidget

        \brief A widget to fix geometries.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT FixGeometryWidget : public QWidget
      {
        Q_OBJECT

        public:

          FixGeometryWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~FixGeometryWidget();

          Ui::FixGeometryWidgetForm* getForm() const;

        public:

          bool fixGeometry();

          void setLayerList(std::list<te::map::AbstractLayerPtr>& layerList);

        protected slots:

          void onTargetDatasourceToolButtonPressed();

          void onTargetFileToolButtonPressed();

        private:

          std::auto_ptr<Ui::FixGeometryWidgetForm> m_ui;
          std::list<te::map::AbstractLayerPtr> m_layers;                //!< List of layers.
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_VECTOR_INTERNAL_FIXGEOMETRYWIDGET_H

