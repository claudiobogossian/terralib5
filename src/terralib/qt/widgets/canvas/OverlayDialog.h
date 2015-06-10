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
  \file terralib/qt/widgets/canvas/OverlayDialog.h

  \brief This file defines a class for a Overlay Dialog.
*/

#ifndef __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_OVERLAYDIALOG_H
#define __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_OVERLAYDIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

// Forward declaraion
namespace Ui { class OverlayDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class OverlayWidget;

      /*!
        \class OverlayDialog

        \brief This class is a dialog for the Overlay widget.
      */
      class TEQTWIDGETSEXPORT OverlayDialog : public QDialog
      {
        public:

          OverlayDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~OverlayDialog();

        public:

          te::qt::widgets::OverlayWidget* getWidget();

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

        private:
          std::auto_ptr<Ui::OverlayDialogForm> m_ui;
          std::auto_ptr<te::qt::widgets::OverlayWidget> m_widgetOverlay;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_OVERLAYDIALOG_H
