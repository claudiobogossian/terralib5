/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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

// TerraLib
#include <terralib/common.h>
//#include <terralib/color.h>
//#include <terralib/geometry.h>
//#include <terralib/srs.h>
//#include <terralib/dataaccess.h>
//#include <terralib/maptools.h>
//#include <terralib/postgis.h>
//#include <terralib/common/TerraLib.h>
#include <terralib/qt/widgets.h>

//Qt
#include <QtGui/QApplication>
#include <QtCore/QFile>

#include "MyWindow.h"
#include "STExamples.h"

#ifdef WIN32
#include <windows.h>
#endif
 
unsigned long long lastMemoryMeasured = 0;
unsigned long long getAvailableMemory()
{
  unsigned long long size = 0;

#ifdef WIN32
  MEMORYSTATUSEX ms;
  ms.dwLength = sizeof(ms);
  GlobalMemoryStatusEx(&ms);
  size =  ms.ullAvailPhys;
#endif

  return size;
}

int main(int argc, char *argv[])
{
  TerraLib::getInstance().initialize();

//te::common::Logger::initialize("main");
//QString name = te::common::Logger::getDefaultConfigFile().c_str();
//te::common::Logger::logInfo("main", "..............................................");
//te::common::Logger::logInfo("main", ".................INIT MY_VIEWER...............");
//te::common::Logger::logInfo("main", "..............................................");
//QString msg, msgg;
//unsigned long long maa, mbb;
//maa = getAvailableMemory();
//msg.setNum((qulonglong)maa/(1<<10));
//msg.insert(0, "before main:");
//msg += " KBytes";
//te::common::Logger::logInfo("main", msg.toStdString().c_str());

  LoadModules();

  QApplication app(argc, argv);
  MyWindow* myWindow = new MyWindow;
  myWindow->resize(900, 600);
  myWindow->show();
  int ret = app.exec();

//mbb = getAvailableMemory();
//msg.setNum((qulonglong)mbb/(1<<10));
//msg.insert(0, "after main:");
//msgg.setNum((qulonglong)((mbb - maa) / (1<<10)));
//msg += "  KBytes, liberou:" + msgg + " KBytes";
//te::common::Logger::logInfo("main", msg.toStdString().c_str());
//te::common::Logger::finalize("main");
  TerraLib::getInstance().finalize();

  return ret;
}
