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
  \file terralib/color/ColorBar.cpp

  \brief Implementation of the color bar concept.
*/

// TerraLib
#include "ColorBar.h"

te::color::ColorBar::ColorBar()
  : m_barSize(0)
{
}

te::color::ColorBar::ColorBar(int size)
{
  RGBAColor c1(255, 0, 0, 255);
  RGBAColor c2(255, 255, 255, 255);
  m_colorMap[0.] = c1;
  m_colorMap[1.] = c2;
  m_barSize = size;
}

te::color::ColorBar::ColorBar(const RGBAColor& colorBegin, const RGBAColor& colorEnd, int size)
{
  m_colorMap[0.] = colorBegin;
  m_colorMap[1.] = colorEnd;
  m_barSize = size;
}

te::color::ColorBar::ColorBar(const ColorBar& rhs)
  : m_colorBarVec(rhs.m_colorBarVec), m_colorSliceVec(rhs.m_colorSliceVec),
    m_name(rhs.m_name), m_barSize(rhs.m_barSize), m_colorMap(rhs.m_colorMap)
{
}

te::color::ColorBar& te::color::ColorBar::operator=(const ColorBar& rhs)
{
  if(this != &rhs)
  {
    m_colorBarVec = rhs.m_colorBarVec;
    m_colorSliceVec = rhs.m_colorSliceVec;
    m_name = rhs.m_name;
    m_barSize = rhs.m_barSize;
    m_colorMap = rhs.m_colorMap;
  }

  return *this;
}

te::color::ColorBar::~ColorBar()
{
}

const std::string& te::color::ColorBar::getName() const
{
  return m_name;
}

void te::color::ColorBar::setName(const std::string& name)
{
    m_name = name;
}

const std::map<double, te::color::RGBAColor>& te::color::ColorBar::getColorMap() const
{
  return m_colorMap;
}

void te::color::ColorBar::generateColorBar() //private
{
  int i, p, pp, s;
  double rd, gd, bd, ad;
  RGBAColor c, cc, cor;
  std::map<int, RGBAColor> colorMap;

  std::map<double, RGBAColor>::iterator iit;

  for(iit = m_colorMap.begin(); iit != m_colorMap.end(); ++iit)
  {
    p = (int)((*iit).first * (double)(m_barSize-1) + .5);
    colorMap[p] = (*iit).second;
  }

  s = 1;
  std::map<int, RGBAColor>::iterator it = colorMap.begin();
  while(it != colorMap.end())
  {
    p = it->first;
    c = it->second;
    if(s <= 0)
      m_colorBarVec.pop_back();
    m_colorBarVec.push_back(c);

    ++it;
    if(it != colorMap.end())
    {
      pp = it->first;
      cc = it->second;
      s = pp - p - 1; // amount of colors to be generated
      if(s >= 1)
      {
        rd = double(cc.getRed() - c.getRed()) / double(s + 1);
        gd = double(cc.getGreen() - c.getGreen()) / double(s + 1);
        bd = double(cc.getBlue() - c.getBlue()) / double(s + 1);
        ad = double(cc.getAlpha() - c.getAlpha()) / double(s + 1);

        for(i = 1; i <= s; ++i)
        {
          int r = (int)(rd * (double)i + .5) + c.getRed();
          int g = (int)(gd * (double)i + .5) + c.getGreen();
          int b = (int)(bd * (double)i + .5) + c.getBlue();
          int a = (int)(ad * (double)i + .5) + c.getAlpha();
          cor.setColor(r, g, b, a);
          m_colorBarVec.push_back(cor);
        }
      }
    }
  }
  while((int)m_colorBarVec.size() < m_barSize)
    m_colorBarVec.push_back(colorMap.rbegin()->second);
}

const std::vector<te::color::RGBAColor>& te::color::ColorBar::getColorBar()
{
  if(m_colorBarVec.empty())
    generateColorBar();

  return m_colorBarVec;
}

