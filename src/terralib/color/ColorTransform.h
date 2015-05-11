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
  \file terralib/color/ColorTransform.h

  \brief A class to transform .
*/

#ifndef __TERRALIB_COLOR_INTERNAL_COLOR_TRANSFORM_H
#define __TERRALIB_COLOR_INTERNAL_COLOR_TRANSFORM_H

// TerraLib
#include "Config.h"
#include "RGBAColor.h"

// STL 
#include <cassert>
#include <cstdlib>
#include <climits>
#include <cmath>

namespace te
{
  namespace color
  {
    /*!
      \class ColorTransform

      \brief A class to transform RGBA color to HSV, HSL and CMYK.

      \ingroup color
    */
    class TECOLOREXPORT ColorTransform
    {
      public:
      
        enum Spec { Invalid, Rgb, Hsv, Cmyk, Hsl };
   
        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief Default constructor of a new color. */
        ColorTransform();

        /*!
          \brief It initializes a new with the given color transform.

          \param colt The color transform.
         */
        ColorTransform(const ColorTransform& colt);

        /*!
          \brief It initializes a new color with the given RGBAColor.

          \param color The RGBAColor.
         */
        ColorTransform(const RGBAColor& color);

        /*!
          \brief It initializes a new color with the given rgba value.

          \param rgba The rgba value.
         */
        ColorTransform(const int& rgba);

        /*!
          \brief It initializes a new color.

          \param r Red component (range from 0 to 255).
          \param g Green component (range from 0 to 255).
          \param b Blue component (range from 0 to 255).
          \param a Alpha component. A range from 0 to 255 (see the macro TE_OPAQUE and TE_TRANSPARENT for the values of totally transparent and totally opaque).
         */
        ColorTransform(const int r, const int g, const int b, const int a=255);

        /*! \brief Destructor */
        ~ColorTransform();

        /*!
          \brief Assigns a copy of a color transform and returns a reference to it.

          \param color The color transfom to be copied.

          \return Returns a reference to it.
        */
        te::color::ColorTransform& operator=(const ColorTransform& colt);

        /*!
          \brief It sets the color.

          \param r Red component (range from 0 to 255).
          \param g Green component (range from 0 to 255).
          \param b Blue component (range from 0 to 255).
          \param a Alpha component. A range from 0 to 255 (see the macro TE_OPAQUE and TE_TRANSPARENT for the values of totally transparent and totally opaque).
         */
        void setRgba(int r, int g, int b, int a);

        /*!
          \brief It sets the color.

          \param rgba The color represented by a number of 4 bytes, where the values ??are in the following order: alpha, red, green and blue. The alpha is the most significant byte.
         */
        void setRgba(int rgba);

        /*!
          \brief It sets the color.

          \param cor The RGBA color.
         */
        void setColor(const RGBAColor& cor);

        /*!
          \brief It gets the color value.

          \param r Red component.
          \param g Green component.
          \param b Blue component.
          \param a Alpha component.
         */
        void getRgba(int* r, int* g, int* b, int* a=0) const;

        /*!
          \brief It returns the color value (0xAARRGGBB).

          \return It returns a number of 4 bytes. The values are on the following order: alpha, red, green and blue. The alpha is the most significant byte.
         */
        int getRgba() const;

        /*!
          \brief It returns the RGBA color.

          \return The RGBA color.
         */
        te::color::RGBAColor getColor() const;

        /*!
          \brief It returns the red component.

          \return The red component.
         */
        int getRed() const;

        /*!
          \brief It returns the green component.

          \return The green component.
         */
        int getGreen() const;

        /*!
          \brief It returns the blue component.

          \return The blue component.
         */
        int getBlue() const;

        /*!
          \brief It returns the alpha component.

          \return The alpha component.
         */
        int getAlpha() const;

        /*!
          \brief It sets the red component.
         */
        void setRed(int red);

        /*!
          \brief It sets the green component.
         */
        void setGreen(int green);

        /*!
          \brief It sets the blue component.
         */
        void setBlue(int blue);

        /*!
          \brief It sets the alpha component.
         */
        void setAlpha(int alpha);

        /*!
          \brief It returns the color tranform in Rgb spec.

          \return The color transform in Rgb spec.

          \note If spec is not RGB, it creates and returns an equivalent.
         */
        te::color::ColorTransform toRgb() const;

        /*!
          \brief It returns the color tranform in Hsv spec.

          \return The color transform in Hsv spec.

          \note If spec is not HSV, it creates and returns an equivalent.
         */
        te::color::ColorTransform toHsv() const;

        /*!
          \brief It returns the color tranform in Hsl spec.

          \return The color transform in Hsl spec.

          \note If spec is not HSL, it creates and returns an equivalent.
         */
        te::color::ColorTransform toHsl() const;

        /*!
          \brief It returns the color tranform in Cmyk spec.

          \return The color transform in Cmyk spec.

          \note If spec is not CMYK, it creates and returns an equivalent.
         */
        te::color::ColorTransform toCmyk() const;

        /*!
          \brief It returns the hue component of this color.

          \return The hue value.

          \note The color is implicitly converted to HSV.
        */
        int getHsvHue() const;

        /*!
          \brief It returns the saturation component of this color.

          \return The saturation value.

          \note The color is implicitly converted to HSV.
        */
        int getHsvSaturation() const;

        /*!
          \brief It returns the value component of this color.

          \return The value.

          \note The color is implicitly converted to HSV.
        */
        int getValue() const;

        /*!
          \brief It gets the h, s, v components of this color.

          \param h The hue component.
          \param s The saturation component.
          \param v The value component.
          \param a The alpha component.

          \note The color is implicitly converted to HSV.
        */
        void getHsv(int *h, int *s, int *v, int *a = 0) const;

