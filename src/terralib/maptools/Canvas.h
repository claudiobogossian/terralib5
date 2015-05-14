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
  \file Canvas.h

  \brief A canvas is an abstraction of a drawing area.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_CANVAS_H
#define __TERRALIB_MAPTOOLS_INTERNAL_CANVAS_H

// TerraLib
#include "../annotationtext/Enums.h"
#include "../color/RGBAColor.h"
#include "Config.h"
#include "Enums.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
// Forward declarations
  namespace gm
  {
    class Envelope;
    class Geometry;
    class GeometryCollection;
    class LineString;
    class MultiLineString;
    class MultiPoint;
    class MultiPolygon;
    class Point;
    class PointM;
    class PointZ;
    class PointZM;
    class Polygon;
  }

  namespace at
  {
    class Text;
  }

  namespace rst
  {
    class Raster;
  }

  namespace map
  {
    /*!
      \class Canvas
      
      \brief A canvas is an abstraction of a drawing area.

      What is a Canvas?

      It is an abstraction of a drawing area. You can use it to:
      <ul>
      <li>draw geographical objects from a layer, with a given visual (or style);</li>
      <li>draw texts;</li>
      <li>draw the map legend;</li>
      <li>create a chart.</li>
      </ul>

      The Canvas can be viewed as one of the rendering surface
      used by an application to show a map.

      \ingroup map

      \sa te::qt::widgets::Canvas, te::agg::Canvas, te::gd::Canvas
    */
    class Canvas : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief Constructor. */
        Canvas() {}

        /*! \brief Virtual destructor. */
        virtual ~Canvas() {}

        //@}

        /** @name Accessor Methods
         *  Methods used to access internal attributes.
         */
        //@{

        /*!
          \brief It sets the world (or window) coordinates area (supposing a cartesian reference system).

          \param llx Lower left x-coordinate of the World.
          \param lly Lower left y-coordinate of the World.
          \param urx Upper right x-coordinate of the World.
          \param ury Upper right y-coordinate of the World.

          \note The coordinates must be in the Spatial Reference System of the features to be drawn in canvas.
        */
        virtual void setWindow(const double& llx, const double& lly,
                               const double& urx, const double& ury) = 0;

        /*!
          \brief It calculates the best aspect ratio for world (or window) coordinates area (supposing a cartesian reference system).

          \param llx    Lower left x-coordinate of the World.
          \param lly    Lower left y-coordinate of the World.
          \param urx    Upper right x-coordinate of the World.
          \param ury    Upper right y-coordinate of the World.
          \param hAlign Horizontal Alignment. It can be left, center or right.
          \param vAlign Vertical Alignment. It can be top, center or bottom.

          \note The input coordinates will be adjusted according to the alignment parameters provided. 
        */
        virtual void calcAspectRatio(double& llx, double& lly, double& urx, double& ury, 
                                     const AlignType hAlign = Center,
                                     const AlignType vAlign = Center) = 0;

        /*!
          \brief It calculates the best aspect ratio for world (or window) coordinates area (supposing a cartesian reference system).

          \param envelope  A rectangle with world coordinates that will be adjusted.
          \param hAlign    Horizontal Alignment. It can be left, center or right.
          \param vAlign    Vertical Alignment. It can be top, center or bottom.

          \note The input coordinates will be adjusted according to the alignment parameters provided. 
        */
        virtual void calcAspectRatio(te::gm::Envelope* envelope,
                                     const AlignType hAlign = Center,
                                     const AlignType vAlign = Center) = 0;

        /*!
          \brief It sets the canvas background color.

          The default is totally transparent (255, 255, 255, 0).

          \param color The background color.

          \note When this method is called, all the canvas content is dropped out.
        */
        virtual void setBackgroundColor(const te::color::RGBAColor& color) = 0;

        /*!
          \brief It returns the canvas background color.

          \return The canvas background color.
        */
        virtual te::color::RGBAColor getBackgroundColor() const = 0;

        /*! \brief It clears the canvas content and fills with the background color. */
        virtual void clear() = 0;

        /*!
          \brief It adjusts the canvas size (width and height).

          \param w The new canvas width.
          \param h The new canvas height.
        */
        virtual void resize(int w, int h) = 0;

        /*!
          \brief It returns the canvas width.

          \return The canvas width.
        */
        virtual int getWidth() const = 0;
        
        /*!
          \brief It returns the canvas height.

          \return The canvas height.
        */
        virtual int getHeight() const = 0;

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
        virtual void draw(const te::gm::Geometry* geom) = 0;

        /*!
          \brief It draws the point on canvas.

          \param point The point.
        */
        virtual void draw(const te::gm::Point* point) = 0;

        /*!
          \brief It draws the MultiPoint on canvas.

          \param mpoint The MultiPoint.
        */
        virtual void draw(const te::gm::MultiPoint* mpoint) = 0;

        /*!
          \brief It draws the LineString on canvas.

          \param line The LineString.
        */
        virtual void draw(const te::gm::LineString* line) = 0;

        /*!
          \brief It draws the MultiLineString on canvas.

          \param mline The MultiLineString.
        */
        virtual void draw(const te::gm::MultiLineString* mline) = 0;

        /*!
          \brief It draws the polygon on canvas.

          \param poly The polygon.
        */
        virtual void draw(const te::gm::Polygon* poly) = 0;

        /*!
          \brief It draws the MultiPolygon on canvas.

          \param mpoly The MultiPolygon.
        */
        virtual void draw(const te::gm::MultiPolygon* mpoly) = 0;

        /*!
          \brief It draws the GeometryCollection on canvas.

          \param g The GeometryCollection.
        */
        virtual void draw(const te::gm::GeometryCollection* g) = 0;

        //@}

        /** @name Image Handling
         *  Methods used to draw an image on Canvas.
         */
        //@{

        /*!
          \brief It saves the canvas content to a file image in the specified format type.

          \param fileName The file name and path where the image will be saved.
          \param t        The image format type (see ImageType enum).
          \param quality  JPEG quality, generally a value between 0 and 95.
          \param fg       Foreground color for WBMP images.
        */
        virtual void save(const char* fileName, ImageType t, int quality = 75, int fg = 0) const = 0;

        /*!
          \brief It returns the internal content as an image in a specific format (PNG, JPEG, ...).

          \param t       The image format type (see ImageType enum).
          \param size    The image size in bytes.
          \param quality JPEG quality, generally a value between 0 and 95.
          \param fg      Foreground color for WBMP images.

          \return The internal content as an image. The caller will take the ownership of the returned pointer.

          \note Use canvas freeImage in order to release the returned image resources.
        */
        virtual char* getImage(ImageType t, std::size_t& size, int quality = 75, int fg = 0) const = 0;

        /*!
          \brief It gets a RGBA color array from internal canvas buffer.

          \return The Pointer RGBA colors.

          \note The caller of this method will take the ownership of the returned array.
        */
        virtual te::color::RGBAColor** getImage(const int x = 0, const int y = 0, const int w = 0, const int h = 0) const = 0;

        /*!
          \brief This is the method that you should use to release an image generated by the canvas.

          \param img A pointer to an image previously created by the canvas.
        */
        virtual void freeImage(char* img) const = 0;

        /*!
          \brief It draws the src image over the canvas.

          \param src   A source image of any type (PNG, JPEG, GIF, ...).
          \param size  The image size in bytes.
          \param t     The image format type (see ImageType enum).
        */
        virtual void drawImage(char* src, std::size_t size, ImageType t) = 0;

        /*!
          \brief It draws the src image over the canvas.

          \param src The source image.
          \param w   The image width (number of columns).
          \param h   The image height (number of rows).
        */
        virtual void drawImage(te::color::RGBAColor** src, int w, int h) = 0;

        /*!
          \brief It draws the src image over the canvas at the specified position (x, y).

          \param x     The canvas start position where the src image will be drawn.
          \param y     The canvas start position where the src image will be drawn.
          \param src   A source image of any type (PNG, JPEG, GIF, ...).
          \param size  The image size in bytes.
          \param t     The image format type (see ImageType enum).
        */
        virtual void drawImage(int x, int y, char* src, std::size_t size, ImageType t) = 0;

        /*!
          \brief It draws the src image over the canvas at the specified position (x, y).

          \param x   The canvas start position where the src image will be drawn.
          \param y   The canvas start position where the src image will be drawn.
          \param src The source image.
          \param w   The image width (number of columns).
          \param h   The image height (number of rows).
        */
        virtual void drawImage(int x, int y, te::color::RGBAColor** src, int w, int h) = 0;

        /*!
          \brief It draws the source image into the rectangle at position (x, y) with the given width and height.

          \param x     The canvas start position where the src image will be drawn.
          \param y     The canvas start position where the src image will be drawn.
          \param w     The rectangle width.
          \param h     The rectangle height.
          \param src   A source image of any type (PNG, JPEG, GIF, ...).
          \param size  The image size in bytes.
          \param t     The image format type (see ImageType enum).
        */
        virtual void drawImage(int x, int y, int w, int h, char* src, std::size_t size, ImageType t) = 0;

        /*!
          \brief It draws the source image into the rectangle at position (x, y) with the given width and height.

          \param x    The canvas start position where the src image will be drawn.
          \param y    The canvas start position where the src image will be drawn.
          \param w    The rectangle width.
          \param h    The rectangle height.
          \param src  The source image.
          \param srcw The source image width (number of columns).
          \param srch The source image height (number of rows).
        */
        virtual void drawImage(int x, int y, int w, int h, te::color::RGBAColor** src, int srcw, int srch) = 0;

        /*!
          \brief It draws the rectangular portion with the origin (sx, sy), width sw and height sh, of the source image, at the point (x, y), with a width of w and a height of h. If sw or sh are equal to zero the width/height of the pixmap is used and adjusted by the offset sx/sy.

          \param x    The canvas start position where the src image will be drawn.
          \param y    The canvas start position where the src image will be drawn.
          \param w    The rectangle width.
          \param h    The rectangle height.
          \param src  A source image of any type (PNG, JPEG, GIF, ...).
          \param size The image size in bytes.
          \param t    The image format type (see ImageType enum).
          \param sx   The source image position.
          \param sy   The source image position.
          \param sw   The source image rectangle width.
          \param sh   The source image rectangle height.
        */
        virtual void drawImage(int x, int y, int w, int h, char* src, std::size_t size, ImageType t, int sx, int sy, int sw, int sh) = 0;

        /*!
          \brief It draws the rectangular portion with the origin (sx, sy), width sw and height sh, of the source image, at the point (x, y), with a width of w and a height of h. If sw or sh are equal to zero the width/height of the pixmap is used and adjusted by the offset sx/sy.

          \param x   The canvas start position where the src image will be drawn.
          \param y   The canvas start position where the src image will be drawn.
          \param w   The rectangle width.
          \param h   The rectangle height.
          \param src The source image.
          \param sx  The source image start position.
          \param sy  The source image start position.
          \param sw  The source image rectangle width.
          \param sh  The source image rectangle height.
        */
        virtual void drawImage(int x, int y, int w, int h, te::color::RGBAColor** src, int sx, int sy, int sw, int sh) = 0;

        /*!
          \brief It draws a raster over the canvas at the specified position (x, y). The raster must be with three 8-bit bands (1R, 2G, 3B), 

          \param x        The canvas start position in x where the raster image will be drawn.
          \param y        The canvas start position in y where the raster image will be drawn.
          \param src      The source raster.
          \param opacity  The opacity value used to draw the image
        */
        virtual void drawImage(int x, int y, te::rst::Raster* src, int opacity = TE_OPAQUE) = 0;

        /*!
          \brief It draws the rectangular portion with the origin (sx, sy), width sw and height sh, of the source raster, 
          starting at the point (x, y), with a width of w and a height of h. If sw or sh are equal to zero the width/height 
          of the pixmap is used and adjusted by the offset sx/sy.

          \param x   The canvas start position where the src image will be drawn.
          \param y   The canvas start position where the src image will be drawn.
          \param w   The rectangle width.
          \param h   The rectangle height.
          \param src The source raster.
          \param sx  The source image start position.
          \param sy  The source image start position.
          \param sw  The source image rectangle width.
          \param sh  The source image rectangle height.
          \param opacity  The opacity value used to draw the image
        */
        virtual void drawImage(int x, int y, int w, int h, te::rst::Raster* src, int sx, int sy, int sw, int sh, int opacity = TE_OPAQUE) = 0;

        //@}

        /** @name Pixel Handling
         *  Methods used to draw a pixel on Canvas.
         */
        //@{

        /*!
          \brief It sets a pixel using the point pen.

          \param x Column.
          \param y Row.
        */
        virtual void drawPixel(int x, int y) = 0;

        /*!
          \brief It sets a pixel to a particular color.

          The color must be an RGBA value. With the following range:
          <ul>
          <li>R: 0-255;</li>
          <li>G: 0-255;</li>
          <li>B: 0-255;</li>
          <li>A: 0-255.</li>

          \param x     Column.
          \param y     Row.
          \param color Pixel color.
        */
        virtual void drawPixel(int x, int y, const te::color::RGBAColor& color) = 0;

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
          \param txt        The text to be drawn.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.
        */
        virtual void drawText(int x, int y,
                              const std::string& txt,
                              float angle = 0.0,
                              te::at::HorizontalAlignment hAlign = te::at::Start,
                              te::at::VerticalAlignment vAlign = te::at::Baseline) = 0;

        /*!
          \brief It draws a text.
          
          Color and font family should be defined in advance.

          \param p          The text entry point in world coordinate.
          \param txt        The text to be drawn.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.
        */
        virtual void drawText(const te::gm::Point* p,
                              const std::string& txt,
                              float angle = 0.0,
                              te::at::HorizontalAlignment hAlign = te::at::Start,
                              te::at::VerticalAlignment vAlign = te::at::Baseline) = 0;

        /*!
          \brief It draws a text.
          
          Color and font family should be defined in advance.

          \param x          The text entry point x in world coordinate.
          \param y          The text entry point y in world coordinate.
          \param txt        The text to be drawn.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.
        */
        virtual void drawText(const double& x, const double& y,
                              const std::string& txt,
                              float angle = 0.0,
                              te::at::HorizontalAlignment hAlign = te::at::Start,
                              te::at::VerticalAlignment vAlign = te::at::Baseline) = 0;

        /*!
          \brief It draws an annotaion text.

          \param txt The annotation text to be drawn.
        */
        virtual void draw(const te::at::Text* txt) = 0;


        /*!
          \brief It returns the text boundary (its enclose rectangle).
          
          Color and font family should be defined in advance.

          \param x          The text entry point x in device coordinate.
          \param y          The text entry point y in device coordinate.
          \param txt        The text to be drawn.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.

          \return The text boundary in world coordinate.

          \note The caller of this method will take the ownership of the returned Polygon.
        */
        virtual te::gm::Polygon* getTextBoundary(int x, int y,
                                                 const std::string& txt,
                                                 float angle = 0.0,
                                                 te::at::HorizontalAlignment hAlign = te::at::Start,
                                                 te::at::VerticalAlignment vAlign = te::at::Baseline) = 0;

        /*!
          \brief It returns the text boundary (its enclose rectangle).
          
          Color and font family should be defined in advance.

          \param p          The text entry point in world coordinate.
          \param txt        The text to be drawn.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.

          \return The text boundary in world coordinate.

          \note The caller of this method will take the ownership of the returned Polygon.
        */
        virtual te::gm::Polygon* getTextBoundary(const te::gm::Point* p,
                                                 const std::string& txt,
                                                 float angle = 0.0,
                                                 te::at::HorizontalAlignment hAlign = te::at::Start,
                                                 te::at::VerticalAlignment vAlign = te::at::Baseline) = 0;

        /*!
          \brief It returns the text boundary (its enclose rectangle).
          
          Color and font family should be defined in advance.

          \param x          The text entry point x in world coordinate.
          \param y          The text entry point y in world coordinate.
          \param txt        The text to be drawn.
          \param angle      The text rotation.
          \param hAlign     The horizontal text alignment.
          \param vAlign     The vertical text alignment.

          \return The text boundary in world coordinates.

          \note The caller of this method will take the ownership of the returned Polygon.
        */
        virtual te::gm::Polygon* getTextBoundary(const double& x, const double& y,
                                                 const std::string& txt,
                                                 float angle = 0.0,
                                                 te::at::HorizontalAlignment hAlign = te::at::Start,
                                                 te::at::VerticalAlignment vAlign = te::at::Baseline) = 0;

        //@}

        /** @name Visual and Style Configuration
         *  Note that the visual (style and symbol) used to draw geometries is affected by the visual methods.
         */
        //@{

        /*!
          \brief It sets the text drawing color.

          \param color The new color for drawing a text.
        */
        virtual void setTextColor(const te::color::RGBAColor& color) = 0;
          
        /*!
          \brief It sets the text opacity.

          \param opacity The new opacity for drawing a text.
        */
        virtual void setTextOpacity(int opacity) = 0;

        /*!
          \brief It sets the text font family.

          \param family The new font family for drawing a text.
        */
        virtual void setFontFamily(const std::string& family) = 0;

        /*!
          \brief It sets the text point Size.

          \param size The new point size for drawing a text.
        */
        virtual void setTextPointSize(double size) = 0;

        /*!
          \brief It sets the text style.

          \param style The new style for drawing a text.
        */
        virtual void setTextStyle(te::at::FontStyle style) = 0;

        /*!
          \brief It sets the text weight.

          \param weight The new weight for drawing a text.
        */
        virtual void setTextWeight(te::at::FontWeight weight) = 0;

        /*!
          \brief It sets the text stretch.

          \param stretch The new stretch for drawing a text.
        */
        virtual void setTextStretch(std::size_t stretch) = 0;

        /*!
          \brief It sets the text underline flag.

          \param b True: for underline. False otherwise.
        */
        virtual void setTextUnderline(bool b) = 0;

        /*!
          \brief It sets the text overline flag.

          \param b True: for overline. False otherwise.
        */
        virtual void setTextOverline(bool b) = 0;

        /*!
          \brief It sets the text strike out flag.

          \param b True: for strike out. False otherwise.
        */
        virtual void setTextStrikeOut(bool b) = 0;

        /*!
          \brief It sets the text color for drawing text decoration.

          \param color The new color for drawing text decoration.
        */
        virtual void setTextDecorationColor(const te::color::RGBAColor& color) = 0;

        /*!
          \brief It sets the width for drawing text decoration.

          \param width The new width for drawing text decoration.
        */
        virtual void setTextDecorationWidth(int width) = 0;

        /*!
          \brief It sets the text contour (outline) drawing color.

          \param color The new color for drawing the contour of texts.
        */
        virtual void setTextContourColor(const te::color::RGBAColor& color) = 0;

        /*!
          \brief It controls the display of the text outline.

          \param b True to display and false to not display.
        */
        virtual void setTextContourEnabled(bool b) = 0;
          
        /*!
          \brief It sets the text contour opacity.

          \param opacity The new opacity for drawing the text contour.
        */
        virtual void setTextContourOpacity(int opacity) = 0;
        
        /*!
          \brief It sets the text contour width.

          \param width The new width for drawing the text contour.
        */
        virtual void setTextContourWidth(int width) = 0;
          
        /*!
          \brief It sets the text justification for multi line text.

          \param just The new justification for drawing a multi line text.
        */
        virtual void setTextJustification(te::at::LineJustification just) = 0;
          
        /*!
          \brief It sets the multi line text spacing.

          \param spacing The new spacing for drawing a multi line text.
        */
        virtual void setTextMultiLineSpacing(int spacing) = 0;

        /*!
          \brief It sets the point drawing color.

          This method will instruct the canvas to draw points like pixels.
          Any pattern previously set will be released.

          \param color The new color for drawing a point.
        */
        virtual void setPointColor(const te::color::RGBAColor& color) = 0;

        /*!
          \brief It sets the point width. If point has a patterns, this pattern is scaled to width.

          \param w The width used to draw point.
        */
        virtual void setPointWidth(int w) = 0;

        /*!
          \brief It sets the point pattern.

          The pattern is a matrix of int's, and each position corresponds to a RGBA color.

          \param pattern The pattern matrix: a square matrix of int's where each position corresponds to a RGBA color.
          \param ncols   Number of columns in the matrix.
          \param nrows   Number of rows in the matrix.
        */
        virtual void setPointPattern(te::color::RGBAColor** pattern, int ncols, int nrows) = 0;

        /*!
          \brief It sets the point pattern using a buffered image.

          \param pattern The buffered image.
          \param size    The buffer image size.
          \param t       The image type used by the style.
        */
        virtual void setPointPattern(char* pattern, std::size_t size, ImageType t) = 0;

        /*!
          \brief It sets the point pattern rotation. Rotation is made ​​from the center of the pattern.

          \param angle The rotation angle in degress.
        */
        virtual void setPointPatternRotation(const double& angle) = 0;

        /*!
          \brief It sets the point pattern opacity.

          \param opacity The pattern opacity.
        */
        virtual void setPointPatternOpacity(int opacity) = 0;

        /*!
          \brief It sets the pen color used to draw line geometries.

          This method will drop any pattern or line style previously set.

          \param color The color to be used by the pen.
        */
        virtual void setLineColor(const te::color::RGBAColor& color) = 0;

        /*!
          \brief It sets the line pattern.

          The pattern is a matrix of int's, and each position corresponds to a RGBA color.

          \param pattern The pattern matrix: a square matrix of int's where each position corresponds to a RGBA color.
          \param ncols   Number of columns in the matrix.
          \param nrows   Number of rows in the matrix.
        */
        virtual void setLinePattern(te::color::RGBAColor** pattern, int ncols, int nrows) = 0;

        /*!
          \brief It sets the line pattern using a buffered image.

          \param pattern The buffered image.
          \param size    The buffer image size.
          \param t       The image type used by the style.
        */
        virtual void setLinePattern(char* pattern, std::size_t size, ImageType t) = 0;

        /*!
          \brief It sets the line pattern rotation. Rotation is made ​​from the center of the pattern.

          \param angle The rotation angle in degress.
        */
        virtual void setLinePatternRotation(const double& angle) = 0;

        /*!
          \brief It sets the line pattern opacity.

          \param opacity The pattern opacity.
        */
        virtual void setLinePatternOpacity(int opacity) = 0;

        /*!
          \brief It sets the line width.

          \param w The line width.
        */
        virtual void setLineWidth(int w) = 0;

        /*!
          \brief It sets the line dash style.

          \param style The line dash style.
        */
        virtual void setLineDashStyle(LineDashStyle style) = 0;

        /*!
          \brief It sets the line dash style to the given pattern.

          \param style The line custom dash style.
          
          \note The style should be specified as an even number of positive double where 
                the entries 0, 2, 4, ... are dashes and 1, 3, 5, ... are spaces.
        */
        virtual void setLineDashStyle(const std::vector<double>& style) = 0;

        /*!
          \brief It sets the line cap style.

          \param style The line cap style.
        */
        virtual void setLineCapStyle(LineCapStyle style) = 0;

        /*!
          \brief It sets the line join style.

          \param style The line join style.
        */
        virtual void setLineJoinStyle(LineJoinStyle style) = 0;

        /*!
          \brief It sets the color used to fill the draw of polygon geometries.

          \param color The color to be used when filling a polygon.
        */
        virtual void setPolygonFillColor(const te::color::RGBAColor& color) = 0;

        /*!
          \brief It sets the pen color used to draw the boundary of polygon geometries.

          \param color The color to be used to outline a polygon.
        */
        virtual void setPolygonContourColor(const te::color::RGBAColor& color) = 0;

        /*!
          \brief It sets the polygon fill pattern.

          The pattern is a matrix of int's, and each position corresponds to a RGBA color.

          \param pattern The style matrix: a square matrix of int's where each position corresponds to a RGBA color.
          \param ncols   Number of columns in the matrix.
          \param nrows   Number of rows in the matrix.
        */
        virtual void setPolygonFillPattern(te::color::RGBAColor** pattern, int ncols, int nrows) = 0;

        /*!
          \brief It sets the polygon fill pattern using a buffered image.

          \param pattern The buffered image.
          \param size    The buffer image size.
          \param t       The image type used by the style.
        */
        virtual void setPolygonFillPattern(char* pattern, std::size_t size, ImageType t) = 0;

        /*!
          \brief It sets the polygon pattern width.

          \param w The pattern width.
        */
        virtual void setPolygonPatternWidth(int w) = 0;

        /*!
          \brief It sets the polygon pattern rotation.

          \param angle The rotation angle in degress.
        */
        virtual void setPolygonPatternRotation(const double& angle) = 0;

        /*!
          \brief It sets the polygon pattern opacity.

          \param opacity The pattern opacity.
        */
        virtual void setPolygonPatternOpacity(int opacity) = 0;

        /*!
          \brief It sets the pen pattern used to draw the boundary of polygon geometries.

          The pattern is a matrix of int's, and each position corresponds to a RGBA color.

          \param pattern The pattern matrix: a square matrix of int's where each position corresponds to a RGBA color.
          \param ncols   Number of columns in the matrix.
          \param nrows   Number of rows in the matrix.
        */
        virtual void setPolygonContourPattern(te::color::RGBAColor** pattern, int ncols, int nrows) = 0;

        /*!
          \brief It sets the pen pattern used to draw the boundary of polygon geometries using a buffered image.

          \param pattern The buffered image.
          \param size    The buffer image size.
          \param t       The image type used by the style.
        */
        virtual void setPolygonContourPattern(char* pattern, std::size_t size, ImageType t) = 0;

        /*!
          \brief It sets the polygon contour width.

          \param w The contour width.
        */
        virtual void setPolygonContourWidth(int w) = 0;

        /*!
          \brief It sets the polygon contour pattern rotation.

          \param angle The rotation angle in degress.
        */
        virtual void setPolygonContourPatternRotation(const double& angle) = 0;

        /*!
          \brief It sets the polygon contour pattern opacity.

          \param opacity The pattern opacity.
        */
        virtual void setPolygonContourPatternOpacity(int opacity) = 0;

        /*!
          \brief It sets the polygon contour dash style.

          \param style The polygon contour dash style.
        */
        virtual void setPolygonContourDashStyle(LineDashStyle style) = 0;

        /*!
          \brief It sets the polygon contour dash style to the given pattern.

          \param style The polygon contour custom dash style.
          
          \note The style should be specified as an even number of positive double where 
                the entries 0, 2, 4, ... are dashes and 1, 3, 5, ... are spaces.
        */
        virtual void setPolygonContourDashStyle(const std::vector<double>& style) = 0;

        /*!
          \brief It sets the polygon contour cap style.

          \param style The polygon contour cap style.
        */
        virtual void setPolygonContourCapStyle(LineCapStyle style) = 0;

        /*!
          \brief It sets the polygon contour join style.

          \param style The polygon contour join style.
        */
        virtual void setPolygonContourJoinStyle(LineJoinStyle style) = 0;

        /*!
          \brief It sets the painter to erase mode.
        */
        virtual void setEraseMode() = 0;

        /*!
          \brief It sets the painter to normal copy source to destination mode.
        */
        virtual void setNormalMode() = 0;

        //@}
    };

  }   // end namespace map
}     // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_CANVAS_H