void te::color::ColorBar::generateSlices(const int& n) //private
{
  int i, j;
  RGBAColor c;
  double d = (double)(m_barSize-1) / (double)(n-1);

  m_colorSliceVec.clear();

  for(i = 0; i < n; ++i)
  {
    j = (int)((double)i * d + .5);
    c = m_colorBarVec[j];
    m_colorSliceVec.push_back(c);
  }
}

const std::vector<te::color::RGBAColor>& te::color::ColorBar::getSlices(const int& n)
{
  m_colorSliceVec.clear();

  if(n == 0)
    return m_colorSliceVec;
  if(n == 1)
  {
    m_colorSliceVec.push_back(m_colorMap.begin()->second);
    return m_colorSliceVec;
  }

  if(m_colorBarVec.empty())
    generateColorBar();

  if((int)m_colorSliceVec.size() != n)
  {
    m_colorSliceVec.clear();
    generateSlices(n);
  }

  return m_colorSliceVec;
}

const te::color::RGBAColor& te::color::ColorBar::getMeanSlice()
{
  if(m_colorBarVec.empty())
    generateColorBar();

  int size = m_colorBarVec.size();
  int mean = (int)((double)size / 2. + .5);

  return m_colorBarVec[mean];
}

const std::vector<te::color::RGBAColor>& te::color::ColorBar::getLowerMeanSlices(const int& n)
{
  if(m_colorBarVec.empty())
    generateColorBar();

  int i, j;
  RGBAColor c;
  double d;

  if(n == 1)
    d = 1;
  else
    d = ((double)(m_barSize-1) / (double)(n-1)) / 2.;

  m_colorSliceVec.clear();

  for(i = 0; i < n; ++i)
  {
    j = (int)((double)i * d + .5);
    c = m_colorBarVec[j];
    m_colorSliceVec.push_back(c);
  }

  return m_colorSliceVec;
}

const std::vector<te::color::RGBAColor>& te::color::ColorBar::getUpperMeanSlices(const int& n)
{
  if(m_colorBarVec.empty())
    generateColorBar();

  int i, j;
  RGBAColor c;
  double d = ((double)(m_barSize-1) / (double)(n-1)) / 2.;

  m_colorSliceVec.clear();

  double halfsize = (double)(m_barSize-1) / 2.;
  int shift = (int)halfsize;

  for(i = 0; i < n; ++i)
  {
    j = ((int)((double)i * d + .5)) + shift;
    c = m_colorBarVec[j];
    m_colorSliceVec.push_back(c);
  }

  return m_colorSliceVec;
}

void te::color::ColorBar::setBarSize(const int& size)
{
  m_colorBarVec.clear();
  m_colorSliceVec.clear();
  m_barSize = size;
}

int te::color::ColorBar::getBarSize()
{
  return m_barSize;
}

void te::color::ColorBar::addColor(const RGBAColor& color, const double& ppos)
{
  double p;
  RGBAColor c;
  std::map<double, RGBAColor>::iterator it = m_colorMap.find(ppos);
  m_colorBarVec.clear();
  m_colorSliceVec.clear();

  double pos = ppos;
  if(pos > 1.)
    pos = 1.;
  else if(pos < 0.)
    pos = 0.;

  if(it == m_colorMap.end())
    m_colorMap[pos] = color;
  else
  {
    c = (*it).second;
    std::map<double, RGBAColor>::iterator itt = it;

    if(pos == 1.)
    {
      --itt;
      p = (*itt).first;
      p = pos - (pos - p) / 2.;
    }
    else if(pos == 0.)
    {
      ++itt;
      p = (*itt).first;
      p = pos + (p - pos) / 2.;
    }

    m_colorMap.erase(it);
    m_colorMap[p] = c;

    m_colorMap[pos] = color;
  }
}

