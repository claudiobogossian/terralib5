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
  \file terralib/se/Mark.h

  \brief A Mark specifies a geometric shape and applies coloring to it.
*/

#ifndef __TERRALIB_SE_INTERNAL_MARK_H
#define __TERRALIB_SE_INTERNAL_MARK_H

// TerraLib
#include "Config.h"

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
// Forward declarations
  namespace xl { class SimpleLink; }

  namespace se
  {
// Forward declarations
    class Fill;
    class InlineContent;
    class Stroke;

    /*!
      \class Mark

      \brief A Mark specifies a geometric shape and applies coloring to it.

      The WellKnownName element gives the well-known name
      of the shape of the mark. Allowed values include at
      least "square", "circle", "triangle", "star", "cross",
      and "x", though map servers may draw a different symbol
      instead if they don't have a shape for all of these. The
      default WellKnownName is "square". Renderings of these
      marks may be made solid or hollow depending on Fill
      and Stroke elements.

      The alternative to a WellKnownName is an external
      mark format. The MarkIndex allows an individual mark
      in a mark archive to be selected. An example format
      for an external mark achive would be a TrueType font
      file, with MarkIndex being used to select an individual
      glyph from that file.

      The Mark element serves two purposes. It allows the
      selection of simple shapes, and, in combination with
      the capability to select and mix multiple external-URL
      graphics and marks, it allows a style to be specified
      that can produce a usable result in a best-effort
      rendering environment, provided that a simple Mark
      is included at the bottom of the list of sources for
      every Graphic.

      \ingroup se

      \sa Graphic, Fill, Stroke, InlineContent
    */
    class TESEEXPORT Mark : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Mark. */
        Mark();

        /*! \brief Destructor. */
        ~Mark();

        //@}

        /*!
          \brief The WellKnownName element gives the well-known name of the shape of the mark.

          Allowed values include at least "square", "circle", "triangle", "star", "cross", and "x".
          The default WellKnownName is "square".
        */
        void setWellKnownName(std::string* name);

        const std::string* getWellKnownName() const;

        void setOnlineResource(te::xl::SimpleLink* link);

        const te::xl::SimpleLink* getOnlineResource() const;

        void setInlineContent(InlineContent* iContent);

        InlineContent* getInlineContent() const;

        void setFormat(std::string* f);

        const std::string* getFormat() const;

        void setMarkIndex(int i);

        int getMarkIndex() const;

        void setFill(Fill* f);

        const Fill* getFill() const;

        void setStroke(Stroke* s);

        const Stroke* getStroke() const;

        /*! \brief It creates a new copy of this object. */
        Mark* clone() const;

      private:

        std::string* m_wellKnownName;         //!< The WellKnownName element gives the well-known name of the shape of the mark. (Optional and if it is set then nor onlineResource_ neither inlineContent_ should be set)
        te::xl::SimpleLink* m_onlineResource; //!< The graphics can either be referenced from an external URL in a common format. (Optional and just one of onlineResource_ or inlineContent_ can be set a time)
        InlineContent* m_inlineContent;       //!< Allows the content of an external graphic object to be included in-line. (Optional and just one of onlineResource_ or inlineContent_ can be set a time)
        std::string* m_format;                //!< Identifies the expected document MIME type of a successful fetch. (Mandatory if onlineResource_ or inlineContent_ is set)
        int m_markIndex;                      //!< It allows an individual mark in a mark archive to be selected. (Optional and it should be set only if onlineResource_ or inlineContent_ is set)
        Fill* m_fill;                         //!< It specifies the pattern for filling an area geometry. (Optional)
        Stroke* m_stroke;                     //!< It specifies the appearance of a linear geometry. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_MARK_H

