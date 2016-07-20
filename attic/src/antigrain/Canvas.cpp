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
  \file terralib/antigrain/Canvas.cpp

  \brief A canvas built on top of Anti Grain Geometry.
 */
//#include <windows.h>
// TerraLib
#include "../annotationtext/Attributes.h"
#include "../annotationtext/Element.h"
#include "../annotationtext/Text.h"
#include "../common/StringUtils.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/LineString.h"
#include "../geometry/LinearRing.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Point.h"
#include "../geometry/PointM.h"
#include "../geometry/PointZ.h"
#include "../geometry/PointZM.h"
#include "../geometry/Polygon.h"
#include "../postgis/EWKBReader.h"
#include "../postgis/Utils.h"

#include "../common/FileDirUtils.h"
#include "Canvas.h"
//#include "Platform.h"
//#include "Utils.h"

#include <agg_renderer_markers.h>
//#include <agg_path_storage.h>
#include <agg_conv_transform.h>
#include <agg_conv_stroke.h>
#include <agg_renderer_scanline.h>
#include <agg_image_accessors.h>
#include <agg_span_pattern_rgb.h>
#include <agg_span_allocator.h>
#include <agg_conv_smooth_poly1.h>
#include <agg_pattern_filters_rgba.h>
#include <agg_renderer_outline_image.h>
#include <agg_renderer_outline_aa.h>
#include <agg_rasterizer_outline_aa.h>
#include <agg_span_interpolator_linear.h>
#include <agg_span_image_filter_rgba.h>
#include <agg_trans_single_path.h>
#include <stdlib.h>
#include <time.h>

// STL
#include <cassert>

//struct trans_roundoff
//{
//    static void transform(double* x, double* y)
//    {
//        *x = floor(*x + 0.5);
//        *y = floor(*y + 0.5);
//    }
//};
static agg::int8u brightness_to_alpha[256 * 3] = 
{
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 254, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 
    253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 252, 
    252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 251, 251, 251, 251, 251, 
    251, 251, 251, 251, 250, 250, 250, 250, 250, 250, 250, 250, 249, 249, 249, 249, 
    249, 249, 249, 248, 248, 248, 248, 248, 248, 248, 247, 247, 247, 247, 247, 246, 
    246, 246, 246, 246, 246, 245, 245, 245, 245, 245, 244, 244, 244, 244, 243, 243, 
    243, 243, 243, 242, 242, 242, 242, 241, 241, 241, 241, 240, 240, 240, 239, 239, 
    239, 239, 238, 238, 238, 238, 237, 237, 237, 236, 236, 236, 235, 235, 235, 234, 
    234, 234, 233, 233, 233, 232, 232, 232, 231, 231, 230, 230, 230, 229, 229, 229, 
    228, 228, 227, 227, 227, 226, 226, 225, 225, 224, 224, 224, 223, 223, 222, 222, 
    221, 221, 220, 220, 219, 219, 219, 218, 218, 217, 217, 216, 216, 215, 214, 214, 
    213, 213, 212, 212, 211, 211, 210, 210, 209, 209, 208, 207, 207, 206, 206, 205, 
    204, 204, 203, 203, 202, 201, 201, 200, 200, 199, 198, 198, 197, 196, 196, 195, 
    194, 194, 193, 192, 192, 191, 190, 190, 189, 188, 188, 187, 186, 186, 185, 184, 
    183, 183, 182, 181, 180, 180, 179, 178, 177, 177, 176, 175, 174, 174, 173, 172, 
    171, 171, 170, 169, 168, 167, 166, 166, 165, 164, 163, 162, 162, 161, 160, 159, 
    158, 157, 156, 156, 155, 154, 153, 152, 151, 150, 149, 148, 148, 147, 146, 145, 
    144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 
    128, 128, 127, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 
    112, 111, 110, 109, 108, 107, 106, 105, 104, 102, 101, 100,  99,  98,  97,  96,  
     95,  94,  93,  91,  90,  89,  88,  87,  86,  85,  84,  82,  81,  80,  79,  78, 
     77,  75,  74,  73,  72,  71,  70,  69,  67,  66,  65,  64,  63,  61,  60,  59, 
     58,  57,  56,  54,  53,  52,  51,  50,  48,  47,  46,  45,  44,  42,  41,  40, 
     39,  37,  36,  35,  34,  33,  31,  30,  29,  28,  27,  25,  24,  23,  22,  20, 
     19,  18,  17,  15,  14,  13,  12,  11,   9,   8,   7,   6,   4,   3,   2,   1
};

class pattern_src_brightness_to_alpha_rgba8
{
public:
    pattern_src_brightness_to_alpha_rgba8(agg::rendering_buffer& rb) : 
        m_rb(&rb), m_pf(*m_rb) {}

    unsigned width()  const { return m_pf.width();  }
    unsigned height() const { return m_pf.height(); }
    agg::rgba8 pixel(int x, int y) const
    {
        agg::rgba8 c = m_pf.pixel(x, y);
        c.a = brightness_to_alpha[c.r + c.g + c.b];
        return c;
    }

private:
    agg::rendering_buffer* m_rb;
    pixfmt m_pf;
};

te::ag::Canvas::Canvas(int w, int h)
  : m_dwidth(w),
    m_dheight(h),
    m_wllx(0.0),
    m_wlly(0.0),
    m_wurx(0.0),
    m_wury(0.0)
{
  m_hdc = ::GetDC(0);
  m_fontEngine = new agg::font_engine_win32_tt_int16(m_hdc);
  m_fontManager = new agg::font_cache_manager<agg::font_engine_win32_tt_int16>(*m_fontEngine);
  m_fontEngine->create_font("Verdana", agg::glyph_ren_outline);
  m_fontEngine->height(16.);

  m_buffer = new unsigned char[m_dwidth * m_dheight * DEPTH/8];
  m_renderingBuffer = agg::rendering_buffer(m_buffer, m_dwidth, m_dheight, -m_dwidth * DEPTH/8);
  m_pixfmt = new pixfmt(m_renderingBuffer);
  m_rendererBase = agg::renderer_base<pixfmt>(*m_pixfmt);

  m_bgColor = color_type(255, 255, 255, 0);
  memset(m_buffer, 255, m_dwidth * m_dheight * DEPTH/8);
//  clear();

  m_polyPatternBuffer = 0;
  m_contourPatternBuffer = 0;
  m_linePatternBuffer = 0;
  m_ptPatternBuffer = 0;

  m_polyFillColor = color_type(255, 0, 0, 255);
  m_polyPatternWidth = 20;
  m_polyContourColor = color_type(0, 0, 0, 255);
  m_polyContourWidth = 1.;

  m_lineColor = color_type(100, 255, 255, 255);
  m_lineWidth = 1.;

  m_ptColor = color_type(255, 0, 255, 255);
  m_ptMarkerType = (agg::marker_e)MarkerTriangleUp;
  m_ptWidth = 16;

  m_txColor = color_type(0, 0, 255, 255);
  m_txBoundaryWidth = 1.;
  m_txBoundaryColor = color_type(255, 0, 255, 255);
  m_txDecorationColor = color_type(255, 0, 0, 255);
  m_txDecorationWidth = 1.;
  m_txXScale = 1.;
}

te::ag::Canvas::~Canvas()
{
  delete m_fontManager;
  delete m_fontEngine;
  ::ReleaseDC(0, m_hdc);

  delete m_pixfmt;
  delete m_buffer;
  delete m_polyPatternBuffer;
  delete m_contourPatternBuffer;
  delete m_linePatternBuffer;
  delete m_ptPatternBuffer;
}

void te::ag::Canvas::setWindow(const double& llx, const double& lly,
                                        const double& urx, const double& ury)
{
  m_wllx = llx;
  m_wlly = lly;
  m_wurx = urx;
  m_wury = ury;

  double xScale = static_cast<double>(m_dwidth) / (urx - llx);
  double yScale = static_cast<double>(m_dheight) / (ury - lly);

  m_matrix.reset();
  m_matrix *= agg::trans_affine_translation(-m_wllx, -m_wlly);
  m_matrix *= agg::trans_affine_scaling(xScale, yScale);
}

void te::ag::Canvas::adjustWorldWindow(double& llx, double& lly, double& urx, double& ury, 
          const HAlignType& ha, const VAlignType& va)
{
  double ww = urx - llx;
  double wh = ury - lly;
  double widthByHeight = (double)getWidth()/(double)getHeight();
  if(widthByHeight > ww/wh)
  {
    double v = ww;
    ww = wh * widthByHeight;

    if(ha == Left)
      urx = llx + ww;
    else if(ha == Right)
      llx = urx - ww;
    else
    {
      llx = llx - (ww - v) / 2.;
      urx = llx + ww;
    }
  }
  else
  {
    double v = wh;
    wh = ww / widthByHeight;

    if(va == Top)
      lly = ury - wh;
    else if(va == Bottom)
      ury = lly + wh;
    else
    {
      lly = lly - (wh - v) / 2.;
      ury = lly + wh;
    }
  }
  setWindow(llx, lly, urx, ury);
}

