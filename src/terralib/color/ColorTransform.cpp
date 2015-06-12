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
  \file terralib/color/ColorTransform.cpp

  \brief Implementation of the color transform concept.
*/

// TerraLib
#include "ColorTransform.h"

te::color::ColorTransform::ColorTransform()
{
  invalidate();
}

te::color::ColorTransform::ColorTransform(const ColorTransform& colt)
{
  m_spec = colt.m_spec;
  m_params = colt.m_params;
}

te::color::ColorTransform::ColorTransform(const RGBAColor& color)
{
  m_spec = Rgb;
  m_params.argb.alpha = (unsigned short)(color.getAlpha() * 0x101);
  m_params.argb.red = (unsigned short)(color.getRed() * 0x101);
  m_params.argb.green = (unsigned short)(color.getGreen() * 0x101);
  m_params.argb.blue = (unsigned short)(color.getBlue() * 0x101);
  m_params.argb.pad = 0;
}

te::color::ColorTransform::ColorTransform(const int& rgba)
{
  m_spec = Rgb;
  m_params.argb.alpha = (unsigned short)(((rgba >> 24) & 0xFF) * 0x101);
  m_params.argb.red = (unsigned short)(((rgba >> 16) & 0xFF) * 0x101);
  m_params.argb.green = (unsigned short)(((rgba >> 8) & 0xFF) * 0x101);
  m_params.argb.blue = (unsigned short)((rgba & 0xFF) * 0x101);
  m_params.argb.pad = 0;
}

te::color::ColorTransform::ColorTransform(const int r, const int g, const int b, const int a)
{
  m_spec = Rgb;
  m_params.argb.alpha = (unsigned short)(a * 0x101);
  m_params.argb.red = (unsigned short)(r * 0x101);
  m_params.argb.green = (unsigned short)(g * 0x101);
  m_params.argb.blue = (unsigned short)(b * 0x101);
  m_params.argb.pad = 0;
}

te::color::ColorTransform::~ColorTransform()
{
}

te::color::ColorTransform& te::color::ColorTransform::operator=(const ColorTransform& colt)
{
  m_spec = colt.m_spec;
  m_params = colt.m_params;
  return *this;
}


void te::color::ColorTransform::setRgba(int r, int g, int b, int a)
{
  m_spec = Rgb;
  m_params.argb.alpha = (unsigned short)(a * 0x101);
  m_params.argb.red = (unsigned short)(r * 0x101);
  m_params.argb.green = (unsigned short)(g * 0x101);
  m_params.argb.blue = (unsigned short)(b * 0x101);
  m_params.argb.pad = 0;
}

void te::color::ColorTransform::setRgba(int rgba)
{
  int a = (rgba >> 24) & 0xff; 
  int r = (rgba >> 16) & 0xff; 
  int g = (rgba >> 8) & 0xff; 
  int b = rgba & 0xff; 
  setRgba(r, g, b, a);
}

void te::color::ColorTransform::setColor(const RGBAColor& cor)
{
  setRgba(cor.getRgba());
}

void te::color::ColorTransform::getRgba(int* r, int* g, int* b, int* a) const
{
  if (!r || !g || !b)
    return;

  if (m_spec != Invalid && m_spec != Rgb) 
  {
    toRgb().getRgba(r, g, b, a);
    return;
  }

  *r = m_params.argb.red   >> 8;
  *g = m_params.argb.green >> 8;
  *b = m_params.argb.blue  >> 8;

  if (a)
    *a = m_params.argb.alpha >> 8;
}

int te::color::ColorTransform::getRgba() const
{
  int r, g, b, a;

  getRgba(&r, &g, &b, &a);

  int rgba = (a << 24) + (r << 16) + (g << 8) + b;
  return rgba;
}

te::color::RGBAColor te::color::ColorTransform::getColor() const
{
  RGBAColor cor = getRgba();
  return cor;
}

int te::color::ColorTransform::getRed() const
{
  if (m_spec != Invalid && m_spec != Rgb)
    return toRgb().getRed();
  return m_params.argb.red >> 8;
}

int te::color::ColorTransform::getGreen() const
{
  if (m_spec != Invalid && m_spec != Rgb)
    return toRgb().getGreen();
  return m_params.argb.green >> 8;
}

