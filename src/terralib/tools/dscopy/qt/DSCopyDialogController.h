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
  \file terralib/tools/dscopy/qt/DSCopyDialogController.h

  \brief Data Source Copy GUI Controller
 */

#ifndef __TERRALIB_TOOLS_DSCOPY_DSCOPYDIALOGCONTROLLER_H
#define __TERRALIB_TOOLS_DSCOPY_DSCOPYDIALOGCONTROLLER_H

// STL
#include <string>
#include <vector>

namespace te
{

  // Forward declarations
  namespace da
  {
    class DataSource;
  }

  namespace tools
  {
    namespace dscopy
    {
      class DSCopyDialogController
      {

        public:
          DSCopyDialogController();
          ~DSCopyDialogController();

          bool setOriginConnectionInfo(std::string dstype, std::string path, std::string& errorMessage);
          bool setOriginConnectionInfo(std::string dstype, std::string host, std::string port, 
            std::string dsname, std::string user, std::string pw, std::string& errorMessage);

          bool setDestinationConnectionInfo(std::string dstype, std::string path, std::string& errorMessage);
          bool setDestinationConnectionInfo(std::string dstype, std::string host, std::string port, 
            std::string dsname, std::string user, std::string pw, std::string& errorMessage);

          void setDatasetsToCopy(std::vector<std::string> datasets);
          std::vector<std::string> getDictionary();
          std::vector<std::string*> getDatasetsName();

          bool copy(std::string& errorMessage);

        private:

          std::vector<std::string> m_datasets;
          te::da::DataSource* m_dsOrigin;       //!< The Origin Data Source Pointer
          te::da::DataSource* m_dsDestination;  //!< The Destination Data Source pointer
      
      };
    }
  }
}

#endif // __TERRALIB_TOOLS_DSCOPY_DSCOPYDIALOGCONTROLLER_H