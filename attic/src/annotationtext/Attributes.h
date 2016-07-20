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
  \file Text.h

  \brief An Text Element consists of a string, location, leader line and may have attribute.
 */

#ifndef __TERRALIB_ANNOTATIONTEXT_INTERNAL_ATTRIBUTES_H
#define __TERRALIB_ANNOTATIONTEXT_INTERNAL_ATTRIBUTES_H

// TerraLib
#include "../common/STLUtils.h"
#include "Config.h"
#include "Enums.h"

// STL
#include <string>

namespace te
{
// Forward declarations
  namespace gm
  {
    class Envelope;
  }

  namespace at
  {
    /*!
      \class Text

      \brief A Text may contain 1 or more Text Elements.
     */
    class TEANNOTATIONTEXTEXPORT Attributes
    {
      public:

        /*! \brief Text Attributes constructor. */
        Attributes();

        /*! \brief Text Attributes constructor.

          \param family The font family.
          \param pointSize The font point size
          \param style The font style.
          \param weight The font weight.
          \param ha The text horizontal alignment.
          \param va The text vertical alignment.
         */
        Attributes(const std::string& family,
                   const double pointSize = 10.0,
                   FontStyle style = NormalStyle,
                   FontWeight weight = NormalWeight,
                   HorizontalAlignment ha = CenterH,
                   VerticalAlignment va = Baseline);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        Attributes(const Attributes& rhs);

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        Attributes& operator=(const Attributes& rhs);

        /*! \brief Text Element destructor. */
        ~Attributes();

        /*! \brief It sets the font family.

          \param family The font family.
         */
        void setFamily(const std::string& family);

        /*! \brief It returns the font family.

          \return The font family.
         */
        const std::string& getFamily() const;

        /*! \brief It sets the font point size.

          \param ps The font point size.
         */
        void setPointSize(const double& ps);

        /*! \brief It returns the font point size.

          \return The font point size.
         */
        const double& getPointSize() const;

        /*! \brief It sets the font style.

          \param style The font style.
         */
        void setStyle(FontStyle style);

        /*! \brief It returns the font style.

          \return The font style.
         */
        FontStyle getStyle() const;

        /*! \brief It sets the font weight.

          \param weight The font weight.
         */
        void setWeight(FontWeight weight);

        /*! \brief It returns the font weight.

          \return The font weight.
         */
        FontWeight getWeight() const;

        /*! \brief It sets the font decoration.

          \param decoration The font decoration.
         */
        void setDecoration(TextDecoration decoration);

        /*! \brief It returns the font decoration.

          \return The font decoration.
         */
        TextDecoration getDecoration() const;

        /*! \brief It sets the letter spacing.

          \param spacing The letter spacing.
         */
        void setLetterSpacing(const double& spacing);

        /*! \brief It returns the letter spacing.

          \return The letter spacing.
         */
        const double& getLetterSpacing() const;

        /*! \brief It sets the word spacing.

          \param spacing The word spacing.
         */
        void setWordSpacing(const double& spacing);

        /*! \brief It returns the word spacing.

          \return The word spacing.
         */
        const double& getWordSpacing() const;

        /*! \brief It sets the text color.

          \param color The text color.
         */
        void setTextColor(const std::string& color);

        /*! \brief It returns the text color.

          \return The text color.
         */
        const std::string& getTextColor() const;

        /*! \brief It sets the text opacity.

          \param opacity The text opacity.
         */
        void setTextOpacity(double& opacity);

        /*! \brief It returns the text opacity.

          \return The text opacity.
         */
        const double& getTextOpacity() const;

        /*! \brief It sets the text stroke color.

          \param color The text stroke color.
         */
        void setTextStrokecolor(const std::string& color);

        /*! \brief It returns the text stroke color.

          \return The text stroke color.
         */
        const std::string& getTextStrokeColor() const;

        /*! \brief It sets the text stroke width.

          \param width The text stroke width.
         */
        void setTextStrokeWidth(const double& width);

        /*! \brief It returns the text stroke width.

          \return The text stroke width.
         */
        const double& getTextStrokeWidth() const;

        /*! \brief It sets the text stroke opacity.

          \param opacity The text stroke opacity.
         */
        void setTextStrokeOpacity(const double& opacity);

        /*! \brief It returns the text stroke opacity.

          \return The text stroke opacity.
         */
        const double& getTextStrokeOpacity() const;

        /*! \brief It sets the horizontal text alignment.

          \param ha The horizontal text alignment.
         */
        void setHorizontalAlignment(HorizontalAlignment ha);

        /*! \brief It returns the horizontal text alignment.

          \return The horizontal text alignment.
         */
        HorizontalAlignment getHorizontalAlignment() const;

        /*! \brief It sets the vertical text alignment.

          \param ha The vertical text alignment.
         */
        void setVerticalAlignment(VerticalAlignment va);

        /*! \brief It returns the vertical text alignment.

          \return The vertical text alignment.
         */
        VerticalAlignment getVerticalAlignment() const;

        /*! \brief It sets the multi line justification.

          \param just The multi line justification.
         */
        void setLineJustification(LineJustification just);

        /*! \brief It returns the multi line justification.

          \return The multi line justification.
         */
        LineJustification getLineJustification() const;

        /*! \brief It sets the multi line spacing.

          \param spacing The multi line spacing.
         */
        void setLineSpacing(const double& spacing);

        /*! \brief It returns the multi line spacing.

          \return The multi line spacing.
         */
        const double& getLineSpacing() const;

      private:

        std::string         m_family;                 //!< font family
        double              m_pointSize;              //!< font point size
        FontStyle           m_style;                  //!< font style
        FontWeight          m_weight;                 //!< font weight
        TextDecoration      m_decoration;             //!< text decoration
        double              m_letterSpacing;          //!< letter spacing (0..1)
        double              m_wordSpacing;            //!< word spacing (0..1)
        std::string         m_fillColor;              //!< text fill color
        double              m_fillOpacity;            //!< text fill color opacity (0..1)
        std::string         m_strokeColor;            //!< text stroke color
        double              m_strokeWidth;            //!< text stroke width (0..1)
        double              m_strokeOpacity;          //!< text stroke color opacity (0..1)
        HorizontalAlignment m_hAlign;                 //!< horizontal text alignment
        VerticalAlignment   m_vAlign;                 //!< vertical text alignment
        LineJustification   m_multiLineJustification; //!< multi line justufucation
        double              m_multiLineSpacing;       //!< multi line spacing
    };

  } // end namespace at
}   // end namespace te

#endif  // __TERRALIB_ANNOTATIONTEXT_INTERNAL_ATTRIBUTES_H

