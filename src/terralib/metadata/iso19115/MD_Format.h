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
 \file terralib/metadata/MD_Format.h
 
 \brief
 */

#ifndef __TERRALIB_MD_INTERNAL_MDFORMAT_H
#define __TERRALIB_MD_INTERNAL_MDFORMAT_H

// TerraLib
#include "../Config.h"


// STL
#include <string>

namespace te
{
  namespace md
  {
    /*!
     \class MD_Format
     
     \brief Format of the data
     */
    struct TEMDEXPORT MD_Format
    {
    public:
      
      MD_Format(const std::string& name, const std::string& version):
      m_name(name),
      m_version(version) {}

      std::string m_name; 
      std::string m_version;
      std::string m_amendmentNumber;
      std::string m_specification;
      std::string m_fileDecompressionTechnique;
      
    };
    
  } // end namespace md
}   // end namespace te

#endif  // __TERRALIB_MD_INTERNAL_MDFORMAT_H

