/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file main.cpp

  \brief A list of examples for the TerraLib Widgets.
 */

#include "Config.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/srs.h>

#include <terralib/qt/widgets/srs/SRSManagerDialog.h>

// QT
#include <QApplication>
#include <qmessagebox.h>

// STL
#include <exception>
#include <iostream>

void SRSManagerDialogExample( int argc, char** argv )
{
  QApplication app(argc,argv);
  
  std::pair<int, std::string> srid = te::qt::widgets::SRSManagerDialog::getSRS(0);
  QMessageBox msgBox;
  QString mess = QString("1) SRId: %1, Authority: %2").arg(srid.first).arg(srid.second.c_str()); 
  msgBox.setText(mess);
  msgBox.exec();
   
  te::qt::widgets::SRSManagerDialog* srsManDialog = new te::qt::widgets::SRSManagerDialog();
  
  while (srsManDialog->exec() == QDialog::Accepted)
  {
    const std::pair<int, std::string>& srid = srsManDialog->getSelectedSRS();
    QMessageBox msgBox;
    QString mess = QString("2) SRId: %1, Authority: %2").arg(srid.first).arg(srid.second.c_str()); 
    msgBox.setText(mess);
    msgBox.exec();
  }
  
  app.exec();
  
  // Getting the result
  
    //...
}


int main(int argc, char** argv)
{
  try
  {
      // initialize Terralib support
    TerraLib::getInstance().initialize();
    
    SRSManagerDialogExample(argc, argv);
    
    TerraLib::getInstance().finalize();
    
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred: " << e.what() << std::endl;
    
    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred!" << std::endl;
    
    return EXIT_FAILURE;
  }  
  return EXIT_SUCCESS;
}