int te::color::ColorTransform::getBlue() const
{
  if (m_spec != Invalid && m_spec != Rgb)
    return toRgb().getBlue();
  return m_params.argb.blue >> 8;
}

int te::color::ColorTransform::getAlpha() const
{
  return m_params.argb.alpha >> 8;
}

void te::color::ColorTransform::setRed(int red)
{
  if (m_spec != Rgb)
    setRgba(red, getGreen(), getBlue(), getAlpha());
  else
    m_params.argb.red = (unsigned short)(red * 0x101);
}

void te::color::ColorTransform::setGreen(int green)
{
  if (m_spec != Rgb)
    setRgba(getRed(), green, getBlue(), getAlpha());
  else
    m_params.argb.green = (unsigned short)(green * 0x101);
}

void te::color::ColorTransform::setBlue(int blue)
{
  if (m_spec != Rgb)
    setRgba(getRed(), getGreen(), blue, getAlpha());
  else
    m_params.argb.blue = (unsigned short)(blue * 0x101);
}

void te::color::ColorTransform::setAlpha(int alpha)
{
    m_params.argb.alpha = (unsigned short)(alpha * 0x101);
}

te::color::ColorTransform te::color::ColorTransform::toRgb() const
{
  if (!isValid() || m_spec == Rgb)
    return *this;

  ColorTransform colt;
  colt.m_spec = Rgb;
  colt.m_params.argb.alpha = m_params.argb.alpha;
  colt.m_params.argb.pad = 0;

  switch (m_spec) 
  {
    case Hsv:
    {
      if (m_params.ahsv.saturation == 0 || m_params.ahsv.hue == USHRT_MAX) 
      {
        // achromatic case
        colt.m_params.argb.red = colt.m_params.argb.green = colt.m_params.argb.blue = m_params.ahsv.value;
        break;
      }

      // chromatic case
      const double h = m_params.ahsv.hue == 36000 ? 0 : m_params.ahsv.hue / 6000.;
      const double s = m_params.ahsv.saturation / double(USHRT_MAX);
      const double v = m_params.ahsv.value / double(USHRT_MAX);
      const int i = int(h);
      const double f = h - i;
      const double p = v * (1.0 - s);

      if (i & 1) 
      {
        const double q = v * (1.0 - (s * f));

        switch (i) 
        {
          case 1:
          colt.m_params.argb.red   = (unsigned short)(round(q * USHRT_MAX));
          colt.m_params.argb.green = (unsigned short)(round(v * USHRT_MAX));
          colt.m_params.argb.blue  = (unsigned short)(round(p * USHRT_MAX));
          break;
          case 3:
          colt.m_params.argb.red   = (unsigned short)(round(p * USHRT_MAX));
          colt.m_params.argb.green = (unsigned short)(round(q * USHRT_MAX));
          colt.m_params.argb.blue  = (unsigned short)(round(v * USHRT_MAX));
          break;
          case 5:
          colt.m_params.argb.red   = (unsigned short)(round(v * USHRT_MAX));
          colt.m_params.argb.green = (unsigned short)(round(p * USHRT_MAX));
          colt.m_params.argb.blue  = (unsigned short)(round(q * USHRT_MAX));
          break;
        }
      } 
      else 
      {
        const double t = v * (1.0 - (s * (1.0 - f)));

        switch (i) 
        {
          case 0:
          colt.m_params.argb.red   = (unsigned short)(round(v * USHRT_MAX));
          colt.m_params.argb.green = (unsigned short)(round(t * USHRT_MAX));
          colt.m_params.argb.blue  = (unsigned short)(round(p * USHRT_MAX));
          break;
          case 2:
          colt.m_params.argb.red   = (unsigned short)(round(p * USHRT_MAX));
          colt.m_params.argb.green = (unsigned short)(round(v * USHRT_MAX));
          colt.m_params.argb.blue  = (unsigned short)(round(t * USHRT_MAX));
          break;
          case 4:
          colt.m_params.argb.red   = (unsigned short)(round(t * USHRT_MAX));
          colt.m_params.argb.green = (unsigned short)(round(p * USHRT_MAX));
          colt.m_params.argb.blue  = (unsigned short)(round(v * USHRT_MAX));
          break;
        }
      }
      break;
    }
    case Hsl:
    {
      if (m_params.ahsl.saturation == 0 || m_params.ahsl.hue == USHRT_MAX) 
      {
        // achromatic case
        colt.m_params.argb.red = colt.m_params.argb.green = colt.m_params.argb.blue = m_params.ahsl.lightness;
      } 
      else if (m_params.ahsl.lightness == 0) 
      {
        // lightness 0 
        colt.m_params.argb.red = colt.m_params.argb.green = colt.m_params.argb.blue = 0;
      } 
      else 
      {
        // chromatic case
        const double h = m_params.ahsl.hue == 36000 ? 0 : m_params.ahsl.hue / 36000.;
        const double s = m_params.ahsl.saturation / double(USHRT_MAX);
        const double l = m_params.ahsl.lightness / double(USHRT_MAX);

        double temp2;
        if (l < double(0.5))
          temp2 = l * (double(1.0) + s);
        else
          temp2 = l + s - (l * s);

        const double temp1 = (double(2.0) * l) - temp2;
        double temp3[3] = { h + (double(1.0) / double(3.0)), h, h - (double(1.0) / double(3.0)) };
        
        for (int i = 0; i != 3; ++i) 
        {
          if (temp3[i] < double(0.0))
            temp3[i] += double(1.0);
          else if (temp3[i] > double(1.0))
            temp3[i] -= double(1.0);

          const double sixtemp3 = temp3[i] * double(6.0);
          if (sixtemp3 < double(1.0))
            colt.m_params.array[i+1] = (unsigned short)(round((temp1 + (temp2 - temp1) * sixtemp3) * USHRT_MAX));
          else if ((temp3[i] * double(2.0)) < double(1.0))
            colt.m_params.array[i+1] = (unsigned short)(round(temp2 * USHRT_MAX));
          else if ((temp3[i] * double(3.0)) < double(2.0))
            colt.m_params.array[i+1] = (unsigned short)(round((temp1 + (temp2 -temp1) * (double(2.0) /double(3.0) - temp3[i]) * double(6.0)) * USHRT_MAX));
          else
            colt.m_params.array[i+1] = (unsigned short)(round(temp1 * USHRT_MAX));
        }
        colt.m_params.argb.red = colt.m_params.argb.red == 1 ? 0 : colt.m_params.argb.red;
        colt.m_params.argb.green = colt.m_params.argb.green == 1 ? 0 : colt.m_params.argb.green;
        colt.m_params.argb.blue = colt.m_params.argb.blue == 1 ? 0 : colt.m_params.argb.blue;
      }
      break;
    }
    case Cmyk:
    {
      const double c = m_params.acmyk.cyan / double(USHRT_MAX);
      const double m = m_params.acmyk.magenta / double(USHRT_MAX);
      const double y = m_params.acmyk.yellow / double(USHRT_MAX);
      const double k = m_params.acmyk.black / double(USHRT_MAX);

      colt.m_params.argb.red   = (unsigned short)(round((1.0 - (c * (1.0 - k) + k)) * USHRT_MAX));
      colt.m_params.argb.green = (unsigned short)(round((1.0 - (m * (1.0 - k) + k)) * USHRT_MAX));
      colt.m_params.argb.blue  = (unsigned short)(round((1.0 - (y * (1.0 - k) + k)) * USHRT_MAX));
      break;
    }
    default:
    break;
  }
  
  return colt;
}

