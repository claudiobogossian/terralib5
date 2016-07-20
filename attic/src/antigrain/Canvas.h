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
  \file terralib/antigrain/Canvas.h

  \brief A canvas built on top of Anti Grain Geometry.
 */

#ifndef __TERRALIB_AGG_INTERNAL_CANVAS_H
#define __TERRALIB_AGG_INTERNAL_CANVAS_H

#define PI 3.141592653589793238462643

// TerraLib
#include "../maptools/Canvas.h"
#include "Config.h"

#include <windows.h>

// AGG
#include "font_win32_tt\agg_font_win32_tt.h"
#include "agg_font_cache_manager.h"
//#define AGG_BGR24 
//#define AGG_RGB24 
//#define AGG_RGBA32 
#define DEPTH 32
//#include <d:\agg-2.5\examples\pixel_formats.h>
#include "agg_pixfmt_rgba.h"
#define pix_format agg::pix_format_rgba32
typedef agg::pixfmt_rgba32 pixfmt;
typedef agg::pixfmt_rgba32_pre pixfmt_pre;
typedef agg::rgba8 color_type;
typedef agg::order_rgba component_order;

#include <agg_path_storage.h>
#include <agg_rendering_buffer.h>
#include <agg_pixfmt_rgb.h>
#include <agg_renderer_base.h>
#include <agg_trans_affine.h>
#include <agg_rasterizer_scanline_aa.h>
#include <agg_scanline_p.h>
#include <agg_renderer_markers.h>
#include <agg_conv_segmentator.h>
#include <agg_trans_single_path.h>

namespace te
{
  namespace ag
  {
    /*!
      \class Canvas

      \brief A canvas built on top of Anti Grain Geometry.

      What is a Canvas?
      <br>
      It is an abstraction of a drawing area. You can use it to:
      <ul>
      <li>draw geographical objects from a layer, with a given visual (or style);</li>
      <li>draw texts;</li>
      <li>draw the map legend;</li>
      <li>create a chart.</li>
      </ul>
     */
    class TEAGGEXPORT Canvas : public te::map::Canvas
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Canvas. */
        Canvas(int w, int h);

        /*! \brief Destructor. */
        ~Canvas();

        //@}

        /** @name Accessor Methods
         *  Methods used to access internal attributes.
         */
        //@{

        /*!
          \brief It sets the world (or window) coordinates area (supposing a cartesian reference system).

          \param llx Lower left x-coordinate of the World (in the spatial coordinate system of the datasets to be drawn).
          \param lly Lower left y-coordinate of the World (in the spatial coordinate system of the datasets to be drawn).
          \param urx Upper right x-coordinate of the World (in the spatial coordinate system of the datasets to be drawn).
          \param ury Upper right y-coordinate of the World (in the spatial coordinate system of the datasets to be drawn).
         */
        void setWindow(const double& llx, const double& lly,
                       const double& urx, const double& ury);

        /*!
          \brief It adjusts the world (or window) coordinates area (supposing a cartesian reference system).

          \param llx Lower left x-coordinate of the World.
          \param lly Lower left y-coordinate of the World.
          \param urx Upper right x-coordinate of the World.
          \param ury Upper right y-coordinate of the World.
          \param hAlign Horizontal Alignment. It can be left, center or right.
          \param vAlign Vertical Alignment. It can be top, center or bottom.

          \note The input coordinates will be adjusted according to the alignment parameters provided. 
         */
        void adjustWorldWindow(double& llx, double& lly, double& urx, double& ury, 
          const HAlignType& ha = HCenter, const VAlignType& va = VCenter);

        /*!
          \brief It sets the canvas background color.

          The default is totally transparent (255, 255, 255, 100%).

          \param color The background color.

          \note When this method is called, all the canvas contents is dropped out.
         */
        void setBackgroundColor(const te::color::RGBAColor& color);

        /*!
          \brief It returns the canvas background color.

          \return The canvas background color.
         */
        te::color::RGBAColor getBackgroundColor() const;

        /*! \brief It clears the canvas content and fills with the background color. */
        void clear();

        /*!
          \brief It adjusts the canvas size (width and height).

          \param w The new canvas width.
          \param h The new canvas height.

          \note Remember to reset the world (or window) coordinates area before drawing.

          \note This will invalidate the internal transformation function (you must call the setWindow method again before drawing something in the canvas).
         */
        void resize(int w, int h);

        /*!
          \brief It returns the canvas width.

          \return The canvas width.
         */
        int getWidth() const { return m_dwidth; }
        
        /*!
          \brief It returns the canvas height.

          \return The canvas height.
         */
        int getHeight() const { return m_dheight; }