void te::color::ColorBar::remove(const RGBAColor& color)
{
  std::map<double, RGBAColor>::iterator it;
  RGBAColor c;
  m_colorBarVec.clear();
  m_colorSliceVec.clear();

  if(m_colorMap.size() > 2)
  {
    for(it = m_colorMap.begin(); it != m_colorMap.end(); ++it)
    {
      c = (*it).second;
      if(c.getRed() == color.getRed() && c.getGreen() == color.getGreen() && c.getBlue() == color.getBlue() && c.getAlpha() == color.getAlpha())
      {
        m_colorMap.erase(it);
        break;
      }
    }
  }
}

void te::color::ColorBar::remove(const int& index)
{
  int i;
  std::map<double, RGBAColor>::iterator it;
  m_colorBarVec.clear();
  m_colorSliceVec.clear();

  if(m_colorMap.size() > 2)
  {
    for(i = 0, it = m_colorMap.begin(); it != m_colorMap.end(); ++i, ++it)
    {
      if(i == index)
      {
        m_colorMap.erase(it);
        break;
      }
    }
  }
}

void te::color::ColorBar::remove(const double& pos)
{
  if(m_colorMap.size() == 2)
    return;

  std::map<double, RGBAColor>::iterator it = m_colorMap.find(pos);

  if(it != m_colorMap.end())
  {
    m_colorBarVec.clear();
    m_colorSliceVec.clear();
    m_colorMap.erase(pos);
  }
}

void te::color::ColorBar::removeReverse(const int& index)
{
  int i;
  std::map<double, RGBAColor>::reverse_iterator it;
  m_colorBarVec.clear();
  m_colorSliceVec.clear();

  if(m_colorMap.size() > 2)
  {
    for(i = 0, it = m_colorMap.rbegin(); it != m_colorMap.rend(); ++i, ++it)
    {
      if(i == index)
      {
        m_colorMap.erase(it->first);
        break;
      }
    }
  }
}

void te::color::ColorBar::changeColor(const double& pos, const RGBAColor& color)
{
  std::map<double, RGBAColor>::iterator it = m_colorMap.find(pos);
  if(it != m_colorMap.end())
  {
    m_colorBarVec.clear();
    m_colorSliceVec.clear();
    it->second = color;
  }
}

void te::color::ColorBar::changeColor(const int& index, const RGBAColor& color)
{
  int i;
  std::map<double, RGBAColor>::iterator it;
  m_colorBarVec.clear();
  m_colorSliceVec.clear();

  for(i = 0, it = m_colorMap.begin(); it != m_colorMap.end(); ++i, ++it)
  {
    if(i == index)
    {
      it->second = color;
      break;
    }
  }
}

void te::color::ColorBar::move(const RGBAColor& color, const double& ppos)
{
  double p, pp;
  RGBAColor c;
  std::map<double, RGBAColor>::iterator it;
  m_colorBarVec.clear();
  m_colorSliceVec.clear();

  double pos = ppos;
  if(pos > 1.)
    pos = 1.;
  else if(pos < 0.)
    pos = 0.;

  for(it = m_colorMap.begin(); it != m_colorMap.end(); ++it)
  {
    c = (*it).second;
      if(c.getRed() == color.getRed() && c.getGreen() == color.getGreen() && c.getBlue() == color.getBlue() && c.getAlpha() == color.getAlpha())
      break;
  }

  if(it == m_colorMap.end()) // color not found
    return;
  
  p = (*it).first;
  if(p == 0. || p == 1.) // The first and last color can not be moved directly.
  {
      if(m_colorMap.size() == 2) // but if you have only two colors, makes the exchange
      {
        std::map<double, RGBAColor>::iterator itt = it;
        if(p == 0.)
          ++itt;
        else
          --itt;
        (*it).second = (*itt).second;
        (*itt).second = color;
      }
      return;
  }

  m_colorMap.erase(it); // remove color

  it = m_colorMap.find(pos);

  if(it == m_colorMap.end()) // If there is no color at index pos
  {
    m_colorMap[pos] = color;
  }
  else
  {
    pp = p;
    c = (*it).second;

    std::map<double, RGBAColor>::iterator itt = it;

    if(pp < pos)
    {
      --itt;
      p = (*itt).first;
      if(pp == p)
      {
        --itt;
        p = (*itt).first;
      }
      p = pos - (pos - p) / 2.;
    }
    else
    {
      ++itt;
      p = (*itt).first;
      if(pp == p)
      {
        ++itt;
        p = (*itt).first;
      }
      p = pos + (p - pos) / 2.;
    }

    m_colorMap.erase(it);
    m_colorMap[p] = c;
    m_colorMap[pos] = color;
  }
}

