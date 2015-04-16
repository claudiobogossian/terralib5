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
  \file terralib/qt/widgets/vector/FixGeometryDialog.h

  \brief A dialog to fix geometries.
*/

#ifndef __TERRALIB_QT_WIDGETS_VECTOR_INTERNAL_FIXGEOMETRYDIALOG_H
#define __TERRALIB_QT_WIDGETS_VECTOR_INTERNAL_FIXGEOMETRYDIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL

#include <memory>

// Qt
#include <QDialog>

namespace Ui { class FixGeometryDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class FixGeometryWidget;

      /*!
        \class FixGeometryDialog

        \brief A dialog to fix geometries.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT FixGeometryDialog : public QDialog
      {
        Q_OBJECT

        public:

          FixGeometryDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~FixGeometryDialog();

          FixGeometryWidget* getWidget() const;

        public:

          void setLayerList(std::list<te::map::AbstractLayerPtr>& layerList);

        protected slots:

          void onOkPushButtonClicked();

          void onCancelPushButtonClicked();

        private:

          std::auto_ptr<Ui::FixGeometryDialogForm> m_ui;
          std::auto_ptr<FixGeometryWidget> m_widget;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_VECTOR_INTERNAL_FIXGEOMETRYDIALOG_H

