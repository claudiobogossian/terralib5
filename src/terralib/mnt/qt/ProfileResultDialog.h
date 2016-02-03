/*  Copyright(C) 2008 National Institute For Space Research(INPE) - Brazil.

This file is part of the TerraLib - a Framework for building GIS enabled applications.

TerraLib is free software : you can redistribute it and / or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License,
or(at your option) any later version.

TerraLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with TerraLib.See COPYING.If not, write to
TerraLib Team at <terralib - team@terralib.org>.
*/


/*!
\file terralib/mnt/qt/ProfileResultDialog.h

\brief A dialog Show Profile Graphic Result
*/

#ifndef __TERRALIB_MNT_INTERNAL_PROFILERESULTDIALOG_H
#define __TERRALIB_MNT_INTERNAL_PROFILERESULTDIALOG_H

// Terralib
#include "../core/Config.h"
#include "../core/Enums.h"
#include "../../geometry/LineString.h"

// STL
#include <list>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class ProfileResultDialogForm; }

namespace te
{
  namespace mnt
  {
    class TEMNTEXPORT ProfileResultDialog : public QDialog
    {
      Q_OBJECT

    public:
      ProfileResultDialog(QString tittle, QString yaxis, std::vector<te::gm::LineString*> &profileSet, std::vector<te::color::RGBAColor>color, QWidget* parent = 0, Qt::WindowFlags f = 0);

      ~ProfileResultDialog();

      protected slots:

      void onHelpPushButtonClicked();
      void onOKPushButtonClicked();

    private:

      std::auto_ptr<Ui::ProfileResultDialogForm> m_ui;

    }; //class ProfileResultDialog
  } //mnt
} //te



#endif //__TERRALIB_MNT_INTERNAL_PROFILERESULTDIALOG_H
