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
  \file PtMarker.cpp
  
  \brief This file contains several utility routines for dealing with point markers.
 */


// TerraLib
#include "../color/RGBAColor.h"
#include "PtMarker.h"

te::color::RGBAColor** te::map::CreateMarker(te::map::PtMarkerType type, int width, const te::color::RGBAColor& cor)
{
  switch(type)
  {
    case te::map::MarkerSquare:
            return CreateMarkerSquare(width, cor);
    case te::map::MarkerDiamond:
            return CreateMarkerDiamond(width, cor);
    case te::map::MarkerCircle:
            return CreateMarkerCircle(width, cor);
    case te::map::MarkerCrossedCircle:
            return CreateMarkerCrossedCircle(width, cor);
    case te::map::MarkerSemiEllipseLeft:
            return CreateMarkerSemiEllipseLeft(width, cor);
    case te::map::MarkerSemiEllipseRight:
            return CreateMarkerSemiEllipseRight(width, cor);
    case te::map::MarkerSemiEllipseUp:
            return CreateMarkerSemiEllipseUp(width, cor);
    case te::map::MarkerSemiEllipseDown:
            return CreateMarkerSemiEllipseDown(width, cor);
    case te::map::MarkerTriangleLeft:
            return CreateMarkerTriangleLeft(width, cor);
    case te::map::MarkerTriangleRight:
            return CreateMarkerTriangleRight(width, cor);
    case te::map::MarkerTriangleUp:
            return CreateMarkerTriangleUp(width, cor);
    case te::map::MarkerTriangleDown:
            return CreateMarkerTriangleDown(width, cor);
    case te::map::MarkerFourRays:
            return CreateMarkerFourRays(width, cor);
    case te::map::MarkerCross:
            return CreateMarkerCross(width, cor);
    case te::map::MarkerX:
            return CreateMarkerX(width, cor);
    case te::map::MarkerDash:
            return CreateMarkerDash(width, cor);
    case te::map::MarkerDot:
            return CreateMarkerDot(width, cor);
    case te::map::MarkerPixel:
            return CreateMarkerPixel(width, cor);
    default:
            return 0;
  }
}

