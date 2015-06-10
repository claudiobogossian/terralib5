/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A Free and Open Source GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*!
  \file terraview/AboutDialog.h

  \brief A Qt dialog showing MGis about window.
*/

#ifndef __TERRAVIEW_INTERNAL_ABOUTDIALOG_H
#define __TERRAVIEW_INTERNAL_ABOUTDIALOG_H

// STL
#include <memory>

// Qt
#include <QDialog>

// Forward declaration
namespace Ui { class AboutDialogForm; }

class AboutDialog : public QDialog
{
  public:

    AboutDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

    ~AboutDialog();

  private:

    std::auto_ptr<Ui::AboutDialogForm> m_ui;
};

#endif  // __TERRAVIEW_INTERNAL_ABOUTDIALOG_H
