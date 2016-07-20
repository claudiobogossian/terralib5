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
  \file terralib/tools/dscopy/core/DSCopy.h

  \brief An utility class to provide methods to copy a Data Source
 */

#ifndef __TERRALIB_TOOLS_DSCOPY_DSCOPY_H
#define __TERRALIB_TOOLS_DSCOPY_DSCOPY_H

// STL
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
      /*!
        \class DSCopy

        \brief Class with methods to manage the application
      */
      class DSCopy
      {
        public:

          /*!
            \brief Init the application

            \param errorMessage String that receive errors description

            \return True if successfully
          */
          bool init(std::string & errorMessage);

          /*!
            \brief Copy

            \param dsTypeOrigin The type of the origin Data Source
            \param dsConnStrOrigin Origin Data Source connection string
            \param dsTypeDestination The type of the destination Data Source
            \param dsConnStrDestination Destination Data SourceConnection string
            \param datasets DataSet names
            \param errorMessage String that receive errors description

            \return True if successfully
          */
          bool copy(std::string dsTypeOrigin, std::string dsConnStrOrigin, std::string dsTypeDestination, 
            std::string dsConnStrDestination, std::vector<std::string*> datasets, std::string& errorMessage);

          /*!
           \brief Copy

            \param dsOrigin The origin Data Source
            \param dsDestination The destination Data Source
            \param datasets DataSet names
            \param errorMessage String that receive errors description

            \return True if successfully
          */   
          bool copy(te::da::DataSource* dsOrigin, te::da::DataSource* dsDestination, std::vector<std::string*> datasets, std::string& errorMessage);
        
        private:

          /*!
            \brief Copy a list of Data Sets

            \param dsOrigin The origin Data Source
            \param dsDestination The destination Data Source
            \param datasets DataSet names
            \param errorMessage String that receive errors description

            \return True if successfully
          */
          bool copyDataSets(te::da::DataSource* dsOrigin, te::da::DataSource* dsDestination, std::vector<std::string*> datasets, std::string& errorMessage);

      };
    }
  }
}

#endif //__TERRALIB_TOOLS_DSCOPY_DSCOPY_H