        //@}

        /** @name Geographical Objects Drawing Methods (WKB or EWKB)
         *  Methods used to draw geographical objects encoded as a WKB or EWKB.
         *  Note that the visual (style and symbol) used to draw geometries is affected by the visual methods.
         */
        //@{

        /*!
          \brief It draws the geometry in WKB format on canvas.

          \param geom The geometry in WKB format.

          \return A pointer to the byte after the geometry data stream.
            
          \note The WKB must be in the machine byte order.
         */
        char* drawGeometry(char* geom);

        /*!
          \brief It draws the point in WKB format on canvas.

          \param point The point in WKB format.

          \return A pointer to the byte after the point data stream.

          \note The WKB must be in the machine byte order.
         */
        char* drawPoint(char* point);

        /*!
          \brief It draws the point in WKB format on canvas.

          \param point The point in WKB format.

          \return A pointer to the byte after the point data stream.

          \note The WKB must be in the machine byte order.
         */
        char* drawPointZ(char* point);

        /*!
          \brief It draws the point in WKB format on canvas.

          \param point The point in WKB format.

          \return A pointer to the byte after the point data stream.

          \note The WKB must be in the machine byte order.
         */
        char* drawPointM(char* point);

        /*!
          \brief It draws the point in WKB format on canvas.

          \param point The point in WKB format.

          \return A pointer to the byte after the point data stream.

          \note The WKB must be in the machine byte order.
         */
        char* drawPointZM(char* point);

        /*!
          \brief It draws the multipoint in WKB format on canvas.

          \param mpoint The MultiPoint in WKB format.

          \return A pointer to the byte after the MultiPoint data stream.

          \note The WKB must be in the machine byte order.
         */
        char* drawMultiPoint(char* mpoint);

        /*!
          \brief It draws the line string in WKB format on canvas.

          \param line The LineString in WKB format.

          \return A pointer to the byte after the LineString data stream.

          \note The WKB must be in the machine byte order.
         */
        char* drawLineString(char* line);

        /*!
          \brief It draws the line string.

          \param v point array.

          \note This method becomes the owner of the point array and is responsible for its deletion.
         */
        void drawLineString(double* v, const size_t& size);  // fazer metodo private

        /*!
          \brief It draws the MultiLineString in WKB format on canvas.

          \param mline The MultiLineString in WKB format.

          \return A pointer to the byte after the MultiLineString data stream.

          \note The WKB must be in the machine byte order.
         */
        char* drawMultiLineString(char* mline);

        /*!
          \brief It draws the polygon in WKB format on canvas.

          \param poly The polygon in WKB format.

          \return A pointer to the byte after the polygon data stream.

          \note The WKB must be in the machine byte order.
         */
        char* drawPolygon(char* poly);

        /*!
          \brief It draws the polygon.

          //\param v Polygon vertex array.

          \param path The polygon path.
          
          \note This method becomes the owner of the path and is responsible for its deletion.
         */
        void drawPolygon(agg::path_storage& path, const int& index); // fazer metodo private

        /*!
          \brief It draws the MultiPolygon in WKB format on canvas.

          \param mpoly The MultiPolygon in WKB format.

          \return A pointer to the byte after the MultiPolygon data stream.

          \note The WKB must be in the machine byte order.
         */
        char* drawMultiPolygon(char* mpoly);

        /*!
          \brief It draws the GeometryCollection in WKB format on canvas.

          \param g The GeometryCollection in WKB format.

          \return A pointer to the byte after the GeometryCollection data stream.

          \note The WKB must be in the machine byte order.
         */
        char* drawGeometryCollection(char* g);

        //@}

        /** @name Geographical Objects Drawing Methods (TerraLib Geometry)
         *  Methods used to draw geographical objects encoded as a TerraLib Geometry.
         *  Note that the visual (style and symbol) used to draw geometries is affected by the visual methods.
         */
        //@{

        /*!
          \brief It draws the geometry on canvas.

          \param geom Any geometry (point, line, polygon, ...).
         */
        void draw(te::gm::Geometry* geom);

        /*!
          \brief It draws the point on canvas.

          \param point The point.
         */
        void draw(te::gm::Point* point);

        /*!
          \brief It draws the point with Z coordinate on canvas.

          \param point The point-z.
         */
        void draw(te::gm::PointZ* point);

        /*!
          \brief It draws the point associated to a M value on canvas.

          \param point The point-m.
         */
        void draw(te::gm::PointM* point);

        /*!
          \brief It draws the point with Z and M values on canvas.

          \param point The point-zm.
         */
        void draw(te::gm::PointZM* point);

