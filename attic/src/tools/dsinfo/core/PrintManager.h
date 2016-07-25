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
  \file PrintManager.h

  \brief Informations printing manager
 */

#ifndef __TERRALIB_TOOLS_DSINFO_PRINTMANAGER_H
#define __TERRALIB_TOOLS_DSINFO_PRINTMANAGER_H

// DSInfo
#include <DSInfo.h>

// STL
#include <iostream>
#include <vector>

/*!
  \class PrintManager

  \brief Class with methods to print the Data SOurce information
*/
class PrintManager
{
  
  public:

    /*!
     \brief Init

      \param dstype Data Source Type
      \param connstr Data Source Connection String

      \return True if successfully
    */
    bool init(std::string dstype,std::string connstr);

    /*!
     \brief Print Data Source Parameters 

      \param dstype Data Source Type
      
      \return void
    */
    void printDataSourceParameters(std::string dsType);

    /*!
     \brief Print All Data Source Informations

      \param pkDetails If want Primary Key Details
      \param fkDetails If want Foreign Key Details
      \param ukDetails If want Unique Key Details
      \param idxDetails If want Index Key Details
      \param ccDetails If want Check Constraint Details
      
      \return void
    */
    void printAllInfo(bool pkDetails, bool fkDetails, bool ukDetails, bool idxDetails, bool ccDetails);

    /*!
     \brief Print Data Set Name List
                
      \return void
    */
    void printDataSetNameList();

    /*!
     \brief Print Data Set Informations

      \param dataSetNameList String vector with the passed Data Set Names
      \param pkDetails If want Primary Key Details
      \param fkDetails If want Foreign Key Details
      \param ukDetails If want Unique Key Details
      \param idxDetails If want Index Key Details
      \param ccDetails If want Check Constraint Details
      
      \return void
    */
    void printDataSetInfo(std::vector<std::string> dataSetNameList, bool pkDetails, bool fkDetails, bool ukDetails, bool idxDetails, bool ccDetails);
    
    /*!
     \brief Print Data Set Informations

      \param dataSetName String with the passed Data Set Name
      \param pkDetails If want Primary Key Details
      \param fkDetails If want Foreign Key Details
      \param ukDetails If want Unique Key Details
      \param idxDetails If want Index Key Details
      \param ccDetails If want Check Constraint Details
      
      \return void
    */
    void printDataSetInfo(std::string dataSetName, bool pkDetails, bool fkDetails, bool ukDetails, bool idxDetails, bool ccDetails);
    
  private:
 
    /*!
     \brief Print Data Source Header
                
      \return void
    */
    void printHeader();

    /*!
     \brief Print Data Source Catalog

      \param pkDetails If want Primary Key Details
      \param fkDetails If want Foreign Key Details
      \param ukDetails If want Unique Key Details
      \param idxDetails If want Index Key Details
      \param ccDetails If want Check Constraint Details
           
      \return void
    */
    void printCatalog(bool pkDetails, bool fkDetails, bool ukDetails, bool idxDetails, bool ccDetails);

    /*!
     \brief Print Data Source Capabilities
                
      \return void
    */
    void printCapabilities();

  
    /*!
     \brief Print Primary Key Details

      \param pk PKInfo struct
           
      \return void
    */
    void printPKDetails(struct PKInfo pk);

    /*!
     \brief Print Foreign Key Details

      \param fk FKInfo struct
           
      \return void
    */
    void printFKDetails(struct FKInfo fk);

    /*!
     \brief Print Unique Key Details

      \param uk UKInfo struct
           
      \return void
    */
    void printUKDetails(struct UKInfo uk);

    /*!
     \brief Index Details

      \param idx IdxInfo struct
           
      \return void
    */
    void printIdxDetails(struct IdxInfo idx);

    /*!
     \brief Print Check Constraint Details

      \param cc CCInfo struct
           
      \return void
    */
    void printCCDetails(struct CCInfo cc);

  private:
  
    DSInfo m_dsi; //!< DSInfo object

};

#endif //__TERRALIB_TOOLS_DSINFO_PRINTMANAGER_H