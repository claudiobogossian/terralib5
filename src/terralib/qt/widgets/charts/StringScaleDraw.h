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
  \file terralib/qt/widgets/charts/StringScaleDraw.h

  \brief A class to represent a set of labels to be used on a histogram's chart.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_STRINGSCALEDRAW_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_STRINGSCALEDRAW_H

//TerraLib
#include "../Config.h"

//QWT
#include <qwt_scale_draw.h>

//STL
#include <set>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
     class TEQTWIDGETSEXPORT StringScaleDraw : public QwtScaleDraw
     {
     public:

      /*!
        \brief Constructor

        It constructs a StringScaleDraw.

        \param labels the list of labels as a set of QStrings

        \note It will not take the ownership of the pointers.
      */
       StringScaleDraw (std::set<QString>& labels);

      /*!
        \brief Constructor

        It constructs a StringScaleDraw.

        \param labels the list of labels as a set of Strings

        \note It will not take the ownership of the pointers.
      */
       StringScaleDraw (std::set<std::string>& labels);

       /*! \brief Destructor. */
       ~StringScaleDraw();

      /*!            
        \brief It returns the list containing the labels.  

        \return The list containing the labels.  

      */

       std::set<QString>& getLabels();
      
      /*!            
        \brief It sets the list containing the labels. 

        \param new_labels The new list of labels. 
      */

      void setLabels(  std::set<QString> new_labels);

      /*!            
        \brief It returns the label at the given index 

        \param new_labels The new list of labels. 
      */

      virtual QwtText label( double value ) const;

     private:

        std::set<QString>  m_labels;  //!< A set of label values
     };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_STRINGSCALEDRAW_H