te::color::RGBAColor** te::map::CreateMarkerFourRays(int width, const te::color::RGBAColor& cor)
{
  int a, b, d, i, k, m, n, ini, fim, kfim;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
    memset(p, 0, width * sizeof(te::color::RGBAColor));
  }

  d = 2;
  if(width < 7)
    d = 1;

  m = width / 2;
  ini = 0;
  if(width%2 == 0)
    ini++;
  for(i = ini; i < width; ++i)
  {
    marker[i][m] = cor;
    marker[m][i] = cor;
  }

  kfim = (int)((double)width * 3. / 8. + .5);
  k = d;
  n = 1;
  while(true)
  {
    ini = k;
    if(width % 2 == 0)
      ini++;
    fim = width - k;
    a = m - n;
    b = m + n;
    for(i = ini; i < fim; ++i)
    {
      marker[i][a] = cor;
      marker[i][b] = cor;
      marker[a][i] = cor;
      marker[b][i] = cor;
    }
    ++n;
    k += d;
    if(k > kfim)
      break;
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerDiamond(int width, const te::color::RGBAColor& cor)
{
  int i, j, k, ini, fim;
  double m;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
    memset(p, 0, width * sizeof(te::color::RGBAColor));
  }

  m = width / 2;
  i = 0;
  k = 0;
  while(i < (int)m)
  {
    ini = (int)m-k;
    fim = (int)m+k;
    for(j = ini; j <= fim; ++j)
      marker[i][j] = cor;
    ++i;
    ++k;
  }
  int f = i - 2;
  while(f >= 0 && i < width)
  {
    for(j = 0; j < width; ++j)
      marker[i][j] = marker[f][j];
    --f;
    ++i;
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerSquare(int width, const te::color::RGBAColor& cor)
{
  int i, j;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
  }

  for(i = 0; i < width; ++i)
  {
    for(j = 0; j < width; ++j)
      marker[i][j] = cor;
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerCircle(int width, const te::color::RGBAColor& cor)
{
  int i, j, m;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
  }

  double a, b, r;
  m = width / 2;
  r = m * m;

  for(i = 0; i < width; ++i)
  {
    for(j = 0; j < width; ++j)
    {
      a = i - m;
      a *= a;
      b = j - m;
      b *= b;
      if(a+b < r)
        marker[i][j] = cor;
    }
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerCrossedCircle(int width, const te::color::RGBAColor& cor)
{
  int i, j, m;


  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
  }

  double a, b, r;
  m = width / 2;
  double w = (double)width * .7;
  r = (int)(w/2. * w/2.);

  for(i = 0; i < width; ++i)
  {
    for(j = 0; j < width; ++j)
    {
      a = i - m;
      a *= a;
      b = j - m;
      b *= b;
      if(a+b < r)
        marker[i][j] = cor;
    }
  }

  for(i = 0; i < width; ++i)
    marker[i][m] = cor;
  for(j = 0; j < width; ++j)
    marker[m][j] = cor;

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerSemiEllipseLeft(int width, const te::color::RGBAColor& cor)
{
  int i, j, mx, my;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
  }

  double a, b, rx, ry;
  mx = width/2 - 1;
  rx = mx * mx;
  my = width - 1;
  ry = my * my;

  for(i = 0; i < width; ++i)
  {
    for(j = 0; j < width; ++j)
    {
      a = i - mx;
      a *= a;
      b = j - my;
      b *= b;
      if(a/rx + b/ry < 1)
        marker[i][j] = cor;
    }
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerSemiEllipseRight(int width, const te::color::RGBAColor& cor)
{
  int i, j, mx, my;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
  }

  double a, b, rx, ry;
  mx = width/2 - 1;
  rx = mx * mx;
  my = width - 1;
  ry = my * my;

  for(i = 0; i < width; ++i)
  {
    for(j = 0; j < width; ++j)
    {
      a = i - mx;
      a *= a;
      b = j;
      b *= b;
      if(a/rx + b/ry < 1)
        marker[i][j] = cor;
    }
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerSemiEllipseUp(int width, const te::color::RGBAColor& cor)
{
  int i, j, mx, my;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
  }

  double a, b, rx, ry;
  mx = width - 1;
  rx = mx * mx;
  my = width/2 - 1;
  ry = my * my;

  for(i = 0; i < width; ++i)
  {
    for(j = 0; j < width; ++j)
    {
      a = i - mx;
      a *= a;
      b = j - my;
      b *= b;
      if(a/rx + b/ry < 1)
        marker[i][j] = cor;
    }
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerSemiEllipseDown(int width, const te::color::RGBAColor& cor)
{
  int i, j, mx, my;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
  }

  double a, b, rx, ry;
  mx = width - 1;
  rx = mx * mx;
  my = width/2 - 1;
  ry = my * my;

  for(i = 0; i < width; ++i)
  {
    for(j = 0; j < width; ++j)
    {
      a = i;
      a *= a;
      b = j - my;
      b *= b;
      if(a/rx + b/ry < 1)
        marker[i][j] = cor;
    }
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerTriangleLeft(int width, const te::color::RGBAColor& cor)
{
  int i, j, k, ini, fim;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
    memset(p, 0, width * sizeof(te::color::RGBAColor));
  }

  j = width - 1;
  k = 0;
  while(j >= 0)
  {
    ini = k;
    fim = width - k;
    for(i = ini; i < fim; ++i)
      marker[i][j] = cor;
    --j;
    if(j%2)
      ++k;
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerTriangleRight(int width, const te::color::RGBAColor& cor)
{
  int i, j, k, ini, fim;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
    memset(p, 0, width * sizeof(te::color::RGBAColor));
  }

  j = 0;
  k = 0;
  while(j < width)
  {
    ini = k;
    fim = width - k;
    for(i = ini; i < fim; ++i)
      marker[i][j] = cor;
    ++j;
    if(j%2)
      ++k;
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerTriangleUp(int width, const te::color::RGBAColor& cor)
{
  int i, j, k, ini, fim;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
    memset(p, 0, width * sizeof(te::color::RGBAColor));
  }

  i = width - 1;
  k = 0;
  while(i >= 0)
  {
    ini = k;
    fim = width - k;
    for(j = ini; j < fim; ++j)
      marker[i][j] = cor;
    --i;
    if(i%2)
      ++k;
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerTriangleDown(int width, const te::color::RGBAColor& cor)
{
  int i, j, k, ini, fim;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
    memset(p, 0, width * sizeof(te::color::RGBAColor));
  }

  i = 0;
  k = 0;
  while(i < width)
  {
    ini = k;
    fim = width - k;
    for(j = ini; j < fim; ++j)
      marker[i][j] = cor;
    ++i;
    if(i%2)
      ++k;
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerCross(int width, const te::color::RGBAColor& cor)
{
  int i, m;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
    memset(p, 0, width * sizeof(te::color::RGBAColor));
  }

  m = width / 2;
  for(i = 0; i < width; ++i)
  {
    marker[i][m] = cor;
    marker[m][i] = cor;
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerX(int width, const te::color::RGBAColor& cor)
{
  int i;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
    memset(p, 0, width * sizeof(te::color::RGBAColor));
  }

  for(i = 0; i < width; ++i)
  {
    marker[i][i] = cor;
    marker[i][width-1-i] = cor;
  }

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerDash(int width, const te::color::RGBAColor& cor)
{
  int i, m;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
    memset(p, 0, width * sizeof(te::color::RGBAColor));
  }

  m = width / 2;
  for(i = 0; i < width; ++i)
    marker[m][i] = cor;

  return marker;
}

te::color::RGBAColor** te::map::CreateMarkerDot(int width, const te::color::RGBAColor& cor)
{
  return te::map::CreateMarkerCircle(width, cor);
}

te::color::RGBAColor** te::map::CreateMarkerPixel(int width, const te::color::RGBAColor& cor)
{
  int i, m;

  te::color::RGBAColor** marker = new te::color::RGBAColor*[width];
  for(i = 0; i < width; ++i)
  {
    te::color::RGBAColor* p = new te::color::RGBAColor[width];
    if(p == 0)
      return 0;
    marker[i] = p;
    memset(p, 0, width * sizeof(te::color::RGBAColor));
  }

  m = width / 2;
  marker[m][m] = cor;

  return marker;
}