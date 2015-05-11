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
  \file terralib/tools/rastermanager/qt/RasterManagerDialog.h

  \brief Raster Manager GUI
 */

#ifndef __TERRALIB_TOOLS_RASTERMANAGER_RASTERMANAGERDIALOG_H
#define __TERRALIB_TOOLS_RASTERMANAGER_RASTERMANAGERDIALOG_H

// TerraLib Raster Manager Tool
#include "../core/RasterManager.h"

// Qt
#include <QtGui/QDialog>
#include <ui_RasterManagerDialog.h>

class RasterManager;

// TerraLib
namespace te
{  
  namespace tools
  {
    namespace rastermanager
    {
      class RasterManagerDialog : public QDialog, public Ui_RasterManagerDialog
      {
        Q_OBJECT

        public:
          RasterManagerDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);

          ~RasterManagerDialog();

        private slots:

          void functionsTabWidget_currentChanged(int id);
          
          void inputPushButton_clicked();
          void outputPushButton_clicked();
          
          void copyPushButton_clicked();
          void reprojectPushButton_clicked();
          void convertPushButton_clicked();
          void resolutionPushButton_clicked();
          void trimPushButton_clicked();
                 
        protected:
          void closeEvent(QCloseEvent* e);

        private:
          RasterManager* m_rm;

      };
    }
  }
}

#endif // __TERRALIB_TOOLS_RASTERMANAGER_RASTERMANAGERDIALOG_H