        /*!
          \brief It draws the multipoint on canvas.

          \param mpoint The MultiPoint.
         */
        void draw(te::gm::MultiPoint* mpoint);

        /*!
          \brief It draws the line string on canvas.

          \param line The LineString.
         */
        void draw(te::gm::LineString* line);

        /*!
          \brief It draws the MultiLineString on canvas.

          \param mline The MultiLineString.
         */
        void draw(te::gm::MultiLineString* mline);

        /*!
          \brief It draws the polygon on canvas.

          \param poly The polygon.
         */
        void draw(te::gm::Polygon* poly);

        /*!
          \brief It draws the MultiPolygon on canvas.

          \param mpoly The MultiPolygon.
         */
        void draw(te::gm::MultiPolygon* mpoly);

        /*!
          \brief It draws the GeometryCollection on canvas.

          \param g The GeometryCollection.
         */
        void draw(te::gm::GeometryCollection* g);

        //@}

        /** @name Image Handling
         *  Methods used to draw an image on Canvas.
         */
        //@{

        /*!
          \brief It returns the internal contents as an image.

          \param t The image format type (see ImageType enum).
          \param size The image size in bytes.

          \return The internal contents as an image. The caller will take the ownership of the returned pointer.

          \note Use canvas freeImage in order to release the returned image resources.
         */
        char* getImage(te::map::Canvas::ImageType t, size_t& size) const;

        /*!
          \brief This is the method that you should use to release a image generated by the canvas.

          \param img A pointer to an image previously created by the canvas.
         */
        void freeImage(char* img) const;

        /*!
          \brief It draws the src pixmap over the canvas.

          \param src The source pixmap.
         */
        void drawImage(char* src, int size, te::map::Canvas::ImageType t);

        /*!
          \brief It draws the src pixmap over the canvas.

          \param src The source pixmap.
          \param w   The image width (number of columns).
          \param h   The image height (number of columns).
         */
        void drawImage(te::color::RGBAColor** src, int w, int h);

        /*!
          \brief It draws the src pixmap over the canvas at the specified position (x, y).

          \param x   The canvas start position where the src image will be drawn.
          \param y   The canvas start position where the src image will be drawn.
          \param src The source pixmap.
         */
        void drawImage(int x, int y, char* src, int size, te::map::Canvas::ImageType t);

        /*!
          \brief It draws the src pixmap over the canvas at the specified position (x, y).

          \param x   The canvas start position where the src image will be drawn.
          \param y   The canvas start position where the src image will be drawn.
          \param src The source pixmap.
          \param w   The image width (number of columns).
          \param h   The image height (number of columns).
         */
        void drawImage(int x, int y, te::color::RGBAColor** src, int w, int h);

        /*!
          \brief It draws the pixmap into the rectangle at position (x, y) with the given width and height.

          \param x   The canvas start position where the src image will be drawn.
          \param y   The canvas start position where the src image will be drawn.
          \param w   The rectangle width.
          \param h   The rectangle height.
          \param src The source pixmap.
         */
        void drawImage(int x, int y, int w, int h, char* src, int size, te::map::Canvas::ImageType t);

        /*!
          \brief It draws the pixmap into the rectangle at position (x, y) with the given width and height.

          \param x    The canvas start position where the src image will be drawn.
          \param y    The canvas start position where the src image will be drawn.
          \param w    The rectangle width.
          \param h    The rectangle height.
          \param src  The source pixmap.
          \param srcw The source image width (number of columns).
          \param srch The source image height (number of columns).
         */
        void drawImage(int x, int y, int w, int h, te::color::RGBAColor** src, int srcw, int srch);

        /*!
          \brief It draws the rectangular portion with the origin (sx, sy), width sw and height sh, of the given pixmap, at the point (x, y), with a width of w and a height of h. If sw or sh are equal to zero the width/height of the pixmap is used and adjusted by the offset sx/sy.

          \param x   The canvas start position where the src image will be drawn.
          \param y   The canvas start position where the src image will be drawn.
          \param w   The rectangle width.
          \param h   The rectangle height.
          \param src The source pixmap.
          \param sx  The source pixmap position.
          \param sy  The source pixmap position.
          \param sw  The source pixmap rectangle width.
          \param sh  The source pixmap rectangle height.
         */
        void drawImage(int x, int y, int w, int h, char* src, int size, te::map::Canvas::ImageType t, int sx, int sy, int sw, int sh);

