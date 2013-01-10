/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Project.h

  \brief Application Project
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_PROJECT_H
#define __TERRALIB_QT_AF_INTERNAL_PROJECT_H

// Terralib
#include "Config.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace map
  {
    // Forward declarations
    class AbstractLayer;
  }

  namespace qt
  {
    namespace af
    {

      /*!
        \class Project

        \brief Application Project

      */
      class TEQTAFEXPORT Project
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructor. */
          Project();

          /*! \brief Virtual destructor. */
          ~Project();

          //@}

          /** @name Accessor Methods
           *  Methods used to access internal attributes.
           */
          //@{

          void setTitle(std::string title);

          std::string getTitle();

          void setAuthor(std::string author);

          std::string getAuthor();

          void setLayers(boost::ptr_vector<te::map::AbstractLayer*> layerList);

          boost::ptr_vector<te::map::AbstractLayer*> getLayers();

          void addLayer(te::map::AbstractLayer* layer);

          void removeLayer(te::map::AbstractLayer* layer);

          //@}
        private:

          std::string m_title;                                    //!< Project title.
          std::string m_author;                                   //!< Project author.
          boost::ptr_vector<te::map::AbstractLayer*> m_layerList; //!< Project layers.
      };
    }
  }
}

#endif  // __TERRALIB_QT_AF_INTERNAL_PROJECT_H
