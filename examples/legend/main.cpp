/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://www.qtsoftware.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/


// TerraLib
#include <terralib/color.h>
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/qt/widgets.h>
#include <terralib/qt/widgets/layer/Legend.h>

#include "Legend.h"

//Qt
#include <QApplication>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadModules()
{  
  te::plugin::PluginInfo* info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/plugin_pgis_info.xml"));
  te::plugin::PluginManager::getInstance().add(info);    
  
  te::plugin::PluginManager::getInstance().loadAll();  
}


int main(int argc, char* argv[])
{
  try
  {  
    TerraLib::getInstance().initialize();  
    
    LoadModules();
  
    QApplication app(argc, argv);
    
    Legend* legend = new Legend;
    
    legend->show();
    
    app.exec();
  }
  catch(const te::common::Exception& e)
  {
    std::cout << std::endl << "An exception has occuried:" << std::endl;
    std::cout << e.what() << std::endl;
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    
    return EXIT_FAILURE;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An unexpected exception has occuried!" << std::endl;
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    
    return EXIT_FAILURE;
  }
  
  te::plugin::PluginManager::getInstance().unloadAll();
  
  // finalize Terralib support
  TerraLib::getInstance().finalize();
  
  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();
  
  return EXIT_SUCCESS;

}