        /*!
          \brief It draws the rectangular portion with the origin (sx, sy), width sw and height sh, of the given pixmap, at the point (x, y), with a width of w and a height of h. If sw or sh are equal to zero the width/height of the pixmap is used and adjusted by the offset sx/sy.

          \param x   The canvas start position where the src image will be drawn.
          \param y   The canvas start position where the src image will be drawn.
          \param w   The rectangle width.
          \param h   The rectangle height.
          \param src The source pixmap.
          \param sx  The source pixmap start position.
          \param sy  The source pixmap start position.
          \param sw  The source pixmap rectangle width.
          \param sh  The source pixmap rectangle height.
         */
        void drawImage(int x, int y, int w, int h, te::color::RGBAColor** src, int sx, int sy, int sw, int sh);

        /*!
          \brief It sets a pixel using the point pen.

          \param x     Column.
          \param y     Line.  
         */
        void drawPixel(int x, int y);

        /*!
          \brief It sets a pixel to a particular color.

          The color must be an RGBA value. With the following range:
          <ul>
          <li>R: 0-255;</li>
          <li>G: 0-255;</li>
          <li>B: 0-255;</li>
          <li>A: 0-255.</li>

          \param x     Column.
          \param y     Line.  
          \param color Pixel color.

          \note This is not optimized for Canvas. Use a QImage instead.
         */
        void drawPixel(int x, int y, const te::color::RGBAColor& color);

        //@}

        /** @name Text Handling
         *  Methods used to draw a text on Canvas.
         */
        //@{

        /*!
          \brief It draws a text.
          
          Color and font family should be defined in advance.

          \param x          The text entry point x in device coordinate.
          \param y          The text entry point y in device coordinate.
          \param tx         The text to be drawed.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.
         */
        void drawText(int x, int y,
                      const std::string& tx,
                      float angle = 0.0,
                      te::at::HorizontalAlignment hAlign = te::at::Start,
                      te::at::VerticalAlignment vAlign = te::at::Baseline);

        /*!
          \brief It draws a text.
          
          Color and font family should be defined in advance.

          \param vin        The line coordinate.
          \param size       The line size.
          \param matrix     The transformation matrix.
          \param tx         The text to be drawed.
         */
        void drawText(double* vin, const int& size, const agg::trans_affine& matrix, const std::string& tx);

        /*!
          \brief It draws a text.
          
          Color and font family should be defined in advance.

          \param wp         The text entry point in world coordinate.
          \param tx         The text to be drawed.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.
         */
        void drawText(const te::gm::Point& wp,
                      const std::string& tx,
                      float angle = 0.0,
                      te::at::HorizontalAlignment hAlign = te::at::Start,
                      te::at::VerticalAlignment vAlign = te::at::Baseline);

        /*!
          \brief It draws a text.
          
          Color and font family should be defined in advance.

          \param x          The text entry point x in world coordinate.
          \param y          The text entry point y in world coordinate.
          \param tx         The text to be drawed.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.
         */
        void drawText(const double& x, const double& y,
                      const std::string& tx,
                      float angle = 0.0,
                      te::at::HorizontalAlignment hAlign = te::at::Start,
                      te::at::VerticalAlignment vAlign = te::at::Baseline);

        /*!
          \brief It draws a annotation text.
          
          \param tx         Anotation text to be drawed.
         */
        void draw(te::at::Text* tx);

        /*!
          \brief It draws the text boundary. 
          
          Color and font family should be defined in advance.

          \param x          The text entry point x in device coordinate.
          \param y          The text entry point y in device coordinate.
          \param tx         The text to be drawed.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.
         */
        void drawTextBoundary(int x, int y,
                            const std::string& tx,
                            float angle = 0.0,
                            te::at::HorizontalAlignment hAlign = te::at::Start,
                            te::at::VerticalAlignment vAlign = te::at::Baseline);

        /*!
          \brief It draws the text boundary.
          
          Color and font family should be defined in advance.

          \param wp         The text entry point in world coordinate.
          \param tx         The text to be drawed.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.
         */
        void drawTextBoundary(const te::gm::Point& wp,
                            const std::string& tx,
                            float angle = 0.0,
                            te::at::HorizontalAlignment hAlign = te::at::Start,
                            te::at::VerticalAlignment vAlign = te::at::Baseline);

        /*!
          \brief It draws the text boundary.
          
          Color and font family should be defined in advance.

          \param x          The text entry point x in world coordinate.
          \param y          The text entry point y in world coordinate.
          \param tx         The text to be drawed.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.
         */
        void drawTextBoundary(const double& x, const double& y,
                            const std::string& tx,
                            float angle = 0.0,
                            te::at::HorizontalAlignment hAlign = te::at::Start,
                            te::at::VerticalAlignment vAlign = te::at::Baseline);