#define MAX_3(a, b, c) ( ( a > b && a > c) ? a : (b > c ? b : c) )
#define MIN_3(a, b, c) ( ( a < b && a < c) ? a : (b < c ? b : c) )

te::color::ColorTransform te::color::ColorTransform::toHsv() const
{
  if (!isValid() || m_spec == Hsv)
    return *this;

  if (m_spec != Rgb)
    return toRgb().toHsv();

  ColorTransform colt;
  colt.m_spec = Hsv;
  colt.m_params.ahsv.alpha = m_params.argb.alpha;
  colt.m_params.ahsv.pad = 0;

  const double r = m_params.argb.red   / double(USHRT_MAX);
  const double g = m_params.argb.green / double(USHRT_MAX);
  const double b = m_params.argb.blue  / double(USHRT_MAX);
  const double max = MAX_3(r, g, b);
  const double min = MIN_3(r, g, b);
  const double delta = max - min;
  colt.m_params.ahsv.value = (unsigned short)(round(max * USHRT_MAX));

  if (fuzzyIsNull(delta))
  {
    // achromatic case, hue is undefined
    colt.m_params.ahsv.hue = USHRT_MAX;
    colt.m_params.ahsv.saturation = 0;
  } 
  else 
  {
    // chromatic case
    double hue = 0;
    colt.m_params.ahsv.saturation = (unsigned short)(round((delta / max) * USHRT_MAX));
    if (fuzzyCompare(r, max)) 
    {
      hue = ((g - b) /delta);
    } 
    else if (fuzzyCompare(g, max)) 
    {
      hue = (2.0 + (b - r) / delta);
    }
    else if (fuzzyCompare(b, max)) 
    {
      hue = (4.0 + (r - g) / delta);
    }
    else 
    {
//      "internal error"
    }
    hue *= 60.0;

    if (hue < 0.0)
      hue += 360.0;
    colt.m_params.ahsv.hue = (unsigned short)(round(hue * 100));
  }

  return colt;
}

