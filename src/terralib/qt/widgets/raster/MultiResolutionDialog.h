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
  \file terralib/qt/widgets/raster/MultiResolutionDialog.h

  \brief A dialog to create multi resolution over a raster.
*/

#ifndef __TERRALIB_QT_WIDGETS_RASTER_INTERNAL_MULTIRESOLUTIONDIALOG_H
#define __TERRALIB_QT_WIDGETS_RASTER_INTERNAL_MULTIRESOLUTIONDIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL

#include <memory>

// Qt
#include <QDialog>

namespace Ui { class MultiResolutionDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MultiResolutionWidget;

      /*!
        \class MultiResolutionDialog

        \brief A dialog to create multi resolution over a raster.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT MultiResolutionDialog : public QDialog
      {
        Q_OBJECT

        public:

          MultiResolutionDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~MultiResolutionDialog();

          MultiResolutionWidget* getWidget() const;

        public:

          void setLayerList(std::list<te::map::AbstractLayerPtr>& layerList);

        protected slots:

          void onOkPushButtonClicked();

        private:

          std::auto_ptr<Ui::MultiResolutionDialogForm> m_ui;
          std::auto_ptr<MultiResolutionWidget> m_widget;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RASTER_INTERNAL_MULTIRESOLUTIONDIALOG_H