        /*!
          \brief It returns text boundary.
          
          Color and font family should be defined in advance.

          \param x          The text entry point x in device coordinate.
          \param y          The text entry point y in device coordinate.
          \param tx         The text to be drawed.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.

          \return The text boundary in world coordinate.

          \note The caller of this method will take the ownership of the returned Polygon.
         */
        te::gm::Polygon* getTextBoundary(int x, int y,
                                       const std::string& tx,
                                       float angle = 0.0,
                                       te::at::HorizontalAlignment hAlign = te::at::Start,
                                       te::at::VerticalAlignment vAlign = te::at::Baseline);

        /*!
          \brief It returns the text boundary.
          
          Color and font family should be defined in advance.

          \param wp         The text entry point in world coordinate.
          \param tx         The text to be drawed.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.

          \return The text boundary in world coordinate.

          \note The caller of this method will take the ownership of the returned Polygon.
         */
        te::gm::Polygon* getTextBoundary(const te::gm::Point& wp,
                                       const std::string& tx,
                                       float angle = 0.0,
                                       te::at::HorizontalAlignment hAlign = te::at::Start,
                                       te::at::VerticalAlignment vAlign = te::at::Baseline);

        /*!
          \brief It returns the text boundary.
          
          Color and font family should be defined in advance.

          \param x          The text entry point x in world coordinate.
          \param y          The text entry point y in world coordinate.
          \param tx         The text to be drawed.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.

          \return The text boundary in world coordinate.

          \note The caller of this method will take the ownership of the returned Polygon.
         */
        te::gm::Polygon* getTextBoundary(const double& x, const double& y,
                                       const std::string& tx,
                                       float angle = 0.0,
                                       te::at::HorizontalAlignment hAlign = te::at::Start,
                                       te::at::VerticalAlignment vAlign = te::at::Baseline);
        
        /*!
          \brief It sets the text drawing color.

          \param color The new color for drawing a text.
         */
        void setTextColor(const te::color::RGBAColor& color);
        
        /*!
          \brief It sets the text opacity.

          \param opacity The new opacity for drawing a text.
         */
        void setTextOpacity(const int& opacity);

        /*!
          \brief It sets the text font family.

          \param family The new font family for drawing a text.
         */
        void setFontFamily(const std::string& family);

        /*!
          \brief It sets the text Point Size.

          \param psize The new point size for drawing a text.
         */
        void setTextPointSize(double psize);

        /*!
          \brief It sets the text style.

          \param style The new style for drawing a text.
         */
        void setTextStyle(te::at::FontStyle style);

        /*!
          \brief It sets the text weight.

          \param weight The new weight for drawing a text.
         */
        void setTextWeight(te::at::FontWeight weight);

        /*!
          \brief It sets the text stretch.

          \param stretch The new stretch for drawing a text.

          \note QT defines the following values:
                QFont::UltraCondensed	50	50
                QFont::ExtraCondensed	62	62
                QFont::Condensed	    75	75
                QFont::SemiCondensed	87	87
                QFont::Unstretched	  100	100
                QFont::SemiExpanded	  112	112
                QFont::Expanded	      125	125
                QFont::ExtraExpanded	150	150
                QFont::UltraExpanded	200	200
        */
        void setTextStretch(size_t stretch);

        /*!
          \brief It sets the text underline flag.

          \param b True: for underline. False other wise.
         */
        void setTextUnderline(bool b);

        /*!
          \brief It sets the text overline flag.

          \param b True: for overline. False other wise.
         */
        void setTextOverline(bool b);

        /*!
          \brief It sets the text strike out flag.

          \param b True: for strike out. False other wise.
         */
        void setTextStrikeOut(bool b);

        /*!
          \brief It sets the text strike out flag.

          \param color The new color for drawing text decoration.
         */
        void setTextDecorationColor(const te::color::RGBAColor& color);

        /*!
          \brief It sets the text strike out flag.

          \param width The new width for drawing text decoration.
         */
        void setTextDecorationWidth(int width);

        /*!
          \brief It sets the text boundary drawing color.

          \param color The new color for drawing a text boundary.
         */
        void setTextBoundaryColor(const te::color::RGBAColor& color);
        
        /*!
          \brief It sets the text boundary opacity.

          \param opacity The new opacity for drawing a text boundary.
         */
        void setTextBoundaryOpacity(const int& opacity);
        
        /*!
          \brief It sets the text boundary width.

          \param width The new width for drawing a text boundary.
         */
        void setTextBoundaryWidth(const int& width);
        
