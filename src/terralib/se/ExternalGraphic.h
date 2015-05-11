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
  \file terralib/se/ExternalGraphic.h

  \brief The ExternalGraphic allows a reference to be made to an external graphic file with a Web URL or to in-line content.
*/

#ifndef __TERRALIB_SE_INTERNAL_EXTERNALGRAPHIC_H
#define __TERRALIB_SE_INTERNAL_EXTERNALGRAPHIC_H

// TerraLib
#include "Config.h"

// STL
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
// Forward declarations
  namespace xl { class SimpleLink; }
  namespace se
  {
// Forward declarations
    class InlineContent;
    class Recode;

    /*!
      \class ExternalGraphic
      
      \brief The ExternalGraphic allows a reference to be made to an external graphic file with a Web URL or to in-line content.

      The OnlineResource sub-element gives the
      URL and the Format sub-element identifies
      the expected document MIME type of a successful
      fetch. Knowing the MIME type in advance allows
      the styler to select the best-supported format
      from the list of URLs with equivalent content.
      Users should avoid referencing external graphics
      that may change at arbitrary times, since many systems
      may cache or permanently store graphic content for
      improved efficiency and reliability. Graphic content
      should be static when at all possible.
      <br>
      The InlineContent sub-element allows the content of an
      external graphic object to be included in-line. The two
      choices for encoding are XML and Base-64-encoded binary,
      as indicated by the encoding attribute. An issue with the
      XML encoding is that the <?xml ...?> tag of the object
      cannot be present inside of the InlineContent tag. The
      external graphic object will be extracted and used like
      the content fetched from an ExternalContent tag.
      <br>
      The ColorReplacement element, which may occur multiple
      times, allows to replace a color in the ExternalGraphic,
      the color specified in the OriginalColor sub-element,
      by another color as a result of a recode function.
      LookUpValue is in this case set to ExternalGraphic,
      both Data and Value elements are set to color values.

      \ingroup se

      \sa Graphic, InlineContent, Recode
    */
    class TESEEXPORT ExternalGraphic : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new ExternalGraphic. */
        ExternalGraphic();

        /*! \brief Destructor. */
        ~ExternalGraphic();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setOnlineResource(te::xl::SimpleLink* onlineResource);

        const te::xl::SimpleLink* getOnlineResource() const;

        void setInlineContent(InlineContent* inlineContent);

        const InlineContent* getInlineContent() const;

        void setFormat(const std::string& f);

        const std::string& getFormat() const;

        void add(Recode* r);

        //@}

        /*! \brief It creates a new copy of this object. */
        ExternalGraphic* clone() const;

      private:

        te::xl::SimpleLink* m_onlineResource;     //!< The URL of the external graphic. (Mandatory if m_inlineContent is absent)
        InlineContent* m_inlineContent;           //!< It allows the content of an external graphic object to be included in-line. (Mandatory if m_onlineResource is absent)
        std::string m_format;                     //!< It identifies the expected document MIME type of a successful fetch. (Mandatory)
        std::vector<Recode*> m_colorReplacements; //!< Transformation of discrete values to other values. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_EXTERNALGRAPHIC_H