int te::color::ColorBar::move(const int& index, const double& ppos)
{
  int i;
  double p, pp;
  RGBAColor c, color;
  std::map<double, RGBAColor>::iterator it;
  m_colorBarVec.clear();
  m_colorSliceVec.clear();

  if(index >= (int)m_colorMap.size())
    return index;

  if(index == 0 || index == (int)(m_colorMap.size()-1))
    return index;

  for(i = 0, it = m_colorMap.begin(); it != m_colorMap.end(); ++i, ++it)
    if(i == index)
      break;

  p = (*it).first;
  pp = move(p, ppos);

  for(i = 0, it = m_colorMap.begin(); it != m_colorMap.end(); ++i, ++it)
    if(it->first == pp)
      break;

  return i;
}

double te::color::ColorBar:: move(const double& p, const double& ppos)
{
  std::map<double, RGBAColor>::iterator it = m_colorMap.find(p);
  if(it == m_colorMap.end())
    return p;

  double pp, d = 1. / (double)m_barSize;
  double pos = ppos;
  if(pos > 1.)
    pos = 1.;
  else if(pos < 0.)
    pos = 0.;

  if(p == pos)
    return pos;
  if(p == 0. || p == 1.)
    return ppos;

  std::map<double, RGBAColor>::iterator itp = m_colorMap.find(pos);

  m_colorBarVec.clear();
  m_colorSliceVec.clear();

  RGBAColor c = it->second, cc;
  m_colorMap.erase(p);

  if(itp == m_colorMap.end())
  {
    addColor(c, pos);
    return pos;
  }

  if(p > pos)
  {
    if(pos != 0.)
    {
      it = m_colorMap.begin();
      pp = it->first;
      while(it->first != pos)
      {
        pp = it->first;
        ++it;
      }
      while(pos-d <= pp)
        d /= 10.;
      pos -= d;
      m_colorMap[pos] = c;
      return pos;
    }
    else
    {
      it = m_colorMap.begin();
      cc = it->second;
      ++it;
      pp = it->first;
      while(pos+d >= pp)
        d /= 10.;
      pos += d;
      m_colorMap[0.] = c;
      m_colorMap[pos] = cc;
      return pos;
    }
  }
  else
  {
    if(pos != 1.)
    {
      it = m_colorMap.begin();
      while(it->first != pos)
        ++it;
      ++it;
      pp = it->first;
      while(pos+d >= pp)
        d /= 10.;
      pos += d;
      m_colorMap[pos] = c;
      return pos;
    }
    else
    {
      it = m_colorMap.end();
      --it;
      cc = it->second;
      --it;
      pp = it->first;
      while(pos-d <= pp)
        d /= 10.;
      pos -= d;
      m_colorMap[1.] = c;
      m_colorMap[pos] = cc;
      return pos;
    }
  }
}

int te::color::ColorBar::moveReverse(const int& index, const double& ppos)
{
  int i;
  double p, pp;
  RGBAColor c, color;
  std::map<double, RGBAColor>::reverse_iterator it;
  m_colorBarVec.clear();
  m_colorSliceVec.clear();

  if(index >= (int)m_colorMap.size())
    return index;

  for(i = 0, it = m_colorMap.rbegin(); it != m_colorMap.rend(); ++i, ++it)
    if(i == index)
      break;
  
  p = (*it).first;
  pp = move(p, ppos);

  for(i = 0, it = m_colorMap.rbegin(); it != m_colorMap.rend(); ++i, ++it)
    if(it->first == pp)
      break;

  return i;
}

