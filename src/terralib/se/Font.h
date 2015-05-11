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
  \file terralib/se/Font.h

  \brief A Font specifies the text font to use in a text symbolizer.
*/

#ifndef __TERRALIB_SE_INTERNAL_FONT_H
#define __TERRALIB_SE_INTERNAL_FONT_H

// TerraLib
#include "Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  {
// Forward declarations
    class SvgParameter;

    /*!
      \class Font
      
      \brief A Font specifies the text font to use in a text symbolizer.

      The allowed SvgParameters are:
      <ul>
      <li>font-family</li>
      <li>font-style: normal, italic, and oblique</li>
      <li>font-weight: normal and bold</li>
      <li>font-size</li>
      </ul>

      \ingroup se

      \sa TextSymbolizer
    */
    class TESEEXPORT Font : public boost::noncopyable
    {
      public:

        /*!
          \enum FontStyleType

          \brief It defines the style to use for a font.
         */
        enum FontStyleType
        {
          StyleNormal,   /*!< Normal font style.  */
          StyleItalic,   /*!< Italic font style.  */
          StyleOblique   /*!< Oblique font style. */
        };

        /*!
          \enum FontWeightType

          \brief It gives the amount of weight or boldness to use for a font.
         */
        enum FontWeightType
        {
          WeightNormal,  /*!< Normal. */
          WeightBold     /*!< Bold.   */
        };

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Font. */
        Font();

        /*! \brief Destructor. */
        ~Font();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief Add a SvgParameter to this Font.

          \note If there is already a SvgParamater with the same name it will be overrided.
          \note The Font object will take the ownership of the informed p pointer.
        */
        void add(SvgParameter* p);

        void setFamily(const std::string& family);

        void setStyle(const FontStyleType& style);

        void setWeight(const FontWeightType& weight);

        void setSize(const std::string& size);

        const SvgParameter* getFamily() const;

        const SvgParameter* getStyle() const;

        const SvgParameter* getWeight() const;

        const SvgParameter* getSize() const;

        //@}

        /*! \brief It creates a new copy of this object. */
        Font* clone() const;

      private:

        void setParameter(const std::string& name, const std::string& value);
        
        const SvgParameter* getParameter(const std::string& name) const;

      private:

        std::map<std::string, SvgParameter*> m_svgParams;              //!< Set of SvgParameters.

        static std::map<FontStyleType, std::string> sm_fontStyleMap;   //!< A map that associates FontStyleType to the correct string value.
        static std::map<FontWeightType, std::string> sm_fontWeightMap; //!< A map that associates FontWeightType to the correct string value.

        static const std::string sm_family;                            //!< SVG/CSS "font-family" parameter.
        static const std::string sm_style;                             //!< SVG/CSS "font-style parameter.
        static const std::string sm_weight;                            //!< SVG/CSS "font-weight" parameter.
        static const std::string sm_size;                              //!< SVG/CSS "font-size" parameter.
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_FONT_H

