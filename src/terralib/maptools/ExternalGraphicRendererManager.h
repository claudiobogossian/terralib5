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
  \file terralib/maptools/ExternalGraphicRendererManager.h

  \brief This is a singleton for managing all external graphic renderers instances available in the system.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_EXTERNALGRAPHICRENDERERMANAGER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_EXTERNALGRAPHICRENDERERMANAGER_H

// TerraLib
#include "../color/RGBAColor.h"
#include "../common/Singleton.h"
#include "../common/ThreadingPolicies.h"
#include "Config.h"

// STL
#include <vector>

namespace te
{
// Forward declaration
  namespace se
  {
    class ExternalGraphic;
  }

  namespace map
  {
// Forward declaration
    class AbstractExternalGraphicRenderer;

    /*!
      \class ExternalGraphicRendererManager

      \brief This is a singleton for managing all external graphic renderers instances available in the system.

      If you want to render a external graphic, use commands like:
      <code>
      te::color::RBGA** image = te::map::ExternalGraphicRendererManager::getInstance().render(eg, size);
      </code>

      \sa AbstractExternalGraphickRenderer
    */
    class TEMAPEXPORT ExternalGraphicRendererManager : public te::common::ClassLevelLockable<ExternalGraphicRendererManager,
                                                                                 ::boost::recursive_mutex,
                                                                                 ::boost::lock_guard< ::boost::recursive_mutex>,
                                                                                 ::boost::lock_guard< ::boost::recursive_mutex> >,
                                                       public te::common::Singleton<ExternalGraphicRendererManager>
    {
      friend class te::common::Singleton<ExternalGraphicRendererManager>;

      public:

        /*!
          \brief It generates the image pattern from the given Symbology Enconding External Graphic element.

          \param eg     The Symbology Enconding External Graphic element that will be used as base to build the image pattern.
          \param height The height of image pattern that will be generated.
          \param width  An output parameter that will be filled with the width of generated image pattern.

          \return The RGBA image that represents the external graphic.

          \note This method will consult all registered external graphic renderers and the first that can process the given element will be used.

          \note The caller will take the ownership of the returned pointer.

          \exception Exception It will throws an exception if the image pattern can not be generated.
        */
        te::color::RGBAColor** render(const te::se::ExternalGraphic* eg, std::size_t height, std::size_t& width);

        /*!
          \brief It adds a new external graphic renderer to the manager.

          \param renderer The new external graphic renderer that will be added on the manager.

          \note The manager will take the owership of the given external graphic renderer.
        */
        void add(AbstractExternalGraphicRenderer* renderer);

     protected:

        /*! \brief It initializes the singleton instance of the external graphic renderer manager. */
        ExternalGraphicRendererManager();

        /*! \brief Singleton destructor. */
        ~ExternalGraphicRendererManager();

      private:

        std::vector<AbstractExternalGraphicRenderer*> m_renderers; //!< The external graphic renderers in the manager.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_EXTERNALGRAPHICRENDERERMANAGER_H
