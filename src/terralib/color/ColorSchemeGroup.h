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
  \file terralib/color/ColorSchemeGroup.h

  \brief This class represents a group of color schemes.
*/

#ifndef __TERRALIB_COLOR_INTERNAL_COLORSCHEMEGROUP_H
#define __TERRALIB_COLOR_INTERNAL_COLORSCHEMEGROUP_H

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
    class ColorScheme;
    class ColorSchemeCatalog;

    /*!
      \class ColorSchemeGroup

      \brief This class represents a group of color schemes.
      
      It is used to group color schemes.

      \ingroup color
      
      \sa ColorScheme, ColorSchemeCatalog, ColorSchemeCatalogManager
    */
    class TECOLOREXPORT ColorSchemeGroup
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new ColorSchemeGroup and adds it to the parent catalog.

          \param name   Color scheme group name.
         */
        ColorSchemeGroup(const std::string& name);

        /*! \brief Destructor. */
        ~ColorSchemeGroup();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the group name.

          \return The group name.
         */
        const std::string& getName() const;

        /*!
          \brief It sets the group name and adjust its entry in its the catalog if needed.

          \param name The group name.
         */
        void setName(const std::string& name);

        /*!
          \brief It returns the group description.

          \return The group description.
         */
        const std::string& getDescription() const;

        /*!
          \brief It sets the group description.

          \param d The group description.
         */
        void setDescription(const std::string& d);

        /*!
          \brief It adds a new color scheme to the group and sets its relationship.

          \param colorScheme The color scheme that will take part of the group.

          \note The color group will take the ownership of the color scheme.

          \note Don't inform a NULL pointer.
         */
        void push_back(ColorScheme* colorScheme);

        /*!
          \brief It just remove the reference to the informed color scheme.

          \param colorScheme The color scheme to be disconnected from this group.

          \note The caller of this method will take the ownership of the color scheme.
         */
        void disconnect(ColorScheme* colorScheme);

        /*!
          \brief It returns a reference to the list of color schemes belonging to this group.

          \return A reference to the list of color schemes belonging to this group.
         */
        const std::vector<ColorScheme*>& getColorSchemes() const;

        /*!
          \brief It returns the parent catalog or NULL if it doesn't belong to a catalog.

          \return The parent catalog or NULL if it doesn't belong to a catalog.
         */
        ColorSchemeCatalog* getParent() const;

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
        ColorSchemeGroup(const ColorSchemeGroup& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
         */
        ColorSchemeGroup& operator=(const ColorSchemeGroup& rhs);

        //@}

        /** @name Auxiliary Methods
         *  Auxiliary methods.
         */
        //@{

        /*!
          \brief It sets the parent color catalog for this group.

          \param parent The parent catalog.
          
          \note This method is intended to be used by a ColorSchemeCatalog object.
         */
        void setParent(ColorSchemeCatalog* parent);

        //@}

      private:

        ColorSchemeCatalog* m_parent;                //!< The parent catalog.
        std::string m_name;                          //!< Color scheme name.
        std::string m_description;                   //!< A brief description about the color scheme, that can helps someone to decide to use it or not (when its use can be best applied).
        std::vector<ColorScheme*> m_colorSchemes;    //!< The group of color schemes.

      friend class ColorSchemeCatalog;
    };

  }   // end namespace color
}     // end namespace te

#endif  // __TERRALIB_COLOR_INTERNAL_COLORSCHEMEGROUP_H

