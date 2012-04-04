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
  \file SelectedChannel.h
  
  \brief A selected channel to be display.
 */

#ifndef __TERRALIB_SE_INTERNAL_SELECTEDCHANNEL_H
#define __TERRALIB_SE_INTERNAL_SELECTEDCHANNEL_H

// TerraLib
#include "../common/BaseVisitable.h"
#include "Config.h"
#include "Visitor.h"

// STL
#include <string>

namespace te
{
  namespace se
  {
// Forward declaration
    class ContrastEnhancement;

    /*!
      \class SelectedChannel
      
      \brief A selected channel to be display.

      \sa ChannelSelection, ContrastEnhancement
     */
    class TESEEXPORT SelectedChannel
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new SelectedChannel. */
        SelectedChannel();

        /*! \brief Destructor. */
        ~SelectedChannel();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{
        
        void setSourceChannelName(const std::string& name);
        std::string getSourceChannelName() { return m_sourceChannelName; }
        void setContrastEnhancement(ContrastEnhancement* c);
        ContrastEnhancement* getContrastEnhancement() { return m_contrastEnhancement; }
        
        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other SelectedChannel.
         */
        SelectedChannel(const SelectedChannel& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other Halo.

          \return A reference for this.
         */
        SelectedChannel& operator=(const SelectedChannel& rhs);

        //@}

      private:

        std::string m_sourceChannelName;             //!< Channels are identified by a system and data-dependent character identifier. Commonly, channels will be labelled as "1", "2" and so on. (Mandatory)
        ContrastEnhancement* m_contrastEnhancement;  //!< Contrast enhancement that can be applied to a channel in isolation. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_SELECTEDCHANNEL_H

