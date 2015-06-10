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
  \file Enumerators.h
  
  \brief Several enumerators for XLink support.

  
 */

#ifndef __TERRALIB_XLINK_INTERNAL_ENUMERATORS_H
#define __TERRALIB_XLINK_INTERNAL_ENUMERATORS_H

namespace te
{
  namespace xl
  { 
    /*!
      \enum ShowType

      \brief The show type attribute is used to communicate the desired presentation of the ending resource on traversal from the starting resource.

      
     */
    enum ShowType
    {
      SHOW_NEW,      /*!< Load ending resource in a new window, frame, pane, or other
                          presentation context.       */
      SHOW_REPLACE,  /*!< Load the resource in the same window, frame, pane, or 
                          other presentation context. */
      SHOW_EMBED,    /*!< Load ending resource in place of the presentation of the 
                          starting resource.          */
      SHOW_OTHER,    /*!< Behavior is unconstrained; examine other markup in the 
                          link for hints.             */
      SHOW_NONE      /*!< Behavior is unconstrained.  */
    };

    /*!
      \enum ActuateType

      \brief The actuate type attribute is used to communicate the desired timing of traversal from the starting resource to the ending resource.

      
     */
    enum ActuateType
    {
      ACTUATE_ONLOAD,     /*!< Traverse to the ending resource immediately on loading 
                               the starting resource.     */
      ACTUATE_ONREQUEST,  /*!< Traverse from the starting resource to the ending 
                               resource only on a post-loading event triggered for 
                               this purpose.              */
      ACTUATE_OTHER,      /*!< Behavior is unconstrained; examine other markup in link 
                               for hints.                 */
      ACTUATE_NONE        /*!< Behavior is unconstrained. */
    };

  } // end namespace xl
}   // end namespace te

#endif  // __TERRALIB_XLINK_INTERNAL_ENUMERATORS_H

