/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file HLDelegateDecorator.h
  \brief Defines a decorator for te::qt::widgets::HighlightDelegate objects.
  \details This way, the behavior of te::qt::widgets::HighlightDelegate is maintained, but we can add new groups of highlighted objects.
  Note that the default delegate does not have an index. It means that it is not possible to change the behavior of the default presentation
  delegate.
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
*/
#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HLDELEGATEDECORATOR_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HLDELEGATEDECORATOR_H

#include "HighlightDelegate.h"

//TerraLib include files
#include <terralib/common/Decorator.h>

//STL include files
#include <set>
#include <string>
#include <vector>

//Forward declarations
class QColor;
class QPainter;
class QStyleOptionViewItem;
class QModelIndex;

namespace te 
{
  namespace qt 
  {
    namespace widgets 
    {

      /*!
        \class HLDelegateDecorator
        \copydoc HLDelegateDecorator.h
      */
      class TEQTWIDGETSEXPORT HLDelegateDecorator : public te::common::Decorator<HighlightDelegate>
      {
      public:
        /*!
          \brief Virtual Destructor.
        */
        virtual ~HLDelegateDecorator();

        /*!
          \brief
        */
        virtual HighlightDelegate* clone ();

        /*!
          \overload void te::qt::widgets::HLDelegateDecorator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
        */
        virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

        /*!
          \brief Updates the set of primary keys.    
          \param pKeyCols Columns positions.
        */
        virtual void setPKeysColumns(const std::vector<size_t>& pKeyCols);

        /*!
          \brief Sets the color for a given class. 

          Usage example:
          \code{.cpp}
          //Use a 3 classes object.
          int pointed = 0,
            queried = 1,
            default = 2;

          //Gets a delegate with 2 classes, and default as the last one.
          te::qt::widgets::HLDelegateDecorator* dec = 
              (te::qt::widgets::HLDelegateDecorator*) te::qt::widgets::HLDelegateDecorator::getDelegate(2);

          //Sets the highlight color of the class of queried objects to green.
          dec->setClassColor(queried, QColor(0, 255, 0));
          \endcode

          \param cPosition Class position. Remember that the last one is used for default presentation.
          \param color The new color for the class.
          \exception If \a cPosition is out of the boundaries of number of classes a te::common::Exception will be raised.
        */
        void setClassColor(const size_t& cPosition, const QColor& color);

        /*!
          \brief Adds objects to be highlighted in some class.

          Usage example:
          \code{.cpp}
          //Use a 3 classes object.
          int pointed = 0,
            queried = 1,
            default = 2;

          //Identifiers to be highlighted.
          std::string myids[] = {"id1", "id2", "id3"} ;
          std::set<std::string> idsToSel(myids, myids+3);

          //Gets a delegate with 2 classes, and default as the last one.
          te::qt::widgets::HLDelegateDecorator* dec = 
              (te::qt::widgets::HLDelegateDecorator*) te::qt::widgets::HLDelegateDecorator::getDelegate(2);

          //Put the ids in the pointed class.
          dec->setClassSelection(pointed, idsToSel);
          \endcode

          \param cPosition Class position.
          \param sel Identifiers of objects to be highlighted.
          \exception If \a cPosition is out of number of classes boundaries a te::common::Exception will be raised.
        */
        void setClassSelection(const size_t& cPosition, const std::set<std::string>& sel);

        /*!
          \brief Returns the delegate in the position \a pos.    
          \param pos The decortation at position in the structure.
          \return The delegate in the defined position.
          \exception If the position required is out of boundaries an exception will be raised.
        */
        HighlightDelegate* getDecorated(const size_t& pos);

        /*!
          \brief Returns the position of the selection. 
          
          Note that the last position corresponds to default group.

          \param pKey Identifier of the register to be searched.
          \return Index of the class that the identifier was associated, or -1 with no group found.
        */
        int getGroupPosition(const std::string& pKey) const;

        /*!
          \brief Returns the number of classes including the default one.
          \return Number of classes the delegate has.
        */
        size_t getNumberOfClasses() const;

        /*!
          \brief Removes all selections in all decorated delegates.
        */
        void clearSelection();

        /*!
          \brief Returns an object with decorations for the number of classes.    
          \param numClasses Number of classes.
          \note The total number of decorations is equal to \a numClasses + 1 (the default one).
        */
        static HighlightDelegate* getDelegate(const size_t& numClasses);

        /*!
          \brief Clones the delegate informations.
        */
//        static HLDelegateDecorator* clone(HLDelegateDecorator* other);

      protected:
        /*!
          \brief Protected constructor.
          \param decorated The delegate to be decorated.
        */
        HLDelegateDecorator(HighlightDelegate * decorated = 0);
      };
    } 
  }
} 

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_HLDELEGATEDECORATOR_H