        /*!
          \brief It sets the multi line text justification.

          \param just The new justification for drawing a multi line text.
         */
        void setTextMultiLineJustification(const te::at::LineJustification& just);
        
        /*!
          \brief It sets the multi line text spacing.

          \param spacing The new spacing for drawing a multi line text.
         */
        void setTextMultiLineSpacing(const int& spacing);


        //@}

        /** @name Visual and Style Configuration
       *  Note that the visual (style and symbol) used to draw geometries is affected by the visual methods.
       */
      //@{

        /*! \brief It sets the point drawing color.

            \param color The new color for drawing a point.
         */
        void setPointColor(const te::color::RGBAColor& color);

        /*! \brief It sets the point drawing color.

            \param w The new width for drawing a point.
         */
        void setPointWidth(int w);

        /*! \brief It sets the point marker type.

            \param type The point marker type.
         */
        void setPointMarkerType(const ptMarkerType& type);

        /*! \brief It sets the point style.

            The style is a matrix of int's, and each position corresponds to a RGBA color.

            \param style The style matrix: a square matrix of int's where each position corresponds to a RGBA color.
            \param ncols Number of columns in the matrix.
            \param nrows Number of rows in the matrix.
         */
        void setPointStyle(te::color::RGBAColor** style, int ncols, int nrows);

        /*! \brief It sets the point style.

            The style is a matrix of int's, and each position corresponds to a RGBA color.

            \param style The style matrix: a square matrix of int's where each position corresponds to a RGBA color.
            \param t     The image type used by the style.
         */
        void setPointStyle(char* style, int size, te::map::Canvas::ImageType t);

        /*! \brief It sets the point style.

            The style is given by an image file. You should only use images that QT can read.

            \param fileName The file name.
         */
        void setPointStyle(const std::string& fileName);

        /*! \brief It sets the pen color used to draw line geometries.

            \param r Red component color value.
            \param g Green component color value.
            \param b Blue component color value.
            \param a Alpha component color.

            \note The alpha value will be an integer between 0 and 100,
                  where 0 is totaly opaque and 100 totaly transparent.
         */
        void setLineColor(const te::color::RGBAColor& color);

        /*! \brief It sets the line style.

            The style is a matrix of int's, and each position corresponds to a RGBA color.

            \param ncols Number of columns in the matrix.
            \param nrows Number of rows in the matrix.
         */
        void setLineStyle(te::color::RGBAColor** style, int ncols, int nrows);

        /*! \brief It sets the line style.

            The style is a matrix of int's, and each position corresponds to a RGBA color.

            \param ncols Number of columns in the matrix.
            \param nrows Number of rows in the matrix.
         */
        void setLineStyle(char* style, int size, te::map::Canvas::ImageType t);

        /*! \brief It sets the line style.

            The style is given by an image file. You should only use images that QT can read.

            \param fileName The file name.
         */
        void setLineStyle(const std::string& fileName);

        /*! \brief It sets the line width.

            \param w The line width.
         */
        void setLineWidth(const int& w);

        /*!
          \brief It sets the color used to fill the draw of polygon geometries.

          \param r Red component color value.
          \param g Green component color value.
          \param b Blue component color value.
          \param a Alpha component color.

          \note The alpha value will be an integer between 0 and 100,
                where 0 is totaly opaque and 100 totaly transparent.
         */
        void setPolygonFillColor(const te::color::RGBAColor& color);

        /*!
          \brief It sets the pen color used to draw the boundary of polygon geometries.

          \param r Red component color value.
          \param g Green component color value.
          \param b Blue component color value.
          \param a Alpha component color.

          \note The alpha value will be an integer between 0 and 100,
                where 0 is totaly opaque and 100 totaly transparent.
         */
        void setPolygonContourColor(const te::color::RGBAColor& color);

        /*!
          \brief It sets the polygon fill style.

          The style is a matrix of int's, and each position corresponds to a RGBA color.

          \param ncols Number of columns in the matrix.
          \param nrows Number of rows in the matrix.
         */
        void setPolygonFillStyle(te::color::RGBAColor** style, int ncols, int nrows);

        /*!
          \brief It sets the polygon fill style.

          The style is a matrix of int's, and each position corresponds to a RGBA color.

          \param ncols Number of columns in the matrix.
          \param nrows Number of rows in the matrix.
         */
        void setPolygonFillStyle(char* style, int size, te::map::Canvas::ImageType t);

        /*! \brief It sets the polygon fill style.

            The style is given by an image file. You should only use images that QT can read.

            \param fileName The file name.
         */
        void setPolygonFillStyle(const std::string& fileName);