void te::ag::Canvas::setBackgroundColor(const te::color::RGBAColor& color)
{
  m_bgColor = color_type(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

te::color::RGBAColor te::ag::Canvas::getBackgroundColor() const
{
  return te::color::RGBAColor(m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
}

void te::ag::Canvas::clear()
{
  m_rendererBase.clear(m_bgColor);
}

void te::ag::Canvas::resize(int w, int h)
{
  m_dwidth = w;
  m_dheight = h;
  delete m_buffer;
  delete m_pixfmt;

  m_buffer = new unsigned char[m_dwidth * m_dheight * DEPTH/8];
  memset(m_buffer, 255, m_dwidth * m_dheight * DEPTH/8);
  m_renderingBuffer = agg::rendering_buffer(m_buffer, m_dwidth, m_dheight, -m_dwidth * DEPTH/8);
  m_pixfmt = new pixfmt(m_renderingBuffer);
  m_rendererBase = agg::renderer_base<pixfmt>(*m_pixfmt);

  clear();
}

char* te::ag::Canvas::drawGeometry(char* geom)
{
  unsigned int wkbGeomType = *((unsigned int*)(geom + 1)); // skip: 1 (byte order)

  te::pgis::Convert2OGCWKBType(wkbGeomType);

  te::gm::GeomType geomType = (te::gm::GeomType)(wkbGeomType);

  switch(geomType)
  {
    case te::gm::MultiPolygonType:
      return drawMultiPolygon(geom);

    case te::gm::PolygonType:
      return drawPolygon(geom);

    case te::gm::PointType:
      return drawPoint(geom);

    case te::gm::MultiLineStringType:
      return drawMultiLineString(geom);

    case te::gm::MultiPointType:
      return drawMultiPoint(geom);

    case te::gm::LineStringType:
      return drawLineString(geom);

    case te::gm::PointZType:
      return drawPointZ(geom);

    case te::gm::PointMType:
      return drawPointM(geom);

    case te::gm::PointZMType:
      return drawPointZM(geom);

    case te::gm::GeometryCollectionType:
      return drawGeometryCollection(geom);

    default:
      assert(false);
      return 0;
  }
}

char* te::ag::Canvas::drawPoint(char* point)
{
  if((*(unsigned int*)(point + 1)) & TE_EWKB_SRID_FLAG)
    point += 4;

  double x = *((double*)(point + 5));   // skip: 1 byte order + sizeof(unsigned int) (geom type = point)
  double y = *((double*)(point + 13));  // skip: 1 byte order + sizeof(unsigned int) (geom type = point) + sizeof(double)

  m_matrix.transform(&x, &y);
  if(m_ptPatternBuffer == 0)
  {
    agg::renderer_markers<agg::renderer_base<pixfmt> > m_rendMarker(m_rendererBase);
    m_rendMarker.fill_color(m_ptColor);
    m_rendMarker.line_color(m_ptColor);
    m_rendMarker.marker((int)x, (int)y, m_ptWidth/2, m_ptMarkerType);
  }
  else
  {
    //x -= (double)m_ptPatternRenderingBuffer.width()/2.;
    //y -= (double)m_ptPatternRenderingBuffer.height()/2.;
    //m_rendererBase.copy_from(m_ptPatternRenderingBuffer, 0, (int)x, (int)y);
    pixfmt pixf_img(m_ptPatternRenderingBuffer);

    pixfmt_pre        pixf_pre(m_renderingBuffer);
    agg::renderer_base<pixfmt_pre> rb_pre(pixf_pre);

    int w = m_ptWidth / 2;
    int h = m_ptPatternRenderingBuffer.height() * m_ptWidth / m_ptPatternRenderingBuffer.width() / 2;
    m_rasterizer.reset();
    m_rasterizer.move_to_d(x-w, y-h);
    m_rasterizer.line_to_d(x+w, y-h);
    m_rasterizer.line_to_d(x+w, y+h);
    m_rasterizer.line_to_d(x-w, y+h);

    typedef agg::span_allocator<color_type> span_alloc_type;
    span_alloc_type sa;
    agg::image_filter_bilinear filter_kernel;
    agg::image_filter_lut filter(filter_kernel, true);

    typedef agg::image_accessor_clone<pixfmt> source_type;
    source_type source(pixf_img);

    agg::trans_affine tr;
    tr.translate(-(x-w), -(y-h));
    double scale = (double)pixf_img.width() / (double)m_ptWidth;
    tr.scale(scale, scale);

    typedef agg::span_interpolator_linear<agg::trans_affine> interpolator_type;
    interpolator_type interpolator(tr);

    agg::span_image_filter_rgba_2x2<source_type, interpolator_type> sg(source, interpolator, filter);
    agg::render_scanlines_aa(m_rasterizer, m_scanline, rb_pre, sa, sg);
  }

  point += 21;  // skip: 1 byte order + sizeof(unsigned int == geom type == point) + 2 * sizeof(double)

  return point;
}

char* te::ag::Canvas::drawPointZ(char* point)
{
  point = drawPoint(point);

  return (point + 8); // skip: point + sizeof(double == z coordinate)
}

char* te::ag::Canvas::drawPointM(char* point)
{
  point = drawPoint(point);

  return (point + 8); // skip: point + sizeof(double == m coordinate)
}

char* te::ag::Canvas::drawPointZM(char* point)
{
  point = drawPoint(point);

  return (point + 16);  // skip: point + 2 * sizeof(double == m coordinate + z coordinate)
}

char* te::ag::Canvas::drawMultiPoint(char* mpoint)
{
  if((*(unsigned int*)(mpoint + 1)) & TE_EWKB_SRID_FLAG)
    mpoint += 4;

  mpoint += 5;  // skip: 1 (byte order) + 4 (geom type == MultiPoint)

// let's know how many points we have in the MultiPoint
  const unsigned int nPts = *((unsigned int*)(mpoint)); 

  mpoint += 4;  // skip: 4 (num points)

  for(register unsigned int i = 0; i != nPts; ++i)
    mpoint = drawPoint(mpoint);

  return mpoint;
}

char* te::ag::Canvas::drawLineString(char* line)
{
  if((*(unsigned int*)(line + 1)) & TE_EWKB_SRID_FLAG)
    line += 4;

// let's know how many points we have in the line
  const unsigned int nPoints = *((unsigned int*)(line + 5)); // skip: 1 (byte order) + 4 (geom type = LineString)

  assert(nPoints >= 2);

// let's go to the first point
  line += 9; // skip: 1 (byte order) + 4 (geom type = LineString) + 4 (number of points)

  size_t size = nPoints*2;
  double* v = new double[size];

  te::gm::Coord2D* coords = (te::gm::Coord2D*)line;

  int j = 0;
  for(register unsigned int i = 0; i != nPoints; ++i)
  { 
    v[j++] = coords[i].x;
    v[j++] = coords[i].y;
  }
  
  drawLineString(v, size);

  line += 16 * nPoints; // skip: 2 * sizeof(double) * number of points
  return line;
}

char* te::ag::Canvas::drawMultiLineString(char* mline)
{
  if((*(unsigned int*)(mline + 1)) & TE_EWKB_SRID_FLAG)
    mline += 4;

  mline += 5;  // skip: 1 (byte order) + 4 (geom type = MultiLineString)

// let's know how many lines we have in the MultiLineString
  const unsigned int nLines = *((unsigned int*)(mline)); 

  mline += 4;  // skip: 4 (num lines)

  for(register unsigned int i = 0; i != nLines; ++i)
    mline = drawLineString(mline);

  return mline;
}

char* te::ag::Canvas::drawPolygon(char* poly)
{
  if((*(unsigned int*)(poly + 1)) & TE_EWKB_SRID_FLAG)
    poly += 4;

// let's know how many rings we have in the polygon
  const unsigned int nRings = *((unsigned int*)(poly + 5)); // skip: 1 (byte order) + 4 (geom type = Polygon)

  assert(nRings > 0);

  agg::path_storage path;
  int pathIndex = path.start_new_path();

// let's go the first ring
  char* ptr = poly + 9; // skip: 1 (byte order) + 4 (geom type = Polygon) + 4 (number of rings)

  for(register unsigned int i = 0; i != nRings; ++i)
  {
    const unsigned int nPoints = *((unsigned int*)ptr);

    assert(nPoints > 3);

    ptr += 4;                // skip: sizeof(unsigned int) => nPoints

    double* v = new double[nPoints*2];
    memcpy(v, (double*)ptr, 16 * nPoints);
    
    ptr += (16 * nPoints); // skip: sizeof(unsigned int) (number of points) + 2 * sizeof(double) * nPts

    path.concat_poly(v, nPoints, true);
    delete []v;
  }

  drawPolygon(path, pathIndex);

  return ptr;
}

char* te::ag::Canvas::drawMultiPolygon(char* mpoly)
{
  if((*(unsigned int*)(mpoly + 1)) & TE_EWKB_SRID_FLAG)
    mpoly += 4;

  mpoly += 5; // skip: 1 (byte order) + 4 (geom type = MultiPolygon)

  const unsigned int nPolygons = *((unsigned int*)mpoly);

  assert(nPolygons > 0);

  mpoly += 4;  // skip: 4 (num polygons)

  for(unsigned int i = 0; i != nPolygons; ++i)
    mpoly = drawPolygon(mpoly);

  return mpoly;
}

char* te::ag::Canvas::drawGeometryCollection(char* g)
{
  if((*(unsigned int*)(g + 1)) & TE_EWKB_SRID_FLAG)
    g += 4;

  g += 5; // skip: 1 (byte order) + 4 (geom type = GeometryCollection)

  const unsigned int nGeoms = *((unsigned int*)g);

  assert(nGeoms > 0);

  g += 4;  // skip: 4 (num geometries)

  for(register unsigned int i = 0; i != nGeoms; ++i)
    g = drawGeometry(g);

  return g;
}

void te::ag::Canvas::draw(te::gm::Geometry* geom)
{
  switch(geom->getGeomTypeId())
  {
    case te::gm::MultiPolygonType:
    case te::gm::MultiPolygonZType:
    case te::gm::MultiPolygonMType:
    case te::gm::MultiPolygonZMType:
      draw(static_cast<te::gm::MultiPolygon*>(geom));
      break;

    case te::gm::PointType:
    case te::gm::PointZType:
    case te::gm::PointMType:
    case te::gm::PointZMType:
      draw(static_cast<te::gm::Point*>(geom));
      break;

    case te::gm::LineStringType:
    case te::gm::LineStringZType:
    case te::gm::LineStringMType:
    case te::gm::LineStringZMType:
      draw(static_cast<te::gm::LineString*>(geom));
      break;

    case te::gm::PolygonType:
    case te::gm::PolygonZType:
    case te::gm::PolygonMType:
    case te::gm::PolygonZMType:
      draw(static_cast<te::gm::Polygon*>(geom));
      break;

    case te::gm::MultiLineStringType:
    case te::gm::MultiLineStringZType:
    case te::gm::MultiLineStringMType:
    case te::gm::MultiLineStringZMType:
      draw(static_cast<te::gm::MultiLineString*>(geom));
      break;

    case te::gm::MultiPointType:
    case te::gm::MultiPointZType:
    case te::gm::MultiPointMType:
    case te::gm::MultiPointZMType:
      draw(static_cast<te::gm::MultiPoint*>(geom));
      break;

    case te::gm::GeometryCollectionType:
    case te::gm::GeometryCollectionZType:
    case te::gm::GeometryCollectionMType:
    case te::gm::GeometryCollectionZMType:
      draw(static_cast<te::gm::GeometryCollection*>(geom));
      break;

    default:
      break;
  }
}

void te::ag::Canvas::draw(te::gm::Point* point)
{
  double x = point->getX();
  double y = point->getY();

  m_matrix.transform(&x, &y);
  if(m_ptPatternBuffer == 0)
  {
    agg::renderer_markers<agg::renderer_base<pixfmt> > m_rendMarker(m_rendererBase);
    m_rendMarker.fill_color(m_ptColor);
    m_rendMarker.line_color(m_ptColor);
    m_rendMarker.marker((int)x, (int)y, m_ptWidth/2, m_ptMarkerType);
  }
  else
  {
    pixfmt pixf_img(m_ptPatternRenderingBuffer);

    pixfmt_pre        pixf_pre(m_renderingBuffer);
    agg::renderer_base<pixfmt_pre> rb_pre(pixf_pre);

    int w = m_ptWidth / 2;
    int h = m_ptPatternRenderingBuffer.height() * m_ptWidth / m_ptPatternRenderingBuffer.width() / 2;
    m_rasterizer.reset();
    m_rasterizer.move_to_d(x-w, y-h);
    m_rasterizer.line_to_d(x+w, y-h);
    m_rasterizer.line_to_d(x+w, y+h);
    m_rasterizer.line_to_d(x-w, y+h);

    typedef agg::span_allocator<color_type> span_alloc_type;
    span_alloc_type sa;
    agg::image_filter_bilinear filter_kernel;
    agg::image_filter_lut filter(filter_kernel, true);

    typedef agg::image_accessor_clone<pixfmt> source_type;
    source_type source(pixf_img);

    agg::trans_affine tr;
    tr.translate(-(x-w), -(y-h));
    double scale = (double)pixf_img.width() / (double)m_ptWidth;
    tr.scale(scale, scale);

    typedef agg::span_interpolator_linear<agg::trans_affine> interpolator_type;
    interpolator_type interpolator(tr);

    agg::span_image_filter_rgba_2x2<source_type, interpolator_type> sg(source, interpolator, filter);
    agg::render_scanlines_aa(m_rasterizer, m_scanline, rb_pre, sa, sg);
  }
}

void te::ag::Canvas::draw(te::gm::PointZ* point)
{
  draw((te::gm::Point*)point);
}

void te::ag::Canvas::draw(te::gm::PointM* point)
{
  draw((te::gm::Point*)point);
}

void te::ag::Canvas::draw(te::gm::PointZM* point)
{
  draw((te::gm::Point*)point);
}

void te::ag::Canvas::draw(te::gm::MultiPoint* mpoint)
{
  size_t size = mpoint->getNumGeometries();

  switch(mpoint->getGeomTypeId())
  {
    case te::gm::PointType :
    case te::gm::MultiPointType :
      for(size_t i = 0; i < size; ++i)
        draw(static_cast<te::gm::Point*>(mpoint->getGeometryN(i)));
    break;

    case te::gm::PointZType :
    case te::gm::MultiPointZType :
      for(size_t i = 0; i < size; ++i)
        draw(static_cast<te::gm::PointZ*>(mpoint->getGeometryN(i)));
    break;

    case te::gm::PointMType :
    case te::gm::MultiPointMType :
      for(size_t i = 0; i < size; ++i)
        draw(static_cast<te::gm::PointM*>(mpoint->getGeometryN(i)));
    break;

    case te::gm::PointZMType :
    case te::gm::MultiPointZMType :
      for(size_t i = 0; i < size; ++i)
        draw(static_cast<te::gm::PointZM*>(mpoint->getGeometryN(i)));
    break;

    default:
      assert(false);
    break;
  }
}

void te::ag::Canvas::draw(te::gm::LineString* line)
{
  te::gm::Coord2D* coords = line->getCoordinates();
  size_t size = line->getNPoints();
  double* v = new double[size*2];

  int j = 0;
  for(register size_t i = 0; i != size; ++i)
  { 
    v[j++] = coords[i].x;
    v[j++] = coords[i].y;
  }

  drawLineString(v, size);
}

void te::ag::Canvas::drawLineString(double* v, const size_t& size)
{
  if(m_linePatternBuffer == 0) // draw solid color with alpha
  { 
    agg::path_storage path;
    int pathIndex = path.start_new_path();
    path.concat_poly(v, size, false);
    delete []v;

    m_rasterizer.gamma(agg::gamma_threshold(.4));

    agg::conv_transform<agg::path_storage> fill(path, m_matrix);
    agg::conv_stroke<agg::conv_transform<agg::path_storage> > stroke(fill);

    stroke.width(m_lineWidth);
    m_rasterizer.add_path(stroke, pathIndex); // without roundoff
    agg::render_scanlines_aa_solid(m_rasterizer, m_scanline, m_rendererBase, m_lineColor); // with anti aliase
    path.free_all();
    path.remove_all();
  }
  else // draw with pattern
  {
    pattern_src_brightness_to_alpha_rgba8 p1(m_linePatternRenderingBuffer);

    agg::pattern_filter_bilinear_rgba8 fltr;           // Filtering functor

    // agg::line_image_pattern is the main container for the patterns. It creates
    // a copy of the patterns extended according to the needs of the filter.
    // agg::line_image_pattern can operate with arbitrary image width, but if the 
    // width of the pattern is power of 2, it's better to use the modified
    // version agg::line_image_pattern_pow2 because it works about 15-25 percent
    // faster than agg::line_image_pattern (because of using simple masking instead 
    // of expensive '%' operation). 
    typedef agg::line_image_pattern<agg::pattern_filter_bilinear_rgba8> pattern_type;
    typedef agg::renderer_base<pixfmt> base_ren_type;
    typedef agg::renderer_outline_image<base_ren_type, pattern_type> renderer_img_type;
    typedef agg::rasterizer_outline_aa<renderer_img_type, agg::line_coord_sat> rasterizer_img_type;

    typedef agg::renderer_outline_aa<base_ren_type> renderer_line_type;
    typedef agg::rasterizer_outline_aa<renderer_line_type, agg::line_coord_sat> rasterizer_line_type;

    //-- Create uninitialized and set the source
    pattern_type patt(fltr);        
    patt.create(p1);
    renderer_img_type ren_img(m_rendererBase, patt);
    rasterizer_img_type ras_img(ren_img);

    //-- create uninitialized and set parameters
    agg::line_profile_aa profile;
    profile.smoother_width(p1.height());                    //optional
    profile.width(p1.height()-4);                              //mandatory!
    renderer_line_type ren_line(m_rendererBase, profile);
    ren_line.color(agg::rgba8(200,200,200));            //mandatory!
    rasterizer_line_type ras_line(ren_line);
    //ras_line.round_cap(true);                       //optional
    //ras_line.line_join(agg::outline_no_join);     //optional

    ren_img.scale_x(1.);
    ren_img.start_x(p1.width()/2.);

    agg::poly_plain_adaptor<double> vs(v, size, false);
    agg::conv_transform<agg::poly_plain_adaptor<double> > trans(vs, m_matrix);
    ras_line.add_path(trans);
    ras_img.add_path(trans);

    delete []v;
  }
}

void te::ag::Canvas::draw(te::gm::MultiLineString* mline)
{
  size_t size = mline->getNumGeometries();

  for(size_t i = 0; i < size; ++i)
    draw(static_cast<te::gm::LineString*>(mline->getGeometryN(i)));
}

void te::ag::Canvas::draw(te::gm::Polygon* poly)
{
  size_t nRings = poly->getNumRings();

  assert(nRings > 0);

  agg::path_storage path;
  int pathIndex = path.start_new_path();

  for(register size_t i = 0; i != nRings; ++i)
  { 
    te::gm::LinearRing* ring = static_cast<te::gm::LinearRing*>(poly->getRingN(i));

    const size_t nPoints = ring->getNPoints();

    assert(nPoints > 3);

    double* v = new double[nPoints*2];
    memcpy(v, (double*)(ring->getCoordinates()), 16 * nPoints);
    
    path.concat_poly(v, nPoints, true);
    delete []v;
  }

  drawPolygon(path, pathIndex);
}

void te::ag::Canvas::drawPolygon(agg::path_storage& path, const int& index)
{
  agg::conv_transform<agg::path_storage> transPath(path, m_matrix);

  if(m_polyPatternBuffer == 0) // fill solid color with alpha
  {
    m_rasterizer.gamma(agg::gamma_threshold(.4));
    m_rasterizer.filling_rule(agg::fill_even_odd);

    m_rasterizer.add_path(transPath, index); // without roundoff
    agg::render_scanlines_aa_solid(m_rasterizer, m_scanline, m_rendererBase, m_polyFillColor); // with anti aliase
  }
  else // fill with pattern with alpha
  {
    if(m_polyPatternWidth != m_polyPatternRenderingBuffer.width()) // adjust pattern size
    {
      int width = m_polyPatternWidth;
      int height = m_polyPatternRenderingBuffer.height() * m_polyPatternWidth / m_polyPatternRenderingBuffer.width();
      agg::int8u* buf = new agg::int8u[width * height * DEPTH/8];
      memset(buf, 0, width * height * DEPTH/8);
      agg::rendering_buffer rbuf(buf, width, height, -width * DEPTH/8);

      pixfmt pixf_img(m_polyPatternRenderingBuffer);
      pixfmt_pre        pixf_pre(rbuf);
      agg::renderer_base<pixfmt_pre> rb_pre(pixf_pre);

      m_rasterizer.reset();
      m_rasterizer.move_to_d(0, 0);
      m_rasterizer.line_to_d(width, 0);
      m_rasterizer.line_to_d(width-1, height-1);
      m_rasterizer.line_to_d(0, height-1);

      typedef agg::span_allocator<color_type> span_alloc_type;
      span_alloc_type sa;
      agg::image_filter_bilinear filter_kernel;
      agg::image_filter_lut filter(filter_kernel, true);

      typedef agg::image_accessor_clone<pixfmt> source_type;
      source_type source(pixf_img);

      agg::trans_affine tr;
      double scale = (double)pixf_img.width() / (double)width;
      tr.scale(scale, scale);

      typedef agg::span_interpolator_linear<agg::trans_affine> interpolator_type;
      interpolator_type interpolator(tr);

      agg::span_image_filter_rgba_2x2<source_type, interpolator_type> sg(source, interpolator, filter);
      agg::render_scanlines_aa(m_rasterizer, m_scanline, rb_pre, sa, sg);
      delete m_polyPatternBuffer;
      m_polyPatternBuffer = buf;
      m_polyPatternRenderingBuffer.attach(m_polyPatternBuffer, width, height, -width *  DEPTH/8);
    }

    // fill pattern with alpha
    pixfmt_pre* pixf_pre = new pixfmt_pre(m_renderingBuffer);
    agg::renderer_base<pixfmt_pre> rb_pre(*pixf_pre);

    typedef agg::wrap_mode_repeat_auto_pow2 repeat_x_type;
    typedef agg::wrap_mode_repeat_auto_pow2 repeat_y_type;
    typedef agg::image_accessor_wrap<pixfmt, repeat_x_type, repeat_y_type> img_source_type;
    typedef agg::span_pattern_rgb<img_source_type> span_gen_type;

    agg::span_allocator<color_type> sa;
    pixfmt img_pixf(m_polyPatternRenderingBuffer);
    img_source_type img_src(img_pixf);
    span_gen_type sg(img_src, 0, 0); // 0, 0 eh o inicio do pattern

    // esta opacidade tem que ser a mesma da opacidade usada no pattern 
    sg.alpha(span_gen_type::value_type(m_polyPatternOpacity));

    m_rasterizer.add_path(transPath);
    agg::render_scanlines_aa(m_rasterizer, m_scanline, rb_pre, sa, sg); 
  }

  if(m_contourPatternBuffer == 0)  // draw simple contour
  {
    agg::conv_stroke<agg::conv_transform<agg::path_storage> > stroke(transPath);
    stroke.width(m_polyContourWidth);
    m_rasterizer.add_path(stroke, index); // without roundoff
    agg::render_scanlines_aa_solid(m_rasterizer, m_scanline, m_rendererBase, m_polyContourColor); // with anti aliase
  }
  else  // draw pattern contour
  {
    pattern_src_brightness_to_alpha_rgba8 p1(m_contourPatternRenderingBuffer);

    agg::pattern_filter_bilinear_rgba8 fltr;           // Filtering functor

    // agg::line_image_pattern is the main container for the patterns. It creates
    // a copy of the patterns extended according to the needs of the filter.
    // agg::line_image_pattern can operate with arbitrary image width, but if the 
    // width of the pattern is power of 2, it's better to use the modified
    // version agg::line_image_pattern_pow2 because it works about 15-25 percent
    // faster than agg::line_image_pattern (because of using simple masking instead 
    // of expensive '%' operation). 
    typedef agg::line_image_pattern<agg::pattern_filter_bilinear_rgba8> pattern_type;
    typedef agg::renderer_base<pixfmt> base_ren_type;
    typedef agg::renderer_outline_image<base_ren_type, pattern_type> renderer_img_type;
    typedef agg::rasterizer_outline_aa<renderer_img_type, agg::line_coord_sat> rasterizer_img_type;

    typedef agg::renderer_outline_aa<base_ren_type> renderer_line_type;
    typedef agg::rasterizer_outline_aa<renderer_line_type, agg::line_coord_sat> rasterizer_line_type;

    //-- Create uninitialized and set the source
    pattern_type patt(fltr);        
    patt.create(p1);
    renderer_img_type ren_img(m_rendererBase, patt);
    rasterizer_img_type ras_img(ren_img);

    //-- create uninitialized and set parameters
    agg::line_profile_aa profile;
    profile.smoother_width(p1.height());                    //optional
    profile.width(p1.height()-4);                              //mandatory!
    renderer_line_type ren_line(m_rendererBase, profile);
    ren_line.color(agg::rgba8(200,200,200));            //mandatory!
    rasterizer_line_type ras_line(ren_line);
    //ras_line.round_cap(true);                       //optional
    //ras_line.line_join(agg::outline_no_join);     //optional

    ren_img.scale_x(1.);
    ren_img.start_x(p1.width()/2.);

    double *v = 0;
    double x, y;
    int i, j, k, n;
    path.rewind(index);
    int size = path.total_vertices();

    for(k = i = 0; i < size; ++i)
    {
      if(path.vertex(i, &x, &y) != 79)
        continue;
      j = k;
      k = i - k;
      v = new double[(k+1)<<1];
      n = 0;
      while(j < i)
      {
        path.vertex(j, &x, &y);
        v[n++] = x;
        v[n++] = y;
        ++j;
      }
      v[n++] = v[0];
      v[n++] = v[1];

      agg::poly_plain_adaptor<double> vs(v, k+1, false);
      agg::conv_transform<agg::poly_plain_adaptor<double> > trans(vs, m_matrix);
      ras_line.add_path(trans);
      ras_img.add_path(trans);
      delete []v;
      ++k;
    }
  }
  path.free_all();
  path.remove_all();  
}

void te::ag::Canvas::draw(te::gm::MultiPolygon* mpoly)
{
  size_t size = mpoly->getNumGeometries();

  for(size_t i = 0; i < size; ++i)
    draw(static_cast<te::gm::Polygon*>(mpoly->getGeometryN(i)));
}

void te::ag::Canvas::draw(te::gm::GeometryCollection* g)
{
  size_t size = g->getNumGeometries();

  for(size_t i = 0; i < size; ++i)
    draw(g->getGeometryN(i));
}

char* te::ag::Canvas::getImage(te::map::Canvas::ImageType t, size_t& size) const
{
  //if(!m_device)
  //  return 0;

  //if(m_device->devType() == 4)
  //  return 0;

  //QByteArray bytes;
  //QBuffer buffer(&bytes);
  //buffer.open(QIODevice::WriteOnly);

  //switch(t)
  //{
  //  case PNG:
  //    ((QPixmap*)m_device)->save(&buffer, Platform::sm_pngFmt);
  //    break;

  //  case JPEG:
  //    ((QPixmap*)m_device)->save(&buffer, Platform::sm_jpegFmt);
  //    break;

  //  case GIF:
  //    ((QPixmap*)m_device)->save(&buffer, Platform::sm_gifFmt);
  //    break;

  //  case BMP:
  //    ((QPixmap*)m_device)->save(&buffer, Platform::sm_bmpFmt);
  //    break;

  //  case XPM:
  //    ((QPixmap*)m_device)->save(&buffer, Platform::sm_xpmFmt);
  //    break;

  //  case XBM:
  //    ((QPixmap*)m_device)->save(&buffer, Platform::sm_xbmFmt);
  //    break;

  //  default:
  //    return 0;
  //};

  //int nbytes = bytes.size();

  //char* result = new char[nbytes];

  //memcpy(result, bytes.data(), nbytes);

  //size = nbytes;

  //return result;
  return 0;
}

void te::ag::Canvas::freeImage(char* img) const
{
  delete [] img;
}

void te::ag::Canvas::drawImage(char* src, int size, te::map::Canvas::ImageType t)
{
}

void te::ag::Canvas::drawImage(te::color::RGBAColor** src, int w, int h)
{
  int i, j, k, n;

  agg::int8u* buf = new agg::int8u[h * w * DEPTH/8];
  for(i = 0; i < h; ++i)
  {
    k = i * 4 * w;
    for(j = 0; j < w; ++j)
    {
      te::color::RGBAColor c = src[i][j];
      n = k + 4 * j;
      buf[n] = (agg::int8u)c.getRed();
      buf[n + 1] = (agg::int8u)c.getGreen();
      buf[n + 2] = (agg::int8u)c.getBlue();
      buf[n + 3] = (agg::int8u)c.getAlpha();
    }
  }
  agg::rendering_buffer rbuf(buf, w, h, -w * DEPTH/8);

  pixfmt pixf_img(rbuf);
  pixfmt_pre pixf_pre(m_renderingBuffer);
  agg::renderer_base<pixfmt_pre> rb_pre(pixf_pre);

  typedef agg::span_allocator<color_type> span_alloc_type;
  span_alloc_type sa;
  agg::image_filter_bilinear filter_kernel;
  agg::image_filter_lut filter(filter_kernel, true);

  typedef agg::image_accessor_clone<pixfmt> source_type;
  source_type source(pixf_img);

  agg::trans_affine tr;
  int ty = m_dheight - rbuf.height();
  tr.translate(0, -ty);
  tr.scale(1., 1.);
  typedef agg::span_interpolator_linear<agg::trans_affine> interpolator_type;
  interpolator_type interpolator(tr);

  m_rasterizer.reset();
  m_rasterizer.move_to_d(0, ty);
  m_rasterizer.line_to_d(w, ty);
  m_rasterizer.line_to_d(w, ty + rbuf.height());
  m_rasterizer.line_to_d(0, ty + rbuf.height());

  agg::span_image_filter_rgba_2x2<source_type, interpolator_type> sg(source, interpolator, filter);
  agg::render_scanlines_aa(m_rasterizer, m_scanline, rb_pre, sa, sg);
  delete buf;
}

void te::ag::Canvas::drawImage(int x, int y, char* src, int size, te::map::Canvas::ImageType t)
{
  //QPixmap pix;
  //const char* f = GetFormat(t);
  //pix.loadFromData((unsigned char*)src, size, f);
  //m_painter.setWorldMatrixEnabled(false);
  //m_painter.drawPixmap(x, y, pix);
  //m_painter.setWorldMatrixEnabled(true);
}

void te::ag::Canvas::drawImage(int x, int y, te::color::RGBAColor** src, int w, int h)
{
  int i, j, k, n;

  agg::int8u* buf = new agg::int8u[h * w * DEPTH/8];
  for(i = 0; i < h; ++i)
  {
    k = i * 4 * w;
    for(j = 0; j < w; ++j)
    {
      te::color::RGBAColor c = src[i][j];
      n = k + 4 * j;
      buf[n] = (agg::int8u)c.getRed();
      buf[n + 1] = (agg::int8u)c.getGreen();
      buf[n + 2] = (agg::int8u)c.getBlue();
      buf[n + 3] = (agg::int8u)c.getAlpha();
    }
  }
  agg::rendering_buffer rbuf(buf, w, h, -w * DEPTH/8);

  pixfmt pixf_img(rbuf);
  pixfmt_pre pixf_pre(m_renderingBuffer);
  agg::renderer_base<pixfmt_pre> rb_pre(pixf_pre);

  typedef agg::span_allocator<color_type> span_alloc_type;
  span_alloc_type sa;
  agg::image_filter_bilinear filter_kernel;
  agg::image_filter_lut filter(filter_kernel, true);

  typedef agg::image_accessor_clone<pixfmt> source_type;
  source_type source(pixf_img);

  agg::trans_affine tr;
  int ty = m_dheight - rbuf.height() -y;
  tr.translate(-x, -ty);
  tr.scale(1., 1.);
  typedef agg::span_interpolator_linear<agg::trans_affine> interpolator_type;
  interpolator_type interpolator(tr);

  m_rasterizer.reset();
  m_rasterizer.move_to_d(x, ty);
  m_rasterizer.line_to_d(x+w, ty);
  m_rasterizer.line_to_d(x+w, ty + rbuf.height());
  m_rasterizer.line_to_d(x, ty + rbuf.height());

  agg::span_image_filter_rgba_2x2<source_type, interpolator_type> sg(source, interpolator, filter);
  agg::render_scanlines_aa(m_rasterizer, m_scanline, rb_pre, sa, sg);
  delete buf;
}

void te::ag::Canvas::drawImage(int x, int y, int w, int h, char* src, int size, te::map::Canvas::ImageType t)
{
  //QPixmap pix;
  //const char* f = GetFormat(t);
  //pix.loadFromData((unsigned char*)src, size, f);
  //m_painter.setWorldMatrixEnabled(false);
  //m_painter.drawPixmap(x, y, w, h, pix);
  //m_painter.setWorldMatrixEnabled(true);
}

void te::ag::Canvas::drawImage(int x, int y, int w, int h, te::color::RGBAColor** src, int srcw, int srch)
{
  int i, j, k, n;

  agg::int8u* buf = new agg::int8u[srch * srcw * DEPTH/8];
  for(i = 0; i < srch; ++i)
  {
    k = i * 4 * srcw;
    for(j = 0; j < srcw; ++j)
    {
      te::color::RGBAColor c = src[i][j];
      n = k + 4 * j;
      buf[n] = (agg::int8u)c.getRed();
      buf[n + 1] = (agg::int8u)c.getGreen();
      buf[n + 2] = (agg::int8u)c.getBlue();
      buf[n + 3] = (agg::int8u)c.getAlpha();
    }
  }
  agg::rendering_buffer rbuf(buf, srcw, srch, -srcw * DEPTH/8);

  pixfmt pixf_img(rbuf);
  pixfmt_pre pixf_pre(m_renderingBuffer);
  agg::renderer_base<pixfmt_pre> rb_pre(pixf_pre);

  typedef agg::span_allocator<color_type> span_alloc_type;
  span_alloc_type sa;
  agg::image_filter_bilinear filter_kernel;
  agg::image_filter_lut filter(filter_kernel, true);

  typedef agg::image_accessor_clone<pixfmt> source_type;
  source_type source(pixf_img);

  agg::trans_affine tr;
  double xscale = (double)pixf_img.width() / (double)w;
  double yscale = (double)pixf_img.height() / (double)h;
  int nh = (int)((double)(rbuf.height())/yscale + .5);
  int ty = m_dheight - nh - y;
  tr.translate(-x, -ty);
  tr.scale(xscale, yscale);

  typedef agg::span_interpolator_linear<agg::trans_affine> interpolator_type;
  interpolator_type interpolator(tr);

  m_rasterizer.reset();
  m_rasterizer.move_to_d(x, ty);
  m_rasterizer.line_to_d(x+w, ty);
  m_rasterizer.line_to_d(x+w, ty+h);
  m_rasterizer.line_to_d(x, ty+h);

  agg::span_image_filter_rgba_2x2<source_type, interpolator_type> sg(source, interpolator, filter);
  agg::render_scanlines_aa(m_rasterizer, m_scanline, rb_pre, sa, sg);
  delete buf;
}

void te::ag::Canvas::drawImage(int x, int y, int w, int h, char* src, int size, te::map::Canvas::ImageType t, int sx, int sy, int sw, int sh)
{
  //QPixmap pix;
  //const char* f = GetFormat(t);
  //pix.loadFromData((unsigned char*)src, size, f);
  //m_painter.setWorldMatrixEnabled(false);
  //m_painter.drawPixmap(x, y, w, h, pix, sx, sy, sw, sh);
  //m_painter.setWorldMatrixEnabled(true);
}

void te::ag::Canvas::drawImage(int x, int y, int w, int h, te::color::RGBAColor** src, int sx, int sy, int sw, int sh)
{
  int i, j, k, n;

  agg::int8u* buf = new agg::int8u[sh * sw * DEPTH/8];
  for(i = sy; i < sh; ++i)
  {
    k = (i - sy) * 4 * sw;
    for(j = sx; j < sw; ++j)
    {
      te::color::RGBAColor c = src[i][j];
      n = k + 4 * (j - sx);
      buf[n] = (agg::int8u)c.getRed();
      buf[n + 1] = (agg::int8u)c.getGreen();
      buf[n + 2] = (agg::int8u)c.getBlue();
      buf[n + 3] = (agg::int8u)c.getAlpha();
    }
  }
  agg::rendering_buffer rbuf(buf, sw, sh, -sw * DEPTH/8);

  pixfmt pixf_img(rbuf);
  pixfmt_pre pixf_pre(m_renderingBuffer);
  agg::renderer_base<pixfmt_pre> rb_pre(pixf_pre);

  typedef agg::span_allocator<color_type> span_alloc_type;
  span_alloc_type sa;
  agg::image_filter_bilinear filter_kernel;
  agg::image_filter_lut filter(filter_kernel, true);

  typedef agg::image_accessor_clone<pixfmt> source_type;
  source_type source(pixf_img);

  agg::trans_affine tr;
  double xscale = (double)pixf_img.width() / (double)w;
  double yscale = (double)pixf_img.height() / (double)h;
  int nh = (int)((double)(rbuf.height())/yscale + .5);
  int ty = m_dheight - nh - y;
  tr.translate(-x, -ty);
  tr.scale(xscale, yscale);

  typedef agg::span_interpolator_linear<agg::trans_affine> interpolator_type;
  interpolator_type interpolator(tr);

  m_rasterizer.reset();
  m_rasterizer.move_to_d(x, ty);
  m_rasterizer.line_to_d(x+w, ty);
  m_rasterizer.line_to_d(x+w, ty+h);
  m_rasterizer.line_to_d(x, ty+h);

  agg::span_image_filter_rgba_2x2<source_type, interpolator_type> sg(source, interpolator, filter);
  agg::render_scanlines_aa(m_rasterizer, m_scanline, rb_pre, sa, sg);
  delete buf;}

void te::ag::Canvas::drawPixel(int x, int y)
{
  int yy = m_dheight - y - 1;
  m_rendererBase.blend_pixel(x, yy, m_ptColor, agg::cover_full);
}

void te::ag::Canvas::drawPixel(int x, int y, const te::color::RGBAColor& color)
{
  int yy = m_dheight - y - 1;
  color_type cor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
  m_rendererBase.blend_pixel(x, yy, cor, agg::cover_full);
}

void te::ag::Canvas::drawText(int x, int y,
                                       const std::string& tx,
                                       float angle,
                                       te::at::HorizontalAlignment hAlign,
                                       te::at::VerticalAlignment vAlign)
{
  agg::trans_affine matrix;
  matrix.scale(m_txXScale, 1.);
  m_fontEngine->transform(matrix);
  double xa = 0.;
  double ya = 0.;
  const char* p = tx.c_str();

  while(*p)
  {
    const agg::glyph_cache* glyph = m_fontManager->glyph(*p);
    if(glyph)
    {
      m_fontManager->add_kerning(&xa, &ya);
      m_fontManager->init_embedded_adaptors(glyph, xa, ya);

      // increment pen position
      xa += glyph->advance_x;
      ya += glyph->advance_y;
    }
    ++p;
  }
  double txWidth = xa;

  double xi = x;
  double yi = y;
  matrix.reset();
  matrix.translate(-xi, -yi);
  matrix.rotate(-angle * PI / 180.); // -angle ou nao???????????
  matrix.translate(xi, yi);

  if(hAlign == te::at::CenterH)
    xi = x + txWidth / 2.;
  else if(hAlign == te::at::End)
    xi = x + txWidth;

  if(vAlign == te::at::Top)
    yi = y + m_fontEngine->height();
  else if(vAlign == te::at::CenterV)
    yi = y + m_fontEngine->height() * 3. /8.;
  else if(vAlign == te::at::Bottom)
    yi = y - m_fontEngine->height() / 4.;

  matrix.transform(&xi, &yi);

  //int size = 2;
  double xx = x - (xi - x);
  double yy = m_dheight - yi;
  double xf = xx + (m_dwidth<<2);
  double yf = yy;
  matrix.reset();
  matrix.translate(-xx, -yy);
  matrix.scale(m_txXScale, 1.);
  matrix.rotate(-angle * PI / 180.); // -angle ou nao???????????
  matrix.translate(xx, yy);
  matrix.transform(&xf, &yf);

  agg::path_storage path;
  int pathIndex = path.start_new_path();
  path.move_to(xx, yy);
  path.line_to(xf, yf);

  agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt> > rendererSolid(m_rendererBase);

  agg::trans_single_path tcurve;
  tcurve.add_path(path);
  tcurve.preserve_x_scale(true);

  conv_font_curve_type fcurves(m_fontManager->path_adaptor());

  conv_font_segm_type  fsegm(fcurves);
  conv_font_trans_type ftrans(fsegm, tcurve);
  fsegm.approximation_scale(3.0);
  fcurves.approximation_scale(2.0);

  xa = 0.;
  ya = 0.;
  p = tx.c_str();

  while(*p)
  {
    const agg::glyph_cache* glyph = m_fontManager->glyph(*p);
    if(glyph)
    {
      if(xa > tcurve.total_length()) 
        break;

      m_fontManager->add_kerning(&xa, &ya);
      m_fontManager->init_embedded_adaptors(glyph, xa, ya);

      if(glyph->data_type == agg::glyph_data_outline)
      {
        m_rasterizer.reset();
        m_rasterizer.add_path(ftrans);
        rendererSolid.color(m_txColor);
        agg::render_scanlines(m_rasterizer, m_scanline, rendererSolid);
      }

      // increment pen position
      xa += glyph->advance_x;
      ya += glyph->advance_y;
    }
    ++p;
  }
  path.free_all();
  path.remove_all();

  if(m_txDecorationType > 0)
  {
    matrix.reset();
    matrix.translate(-xx, -yy);
    matrix.rotate(-angle * PI / 180.); // -angle ou nao???????????
    matrix.translate(xx, yy);

    xf = xx + xa;
    yf = yy + ya + m_fontEngine->height();
    yy -= m_fontEngine->height()/4.;

    if(m_txDecorationType == 1)
      yy += m_fontEngine->height()/4.;
    else if(m_txDecorationType == 2)
      yy += m_fontEngine->height() + m_fontEngine->height()/4.;
    else if(m_txDecorationType == 3)
      yy += (m_fontEngine->height() + m_fontEngine->height()/4.) / 2.;

    pathIndex = path.start_new_path();
    double a, b;
    a = xx;
    b = yy;
    matrix.transform(&a, &b);
    path.move_to(a, b);
    a = xf;
    b = yy;
    matrix.transform(&a, &b);
    path.line_to(a, b);
    path.close_polygon(false);
    matrix.reset();
    m_rasterizer.gamma(agg::gamma_threshold(.4));
    agg::conv_transform<agg::path_storage> fill(path, matrix);
    agg::conv_stroke<agg::conv_transform<agg::path_storage> > stroke(fill);
    stroke.width(m_txDecorationWidth);
    m_rasterizer.add_path(stroke, pathIndex); // without roundoff
    agg::render_scanlines_aa_solid(m_rasterizer, m_scanline, m_rendererBase, m_txDecorationColor); // with anti aliase

    path.free_all();
    path.remove_all();  
  }
}

void te::ag::Canvas::drawText(double* vin, const int& size, const agg::trans_affine& matrix, const std::string& tx)
{
  int fontSize = 24;
  int distx = 0;
  int disty = 0;
  int i, s = size<<1;
  //char buf[200];
  //FILE* fp = fopen("C:\\lixo\\testetxt.txt", "a");
  double *v = 0;
  if(matrix.is_identity() == false)
  {
    v = new double[size<<1];
    double x, y;
    int i = 0;
    while(i < s)
    {
      x = vin[i];
      y = vin[i+1];
      matrix.transform(&x, &y);
      v[i++] = (int)(x+.5);
      v[i++] = (int)(y+.5);
      if(i > 3)
      {
        distx += abs((int)(v[i-2] - v[i-4]));
        disty += abs((int)(v[i-1] - v[i-3]));
      }
      //sprintf(buf, "%d %d\n", (int)x, (int)y);
      //fwrite(buf, 1, strlen(buf), fp);
    }
    //fwrite("\n\n\n", 1, strlen("\n\n\n"), fp);
    //fclose(fp);
    delete []vin;

    if(distx == 0 && disty == 0)
    {
      delete []v;
      return;
    }
  }
  else
  {
    v = vin;
    i = 4;
    while(i <= s)
    {
      distx += abs((int)(v[i-2] - v[i-4]));
      disty += abs((int)(v[i-1] - v[i-3]));
      i += 2;
    }
  }

  int len = (int)tx.size() * fontSize/2;
  int dist = (int)sqrt((double)(distx * distx + disty * disty));
  if(len > dist)
  {
    delete []v;
    return;
  }

  agg::path_storage path;
  path.start_new_path();
  //int pathIndex = path.start_new_path();
  path.concat_poly(v, size, false);
  delete []v;

  agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt> > rendererSolid(m_rendererBase);

  agg::trans_single_path tcurve;
  tcurve.add_path(path);
  tcurve.preserve_x_scale(true);

  conv_font_curve_type fcurves(m_fontManager->path_adaptor());

  conv_font_segm_type  fsegm(fcurves);
  conv_font_trans_type ftrans(fsegm, tcurve);
  fsegm.approximation_scale(3.0);
  fcurves.approximation_scale(2.0);

  m_fontEngine->height(fontSize);

  bool b = true;
  std::string font = m_fontEngine->font_signature();
  if(font.find("Californian FB") == std::string::npos)
    b = m_fontEngine->create_font("Californian FB", agg::glyph_ren_outline);

  if(b)
  {
    double x = (double)dist/2. - (double)len/2.; // distancia horizontal do texto em relacao ao inicio da linha
    double y = 2.0; // distancia vertical do texto em relacao aa linha
    const char* p = tx.c_str();

    while(*p)
    {
      const agg::glyph_cache* glyph = m_fontManager->glyph(*p);
      if(glyph)
      {
        if(x > tcurve.total_length()) 
          break;

        m_fontManager->add_kerning(&x, &y);
        m_fontManager->init_embedded_adaptors(glyph, x, y);

        if(glyph->data_type == agg::glyph_data_outline)
        {
          m_rasterizer.reset();
          m_rasterizer.add_path(ftrans);
          rendererSolid.color(agg::rgba8(0, 0, 0));
          agg::render_scanlines(m_rasterizer, m_scanline, rendererSolid);
        }

        // increment pen position
        x += glyph->advance_x;
        y += glyph->advance_y;
      }
      ++p;
    }
  } 
  path.free_all();
  path.remove_all();
}

void te::ag::Canvas::drawText(const te::gm::Point& wp,
                                       const std::string& tx,
                                       float angle,
                                       te::at::HorizontalAlignment hAlign,
                                       te::at::VerticalAlignment vAlign)
{
  double x = wp.getX();
  double y = wp.getY();

  m_matrix.transform(&x, &y);
  drawText((int)x, (int)y, tx, angle, hAlign, vAlign);
}

void te::ag::Canvas::drawText(const double& xi, const double& yi,
                                       const std::string& tx,
                                       float angle,
                                       te::at::HorizontalAlignment hAlign,
                                       te::at::VerticalAlignment vAlign)
{
  double x = xi;
  double y = yi;

  m_matrix.transform(&x, &y);
  drawText((int)x, (int)y, tx, angle, hAlign, vAlign);
}

void te::ag::Canvas::draw(te::at::Text* tx)
{
  size_t size = tx->getElements().size();
  if(size == 0)
    return;
 
  size_t i = 0;
  while(tx->getElements()[i]->getAttributes() == 0 && i < size)
    ++i;

  te::at::HorizontalAlignment hAlign = te::at::CenterH;
  te::at::VerticalAlignment vAlign = te::at::Baseline;

  while(i < size)
  {
    // set attributes
    te::at::Element* element = tx->getElements()[i];
    te::at::Attributes* atr = (te::at::Attributes*)element->getAttributes();
    if(atr)
    {
      // get font family
      std::string family = ((te::at::Attributes*)atr)->getFamily();
      setFontFamily(family);

      // get font point size
      const double& pointSize = atr->getPointSize();
      setTextPointSize(pointSize);

      // get font weight
      const te::at::FontWeight& weight = atr->getWeight();
      setTextWeight(weight);

      // get font style
      const te::at::FontStyle& style = atr->getStyle();
      setTextStyle(style);

      // get font decoration
      const te::at::TextDecoration& decoration = atr->getDecoration();
      if(decoration == te::at::None)
      {
        setTextUnderline(false);
        setTextOverline(false);
        setTextStrikeOut(false);
      }
      else if(decoration == te::at::Overline)
        setTextOverline(true);
      else if(style == te::at::Underline)
        setTextUnderline(true);
      else
        setTextStrikeOut(true);

      // get letter spacing
      m_textLetterSpacing = atr->getLetterSpacing();
      // nao sei o que fazer ????????????

      // get word spacing
      m_textWordSpacing = atr->getLetterSpacing();
      // nao sei o que fazer ????????????

      // get text color and opacity
      //const std::string& cor = atr->getTextColor();
      //QColor qcor(cor.c_str());
      int opacity = (int)(atr->getTextOpacity() * 255.);
      setTextOpacity(opacity);

  //    // get stroke color and opacity
  //    const std::string& sCor = atr->getTextStrokeColor();
  //    QColor qscor(sCor.c_str());
  //    int sOpacity = atr->getTextStrokeOpacity() * 255.;
  //    qscor.setAlpha(sOpacity);
  //    m_textPen = QPen(qscor);

  //    // get stroke width
  //    m_textPen.setWidth(10. * atr->getTextStrokeWidth() * (double)getResolution() / 72.);

      // get horizontal alignment
      hAlign = atr->getHorizontalAlignment();

      // get vertical alignment
      vAlign = atr->getVerticalAlignment();

      // get multi line justification
      m_textLineJustification = atr->getLineJustification();
      // parece haver confusao entre horizontal alignment e line justification ????????????
      // por enquanto estou usando somente horizontal aligment.

      // get multi line spacing
      m_textLineSpacing = atr->getLineSpacing();
      // como usar isto (0...1) ????????????
    }

    if(element->getLeaderLine() == 0) // without leaderLine
    {
       //supondo que location e' do tipo ponto
      const te::gm::Point* pt = static_cast<const te::gm::Point*>(element->getLocation());
      std::string text = element->getValue();
      size_t p = text.find("\n");
      double hline = 10.;
      te::gm::Point* pclone = 0;
      if(p != std::string::npos)
      {
        te::gm::Polygon* poly = te::ag::Canvas::getTextBoundary(*pt, "A");
        te::gm::Envelope* env = (te::gm::Envelope*)poly->getBoundary();
        hline = env->m_ury - env->m_lly;
        hline *= m_textLineSpacing;
        pclone = (te::gm::Point*)pt->clone();
        delete poly;
        delete env;
      }
      while(p != std::string::npos) // multiline text
      {
        std::string t = text.substr(0, p);
        drawText(*pclone, t, 0., hAlign, vAlign);
        text = text.substr(p+1);
        p = text.find("\n");
        pclone->setY(pclone->getY() - hline); // next line
      }
      if(pclone) // multiline
        drawText(*pclone, text, 0., hAlign, vAlign);
      else
        drawText(*pt, text, 0., hAlign, vAlign);
    }
    else // with leaderLine
    {
    }

    ++i;
  }
}

void te::ag::Canvas::drawTextBoundary(int x, int y, const std::string& tx, float angle, te::at::HorizontalAlignment hAlign, te::at::VerticalAlignment vAlign)
{
  agg::trans_affine matrix;
  matrix.scale(m_txXScale, 1.);
  m_fontEngine->transform(matrix);
  double xa = 0.;
  double ya = 0.;
  const char* p = tx.c_str();

  while(*p)
  {
    const agg::glyph_cache* glyph = m_fontManager->glyph(*p);
    if(glyph)
    {
      m_fontManager->add_kerning(&xa, &ya);
      m_fontManager->init_embedded_adaptors(glyph, xa, ya);

      // increment pen position
      xa += glyph->advance_x;
      ya += glyph->advance_y;
    }
    ++p;
  }
  double txWidth = xa;

  double xi = x;
  double yi = y;
  matrix.reset();
  matrix.translate(-xi, -yi);
  matrix.rotate(-angle * PI / 180.); // -angle ou nao???????????
  matrix.translate(xi, yi);

  if(hAlign == te::at::CenterH)
    xi = x + txWidth / 2.;
  else if(hAlign == te::at::End)
    xi = x + txWidth;

  if(vAlign == te::at::Top)
    yi = y + m_fontEngine->height();
  else if(vAlign == te::at::CenterV)
    yi = y + m_fontEngine->height() * 3. /8.;
  else if(vAlign == te::at::Bottom)
    yi = y - m_fontEngine->height() / 4.;

  matrix.transform(&xi, &yi);

  //int size = 2;
  double xx = x - (xi - x);
  double yy = m_dheight - yi;
  double xf = xx + (m_dwidth<<2);
  double yf = yy;
  matrix.reset();
  matrix.translate(-xx, -yy);
  matrix.scale(m_txXScale, 1.);
  matrix.rotate(-angle * PI / 180.); // -angle ou nao???????????
  matrix.translate(xx, yy);
  matrix.transform(&xf, &yf);

  agg::path_storage path;
  int pathIndex = path.start_new_path();
  path.move_to(xx, yy);
  path.line_to(xf, yf);

  agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt> > rendererSolid(m_rendererBase);

  agg::trans_single_path tcurve;
  tcurve.add_path(path);
  tcurve.preserve_x_scale(true);

  conv_font_curve_type fcurves(m_fontManager->path_adaptor());

  conv_font_segm_type  fsegm(fcurves);
  conv_font_trans_type ftrans(fsegm, tcurve);
  fsegm.approximation_scale(3.0);
  fcurves.approximation_scale(2.0);

  xa = 0.;
  ya = 0.;
  p = tx.c_str();

  while(*p)
  {
    const agg::glyph_cache* glyph = m_fontManager->glyph(*p);
    if(glyph)
    {
      m_fontManager->add_kerning(&xa, &ya);
      m_fontManager->init_embedded_adaptors(glyph, xa, ya);

      // increment pen position
      xa += glyph->advance_x;
      ya += glyph->advance_y;
    }
    ++p;
  }
  path.free_all();
  path.remove_all();

  // calculo do poligono envolvente
  matrix.reset();
  matrix.translate(-xx, -yy);
  matrix.rotate(-angle * PI / 180.); // -angle ou nao???????????
  matrix.translate(xx, yy);

  xf = xx + xa;
  yf = yy + ya + m_fontEngine->height();
  yy -= m_fontEngine->height()/4.;


  pathIndex = path.start_new_path();
  double a, b;
  a = xx;
  b = yy;
  matrix.transform(&a, &b);
  path.move_to(a, b);
  a = xf;
  b = yy;
  matrix.transform(&a, &b);
  path.line_to(a, b);
  a = xf;
  b = yf;
  matrix.transform(&a, &b);
  path.line_to(a, b);
  a = xx;
  b = yf;
  matrix.transform(&a, &b);
  path.line_to(a, b);
  path.close_polygon(true);

  matrix.reset();
  m_rasterizer.gamma(agg::gamma_threshold(.4));
  agg::conv_transform<agg::path_storage> fill(path, matrix);
  agg::conv_stroke<agg::conv_transform<agg::path_storage> > stroke(fill);
  stroke.width(m_txBoundaryWidth);
  m_rasterizer.add_path(stroke, pathIndex); // without roundoff
  agg::render_scanlines_aa_solid(m_rasterizer, m_scanline, m_rendererBase, m_txBoundaryColor); // with anti aliase

  path.free_all();
  path.remove_all();
}

void te::ag::Canvas::drawTextBoundary(const te::gm::Point& wp, const std::string& tx, float angle, te::at::HorizontalAlignment hAlign, te::at::VerticalAlignment vAlign)
{
  double x = wp.getX();
  double y = wp.getY();

  m_matrix.transform(&x, &y);
  drawTextBoundary((int)x, (int)y, tx, angle, hAlign, vAlign);
}

void te::ag::Canvas::drawTextBoundary(const double& xi, const double& yi, const std::string& tx, float angle, te::at::HorizontalAlignment hAlign, te::at::VerticalAlignment vAlign)
{
  double x = xi;
  double y = yi;

  m_matrix.transform(&x, &y);
  drawTextBoundary((int)x, (int)y, tx, angle, hAlign, vAlign);
}

te::gm::Polygon* te::ag::Canvas::getTextBoundary(int x, int y, const std::string& tx, float angle, te::at::HorizontalAlignment hAlign, te::at::VerticalAlignment vAlign)
{
  agg::trans_affine matrix;
  matrix.scale(m_txXScale, 1.);
  m_fontEngine->transform(matrix);
  double xa = 0.;
  double ya = 0.;
  const char* p = tx.c_str();

  while(*p)
  {
    const agg::glyph_cache* glyph = m_fontManager->glyph(*p);
    if(glyph)
    {
      m_fontManager->add_kerning(&xa, &ya);
      m_fontManager->init_embedded_adaptors(glyph, xa, ya);

      // increment pen position
      xa += glyph->advance_x;
      ya += glyph->advance_y;
    }
    ++p;
  }
  double txWidth = xa;

  double xi = x;
  double yi = y;
  matrix.reset();
  matrix.translate(-xi, -yi);
  matrix.rotate(-angle * PI / 180.); // -angle ou nao???????????
  matrix.translate(xi, yi);

  if(hAlign == te::at::CenterH)
    xi = x + txWidth / 2.;
  else if(hAlign == te::at::End)
    xi = x + txWidth;

  if(vAlign == te::at::Top)
    yi = y + m_fontEngine->height();
  else if(vAlign == te::at::CenterV)
    yi = y + m_fontEngine->height() * 3. /8.;
  else if(vAlign == te::at::Bottom)
    yi = y - m_fontEngine->height() / 4.;

  matrix.transform(&xi, &yi);

  //int size = 2;
  double xx = x - (xi - x);
  double yy = m_dheight - yi;
  double xf = xx + (m_dwidth<<2);
  double yf = yy;
  matrix.reset();
  matrix.translate(-xx, -yy);
  matrix.scale(m_txXScale, 1.);
  matrix.rotate(-angle * PI / 180.); // -angle ou nao???????????
  matrix.translate(xx, yy);
  matrix.transform(&xf, &yf);

  agg::path_storage path;
  path.start_new_path();
  //int pathIndex = path.start_new_path();
  path.move_to(xx, yy);
  path.line_to(xf, yf);

  agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt> > rendererSolid(m_rendererBase);

  agg::trans_single_path tcurve;
  tcurve.add_path(path);
  tcurve.preserve_x_scale(true);

  conv_font_curve_type fcurves(m_fontManager->path_adaptor());

  conv_font_segm_type  fsegm(fcurves);
  conv_font_trans_type ftrans(fsegm, tcurve);
  fsegm.approximation_scale(3.0);
  fcurves.approximation_scale(2.0);

  xa = 0.;
  ya = 0.;
  p = tx.c_str();

  while(*p)
  {
    const agg::glyph_cache* glyph = m_fontManager->glyph(*p);
    if(glyph)
    {
      m_fontManager->add_kerning(&xa, &ya);
      m_fontManager->init_embedded_adaptors(glyph, xa, ya);

      // increment pen position
      xa += glyph->advance_x;
      ya += glyph->advance_y;
    }
    ++p;
  }
  path.free_all();
  path.remove_all();

  // calculo do poligono envolvente
  matrix.reset();
  matrix.translate(-xx, -yy);
  matrix.rotate(-angle * PI / 180.); // -angle ou nao???????????
  matrix.translate(xx, yy);

  xf = xx + xa;
  yf = yy + ya + m_fontEngine->height();
  yy -= m_fontEngine->height()/4.;

  te::gm::LinearRing* lr = new te::gm::LinearRing(4, te::gm::LineStringType);
  te::gm::Polygon* poly = new te::gm::Polygon(1, te::gm::PolygonType);
  poly->setRingN(0, lr);

  double a, b;
  a = xx;
  b = yy;
  matrix.transform(&a, &b);
  m_matrix.invert().transform(&a, &b);
  lr->setPoint(0, a, b);

  a = xf;
  b = yy;
  matrix.transform(&a, &b);
  m_matrix.invert().transform(&a, &b);
  lr->setPoint(1, a, b);

  a = xf;
  b = yf;
  matrix.transform(&a, &b);
  m_matrix.invert().transform(&a, &b);
  lr->setPoint(2, a, b);

  a = xx;
  b = yf;
  matrix.transform(&a, &b);
  m_matrix.invert().transform(&a, &b);
  lr->setPoint(3, a, b);

  return poly;
}

te::gm::Polygon* te::ag::Canvas::getTextBoundary(const te::gm::Point& wp, const std::string& tx, float angle, te::at::HorizontalAlignment hAlign, te::at::VerticalAlignment vAlign)
{
  double x = wp.getX();
  double y = wp.getY();

  m_matrix.transform(&x, &y);
  return getTextBoundary((int)x, (int)y, tx, angle, hAlign, vAlign);
}

te::gm::Polygon* te::ag::Canvas::getTextBoundary(const double& xi, const double& yi, const std::string& tx, float angle, te::at::HorizontalAlignment hAlign, te::at::VerticalAlignment vAlign)
{
  double x = xi;
  double y = yi;

  m_matrix.transform(&x, &y);
  return getTextBoundary((int)x, (int)y, tx, angle, hAlign, vAlign);
}

void te::ag::Canvas::setTextColor(const te::color::RGBAColor& color)
{
  m_txColor = color_type(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

void te::ag::Canvas::setTextOpacity(const int& opacity)
{
  double v = (double)opacity/255.;
  m_txColor.opacity(v);
}

void te::ag::Canvas::setFontFamily(const std::string& family)
{
  if(m_fontEngine->create_font(family.c_str(), agg::glyph_ren_outline) == false)
    m_fontEngine->create_font("Verdana", agg::glyph_ren_outline);
}

void te::ag::Canvas::setTextPointSize(double psize)
{
  double size = 1.8 * psize; // para tentar ficar igual ao QT
  m_fontEngine->height(size);
  std::string f = m_fontEngine->font_signature();
  size_t i = f.find(",");
  if(i != std::string::npos)
    f.erase(i);
  setFontFamily(f);
}

void te::ag::Canvas::setTextStyle(te::at::FontStyle style)
{
  if(style == te::at::NormalStyle)
    m_fontEngine->italic(false);
  else
    m_fontEngine->italic(true);

  std::string f = m_fontEngine->font_signature();
  size_t i = f.find(",");
  if(i != std::string::npos)
    f.erase(i);
  setFontFamily(f);
}

void te::ag::Canvas::setTextWeight(te::at::FontWeight weight)
{
  m_fontEngine->weight(weight);
  std::string f = m_fontEngine->font_signature();
  size_t i = f.find(",");
  if(i != std::string::npos)
    f.erase(i);
  setFontFamily(f);
}

void te::ag::Canvas::setTextStretch(size_t stretch)
{
  m_txXScale = (double)stretch/100.;
}

void te::ag::Canvas::setTextUnderline(bool b)
{
  if(b)
    m_txDecorationType = 1;
  else if(m_txDecorationType == 1)
    m_txDecorationType = 0;
}

void te::ag::Canvas::setTextOverline(bool b)
{
  if(b)
    m_txDecorationType = 2;
  else if(m_txDecorationType == 2)
    m_txDecorationType = 0;
}

void te::ag::Canvas::setTextStrikeOut(bool b)
{
  if(b)
    m_txDecorationType = 3;
  else if(m_txDecorationType == 3)
    m_txDecorationType = 0;
}

void te::ag::Canvas::setTextDecorationColor(const te::color::RGBAColor& color)
{
  m_txDecorationColor = color_type(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

void te::ag::Canvas::setTextDecorationWidth(int width)
{
  m_txDecorationWidth = width;
}

void te::ag::Canvas::setTextBoundaryColor(const te::color::RGBAColor& color)
{
  m_txBoundaryColor = color_type(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

void te::ag::Canvas::setTextBoundaryOpacity(const int& opacity)
{
  double v = (double)opacity/255.;
  m_txBoundaryColor.opacity(v);
}

void te::ag::Canvas::setTextBoundaryWidth(const int& width)
{
  m_txBoundaryWidth = (double)width;
}
          
void te::ag::Canvas::setTextMultiLineJustification(const te::at::LineJustification& just)
{
  m_textLineJustification = (int)just;
}
          
void te::ag::Canvas::setTextMultiLineSpacing(const int& spacing)
{
  m_textLineSpacing = spacing;
}

void te::ag::Canvas::setPointColor(const te::color::RGBAColor& color)
{
  m_ptColor = color_type(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
  delete []m_ptPatternBuffer;
  m_ptPatternBuffer = 0;
}

void te::ag::Canvas::setPointWidth(int w)
{
  m_ptWidth = w;
}

void te::ag::Canvas::setPointMarkerType(const ptMarkerType& type)
{
  m_ptMarkerType = (agg::marker_e)type;
}

void te::ag::Canvas::setPointStyle(te::color::RGBAColor** style, int ncols, int nrows)
{
  int i, j, k, n;
  delete [] m_ptPatternBuffer;
  unsigned int tam = ncols * nrows * DEPTH/8;
  m_ptPatternBuffer = new agg::int8u[tam];
  for(i = 0; i < nrows; ++i)
  {
    k = i * 4 * ncols;
    for(j = 0; j < ncols; ++j)
    {
      te::color::RGBAColor c = style[i][j];
      n = k + 4 * j;
      m_ptPatternBuffer[n] = (agg::int8u)c.getRed();
      m_ptPatternBuffer[n + 1] = (agg::int8u)c.getGreen();
      m_ptPatternBuffer[n + 2] = (agg::int8u)c.getBlue();
      m_ptPatternBuffer[n + 3] = (agg::int8u)c.getAlpha();
    }
  }
  m_ptPatternRenderingBuffer.attach(m_ptPatternBuffer, ncols, nrows, -ncols * DEPTH/8);
}

void te::ag::Canvas::setPointStyle(char* style, int size, te::map::Canvas::ImageType t)
{
  //const char* format = te::ag:GetFormat(t);
  //QImage ima(2, 2, QImage::Format_ARGB32);

  //bool b = ima.loadFromData((const uchar*)style, size, format);
  //if(b)
  //{
  //  QMatrix matrix = m_matrix.inverted();
  //  QBrush brush;
  //  brush.setTextureImage(ima);
  //  brush.setMatrix(matrix);
  //  m_ptPen.setBrush(brush);
  //  m_painter.setPen(m_ptPen);
  //  m_ptPattern.clear();
  //}
}

void te::ag::Canvas::setPointStyle(const std::string& fileName)
{
  int i, j, wdata, hdata;
  unsigned char* data = (unsigned char*)getData(fileName, wdata, hdata);
  if(data == 0)
    return;

  delete [] m_ptPatternBuffer;
  unsigned int tam = wdata * hdata * DEPTH/8;
  m_ptPatternBuffer = new agg::int8u[tam];
  memset(m_ptPatternBuffer, 0, tam); // 0 de opacidade

  for(i = 0; i < hdata; ++i)
  {
    int c = i * 3 * wdata;
    int d = i * 4 * wdata;
    for(j = 0; j < wdata; ++j)
    {
      int a = c+j*3;
      int b = d+j*4;
      if(data[a] != 0 && data[a+1] != 0 && data[a+2] != 0)
      {
        m_ptPatternBuffer[b] = data[a];
        m_ptPatternBuffer[b+1] = data[a+1];
        m_ptPatternBuffer[b+2] = data[a+2];
        m_ptPatternBuffer[b+3] = 255;
      }
    }
  }
  delete []data;
  m_ptPatternRenderingBuffer.attach(m_ptPatternBuffer, wdata, hdata, -wdata * DEPTH/8);
}

void te::ag::Canvas::setLineColor(const te::color::RGBAColor& color)
{
  m_lineColor = color_type(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
  delete []m_linePatternBuffer;
  m_linePatternBuffer = 0;
}

void te::ag::Canvas::setLineStyle(te::color::RGBAColor** style, int ncols, int nrows)
{
  int i, j, k, n;
  delete [] m_linePatternBuffer;
  unsigned int tam = ncols * nrows * DEPTH/8;
  m_linePatternBuffer = new agg::int8u[tam];
  for(i = 0; i < nrows; ++i)
  {
    k = i * 4 * ncols;
    for(j = 0; j < ncols; ++j)
    {
      te::color::RGBAColor c = style[i][j];
      n = k + 4 * j;
      m_linePatternBuffer[n] = (agg::int8u)c.getRed();
      m_linePatternBuffer[n + 1] = (agg::int8u)c.getGreen();
      m_linePatternBuffer[n + 2] = (agg::int8u)c.getBlue();
      m_linePatternBuffer[n + 3] = 255;
    }
  }
  m_linePatternRenderingBuffer.attach(m_linePatternBuffer, ncols, nrows, -ncols * DEPTH/8);
}

void te::ag::Canvas::setLineStyle(char* style, int size, te::map::Canvas::ImageType t)
{
  //const char* format = te::ag:GetFormat(t);
  //QImage ima(2, 2, QImage::Format_ARGB32);

  //bool b = ima.loadFromData((const uchar*)style, size, format);
  //if(b)
  //{
  //  QMatrix matrix = m_matrix.inverted();
  //  QBrush brush;
  //  brush.setTextureImage(ima);
  //  double scale = (double)m_lnPen.width() / (double)ima.width();
  //  matrix.scale(scale, scale);
  //  brush.setMatrix(matrix);
  //  m_lnPen.setBrush(brush);
  //  m_painter.setPen(m_lnPen);
  //}
}

void te::ag::Canvas::setLineStyle(const std::string& fileName)
{
  int i, j, wdata, hdata;
  unsigned char* data = (unsigned char*)getData(fileName, wdata, hdata);
  if(data == 0)
    return;

  delete []m_linePatternBuffer;
  m_linePatternBuffer = new agg::int8u[wdata * hdata * DEPTH/8];

  for(i = 0; i < hdata; ++i)
  {
    int c = i * 3 * wdata;
    int d = i * 4 * wdata;
    for(j = 0; j < wdata; ++j)
    {
      int a = c+j*3;
      int b = d+j*4;
      m_linePatternBuffer[b] = data[a];
      m_linePatternBuffer[b+1] = data[a+1];
      m_linePatternBuffer[b+2] = data[a+2];
      m_linePatternBuffer[b+3] = 255;
    }
  }
  delete []data;
  m_linePatternRenderingBuffer.attach(m_linePatternBuffer, wdata, hdata, -wdata * DEPTH/8);
}

void te::ag::Canvas::setLineWidth(const int& w)
{
  m_lineWidth = w;
}

void te::ag::Canvas::setPolygonFillColor(const te::color::RGBAColor& color)
{
  m_polyFillColor = color_type(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
  delete []m_polyPatternBuffer;
  m_polyPatternBuffer = 0;
}

void te::ag::Canvas::setPolygonContourColor(const te::color::RGBAColor& color)
{
  m_polyContourColor = color_type(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
  delete []m_contourPatternBuffer;
  m_contourPatternBuffer = 0;
}

void te::ag::Canvas::setPolygonFillStyle(te::color::RGBAColor** style, int ncols, int nrows)
{
  int i, j, k, n;
  delete [] m_polyPatternBuffer;
  unsigned int tam = ncols * nrows * DEPTH/8;
  m_polyPatternBuffer = new agg::int8u[tam];
  for(i = 0; i < nrows; ++i)
  {
    k = i * 4 * ncols;
    for(j = 0; j < ncols; ++j)
    {
      te::color::RGBAColor c = style[i][j];
      n = k + 4 * j;
      m_polyPatternBuffer[n] = (agg::int8u)c.getRed();
      m_polyPatternBuffer[n + 1] = (agg::int8u)c.getGreen();
      m_polyPatternBuffer[n + 2] = (agg::int8u)c.getBlue();
      m_polyPatternBuffer[n + 3] = m_polyPatternOpacity;
    }
  }
  m_polyPatternRenderingBuffer.attach(m_polyPatternBuffer, ncols, nrows, -ncols * DEPTH/8);
}

void te::ag::Canvas::setPolygonFillStyle(char* style, int size, te::map::Canvas::ImageType t)
{
  //const char* format = te::ag:GetFormat(t);
  //QImage ima(2, 2, QImage::Format_ARGB32);

  //bool b = ima.loadFromData((const uchar*)style, size, format);
  //if(b)
  //{
  //  QMatrix matrix = m_matrix.inverted();
  //  double scale = (double)m_polyPatternWidth / (double)ima.width();
  //  matrix.scale(scale, scale);
  //  m_polyBrush.setMatrix(matrix);
  //  m_polyBrush.setTextureImage(ima);
  //  m_painter.setBrush(m_polyBrush);
  //}
}

void te::ag::Canvas::setPolygonFillStyle(const std::string& fileName)
{
  int a, b, c, d, i, j, wdata, hdata;
  unsigned char* data = (unsigned char*)getData(fileName, wdata, hdata);
  if(data == 0)
    return;

  delete [] m_polyPatternBuffer;
  m_polyPatternBuffer = new agg::int8u[wdata * hdata * DEPTH/8];
  memset(m_polyPatternBuffer, 0, wdata * hdata * DEPTH/8); // 0 de opacidade
  for(i = 0; i < hdata; ++i)
  {
    c = i * 3 * wdata;
    d = i * 4 * wdata;
    for(j = 0; j < wdata; ++j)
    {
      a = c+j*3;
      b = d+j*4;
      m_polyPatternBuffer[b] = data[a];
      m_polyPatternBuffer[b+1] = data[a+1];
      m_polyPatternBuffer[b+2] = data[a+2];
      m_polyPatternBuffer[b+3] = m_polyPatternOpacity;
    }
  }
  delete []data;
  m_polyPatternRenderingBuffer.attach(m_polyPatternBuffer, wdata, hdata, -wdata * DEPTH/8);

//teste.............................................................../////
          //char* buf = new char[wdata * hdata * 3];
          //for(i = 0; i < hdata; ++i)
          //{
          //  int c = i * 3 * wdata;
          //  int d = i * 4 * wdata;
          //  for(j = 0; j < wdata; ++j)
          //  {
          //    int a = c+j*3;
          //    int b = d+j*4;
          //    buf[a] = m_polyPatternBuffer[b+0];
          //    buf[a+1] = m_polyPatternBuffer[b+1];
          //    buf[a+2] = m_polyPatternBuffer[b+2];
          //  }
          //}

          //std::string name = fileName;
          //size_t p = name.find(".");
          //name.insert(p, "_pattern");
          //FILE* fp = fopen(name.c_str(), "wb");
          //if(fp)
          //{
          //    fprintf(fp, "P6 %d %d 255 ", wdata, hdata);
          //    fwrite(buf, 1, wdata * hdata * 3, fp);
          //    fclose(fp);
          //    delete []buf;
          //    return;
          //}
          //delete []buf;
//teste.............................................................../////
}

void te::ag::Canvas::setPolygonPatternWidth(const int& w)
{
  m_polyPatternWidth = w;
}

void te::ag::Canvas::setPolygonPatternOpacity(const unsigned char& opac)
{
  m_polyPatternOpacity = opac;
}

void te::ag::Canvas::setPolygonContourStyle(te::color::RGBAColor** style, int ncols, int nrows)
{
  int i, j, k, n;
  delete [] m_contourPatternBuffer;
  unsigned int tam = ncols * nrows * DEPTH/8;
  m_contourPatternBuffer = new agg::int8u[tam];
  for(i = 0; i < nrows; ++i)
  {
    k = i * 4 * ncols;
    for(j = 0; j < ncols; ++j)
    {
      te::color::RGBAColor c = style[i][j];
      n = k + 4 * j;
      m_contourPatternBuffer[n] = (agg::int8u)c.getRed();
      m_contourPatternBuffer[n + 1] = (agg::int8u)c.getGreen();
      m_contourPatternBuffer[n + 2] = (agg::int8u)c.getBlue();
      m_contourPatternBuffer[n + 3] = 255;
    }
  }
  m_contourPatternRenderingBuffer.attach(m_contourPatternBuffer, ncols, nrows, -ncols * DEPTH/8);
}

void te::ag::Canvas::setPolygonContourStyle(char* style, int size, te::map::Canvas::ImageType t)
{
  //const char* format = te::ag:GetFormat(t);
  //QImage ima(2, 2, QImage::Format_ARGB32);

  //bool b = ima.loadFromData((const uchar*)style, size, format);
  //if(b)
  //{
  //  QMatrix matrix = m_matrix.inverted();
  //  QBrush brush;
  //  brush.setTextureImage(ima);
  //  double scale = (double)m_polyPen.width() / (double)ima.width();
  //  matrix.scale(scale, scale);
  //  brush.setMatrix(matrix);
  //  m_polyPen.setBrush(brush);
  //  m_painter.setPen(m_polyPen);
  //}
}

void te::ag::Canvas::setPolygonContourStyle(const std::string& fileName)
{
  int i, j, wdata, hdata;
  unsigned char* data = (unsigned char*)getData(fileName, wdata, hdata);
  if(data == 0)
    return;

  delete []m_contourPatternBuffer;
  m_contourPatternBuffer = new agg::int8u[wdata * hdata * DEPTH/8];

  for(i = 0; i < hdata; ++i)
  {
    int c = i * 3 * wdata;
    int d = i * 4 * wdata;
    for(j = 0; j < wdata; ++j)
    {
      int a = c+j*3;
      int b = d+j*4;
      m_contourPatternBuffer[b] = data[a];
      m_contourPatternBuffer[b+1] = data[a+1];
      m_contourPatternBuffer[b+2] = data[a+2];
      m_contourPatternBuffer[b+3] = 255;
    }
  }
  delete []data;
  m_contourPatternRenderingBuffer.attach(m_contourPatternBuffer, wdata, hdata, -wdata * DEPTH/8);
}

void te::ag::Canvas::setPolygonContourWidth(const int& w)
{
  m_polyContourWidth = w;
}

void te::ag::Canvas::save(const char* fileName,
                                   te::map::Canvas::ImageType t,
                                   int quality,
                                   int fg) const
{
  //if(m_device->devType() != 4)
  //  ((QPixmap*)m_device)->save(fileName, GetFormat(t), quality);
}

//void te::ag::Canvas::drawText(const QPoint& p,
//                                       const std::string& tx,
//                                       float angle,
//                                       te::at::HorizontalAlignment hAlign,
//                                       te::at::VerticalAlignment vAlign)
//{
//  if(tx.empty())
//    return;
//
//  double ax, ay; // alignment position
//  QString qtx(tx.c_str());
//  QPainterPath path;
//  QFontMetrics fm(m_font);
//  QRectF rec(fm.boundingRect(qtx));
//
//  if(hAlign == te::at::Start)
//    ax = rec.left();
//  else if(hAlign == te::at::CenterH)
//    ax = rec.center().x();
//  else
//    ax = rec.right();
//
//  if(vAlign == te::at::Bottom)
//    ay = rec.bottom();
//  else if(vAlign == te::at::CenterV)
//    ay = rec.center().y();
//  else if(vAlign == te::at::Top)
//    ay = rec.top();
//  else
//    ay = 0.;
//
//  path.addText(-ax, -ay, m_font, qtx);
//
//  m_painter.resetMatrix();
//  m_painter.translate(p);
//  if(angle != 0.)
//    m_painter.rotate(angle);
//  m_painter.fillPath(path, m_textBrush);
//
//  if(m_textPen.width() != 0)
//  {
//    m_painter.setPen(m_textPen);
//    m_painter.drawPath(path);
//  }
//  m_painter.setMatrix(m_matrix);
//}
//
//void te::ag::Canvas::drawTextBoundary(const QPoint& p, const std::string& tx, float angle, te::at::HorizontalAlignment hAlign, te::at::VerticalAlignment vAlign)
//{
//  if(tx.empty())
//    return;
//
//  double ax, ay; // alignment position
//  QString qtx(tx.c_str());
//  QPainterPath path;
//  QFontMetrics fm(m_font);
//  QRectF rec(fm.boundingRect(qtx));
//  QRectF wrec(m_matrix.inverted().mapRect(rec));
//
//  if(hAlign == te::at::Start)
//    ax = wrec.left();
//  else if(hAlign == te::at::CenterH)
//    ax = wrec.center().x();
//  else
//    ax = wrec.right();
//
//// reverse top-bottom
//  if(vAlign == te::at::Bottom)
//    ay = wrec.top();
//  else if(vAlign == te::at::CenterV)
//    ay = wrec.center().y();
//  else if(vAlign == te::at::Top)
//    ay = wrec.bottom();
//  else // Baseline
//    ay = m_matrix.inverted().map(QPoint(0, 0)).y();
//
//  wrec.translate(-ax, -ay); // rotation point on text alignment
//  QPolygonF polf(wrec);
//
//  if(angle != 0.)
//  {
//    QMatrix m;
//    m.rotate(-angle);
//    polf = m.map(polf);
//  }
//  QPointF wp = m_matrix.inverted().map(QPointF(p));
//  polf.translate(wp.x(), wp.y()); // translate to entry point
//
//  m_painter.setPen(m_textPen);
//  m_painter.drawPolygon(polf);
//}
//
//te::gm::Polygon* te::ag::Canvas::getTextBoundary(const QPoint& p, const std::string& tx, float angle, te::at::HorizontalAlignment hAlign, te::at::VerticalAlignment vAlign)
//{
//  if(tx.empty())
//    return 0;
//
//  double ax, ay; // alignment position
//  QString qtx(tx.c_str());
//  QPainterPath path;
//  QFontMetrics fm(m_font);
//  QRectF rec(fm.boundingRect(qtx));
//  QRectF wrec(m_matrix.inverted().mapRect(rec));
//
//  if(hAlign == te::at::Start)
//    ax = wrec.left();
//  else if(hAlign == te::at::CenterH)
//    ax = wrec.center().x();
//  else
//    ax = wrec.right();
//
//// reverse top-bottom
//  if(vAlign == te::at::Bottom)
//    ay = wrec.top();
//  else if(vAlign == te::at::CenterV)
//    ay = wrec.center().y();
//  else if(vAlign == te::at::Top)
//    ay = wrec.bottom();
//  else // Baseline
//    ay = m_matrix.inverted().map(QPoint(0, 0)).y();
//
//  wrec.translate(-ax, -ay); // rotation point on text alignment
//  QPolygonF polf(wrec);
//
//  if(angle != 0.)
//  {
//    QMatrix m;
//    m.rotate(-angle);
//    polf = m.map(polf);
//  }
//  QPointF wp = m_matrix.inverted().map(QPointF(p));
//  polf.translate(wp.x(), wp.y()); // translate to entry point
//
////converter QPolygon para te::gm::Polygon. Como fazer isso?
//  te::gm::LinearRing* lr = new te::gm::LinearRing(4, te::gm::LineStringType);
//  te::gm::Polygon* poly = new te::gm::Polygon(1, te::gm::PolygonType);
//  poly->setRingN(0, lr);
//  lr->setPoint(0, polf.at(0).x(), polf.at(0).y());
//  lr->setPoint(1, polf.at(1).x(), polf.at(1).y());
//  lr->setPoint(2, polf.at(2).x(), polf.at(2).y());
//  lr->setPoint(3, polf.at(3).x(), polf.at(3).y());
//  return poly;  
//}
//
//QPixmap* te::ag::Canvas::getPixmap() const
//{
//  return m_pixmap; 
//}
//
//void te::ag::Canvas::setDevice(QPaintDevice* device)
//{
//  m_painter.end();
//  if(device)
//    m_device = device;
//  else
//    m_device = m_pixmap;
//  m_painter.begin(m_device);
//  m_dwidth = m_device->width();
//  m_dheight = m_device->height();
//}
//
//int te::ag::Canvas::getResolution()
//{
//  if(m_device->devType() == 4)
//    return ((QPrinter*)m_device)->resolution();
//  else
//    return m_device->logicalDpiX();
//}
//
//QMatrix te::ag::Canvas::getMatrix()
//{
//  return m_matrix;
//}

unsigned char* te::ag::Canvas::getBuffer() const
{
  return m_buffer;
}

te::color::RGBAColor** te::ag::Canvas::getColorsFromBuffer(int x, int y, int w, int h) const
{
  int i, j, k, n;
  te::color::RGBAColor** colors;

  if(x == 0 && y == 0 && w == 0 && h == 0)
  {
    colors = new te::color::RGBAColor*[m_dheight];
    for(i = 0; i < m_dheight; ++i)
      colors[i] = new te::color::RGBAColor[m_dwidth];

    for(i = 0; i < m_dheight; ++i)
    {
      k = (i<<2) * m_dwidth;
      for(j = 0; j < m_dwidth; ++j)
      {
        n = k + (j<<2);
        te::color::RGBAColor cor(m_buffer[n], m_buffer[n+1], m_buffer[n+2], m_buffer[n+3]);
        colors[i][j] = cor;
      }
    }
  }
  else
  {
    colors = new te::color::RGBAColor*[h];
    for(i = 0; i < h; ++i)
      colors[i] = new te::color::RGBAColor[w];

    for(i = 0; i < h; ++i)
    {
      k = ((y+i)<<2) * m_dwidth;
      for(j = 0; j < w; ++j)
      {
        n = k + ((x+j)<<2);
        te::color::RGBAColor cor(m_buffer[n], m_buffer[n+1], m_buffer[n+2], m_buffer[n+3]);
        colors[i][j] = cor;
      }
    }
  }
  return colors;
}

bool te::ag::Canvas::write_ppm(const std::string& file_name)
{
  if(DEPTH == 24)
  {
    FILE* fp = fopen(file_name.c_str(), "wb");
    if(fp)
    {
        fprintf(fp, "P6 %d %d 255 ", m_dwidth, m_dheight);
        fwrite(m_buffer, 1, m_dwidth * m_dheight * 3, fp);
        fclose(fp);
        return true;
    }
    return false;
  }
  else
  {
    char* buf = new char[m_dwidth * m_dheight * 3];
    int i, j;
    for(i = 0; i < m_dheight; ++i)
    {
      int c = i * 3 * m_dwidth;
      int d = i * 4 * m_dwidth;
      for(j = 0; j < m_dwidth; ++j)
      {
        int a = c+j*3;
        int b = d+j*4;
        buf[a] = m_buffer[b+0];
        buf[a+1] = m_buffer[b+1];
        buf[a+2] = m_buffer[b+2];
      }
    }

    FILE* fp = fopen(file_name.c_str(), "wb");
    if(fp)
    {
        fprintf(fp, "P6 %d %d 255 ", m_dwidth, m_dheight);
        fwrite(buf, 1, m_dwidth * m_dheight * 3, fp);
        fclose(fp);
        delete []buf;
        return true;
    }
    delete []buf;
    return false;
  }
}

char* te::ag::Canvas::getData(const std::string& fileName, int& w, int &h)
{
  std::string fe = te::common::GetFileExtension(fileName);
  fe = te::common::Convert2UCase(fe);
  if(fe != "PPM")
    return 0;

  size_t p, pp;
  int size;
  char c, buf[256];
  std::string s, ss;

  FILE* fp = fopen(fileName.c_str(), "rb");
  if(fp == 0)
    return 0;

  fread(buf, 1, 256, fp);
  s = buf;
  c = buf[2];
  if(c == ' ')
  {
    p = s.find(c);
    pp = p + 1;
    p = s.find(c, pp);
    ss = s.substr(pp, p-pp);
    sscanf(ss.c_str(), "%d", &w);
    pp = p + 1;
    p = s.find(c, pp);
    ss = s.substr(pp, p-pp);
    sscanf(ss.c_str(), "%d", &h);
    size = w * h * 3;
    p = s.find(c, p+1);
  }
  else
  {
    p = s.find(c);
    p = s.find(c, p+1);
    pp = p + 1;
    p = s.find(c, pp);
    ss = s.substr(pp, p-pp);
    sscanf(ss.c_str(), "%d %d", &w, &h);
    size = w * h * 3;
    pp = p + 1;
    p = s.find(c, pp);
  }
  fseek(fp, p+1, 0);

  char* data = new char[size];
  fread(data, 1, size, fp);
  fclose(fp);
  return data;
}