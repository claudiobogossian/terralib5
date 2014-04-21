/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.
  
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


#ifndef TE_QT_AF_XMLFORMATTER_H
#define TE_QT_AF_XMLFORMATTER_H

//STL
#include <string>

namespace te {

  //Forward declarations
  namespace map { class AbstractLayer; }
  namespace da { class DataSourceInfo; }

  namespace qt {
    namespace af {

      // Forward declaration
      class Project;
      
      class XMLFormatter
      {
        public:

          static void format(Project* p, const bool& encode);

          static void format(te::map::AbstractLayer* l, const bool& encode);

          static void format(te::da::DataSourceInfo* d, const bool& encode);

          static void formatDataSourceInfos(const bool& encode);

          static std::string format (const std::string& s, const bool& encode);
      };
      
    } // namespace af
  } // namespace qt
} // namespace te

#endif // TE_QT_AF_XMLFORMATTER_H