te::color::ColorTransform te::color::ColorTransform::toHsl() const
{
  if (!isValid() || m_spec == Hsl)
    return *this;

  if (m_spec != Rgb)
    return toRgb().toHsl();

  ColorTransform colt;
  colt.m_spec = Hsl;
  colt.m_params.ahsl.alpha = m_params.argb.alpha;
  colt.m_params.ahsl.pad = 0;

  const double r = m_params.argb.red   / double(USHRT_MAX);
  const double g = m_params.argb.green / double(USHRT_MAX);
  const double b = m_params.argb.blue  / double(USHRT_MAX);
  const double max = MAX_3(r, g, b);
  const double min = MIN_3(r, g, b);
  const double delta = max - min;
  const double delta2 = max + min;
  const double lightness = double(0.5) * delta2;
  colt.m_params.ahsl.lightness = (unsigned short)(round(lightness * USHRT_MAX));

  if (fuzzyIsNull(delta)) 
  {
    // achromatic case, hue is undefined
    colt.m_params.ahsl.hue = 0;
    colt.m_params.ahsl.saturation = 0;
  } 
  else 
  {
    // chromatic case
    double hue = 0;
    if (lightness < double(0.5))
      colt.m_params.ahsl.saturation = (unsigned short)(round((delta / delta2) * USHRT_MAX));
    else
      colt.m_params.ahsl.saturation = (unsigned short)(round((delta / (double(2.0) - delta2)) * USHRT_MAX));

    if (fuzzyCompare(r, max)) 
    {
      hue = ((g - b) /delta);
    } 
    else if (fuzzyCompare(g, max)) 
    {
      hue = (2.0 + (b - r) / delta);
    } 
    else if (fuzzyCompare(b, max)) 
    {
      hue = (4.0 + (r - g) / delta);
    } 
    else 
    {
//      "internal error"
    }

    hue *= 60.0;
    if (hue < 0.0)
      hue += 360.0;
    colt.m_params.ahsl.hue = (unsigned short)(round(hue * 100));
  }

  return colt;
}

te::color::ColorTransform te::color::ColorTransform::toCmyk() const
{
  if (!isValid() || m_spec == Cmyk)
    return *this;

  if (m_spec != Rgb)
    return toRgb().toCmyk();

  ColorTransform colt;
  colt.m_spec = Cmyk;
  colt.m_params.acmyk.alpha = m_params.argb.alpha;

  // rgb -> cmy
  const double r = m_params.argb.red   / double(USHRT_MAX);
  const double g = m_params.argb.green / double(USHRT_MAX);
  const double b = m_params.argb.blue  / double(USHRT_MAX);
  double c = 1.0 - r;
  double m = 1.0 - g;
  double y = 1.0 - b;

  // cmy -> cmyk
  const double k = vMin(c, vMin(m, y));

  if (!fuzzyIsNull(k - 1)) 
  {
    c = (c - k) / (1.0 - k);
    m = (m - k) / (1.0 - k);
    y = (y - k) / (1.0 - k);
  }

  colt.m_params.acmyk.cyan    = (unsigned short)(round(c * USHRT_MAX));
  colt.m_params.acmyk.magenta = (unsigned short)(round(m * USHRT_MAX));
  colt.m_params.acmyk.yellow  = (unsigned short)(round(y * USHRT_MAX));
  colt.m_params.acmyk.black   = (unsigned short)(round(k * USHRT_MAX));

  return colt;
}

