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
  \file Event.h

  \brief This file contains a class to represent an event.
*/

#ifndef __TERRALIB_ST_INTERNAL_EVENT_H
#define __TERRALIB_ST_INTERNAL_EVENT_H

//STL
#include <vector>

//ST
#include "../../Config.h"

// Forward declarations
namespace te { namespace dt { class AbstractData; class DateTime;  } }
namespace te { namespace gm { class Geometry;  } }


namespace te
{
  namespace st
  {
    // Forward declarations
    class Object;
   
    /*!
      \class Event

      \brief A class to represent an event.

      An event is an individual episode with a definite beginning and end 
      which can involve one or more objects. 

      \ingroup st

      \sa Object 
    */
    class TESTEXPORT Event
    {
      public:

        /*! \name Event Constructors */
        //@{
        
        /*! 
          \brief Constructor. 

          \param id  The identification of the event. 
          \param t   The time when the event happened.  
          \param g   The location where the event happened.
                
          \note It will take the ownership of the input pointers.
        */
        Event(const std::string& id, te::dt::DateTime* t, te::gm::Geometry* g);

        /*! 
          \brief Constructor. 

          \param id  The identification of the event. 
          \param t   The time when the event happened.  
          \param g   The location where the event happened.
          \param objs The objects involved to the event.
                
          \note It will take the ownership of the input pointers.
          \note It will NOT take the ownership of the involved objects.
        */
        Event(const std::string& id, te::dt::DateTime* t, te::gm::Geometry* g, 
              std::vector<te::st::Object*> objs);
     
        /*! 
          \brief Copy constructor. 
        */
        Event(const Event& ts);
        //@}

        /*! 
          \brief Copy assignment operator
        */
        Event& operator=(const Event& other);

        /*!
          \brief It returns a clone of this event. 

          \return A new event. 

          \note The caller will take the ownership of the returned pointer.
        */
        Event* clone() const;

        /*!
          \brief It returns the object identification.

          \return The object identification.
        */
        std::string getId() const;
        
        /*!
          \brief It sets the object identification.

          \param id The object identification.
        */
        void setId(const std::string& id);

        /*!
          \brief It returns the time when the event happened. 

          \return A pointer to the time when the event happened.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::dt::DateTime* getTime() const;

        /*!
          \brief It sets the time when the event happened.

          \param t The time when the event happened.

          \note It will take the ownership of the input pointer.
        */
        void setTime(te::dt::DateTime* t);

        /*!
          \brief It returns the location where the event happened. 

          \return A pointer to the location where the event happened.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::gm::Geometry* getLocation() const;

        /*!
          \brief It sets the location where the event happened.

          \param t The location where the event happened.

          \note It will take the ownership of the input pointer.
        */
        void setLocation(te::gm::Geometry* g);

        /*!
          \brief It returns the objects involved to the event. 

          \return A pointer to the objects involved to the event. 

          \note The caller will NOT take the ownership of the output pointers.
        */
        void getObjects(std::vector<te::st::Object*>& output) const;

        /*!
          \brief It sets the objects involved to the event. 

          \param objs The objects involved to the event. 

          \note It will NOT take the ownership of the input objects.
        */
        void setObjects(std::vector<te::st::Object*>& objs);
                                                
        /*! \brief Virtual destructor. */
        virtual ~Event(); 

      private:

        std::string                       m_id;          //!< The identification of the event. 
        std::auto_ptr<te::dt::DateTime>   m_time;        //!< The time when the event happened.  
        std::auto_ptr<te::gm::Geometry>   m_location;    //!< The location where the event happened.
        std::vector<te::st::Object*>      m_objects;     //!< The objects involved to the event. 
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_EVENT_H

