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
\file terralib/mnt/qt/VolumeResultDialog.h

\brief A dialog Show Calculate Volume Results
*/

#ifndef __TERRALIB_MNT_INTERNAL_VOLUMERESULTDIALOG_H
#define __TERRALIB_MNT_INTERNAL_VOLUMERESULTDIALOG_H

// Terralib
#include "../core/Config.h"
#include "../core/Enums.h"

// STL
#include <list>
#include <memory>

// Qt
#include <QDialog>

namespace Ui { class VolumeResultDialogForm; }

namespace te
{
  namespace mnt
  {
    class TEMNTEXPORT VolumeResultDialog : public QDialog
    {
      Q_OBJECT

    public:
      VolumeResultDialog(std::vector<std::string> &polyvec,
                         std::vector<std::string> &cortevec,
                         std::vector<std::string> &aterrovec,
                         std::vector<std::string> &areavec,
                         std::vector<std::string> &iquotavec,
                         std::string &attr,
                         QWidget* parent = 0,
                         Qt::WindowFlags f = 0);

      ~VolumeResultDialog();

      void saveVolume(const std::string& path);

      protected slots:

      void onHelpPushButtonClicked();
      void onSavePushButtonClicked();
      void onCancelPushButtonClicked();

    private:

      std::auto_ptr<Ui::VolumeResultDialogForm> m_ui;

    }; //class VolumeResultDialog
  } //mnt
} //te



#endif //__TERRALIB_MNT_INTERNAL_VOLUMERESULTDIALOG_H