        /*! \brief It sets the polycon brush icon width.

            \param w The icon width.
         */
        void setPolygonPatternWidth(const int& w);

        /*! \brief It sets the polycon brush icon width.

            \param w The icon width.
         */
        void setPolygonPatternOpacity(const unsigned char& opac);

        /*!
          \brief It sets the pen style used to draw the boundary of polygon geometries.

          The style is a matrix of int's, and each position corresponds to a RGBA color.

          \param style The style matrix.
          \param ncols Number of columns in the matrix.
          \param nrows Number of rows in the matrix.
         */
        void setPolygonContourStyle(te::color::RGBAColor** style, int ncols, int nrows);

        /*!
          \brief It sets the pen style used to draw the boundary of polygon geometries.

          The style is a matrix of int's, and each position corresponds to a RGBA color.

          \param ncols Number of columns in the matrix.
          \param nrows Number of rows in the matrix.
         */
        void setPolygonContourStyle(char* style, int size, te::map::Canvas::ImageType t);

        /*! \brief It sets the polygon contour style.

            The style is given by an image file. You should only use images that QT can read.

            \param fileName The file name.
         */
        void setPolygonContourStyle(const std::string& fileName);

        /*! \brief It sets the polycon contour width.

            \param w The contour width.
         */
        void setPolygonContourWidth(const int& w);

        //@}

        /** @name Image Generation Methods
         *  These methods can be used to retrieve or save the canvas contents from/to an image.
         */
        //@{

        /*!
          \brief It saves the canvas content to a file image in the specified format type.

          \param fileName The file name and path where the image will be saved.
          \param t        The image format type (see ImageType enum).
          \param quality  JPEG quality, generally a value between 0 and 95.
          \param fg       Foreground color for WBMP images.

          \warning Qt Canvas extended method.
         */
        void save(const char* fileName,
                  te::map::Canvas::ImageType t,
                  int quality = 75,
                  int fg = 0) const;

      /*!
        \brief It gets a RGBA colors from buffer.

        \return The Pointer RGBA colors.

        \note The caller of this method will take the ownership of the returned pointer.
       */
        te::color::RGBAColor** getColorsFromBuffer(int x=0, int y=0, int w=0, int h=0) const;

      /*!
        \brief It gets a RGBA colors from buffer.

        \return The Pointer RGBA colors.

        \note The caller of this method will take the ownership of the returned pointer.
       */
        unsigned char* getBuffer() const;
        //@}

        ///** @name Qt Canvas Specific Methos
        // *  Methods that belongs only to Qt Canvas.
        // */
        ////@{

        ///*!
        //  \brief It draws a text.
        //  
        //  Color and font family should be defined in advance.

        //  \param p          The text entry point i device coordinate.
        //  \param tx         The text to be drawed.
        //  \param angle      The text rotation.
        //  \param hAlign     The horizontal text alignment.
        //  \param vAlign     The vertical text alignment.

        //  \note Qt Canvas extended method.
        // */
        //void drawText(const QPoint& p,
        //              const std::string& tx,
        //              float angle = 0.0, 
        //              te::at::HorizontalAlignment hAlign = te::at::Start,
        //              te::at::VerticalAlignment vAlign = te::at::Baseline);

        ///*!
        //  \brief It draws the text boundary.
        //  
        //  Color and font family should be defined in advance.

        //  \param p          The text entry point in device coordinate.
        //  \param tx         The text to be drawed.
        //  \param angle      The text rotation.
        //  \param hAlign     The horizontal text alignment.
        //  \param vAlign     The vertical text alignment.

        //  \warning Qt Canvas extended method.
        // */
        //void drawTextBorder(const QPoint& p, const std::string& tx, float angle = 0.0, te::at::HorizontalAlignment hAlign = te::at::Start, te::at::VerticalAlignment vAlign = te::at::Baseline);

        ///*!
        //  \brief It returns text boundary. Color and font family should be defined in advance.

        //  \param p          The text entry point in device coordinate.
        //  \param tx         The text to be drawed.
        //  \param angle      The text rotation.
        //  \param hAlign     The horizontal text alignment.
        //  \param vAlign     The vertical text alignment.

        //  \return The text boundary in world coordinate.

        //  \note The caller of this method will take the ownership of the returned Polygon.

        //  \warning Qt Canvas extended method.
        // */
        //te::gm::Polygon* getTextBorder(const QPoint& p, const std::string& tx, float angle = 0.0, te::at::HorizontalAlignment hAlign = te::at::Start, te::at::VerticalAlignment vAlign = te::at::Baseline);

