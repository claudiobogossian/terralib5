/*  Copyright (C) 2001-2012 National Institute For Space Research (INPE) - Brazil.
 
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
 
 \brief To hold utilitary functions for the SRS module.
 */


#ifndef __TERRALIB_SRS_INTERNAL_UTILS_H
#define __TERRALIB_SRS_INTERNAL_UTILS_H
namespace te
{
  namespace srs
  {
    /*! 
     \brief This funcions initialize the Coordinate System Manager from a JSON resource file.
     This funcions initialize the Coordinate System Manager from a JSON resource file named "srsjson.txt",
     located within the TERRALIB_INITIALIZATION_FILES path.
     
     \exception It throws exception if the file is not found ou has an invalid sintax.
     \note This function should be called one time only.
     
     */
    void InitManagerFromJSON();
  }
}


#endif // __TERRALIB_SRS_INTERNAL_UTILS_H