int te::color::ColorTransform::getHsvHue() const
{
  if (m_spec != Invalid && m_spec != Hsv)
    return toHsv().getHsvHue();
  return m_params.ahsv.hue == USHRT_MAX ? -1 : m_params.ahsv.hue / 100;
}

int te::color::ColorTransform::getHsvSaturation() const
{
  if (m_spec != Invalid && m_spec != Hsv)
    return toHsv().getHsvSaturation();
  return m_params.ahsv.saturation >> 8;
}

int te::color::ColorTransform::getValue() const
{
  if (m_spec != Invalid && m_spec != Hsv)
    return toHsv().getValue();
  return m_params.ahsv.value >> 8;
}

void te::color::ColorTransform::getHsv(int *h, int *s, int *v, int *a) const
{
  if (!h || !s || !v)
    return;

  if (m_spec != Invalid && m_spec != Hsv) 
  {
    toHsv().getHsv(h, s, v, a);
    return;
  }

  *h = m_params.ahsv.hue == USHRT_MAX ? -1 : m_params.ahsv.hue / 100;
  *s = m_params.ahsv.saturation >> 8;
  *v = m_params.ahsv.value      >> 8;

  if (a)
    *a = m_params.ahsv.alpha >> 8;
}

void te::color::ColorTransform::setHsv(int h, int s, int v, int a)
{
  if (h < -1 || (unsigned int)s > 255 || (unsigned int)v > 255 || (unsigned int)a > 255) 
  {
    //"HSV parameters out of range"
    invalidate();
    return;
  }

  m_spec = Hsv;
  m_params.ahsv.alpha      = (unsigned short)(a * 0x101);
  m_params.ahsv.hue        = (unsigned short)(h == -1 ? USHRT_MAX : (h % 360) * 100);
  m_params.ahsv.saturation = (unsigned short)(s * 0x101);
  m_params.ahsv.value      = (unsigned short)(v * 0x101);
  m_params.ahsv.pad        = 0;
}

int te::color::ColorTransform::getCyan() const
{
  if (m_spec != Invalid && m_spec != Cmyk)
    return toCmyk().getCyan();
  return m_params.acmyk.cyan >> 8;
}

int te::color::ColorTransform::getMagenta() const
{
  if (m_spec != Invalid && m_spec != Cmyk)
    return toCmyk().getMagenta();
  return m_params.acmyk.magenta >> 8;
}

int te::color::ColorTransform::getYellow() const
{
  if (m_spec != Invalid && m_spec != Cmyk)
    return toCmyk().getYellow();
  return m_params.acmyk.yellow >> 8;
}

int te::color::ColorTransform::getBlack() const
{
  if (m_spec != Invalid && m_spec != Cmyk)
    return toCmyk().getBlack();
  return m_params.acmyk.black >> 8;
}

void te::color::ColorTransform::getCmyk(int *c, int *m, int *y, int *k, int *a)
{
  if (!c || !m || !y || !k)
    return;

  if (m_spec != Invalid && m_spec != Cmyk) 
  {
    toCmyk().getCmyk(c, m, y, k, a);
    return;
  }

  *c = m_params.acmyk.cyan >> 8;
  *m = m_params.acmyk.magenta >> 8;
  *y = m_params.acmyk.yellow >> 8;
  *k = m_params.acmyk.black >> 8;

  if (a)
  *a = m_params.acmyk.alpha >> 8;
}

