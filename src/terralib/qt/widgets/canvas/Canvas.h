/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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

  \brief A canvas built on top of Qt.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CANVAS_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CANVAS_H

// TerraLib
#include "../../../maptools/Canvas.h"
#include "../Config.h"

// Qt
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <QtGui/QPolygonF>
#include <QtGui/QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class Canvas

        \brief A canvas built on top of Qt.

        \sa te::map::Canvas
       */
      class TEQTWIDGETSEXPORT Canvas : public te::map::Canvas
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*!
            \brief It initializes a new Canvas and associates a pixmap as the default device.

            \param w The internal pixmap width in pixels.
            \param h The internal pixmap height in pixels.            
           */
          Canvas(int w, int h);

          /*! \brief Destructor. */
          ~Canvas();

          //@}

          /** @name Methods Inherited From Abstract Canvas
           *  These methods are re-implementations from abstract canvas.
           */
          //@{


          void setWindow(const double& llx, const double& lly,
                         const double& urx, const double& ury);

          void calcAspectRatio(double& llx, double& lly, double& urx, double& ury, 
                               const te::map::AlignType hAlign = te::map::Center, const te::map::AlignType vAlign = te::map::Center);

          void calcAspectRatio(te::gm::Envelope* envelope,
                               const te::map::AlignType hAlign = te::map::Center,
                               const te::map::AlignType vAlign = te::map::Center);

          void setBackgroundColor(const te::color::RGBAColor& color);

          te::color::RGBAColor getBackgroundColor() const;

          void clear();

          void resize(int w, int h);

          int getWidth() const;
          
          int getHeight() const;

          void draw(const te::gm::Geometry* geom);

          void draw(const te::gm::Point* point);

          void draw(const te::gm::MultiPoint* mpoint);

          void draw(const te::gm::LineString* line);

          void draw(const te::gm::MultiLineString* mline);

          void draw(const te::gm::Polygon* poly);

          void draw(const te::gm::MultiPolygon* mpoly);

          void draw(const te::gm::GeometryCollection* g);

          void save(const char* fileName,
                    te::map::ImageType t,
                    int quality = 75,
                    int fg = 0) const;

          char* getImage(te::map::ImageType t, std::size_t& size, int quality = 75, int fg = 0) const;

          te::color::RGBAColor** getImage(const int x = 0, const int y = 0, const int w = 0, const int h = 0) const;

          void freeImage(char* img) const;

          void drawImage(char* src, std::size_t size, te::map::ImageType t);

          void drawImage(te::color::RGBAColor** src, int w, int h);

          void drawImage(int x, int y, char* src, std::size_t size, te::map::ImageType t);

          void drawImage(int x, int y, te::color::RGBAColor** src, int w, int h);

          void drawImage(int x, int y, int w, int h, char* src, std::size_t size, te::map::ImageType t);

          void drawImage(int x, int y, int w, int h, te::color::RGBAColor** src, int srcw, int srch);

          void drawImage(int x, int y, int w, int h, char* src, std::size_t size, te::map::ImageType t, int sx, int sy, int sw, int sh);

          void drawImage(int x, int y, int w, int h, te::color::RGBAColor** src, int sx, int sy, int sw, int sh);

          void drawImage(int x, int y, te::rst::Raster* src);

          void drawImage(int x, int y, int w, int h, te::rst::Raster* src, int sx, int sy, int sw, int sh);

          void drawPixel(int x, int y);

          void drawPixel(int x, int y, const te::color::RGBAColor& color);

          void drawText(int x, int y,
                        const std::string& txt,
                        float angle = 0.0,
                        te::at::HorizontalAlignment hAlign = te::at::Start,
                        te::at::VerticalAlignment vAlign = te::at::Baseline);

          void drawText(const te::gm::Point* p,
                        const std::string& txt,
                        float angle = 0.0,
                        te::at::HorizontalAlignment hAlign = te::at::Start,
                        te::at::VerticalAlignment vAlign = te::at::Baseline);

          void drawText(const double& x, const double& y,
                        const std::string& txt,
                        float angle = 0.0,
                        te::at::HorizontalAlignment hAlign = te::at::Start,
                        te::at::VerticalAlignment vAlign = te::at::Baseline);

          void draw(const te::at::Text* tx);

          te::gm::Polygon* getTextBoundary(int x, int y,
                                           const std::string& txt,
                                           float angle = 0.0,
                                           te::at::HorizontalAlignment hAlign = te::at::Start,
                                           te::at::VerticalAlignment vAlign = te::at::Baseline);

          te::gm::Polygon* getTextBoundary(const te::gm::Point* p,
                                           const std::string& txt,
                                           float angle = 0.0,
                                           te::at::HorizontalAlignment hAlign = te::at::Start,
                                           te::at::VerticalAlignment vAlign = te::at::Baseline);

          te::gm::Polygon* getTextBoundary(const double& x, const double& y,
                                           const std::string& txt,
                                           float angle = 0.0,
                                           te::at::HorizontalAlignment hAlign = te::at::Start,
                                           te::at::VerticalAlignment vAlign = te::at::Baseline);
          
          void setTextColor(const te::color::RGBAColor& color);
          
          void setTextOpacity(int opacity);

          void setFontFamily(const std::string& family);

          void setTextPointSize(double psize);

          /////////////////////////////////////////

          void setTextStyle(te::at::FontStyle style);

          void setTextWeight(te::at::FontWeight weight);

          void setTextStretch(std::size_t stretch);

          void setTextUnderline(bool b);

          void setTextOverline(bool b);

          void setTextStrikeOut(bool b);

          void setTextDecorationColor(const te::color::RGBAColor& color);

          void setTextDecorationWidth(int width);

          void setTextContourColor(const te::color::RGBAColor& color);

          void setTextContourEnabled(bool b);
          
          void setTextContourOpacity(int opacity);
          
          void setTextContourWidth(int width);
          
          void setTextJustification(te::at::LineJustification just);
          
          void setTextMultiLineSpacing(int spacing);

          void setPointColor(const te::color::RGBAColor& color);

          void setPointWidth(int w);

          void setPointMarker(te::map::PtMarkerType type, int w = 0);

          void setPointMarkerColor(const te::color::RGBAColor& color);

          void setPointPattern(te::color::RGBAColor** pattern, int ncols, int nrows);

          void setPointPattern(char* pattern, std::size_t size, te::map::ImageType t);

          void setLineColor(const te::color::RGBAColor& color);

          void setLinePattern(te::color::RGBAColor** pattern, int ncols, int nrows);

          void setLinePattern(char* pattern, std::size_t size, te::map::ImageType t);

          void setLineWidth(int w);

          void setPolygonFillColor(const te::color::RGBAColor& color);

          void setPolygonContourColor(const te::color::RGBAColor& color);

          void setPolygonFillPattern(te::color::RGBAColor** pattern, int ncols, int nrows);

          void setPolygonFillPattern(char* pattern, std::size_t size, te::map::ImageType t);

          void setPolygonPatternWidth(int w);

          void setPolygonPatternOpacity(int opacity);

          void setPolygonContourPattern(te::color::RGBAColor** pattern, int ncols, int nrows);

          void setPolygonContourPattern(char* pattern, std::size_t size, te::map::ImageType t);

          void setPolygonContourWidth(int w);

          //@}

          /** @name Qt Canvas Specific Methos
           *  Methods that belongs only to Qt Canvas.
           */
          //@{

          /*!
            \brief It draws a text.
            
            Color and font family should be defined in advance.

            \param p          The text entry point i device coordinate.
            \param txt        The text to be drawed.
            \param angle      The text rotation.
            \param hAlign     The horizontal text alignment.
            \param vAlign     The vertical text alignment.

            \note Qt Canvas extended method.
           */
          void drawText(const QPoint& p,
                        const std::string& txt,
                        float angle = 0.0, 
                        te::at::HorizontalAlignment hAlign = te::at::Start,
                        te::at::VerticalAlignment vAlign = te::at::Baseline);

          /*!
            \brief It returns text boundary. Color and font family should be defined in advance.

            \param p          The text entry point in device coordinate.
            \param tx         The text to be drawed.
            \param angle      The text rotation.
            \param hAlign     The horizontal text alignment.
            \param vAlign     The vertical text alignment.

            \return The text boundary in world coordinate.

            \note The caller of this method will take the ownership of the returned Polygon.

            \warning Qt Canvas extended method.
           */
          te::gm::Polygon* getTextBoundary(const QPoint& p, const std::string& txt, float angle = 0.0,
                                           te::at::HorizontalAlignment hAlign = te::at::Start,
                                           te::at::VerticalAlignment vAlign = te::at::Baseline);

          /*!
            \brief It returns the internal pixmap used to draw geographical objects.

            \return The internal pixmap used to draw geographical objects. Don't delete it!

            \warning Qt Canvas extended method.

            \note You can not delete this pointer.
           */
          QPixmap* getPixmap() const; 

          /*!
            \brief It sets new device as QPrinter.

            \param device        The new paint device.
            \param takeOwnerShip If true the canvas will take the ownership of the given device otherwise it is the caller responsability to release the device.
           */
          void setDevice(QPaintDevice* device, bool takeOwnerShip); 

          /*!
            \brief It returns the device resolution.

            \return The device resolution.
           */
          int getResolution(); 

          /*!
            \brief It returns the matrix.

            \return The marix.
           */
          QMatrix getMatrix(); 

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
          Canvas(const Canvas& rhs);

          /*!
            \brief Assignment operator not allowed.

            \param rhs The right-hand-side copy that would be used to copy from.

            \return A reference to this object.
           */
          Canvas& operator=(const Canvas& rhs);

          //@}

        private:          

          QMatrix m_matrix;         //!< Matrix that transforms the world coordinate to device coordinate.

          QPainter m_painter;       //!< The painter used to draw geometric objects.

          bool m_isDeviceOwner;     //!< Tells if canvas is the owner of the paint device.
                    
          QColor m_bgColor;         //!< Canvas background color. Defaults: white fully transparent.

          QPointF m_pt;                         //!< Point buffer to avoid creating another point instance.
          QColor m_ptColor;                     //!< The color used to draw point (pixel) or marker.
          QImage* m_ptImg;                      //!< The marker or pattern used to draw points.
          te::map::PtMarkerType m_ptMarkerType; //!< The type of pointe marker.
          QColor m_ptMarkerColor;               //!< The color used to draw point (pixel) or marker.
          int m_ptVOffset;                      //!< Vertical offset in pixels (in device coordinate) applied to point pattern or marker.
          int m_ptHOffset;                      //!< Horizontal offset in pixels (in device coordinate) applied to point pattern or marker.
          int m_ptWidth;                        //!< The width for point markers and point pattern

          QPen m_lnPen;             //!< The pen used to draw lines.

          QPen m_polyPen;           //!< The pen used to draw polygons.
          QBrush m_polyBrush;       //!< The brush used to draw polygons.
          int m_polyPatternWidth;   //!< The size used to draw the pattern.

          QPen m_txtContourPen;         //!< The pen used to draw the text contour.
          bool m_txtContourEnabled;     //!< The flag indicates whether the outline of the text should be drawn.
          QBrush m_txtBrush;            //!< The brush used to draw texts.
          QFont m_font;                 //!< The text font.
          int m_txtLetterSpacing;       //!< Text letter spacing.
          int m_txtWordSpacing;         //!< Text word spacing.
          //int m_txtLineJustification;   //!< Text multi line justification.
          int m_txtLineSpacing;         //!< Text multi line spacing.

          std::map<std::string, QPixmap*> m_patterns; //!< The pixmap styles used to draw patterns.
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CANVAS_H

