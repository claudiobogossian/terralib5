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

        \ingroup af
      */
      class TEQTAFEXPORT Project
      {
        public:

          /*! \brief Constructor. */
          Project();

          /*! \brief Destructor. */
          ~Project();

          /*!
            \brief It sets the title of the project.

            \param title The title of the project to be set.
          */
          void setTitle(const std::string& title);

          /*!
            \brief It gets the title of the project.

            \return The title of the project.
          */
          const std::string& getTitle() const;

          /*!
            \brief It sets the author of the project.

            \param title The author of the project to be set.
          */
          void setAuthor(const std::string& author);

          /*!
            \brief It gets the author of the project.

            \return The author of the project.
          */
          const std::string& getAuthor() const;

          /*!
            \brief It gets all the top layers of the project (folder and single layers).

            \return The list of all the top layers of the project (folder and single layers).
          */
          const std::list<te::map::AbstractLayerPtr>& getTopLayers() const;

          /*!
            \brief It gets all the top layers of the project (folder and single layers).

            \return The list of all the top layers of the project (folder and single layers).
          */
          std::list<te::map::AbstractLayerPtr>& getTopLayers();

          /*!
            \brief It gets all the layers (single and folder layers) of the project.

            \param invalid If including invalid layers.

            \return The list of all the layers (single and folder layers) of the project.
          */
          std::list<te::map::AbstractLayerPtr> getAllLayers(bool invalid = true);

          /*!
            \brief It gets all the single layers of the project.

            \param invalid If including invalid layers.

            \return The list of all the single layers of the project.
          */
          std::list<te::map::AbstractLayerPtr> getSingleLayers(bool invalid = true);

          /*!
            \brief It gets all the single layers that are visible.

            \param invalid If including invalid layers.

            \return The list of all the single layers that are visible.
          */
          std::list<te::map::AbstractLayerPtr> getVisibleSingleLayers(bool invalid = true);

          /*!
            \brief It sets the top layers of the project.

            \param layers The layers that will be set as the top layers of the project.
          */
          void setTopLayers(const std::list<te::map::AbstractLayerPtr>& layers);

          /*!
            \brief It gets all the layers that are selected.

            \param invalid If including invalid layers.

            \return The list of all the layers that are selected.
          */
          const std::list<te::map::AbstractLayerPtr> getSelectedLayers(bool invalid = true) const;

          /*!
            \brief It sets all the layers that are selected.

            \param  selectedLayers The list of all the layers that are selected.
          */
           void setSelectedLayers(const std::list<te::map::AbstractLayerPtr>& selectedLayers);

          /*!
            \brief It adds the specified layer to the list of layers of the parent layer.
                   If the parent layer is not specified, the layer is added as a top layer of the project.

            \param layer       The layer that will be added to the list of layers of the project.
            \param parentLayer The parent layer where the layer will be added.
          */
          void add(const te::map::AbstractLayerPtr& layer,
                   const te::map::AbstractLayerPtr& parentLayer = te::map::AbstractLayerPtr());

          /*!
            \brief It removes the specified layer from the project.

            \param layer The layer to be removed from the project.
          */
          void remove(const te::map::AbstractLayerPtr& layer);

          /*!
            \brief It informs if the project has changed.

            \return True, if the project has changed; otherwise, it returns false.
          */
          bool hasChanged();

          /*!
            \brief It sets the filename where the project will be saved.

            \param fName The file name where the project will be saved.
          */
          void setFileName(const std::string& fName);

          /*!
            \brief It gets the filename where the project is saved.

            \return The file name where the project is saved.
          */
          const std::string& getFileName() const;

          /*!
            \brief It sets the project status as changed or not.

            \param changed The flag that specifies if the project is to be set as changed or not.
          */
          void setProjectAsChanged(const bool& changed);

          /*!
            \brief It clears the project, setting it as having no layers.
          */
          void clear();

        private:

          std::string m_title;                                      //!< The title of the project.
          std::string m_author;                                     //!< The author of the project.
          std::list<te::map::AbstractLayerPtr> m_topLayers;         //!< The list of top layers of the project.
          std::list<te::map::AbstractLayerPtr> m_selectedLayers;    //!< The list of selected layers of the project.
          bool m_changed;                                           //!< Flag indicating that the project needs to be saved.
          std::string m_fileName;                                   //!< The project file.
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_INTERNAL_PROJECT_H
