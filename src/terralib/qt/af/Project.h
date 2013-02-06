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
  \file terralib/qt/af/Project.h

  \brief This class models the concept of a project for the TerraLib Application Framework.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_PROJECT_H
#define __TERRALIB_QT_AF_INTERNAL_PROJECT_H

// Terralib
#include "../../maptools/AbstractLayer.h"
#include "Config.h"

// STL
#include <list>
#include <string>

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class Project

        \brief This class models the concept of a project for the TerraLib Application Framework.
      */
      class TEQTAFEXPORT Project
      {
        public:

          /*! \brief Constructor. */
          Project();

          /*! \brief Destructor. */
          ~Project();

          void setTitle(const std::string& title);

          const std::string& getTitle() const;

          void setAuthor(const std::string& author);

          const std::string& getAuthor() const;

          const std::list<te::map::AbstractLayerPtr>& getLayers() const;

          std::list<te::map::AbstractLayerPtr>& getLayers();

          void add(const te::map::AbstractLayerPtr& layer);

          void remove(const te::map::AbstractLayerPtr& layer);

        private:

          std::string m_title;                            //!< Project title.
          std::string m_author;                           //!< Project author.
          std::list<te::map::AbstractLayerPtr> m_layers;  //!< Project layers.
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_INTERNAL_PROJECT_H
