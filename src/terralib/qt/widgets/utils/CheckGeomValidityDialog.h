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
\file terralib/qt/widgets/utils/CheckGeomValidityDialog.h

\brief A dialog used to check geometries validity.
*/

#ifndef __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_CHECKGEOMVALIDITYDIALOG_H
#define __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_CHECKGEOMVALIDITYDIALOG_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

#include "../canvas/MapDisplay.h"

// Qt
#include <QDialog>
#include <QTableWidgetItem>

namespace Ui { class CheckGeomValidityDialogForm; }

namespace te
{
  namespace gm
  {
    struct Coord2D;
  }

  namespace qt
  {
    namespace widgets
    {

      /*!
      \class CheckGeomValidityDialog

      \brief A dialog used to check geometries validity.
      */
      class TEQTWIDGETSEXPORT CheckGeomValidityDialog : public QDialog
      {
        Q_OBJECT

        public:

          CheckGeomValidityDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~CheckGeomValidityDialog();

          void setLayers(std::list<te::map::AbstractLayerPtr> layers, te::map::AbstractLayerPtr selectedLayer = 0);

          void setMapDisplay(te::qt::widgets::MapDisplay* md);

        private:

          void drawMark();

        protected slots:

          void onVerifyPushButtonClicked();

          void onTableWidgetItemDoubleClicked(QTableWidgetItem* item);

          void onExtentChanged();

        private:

          std::auto_ptr<Ui::CheckGeomValidityDialogForm> m_ui;

          te::qt::widgets::MapDisplay* m_mapDisplay;

          te::map::AbstractLayerPtr m_currentLayer;

          std::unique_ptr<te::gm::Point> m_currentCoord;
        
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_UTILS_INTERNAL_CHECKGEOMVALIDITYDIALOG_H