        /*!
          \brief It sets the h, s, v components of this color.

          \param h The hue component.
          \param s The saturation component.
          \param v The value component.
          \param a The alpha component.

          \note The color is converted to HSV.
        */
        void setHsv(int h, int s, int v, int a = 255);

        /*!
          \brief It gets the cyan component of this color.

          \note The color is implicitly converted to Cmyk.
        */
        int getCyan() const;

        /*!
          \brief It gets the magenta component of this color.

          \note The color is implicitly converted to Cmyk.
        */
        int getMagenta() const;

        /*!
          \brief It gets the yellow component of this color.

          \note The color is implicitly converted to Cmyk.
        */
        int getYellow() const;

        /*!
          \brief It gets the black component of this color.

          \note The color is implicitly converted to Cmyk.
        */
        int getBlack() const;

        /*!
          \brief It gets the c, m, y, k components of this color.

          \param c The cyan component.
          \param m The magenta component.
          \param y The yellow component.
          \param k The black component.
          \param a The alpha component.

          \note The color is implicitly converted to Cmyk.
        */
        void getCmyk(int *c, int *m, int *y, int *k, int *a = 0);

        /*!
          \brief It sets the c, m, y, k components of this color.

          \param c The cyan component.
          \param m The magenta component.
          \param y The yellow component.
          \param k The black component.
          \param a The alpha component.

          \note The color is converted to Cmyk.
        */
        void setCmyk(int c, int m, int y, int k, int a = 255);

        /*!
          \brief It returns the hue component of this color.

          \return The hue value.

          \note The color is implicitly converted to HSL.
        */
        int getHslHue() const;

        /*!
          \brief It returns the saturation component of this color.

          \return The saturation value.

          \note The color is implicitly converted to HSL.
        */
        int getHslSaturation() const;

        /*!
          \brief It returns the lightness component of this color.

          \return The lightness value.

          \note The color is implicitly converted to HSL.
        */
        int getLightness() const;

        /*!
          \brief It gets the h, s, l components of this color.

          \param h The cyan component.
          \param s The magenta component.
          \param l The yellow component.
          \param a The alpha component.

          \note The color is implicitly converted to HSL.
        */
        void getHsl(int *h, int *s, int *l, int *a = 0) const;

        /*!
          \brief It sets the h, s, l components of this color.

          \param h The cyan component.
          \param s The magenta component.
          \param l The yellow component.
          \param a The alpha component.

          \note The color is converted to HSL.
        */
        void setHsl(int h, int s, int l, int a = 255);

        /*!
          \brief It compares the color tranforms are iqual.

          \param colt The color transform to be compared.
            
          \return Returns true if this color transform has the same properties.
       */
        bool operator==(const ColorTransform& colt) const;

        /*!
          \brief It compares the color tranforms are different.

          \param colt The color transform to be compared.
            
          \return Returns true if this color transform has not the same properties.
       */
        bool operator!=(const ColorTransform& colt) const;

        //@}

      private:

        /*!
          \brief Marks the color transform as invalid and sets all components to zero (alpha is set to fully opaque).
         */
        void invalidate();

        /*!
          \brief It checks if the color is valid.

          \return True if valid color.
         */
        inline bool isValid() const;

        /*!
          \brief It rounds the double value to int value.

          \return The int value rounded.
         */
        int round(double d) const;

        /*!
          \brief It compares the double valuea are iqual.

          \param p1 First value.
          \param p2 Second value.

          \return True if values are equal.
         */
        bool fuzzyCompare(double p1, double p2) const;

        /*!
          \brief It verifies the double is null.

          \param d Value to be verified.

          \return True if value is null.
         */
        bool fuzzyIsNull(double d) const;

        template <typename T>
        const T& vMin(const T &a, const T &b) const {if (a < b) return a; return b;}


      private:
        Spec m_spec;

        union 
        {
          struct 
          {
            unsigned short alpha;
            unsigned short red;
            unsigned short green;
            unsigned short blue;
            unsigned short pad;
          } argb;

          struct 
          {
            unsigned short alpha;
            unsigned short hue;
            unsigned short saturation;
            unsigned short value;
            unsigned short pad;
          } ahsv;

          struct 
          {
            unsigned short alpha;
            unsigned short cyan;
            unsigned short magenta;
            unsigned short yellow;
            unsigned short black;
          } acmyk;

          struct 
          {
            unsigned short alpha;
            unsigned short hue;
            unsigned short saturation;
            unsigned short lightness;
            unsigned short pad;
          } ahsl;

          unsigned short array[5];
        } m_params;
    };

    inline void ColorTransform::invalidate()
    {
        m_spec = Invalid;
        m_params.argb.alpha = USHRT_MAX;
        m_params.argb.red = 0;
        m_params.argb.green = 0;
        m_params.argb.blue = 0;
        m_params.argb.pad = 0;
    }

    inline bool ColorTransform::isValid() const
    { 
      return m_spec != Invalid; 
    }

    inline int ColorTransform::round(double d) const
    { 
      return d >= 0.0 ? int(d + 0.5) : int(d - int(d-1) + 0.5) + int(d-1); 
    }

    inline bool ColorTransform::fuzzyCompare(double p1, double p2) const
    {
        return (fabs(p1 - p2) <= 0.000000000001 * vMin(fabs(p1), fabs(p2)));
    }

    inline bool ColorTransform::fuzzyIsNull(double d) const
    {
        return fabs(d) <= 0.000000000001;
    }

    //template <typename T>
    //inline const T& vMin(const T &a, const T &b) const
    //{
    //  if (a < b) 
    //    return a; 
    //  return b; 
    //}

  }   // end namespace color
}     // end namespace te

#endif  // __TERRALIB_COLOR_INTERNAL_COLOR_TRANSFORM_H

