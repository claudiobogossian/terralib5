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

/*!
 * \file terralib/qt/af/XMLFormatter.h
 *
 * \brief A class for xml serialization formatting strings.
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
      
      /*!
         \class XMLFormatter

         \brief A class that formats strings.

         Use this class for changing strings, contained in some TerraLib objects, to XML percentage encode format and/or to human readable format. Use XML formatting before serializing the object to a XML file, for example, and human readable format after read the object from a XML.

         \ingroup af
       */
      class XMLFormatter
      {
        public:

          /*!
            \brief Formats the project informations.

            \param p The project.

            \param encode \a Pass true to change for XML percentage format and \a false for human readable format.
           */
          static void format(Project* p, const bool& encode);

          /*!
            \brief Formats the layer informations.

            \param l The layer.

            \param encode \a Pass true to change for XML percentage format and \a false for human readable format.

            \note The sublayers will be formatted too, using the \a encode passed.
           */
          static void format(te::map::AbstractLayer* l, const bool& encode);

          /*!
            \brief Formats the data source informations.

            \param d The data source info.

            \param encode \a Pass true to change for XML percentage format and \a false for human readable format.
           */
          static void format(te::da::DataSourceInfo* d, const bool& encode);

          /*!
            \brief Formats all data source informations registered in the \a te::da::DataSourceInfoManager object.

            \param encode \a Pass true to change for XML percentage format and \a false for human readable format.

            \sa te::da::DataSourceInfoManager
           */
          static void formatDataSourceInfos(const bool& encode);

          /*!
            \brief Formats the string.

            \param s The string.

            \param encode \a Pass true to change for XML percentage format and \a false for human readable format.
           */
          static std::string format (const std::string& s, const bool& encode);
      };
      
    } // namespace af
  } // namespace qt
} // namespace te

#endif // TE_QT_AF_XMLFORMATTER_H
