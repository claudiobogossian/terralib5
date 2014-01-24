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
  \file terralib/qt/widgets/classification/ROIManagerDialog.h

  \brief This file has the ROIManagerDialog class.
*/

#ifndef __TERRALIB_QT_WIDGETS_CLASSIFICATION_INTERNAL_ROIMANAGERDIALOG_H
#define __TERRALIB_QT_WIDGETS_CLASSIFICATION_INTERNAL_ROIMANAGERDIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QDialog>

// Forward declaraion
namespace Ui { class ROIManagerDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class ROIManagerWidget;

      /*!
        \class ROIManagerDialog

        \brief This class is a dialog for the ROI Manager widget.
      */
      class TEQTWIDGETSEXPORT ROIManagerDialog : public QDialog
      {
        Q_OBJECT

        public:

          ROIManagerDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ROIManagerDialog();

        public:

          te::qt::widgets::ROIManagerWidget* getWidget();

          /*!
            \brief This method is used to set the list of layers
            
          */
          void setList(std::list<te::map::AbstractLayerPtr>& layerList);
          
          /*!
            \brief This method is used to set current layer
            
          */
          void set(te::map::AbstractLayerPtr layer);

        protected:

          void closeEvent(QCloseEvent* e);

        signals:

          void roiManagerClosed();

        private:

          std::auto_ptr<Ui::ROIManagerDialogForm> m_ui;

          std::auto_ptr<te::qt::widgets::ROIManagerWidget> m_widgetROIWidget;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CLASSIFICATION_INTERNAL_ROIMANAGERDIALOG_H
