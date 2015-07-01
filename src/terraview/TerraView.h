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
  \file terraview/TerraView.h

  \brief The main class of TerraView.
*/

#ifndef __TERRAVIEW_INTERNAL_TERRAVIEW_H
#define __TERRAVIEW_INTERNAL_TERRAVIEW_H

// TerraLib
#include <terralib/qt/af/BaseApplication.h>

// STL
#include <string>

// Forward declarations
class QWidget;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class HelpManagerImpl;
    }
  }
}

/*!
  \brief The main class of TerraView.

  \sa te::qt::af::BaseApplication
*/
class TerraView : public te::qt::af::BaseApplication
{
  Q_OBJECT

  public:

    TerraView(QWidget* parent = 0);

    ~TerraView();

    void init();

    void init(const std::string& configFile);

    void startProject(const QString& projectFileName);

  protected:

    void makeDialog();

  protected slots:

    void showAboutDialog();

    void onHelpTriggered();

  protected:

    te::qt::widgets::HelpManagerImpl* m_helpManager;
};

#endif  // __TERRAVIEW_INTERNAL_TERRAVIEW_H