        ///*!
        //  \brief It returns the internal pixmap used to draw geographical objects.

        //  \return The internal pixmap used to draw geographical objects. Don't delete it!

        //  \warning Qt Canvas extended method.

        //  \note You can not delete this pointer.
        // */
        //QPixmap* getPixmap() const; 

        ///*!
        //  \brief It sets new device as QPrinter.

        //  \param device The new paint device.

        //  \note The canvas becomes owner of the device. You can not delete this pointer
        // */
        //void setDevice(QPaintDevice* device); 

        ///*!
        //  \brief It returns the device resolution.

        //  \return The device resolution.
        // */
        //int getResolution(); 

        ///*!
        //  \brief It returns the matrix.

        //  \return The marix.
        // */
        //QMatrix getMatrix(); 

        //@}

        bool write_ppm(const std::string& file_name);
        char* getData(const std::string& file_name, int& w, int &h);

      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        Canvas(const Canvas& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
         */
        Canvas& operator=(const Canvas& rhs);

        //@}

      private:

        HDC m_hdc;
        int m_dwidth;                               //!< The device width.
        int m_dheight;                              //!< The device height.
        double m_wllx;                              //!< Lower left x-coordinate of the World (in the spatial coordinate system of the datasets to be drawn).
        double m_wlly;                              //!< Lower left y-coordinate of the World (in the spatial coordinate system of the datasets to be drawn).
        double m_wurx;                              //!< Upper right x-coordinate of the World (in the spatial coordinate system of the datasets to be drawn).
        double m_wury;                              //!< Upper right y-coordinate of the World (in the spatial coordinate system of the datasets to be drawn).
        unsigned char* m_buffer;
        agg::rendering_buffer m_renderingBuffer;
        pixfmt* m_pixfmt;
        agg::renderer_base<pixfmt> m_rendererBase;
        agg::rasterizer_scanline_aa<> m_rasterizer;
        agg::scanline_p8              m_scanline;
        agg::trans_affine m_matrix;                 //!< Matrix that transforms the world coordinate to device coordinate.

        color_type m_bgColor;                       //!< Canvas background color. Defaults: white fully transparent.

        color_type m_polyFillColor;                 //!< Polygon fill color.
        color_type m_polyContourColor;              //!< Polygon contoyr color.
        double m_polyContourWidth;                  //!< Polygon contour width.
        unsigned int m_polyPatternWidth;            //!< The pattern width used to fill a polygon.
        agg::int8u*           m_polyPatternBuffer;
        agg::rendering_buffer m_polyPatternRenderingBuffer;
        unsigned char m_polyPatternOpacity;            //!< The pattern opacity.
        agg::int8u*           m_contourPatternBuffer;
        agg::rendering_buffer m_contourPatternRenderingBuffer;

        color_type m_lineColor;                     //!< Line color.
        double m_lineWidth;                         //!< Line width.
        agg::int8u*           m_linePatternBuffer;
        agg::rendering_buffer m_linePatternRenderingBuffer;

        color_type m_ptColor;                     //!< Point color.
        int m_ptWidth;                            //!< Point width.
        agg::marker_e m_ptMarkerType;
        agg::int8u*           m_ptPatternBuffer;
        agg::rendering_buffer m_ptPatternRenderingBuffer;

        typedef agg::font_engine_win32_tt_int16                                  font_engine_type;
        typedef agg::font_cache_manager<font_engine_type>                        font_manager_type;
        typedef agg::conv_curve<font_manager_type::path_adaptor_type>            conv_font_curve_type;
        typedef agg::conv_segmentator<conv_font_curve_type>                      conv_font_segm_type;
        typedef agg::conv_transform<conv_font_segm_type, agg::trans_single_path> conv_font_trans_type;
        font_engine_type                                                         *m_fontEngine;
        font_manager_type                                                        *m_fontManager;
        color_type m_txColor;                     //!< Text color.
        color_type m_txBoundaryColor;                     //!< Text boundary color.
        double m_txBoundaryWidth;                     //!< Text boundary color.
        double m_txXScale;
        int m_txDecorationType;                    //!< none=0, underline=1, overline=2 or strikeout=3.
        color_type m_txDecorationColor;            
        double m_txDecorationWidth;                 
        double m_textLetterSpacing;                    //!< Text letter spacing.
        double m_textWordSpacing;                      //!< Text word spacing.
        double m_textLineSpacing;                      //!< Text multi line spacing.
        int m_textLineJustification;                //!< Text multi line justification.
    }; 
  }   // end namespace agg
}     // end namespace te

#endif  // __TERRALIB_AGG_INTERNAL_CANVAS_H

