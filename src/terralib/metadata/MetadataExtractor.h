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
 \file terralib/metadata/MetadataExtractor.h
 */


#ifndef __TERRALIB_METADATA_INTERNAL_METADATAEXTRACTOR_H
#define __TERRALIB_METADATA_INTERNAL_METADATAEXTRACTOR_H

namespace te
{
  namespace da { class DataSet; }
  
  namespace md 
  { 
    class MD_Metadata; 
    
//    /*! 
//     \brief Extract some metadata from a layer. 
//     \param l Pointer to a layer. Do not pass null.
//     \return a pointer to a metadata record if success or null otherwise.
//     */
//    TEMDEXPORT MD_Metadata* Extract(const te::map::Layer* l);
    
    /*! 
     \brief Extract some metadata from a layer. 
     \param d Pointer to a layer. Do not pass null.
     \return a pointer to a metadata record if success or null otherwise.
     */
    TEMDEXPORT MD_Metadata* Extract(const te::da::DataSet* d);    
    
  } // end namespace md
}   // end namespace te

#endif // __TERRALIB_METADATA_INTERNAL_METADATAEXTRACTOR_H
