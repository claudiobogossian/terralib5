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
  \file terralib/qt/widgets/rp/RpToolsDialog.h

  \brief This file defines a class for a Raster Navigator Dialog.
*/

#ifndef __TERRALIB_QT_WIDGETS_RP_INTERNAL_RPTOOLSDIALOG_H
#define __TERRALIB_QT_WIDGETS_RP_INTERNAL_RPTOOlSDIALOG_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../maptools/AbstractLayer.h"
#endif
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QDialog>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class RpToolsWidget;

      /*!
        \class RpToolsDialog

        \brief This class is a dialog for the RpTools widget.
      */
      class TEQTWIDGETSEXPORT RpToolsDialog : public QDialog
      {
        Q_OBJECT

        public:

          RpToolsDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~RpToolsDialog();

        public:

          te::qt::widgets::RpToolsWidget* getWidget();

          /*!
            \brief This method is used to set the selected layer for mixture model operation
            
            \param layer The layer ptr 

            \note This layer MUST HAVE a valid raster object.
          */
          void set(te::map::AbstractLayerPtr layer);

        protected:

          void closeEvent(QCloseEvent* e);

        signals:

          void navigatorClosed();


        private:

          std::auto_ptr<te::qt::widgets::RpToolsWidget> m_navigator;

          te::map::AbstractLayerPtr m_layer;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_RP_INTERNAL_RPTOOlSDIALOG_H
