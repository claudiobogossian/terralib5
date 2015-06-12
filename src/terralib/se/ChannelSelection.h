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
  \file terralib/se/ChannelSelection.h

  \brief ChannelSelection specifies the false-color channel selection for a multi-spectral raster source (such as a multi-band satellite-imagery source).
*/

#ifndef __TERRALIB_SE_INTERNAL_CHANNELSELECTION_H
#define __TERRALIB_SE_INTERNAL_CHANNELSELECTION_H

// TerraLib
#include "Config.h"
#include "Enums.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace se
  { 
// Forward declaration
    class SelectedChannel;

    /*!
      \class ChannelSelection
      
      \brief ChannelSelection specifies the false-color channel selection for a multi-spectral raster source (such as a multi-band satellite-imagery source).

      Either a channel may be selected to display in
      each of red, green, and blue, or a single channel
      may be selected to display in grayscale. (The
      spelling "gray" is used since it seems to be
      more common on the Web than "grey" by a ratio
      of about 3:1.) Contrast enhancement may be applied
      to each channel in isolation. Channels are identified
      by a system and data-dependent character identifier.
      Commonly, channels will be labelled as "1", "2", etc.

      \ingroup se

      \sa RasterSymbolizer, SelectedChannel
    */
    class TESEEXPORT ChannelSelection : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new ChannelSelection. */
        ChannelSelection();

        /*! \brief Destructor. */
        ~ChannelSelection();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setRedChannel(SelectedChannel* c);

        SelectedChannel* getRedChannel() const { return m_redChannel; }

        void setGreenChannel(SelectedChannel* c);

        SelectedChannel* getGreenChannel() const { return m_greenChannel; }

        void setBlueChannel(SelectedChannel* c);

        SelectedChannel* getBlueChannel() const { return m_blueChannel; }

        void setGrayChannel(SelectedChannel* c);

        SelectedChannel* getGrayChannel() const { return m_grayChannel; }

        void setColorCompositionType(ColorCompositionType cct);

        ColorCompositionType getColorCompositionType() const { return m_colorCompType; }
        
        //@}

        /*! \brief It creates a new copy of this object. */
        ChannelSelection* clone() const;

      private:

        SelectedChannel* m_redChannel;     //!< Either a channel may be selected to display in each of red, green, and blue, or a single channel may be selected to display in grayscale. (Mandatory if grayChannel_ is not informed)
        SelectedChannel* m_greenChannel;   //!< Either a channel may be selected to display in each of red, green, and blue, or a single channel may be selected to display in grayscale. (Mandatory if grayChannel_ is not informed)
        SelectedChannel* m_blueChannel;    //!< Either a channel may be selected to display in each of red, green, and blue, or a single channel may be selected to display in grayscale. (Mandatory if grayChannel_ is not informed)
        SelectedChannel* m_grayChannel;    //!< Either a channel may be selected to display in each of red, green, and blue, or a single channel may be selected to display in grayscale. (Mandatory if redChannel_, greenChannel_ and blueChannel_ is not informed)

        ColorCompositionType m_colorCompType; //!< Define the color composition, this is not defined in SE spec.
    };    

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_CHANNELSELECTION_H

