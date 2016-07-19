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
  \file Utils.h

  \brief Terralib Data Source Info Tool Utils 
*/

#ifndef __TERRALIB_TOOLS_DSINFO_UTILS_H
#define __TERRALIB_TOOLS_DSINFO_UTILS_H

// Terralib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/plugin.h>

// STL
#include <iostream>
#include <string>
#include <map>

/*!
  \class Utils

  \brief Class with help methods for the application
*/
class Utils
{
  public:

    /*!
     \brief Load Terralib modules

      \param errorMessage String that receive methods errors description

      \return True if successfully
    */
    static bool loadModules(std::string & errorMessage);

    /*!
     \brief Method to connect with a Data Source
      
      \param dsType Dataq Source Type
      \param dataSource Return DataSource 
      \param errorMessage String that receive methods errors description

      \return True if successfully
    */
    static bool getDataSource(std::string dsType, std::string connStr, te::da::DataSource* & dataSource, std::string & errorMessage);

    static std::string getIdxTypeName(int id);

    /*!
     \brief Getting Primary Key Properties Names

      \param pk Primary Key target

      \return String vector with the Primary Key properties names.
    */
    static std::vector<std::string> getPKPropertiesNames(te::da::PrimaryKey* pk);

    /*!
     \brief Getting Foreign Key Properties Names

      \param fk Foreign Key target

      \return String vector with the Foreign Key properties names.
    */
    static std::vector<std::string> getFKPropertiesNames(te::da::ForeignKey* fk);

    /*!
     \brief Getting Unique Key Properties Names

      \param uk Unique Key target

      \return String vector with the Unique Key properties names.
    */
    static std::vector<std::string> getUKPropertiesNames(te::da::UniqueKey* uk);

    /*!
     \brief Getting Index Properties Names

      \param idx Index Key target

      \return String vector with the Index properties names.
    */
    static std::vector<std::string> getIdxPropertiesNames(te::da::Index* idx);

    static te::da::DataSet* getDataSet(te::da::DataSource* dataSource, std::string dataSetName);

}; 

#endif // __TERRALIB_TOOLS_DSINFO_UTILS_H