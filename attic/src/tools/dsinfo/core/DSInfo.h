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
  \file DSInfo.h

  \brief Data Source Informations tool
 */

#ifndef __TERRALIB_TOOLS_DSINFO_DSINFO_H
#define __TERRALIB_TOOLS_DSINFO_DSINFO_H

// DSInfo
#include <Utils.h>

// TerraLib
#include <terralib/common.h>
#include <terralib/common/TerraLib.h>
#include <terralib/common/AbstractFactory.h>
#include <terralib/plugin/Plugin.h>
#include <terralib/plugin/PluginManager.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/datatype.h>

// STL
#include <iostream>

struct PKInfo;
struct FKInfo;     
struct UKInfo;    
struct IdxInfo;
struct CCInfo;
struct DataSetStruct;  
struct DataSourceHeader;

/*!
  \class DSInfo

  \brief Class with methods to manage the application
*/
class DSInfo
{
  public: 
    
    /*!
     \brief Class Constructor
    */
    DSInfo();
     
    /*!
     \brief Init the application

      \param dstype Data Source Type
      \param connstr Data Source Connection String

      \return True if successfully
    */
    bool init(std::string dstype, std::string connstr);   

    /*!
     \brief Get the Data Source Capabilities
           
      \return String/String Map with the Data Source Capabilities
    */
    std::map<std::string, std::string> getCapabilities();
        
    /*!
     \brief Get the Data Source Header
           
      \return Data Source Header struct
    */
    DataSourceHeader getDataSourceHeader();

    /*!
     \brief Get all Data Sets informations
           
      \return DataSetStruct Vector
    */
    std::vector<DataSetStruct> getDataSetsInfo();

     /*!
     \brief Get all Data Sets names
           
      \return String vector with the Names
    */
    std::vector<std::string*> getDataSetNameList();

    /*!
     \brief Get Data Source Types connection parameters

      \param dataSourceType Data Source Type, if passed by the user
           
      \return String/String Vector Map with the Data Source Types connection parameters
    */
    std::map<std::string, std::vector<std::string>> getDataSourceParameters(std::string dataSourceType);

    /*!
     \brief Get specific Data Set information

      \param dataSetName Data Set Name
           
      \return DataSetStruct with the informations
    */
    DataSetStruct getDataSetInfo(std::string dataSetName);
  private:
   
    /*!
     \brief Get primary key information

      \param dataSetName Data Set Name
           
      \return PKInfo struct
    */
    PKInfo getPrimaryKeyInfo(std::string dataSetName);

    /*!
     \brief Get foreign keys information

      \param dataSetName Data Set Name
           
      \return FKInfo struct vector
    */
    std::vector<FKInfo> getForeignKeyInfo(std::string dataSetName);

    /*!
     \brief Get unique keys information

      \param dataSetName Data Set Name
           
      \return UKInfo struct vector
    */
    std::vector<UKInfo> getUniqueKeyInfo(std::string dataSetName);

    /*!
     \brief Get indexes information

      \param dataSetName Data Set Name
           
      \return IdxInfo struct vector
    */
    std::vector<IdxInfo> getIndexesKeyInfo(std::string dataSetName);

    /*!
     \brief Get check constraint information

      \param dataSetName Data Set Name
           
      \return CCInfo struct vector
    */
    std::vector<CCInfo> getCheckConstraintInfo(std::string dataSetName);    
    

  private:
    std::string m_dataSourceType;     //!< The Data Source Type
    std::string m_connStr;            //!< The Data Source Connection String
    te::da::DataSource* m_dataSource; //!< The Data Source

};

/*!
  \brief Struct that stores information about a primary key using the most common data types
*/
struct PKInfo
{
  std::string name;                     //!< Primary Key Name
  std::vector<std::string> porperties;  //!< Primary Key Properties in a string vector
};
    
/*!
  \brief Struct that stores information about a foreign key using the most common data types
*/
struct FKInfo
{
  std::string name;                     //!< Foreign Key Name
  std::vector<std::string> porperties;  //!< Foreign Key Properties in a string vector
};
  
/*!
  \brief Struct that stores information about a unique key using the most common data types
*/
struct UKInfo
{
  std::string name;                     //!< Unique Key Name
  std::vector<std::string> porperties;  //!< Unique Key Properties in a string vector
};
    
/*!
  \brief Struct that stores information about a index using the most common data types
*/
struct IdxInfo
{
  std::string name;                     //!< Index Name
  std::string type;                     //!< Index Type
  std::vector<std::string> porperties;  //!< Index Properties in a string vector  
};

/*!
  \brief Struct that stores information about a check constraint using the most common data types
*/
struct CCInfo
{
  std::string name;       //!< Check Constraint Name
  std::string expression; //!< Check Constraint Expression
};

/*!
  \brief Struct that stores information about a data set using the most common data types
*/
struct DataSetStruct
{
  std::string name;                 //!< Data Set Name
  bool hasPk;                       //!< If has Primary key
  PKInfo pkInfo;                    //!< Primary Key struct
  std::vector<FKInfo> vecFkInfo;    //!< Foreign Key struct vector
  std::vector<UKInfo> vecUkInfo;    //!< Unique Key struct vector
  std::vector<IdxInfo> vecIdxInfo;  //!< Index struct vector
  std::vector<CCInfo> vecCcInfo;    //!< Check Constraint struct vector
};    

/*!
  \brief Struct that stores information about a data source header
*/
struct DataSourceHeader
{
  std::string name; //!< Data Source Name
  std::string type; //!< Data Source Type
};

#endif // __TERRALIB_TOOLS_DSINFO_DSINFO_H