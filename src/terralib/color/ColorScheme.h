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
  \file terralib/color/ColorScheme.h

  \brief The concept of color scheme.
*/

#ifndef __TERRALIB_COLOR_INTERNAL_COLORSCHEME_H
#define __TERRALIB_COLOR_INTERNAL_COLORSCHEME_H

// TerraLib
#include "RGBAColor.h"

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
      \class ColorScheme

      \brief It models the concept of color scheme.
      
      It is used to construct color scheme catalogs,
      that helps the user to decide the right colors
      to use in a map. A color scheme has a name
      and a set of list colors. Each list can have a
      certain number of colors.

      \ingroup color
      
      \sa ColorSchemeGroup, ColorSchemeCatalog, ColorSchemeCatalogManager
     */
    class TECOLOREXPORT ColorScheme
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new ColorScheme and adds it to the parent group of schemes.

          \param name Color schema name.
         */
        ColorScheme(const std::string& name);

        /*! \brief Destructor */
        ~ColorScheme();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the color schema name.

          \return The color schema name.
         */
        const std::string& getName() const;

        /*!
          \brief It sets the color schema name and adjust its entry in the scheme group if needed.

          \param name The new color schema name.
         */
        void setName(const std::string& name);

        /*!
          \brief It adds a new list of colors to the color scheme.

          \param colors The new list of colors to be added to the scheme.

          \note The color scheme will take the ownership of the color list.

          \note Don't inform a NULL pointer.
         */
        void push_back(std::vector<RGBAColor>* colors);

        /*!
          \brief It returns all color lists.

          \return All color lists.
         */
        const std::vector<std::vector<RGBAColor>* >& getColors() const;

        /*!
          \brief It returns a color list having the specified number of elements or NULL if none is found.

          This method will look up a list of color in the scheme that has the choosen
          number of colors. If it can not find one, it will return NULL.

          \param size The number of elements needed for the color scheme.

          \return A color list having the specified number of elements or NULL if none is found.

          \note size must be a value greater than 0.
         */
        std::vector<RGBAColor>* getColors(size_t size) const;

        /*!
          \brief It returns the parent scheme group or NULL if it doesn't belong to a group.

          \return The parent scheme group or NULL if it doesn't belong to a group.
         */
        ColorSchemeGroup* getParent() const;

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
        ColorScheme(const ColorScheme& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
         */
        ColorScheme& operator=(const ColorScheme& rhs);

        //@}

        /** @name Auxiliary Methods
         *  Auxiliary methods.
         */
        //@{

        /*!
          \brief It sets the parent color scheme group for this scheme.

          \param parent The parent color scheme group.

          \note This method is intended to be used by a ColorSchemeGroup object.
         */
        void setParent(ColorSchemeGroup* parent);

        //@}

      private:

        ColorSchemeGroup* m_parent;                        //!< A pointer to the parent group.
        std::string m_name;                                //!< Color scheme name.
        std::vector<std::vector<RGBAColor>* > m_colorSet;  //!< A set of color list.

      friend class ColorSchemeGroup;
    };

  }   // end namespace color
}     // end namespace te

#endif  // __TERRALIB_COLOR_INTERNAL_COLORSCHEME_H

