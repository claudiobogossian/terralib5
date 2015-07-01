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
  \file terralib/color/ColorSchemeCatalog.h

  \brief A catalog for color schemes groups.
*/

#ifndef __TERRALIB_COLOR_INTERNAL_COLORSCHEMECATALOG_H
#define __TERRALIB_COLOR_INTERNAL_COLORSCHEMECATALOG_H

// TerraLib
#include "Config.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace color
  {
// Forward declarations
    class ColorSchemeGroup;

    /*!
      \class ColorSchemeCatalog

      \brief A catalog for color schemes.
      
      A color scheme catalog contains several
      color scheme groups that can be choosed by the user
      in order to color a given map.

      \ingroup color

      \sa ColorScheme, ColorSchemeGroup, ColorSchemeCatalogManager

      \note Any doubts, please, send us an e-mail to help improve the documentation quality!
    */
    class TECOLOREXPORT ColorSchemeCatalog
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new ColorSchemeCatalog.

          \param name The catalog name.
         */
        ColorSchemeCatalog(const std::string& name);

        /*! \brief Destructor. */
        ~ColorSchemeCatalog();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the catalog name.

          \return The catalog name.
         */
        const std::string& getName() const;

        /*!
          \brief It changes the catalog name and adjusts its entry in the catalog manager if needed.

          \param newName The new catalog name (must be a non-empty value).
         */
        void setName(const std::string& newName);

        /*!
          \brief It returns the catalog description.

          \return The catalog description.
         */
        const std::string& getDescription() const;

        /*!
          \brief It sets the catalog description.

          \param d The catalog description.
         */
        void setDescription(const std::string& d);

        /*!
          \brief It returns the catalog author name.

          \return The catalog author name.
         */
        const std::string& getAuthor() const;

        /*!
          \brief It sets the catalog author name.

          \param author The catalog author name.
         */
        void setAuthor(const std::string& author);

        /*!
          \brief It returns the catalog copyright notice.

          \return The catalog copyright notice.
         */
        const std::string& getCopyright() const;

        /*!
          \brief It sets the catalog copyright notice.

          \param c The catalog copyright notice.
         */
        void setCopyright(const std::string& c);

        /*!
          \brief It returns a link for more information about the catalog.

          \return A link for more information about the catalog.
         */
        const std::string& getOnlineResource() const;

        /*!
          \brief It sets the link for more information about the catalog.

          \param r The link for more information about the catalog.
         */
        void setOnlineResource(const std::string& r);

        /*!
          \brief It adds a new group to the catalog and sets its relationship.

          \param group The group that will take part of the catalog.

          \note The catalog will take the ownership of the group.

          \note Don't inform a NULL pointer.
         */
        void push_back(ColorSchemeGroup* group);

        /*!
          \brief It removes the internal reference to the group.

          \param group The group to remove the internal reference.

          \note The caller of thi smethod will take the ownership of the group.
         */
        void disconnect(ColorSchemeGroup* group);

        /*!
          \brief It returns the list of color scheme groups in the catalog.

          \return The list of color scheme groups in the catalog.
         */
        const std::vector<ColorSchemeGroup*>& getColorSchemeGroups() const;

        //@}

      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        ColorSchemeCatalog(const ColorSchemeCatalog& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
         */
        ColorSchemeCatalog& operator=(const ColorSchemeCatalog& rhs);

        //@}

      private:

        std::string m_name;                                  //!< Catalog name.                
        std::string m_description;                           //!< A brief description about the catalog.
        std::string m_author;                                //!< Information about catalog author.
        std::string m_copyright;                             //!< Copyright notice.
        std::string m_onlineResource;                        //!< A link location for more information on the catalog.
        std::vector<ColorSchemeGroup*> m_colorSchemeGroups;  //!< The color scheme groups.
    };

  }   // end namespace color
}     // end namespace te

#endif  // __TERRALIB_COLOR_INTERNAL_COLORSCHEMECATALOG_H