void te::color::ColorTransform::setCmyk(int c, int m, int y, int k, int a)
{
  if (c < 0 || c > 255
  || m < 0 || m > 255
  || y < 0 || y > 255
  || k < 0 || k > 255
  || a < 0 || a > 255) 
  {
    //"CMYK parameters out of range"
    return;
  }

  m_spec = Cmyk;
  m_params.acmyk.alpha   = (unsigned short)(a * 0x101);
  m_params.acmyk.cyan    = (unsigned short)(c * 0x101);
  m_params.acmyk.magenta = (unsigned short)(m * 0x101);
  m_params.acmyk.yellow  = (unsigned short)(y * 0x101);
  m_params.acmyk.black   = (unsigned short)(k * 0x101);
}

int te::color::ColorTransform::getHslHue() const
{
  if (m_spec != Invalid && m_spec != Hsl)
    return toHsl().getHslHue();
  return m_params.ahsl.hue == USHRT_MAX ? -1 : m_params.ahsl.hue / 100;
}

int te::color::ColorTransform::getHslSaturation() const
{
  if (m_spec != Invalid && m_spec != Hsl)
    return toHsl().getHslSaturation();
  return m_params.ahsl.saturation >> 8;
}

int te::color::ColorTransform::getLightness() const
{
  if (m_spec != Invalid && m_spec != Hsl)
    return toHsl().getLightness();
  return m_params.ahsl.lightness >> 8;
}

void te::color::ColorTransform::getHsl(int *h, int *s, int *l, int *a) const
{
  if (!h || !s || !l)
    return;

  if (m_spec != Invalid && m_spec != Hsl) 
  {
    toHsl().getHsl(h, s, l, a);
    return;
  }

  *h = m_params.ahsl.hue == USHRT_MAX ? -1 : m_params.ahsl.hue / 100;
  *s = m_params.ahsl.saturation >> 8;
  *l = m_params.ahsl.lightness  >> 8;

  if (a)
    *a = m_params.ahsl.alpha >> 8;
}

void te::color::ColorTransform::setHsl(int h, int s, int l, int a)
{
  if (h < -1 || (unsigned short)s > 255 || (unsigned short)l > 255 || (unsigned short)a > 255) 
  {
    //"HSV parameters out of range"
    invalidate();
    return;
  }

  m_spec = Hsl;
  m_params.ahsl.alpha      = (unsigned short)(a * 0x101);
  m_params.ahsl.hue        = (unsigned short)(h == -1 ? USHRT_MAX : (h % 360) * 100);
  m_params.ahsl.saturation = (unsigned short)(s * 0x101);
  m_params.ahsl.lightness  = (unsigned short)(l * 0x101);
  m_params.ahsl.pad        = 0;
}

bool te::color::ColorTransform::operator==(const ColorTransform& colt) const
{
  if (m_spec == Hsl && m_spec == colt.m_spec) 
  {
    return (m_params.argb.alpha == colt.m_params.argb.alpha
      && ((((m_params.ahsl.hue % 36000) == (colt.m_params.ahsl.hue % 36000)))
      || (m_params.ahsl.hue == colt.m_params.ahsl.hue))
      && (abs(m_params.ahsl.saturation - colt.m_params.ahsl.saturation) < 50
      || m_params.ahsl.lightness == 0
      || colt.m_params.ahsl.lightness == 0
      || m_params.ahsl.lightness == USHRT_MAX
      || colt.m_params.ahsl.lightness == USHRT_MAX)
      && (abs(m_params.ahsl.lightness - colt.m_params.ahsl.lightness)) < 50);
  } 
  else 
  {
    return (m_spec == colt.m_spec
      && m_params.argb.alpha == colt.m_params.argb.alpha
      && (((m_spec == Hsv)
        && ((m_params.ahsv.hue % 36000) == (colt.m_params.ahsv.hue % 36000)))
      || (m_params.ahsv.hue == colt.m_params.ahsv.hue))
      && m_params.argb.green == colt.m_params.argb.green
      && m_params.argb.blue  == colt.m_params.argb.blue
      && m_params.argb.pad   == colt.m_params.argb.pad);
  }
}

bool te::color::ColorTransform::operator!=(const ColorTransform& colt) const
{ 
  return !operator==(colt); 
}

