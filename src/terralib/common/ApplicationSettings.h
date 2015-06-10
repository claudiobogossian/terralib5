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
  \file terralib/common/ApplicationSettings.h

  \brief A class for managing application settings.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_APPLICATIONSETTINGS_H
#define __TERRALIB_COMMON_INTERNAL_APPLICATIONSETTINGS_H

// TerraLib
#include "Config.h"
#include "ThreadingPolicies.h"

// STL
#include <string>

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

namespace te
{
  namespace common
  {
    /*!
      \class ApplicationSettings

      \brief A class for managing application settings.

      This class can be used to keep user preferences that
      customize the application's appearance and behavior.
      It can also be used to keep settings applied to
      the system (for all users).

      For each setting you must provide an unique name.
      The rule for setting names are the same as
      for XML element names: it can be any combination of letters,
      numbers, or an underscore, they must not start with a number
      and cannot have white spaces.

      \ingroup common

      \todo Make some methods that accept arrays (get and set)!!!
    */
    class ApplicationSettings
          : public ObjectLevelLockable<ApplicationSettings,
                                       ::boost::recursive_mutex,
                                       ::boost::lock_guard< ::boost::recursive_mutex>,
                                       ::boost::lock_guard< ::boost::recursive_mutex> >,
            public ::boost::noncopyable
    {
      public:

        /*! \brief Constructor. */
        ApplicationSettings();

        /*! \brief The destructor will automatically save the settings to a file if one is set. */
        ~ApplicationSettings();

        /*!
          \brief It stores the value according to the key. 

          \param key   The key used to identify the given value.
                       The key must follow the same rules as XML elements and they can be
                       subclassified using a dot notation: setValue(key.subkey.subsubkey, value).
          \param value The value to be stored, any valid string.

          \note If you want to store a binary value you can encode it as an hex-string.

          \note Thread-safe depending on ThreadingPolicy.
        */
        void setValue(const std::string& key, const std::string& value);

        /*!
          \brief It returns the value for a given key or empty.

          \param key The key used to identify the searched value.

          \return The value for a given key or empty.

          \note Thread-safe depending on ThreadingPolicy.
        */
        std::string getValue(const std::string& key);

        /*!
          \brief It initializes the application settings.

          \param settingsFile The name of the application settings file.

          \note It discards all the settings and doesnt't save changes.

          \note If the informed file doesn't exist application settings will be empty after the excution of this method. 

          \note Thread-safe depending on ThreadingPolicy.

          \todo Este metodo deveria aceitar uma URL!!!!!!
        */
        void load(const std::string& settingsFile);

        /*!
          \brief It updates the application settings.

          \note This function is called automatically from ApplicationSettings's destructor.

          \note If application settings is empty it discards the config file.

          \note Thread-safe depending on ThreadingPolicy.
        */
        void update();

        /*!
          \brief This method can be used by clients to inform manually
                 that the internal state has changed.

          This method is useful when accessing the internal settings structure.

          \note Thread-safe depending on ThreadingPolicy.
        */
        void changed();

        /*!
          \brief It return a reading reference to the internal settings.

          \return A read reference to the internal settings.

          \note Take care when keeping a reference to this property in a multi-thread environment.
        */
        const boost::property_tree::ptree& getAllSettings() const;

        /*!
          \brief It return a reference to the internal settings.

          \return A reference to the internal settings.

          \note Depending on the ThreadingPolicy you can synchronize the read/write from/to the settings.
                The client of this method will be responsible for informing changes. Release any lock before calling
                the update method or any other thread-safe method if you are not using a recursive mutex in the
                threading policy.
        */
        boost::property_tree::ptree& getAllSettings();

      protected:

        boost::property_tree::ptree m_settings;   //!< This will keep our settings in memory.
        std::string m_file;                       //!< The settings file name.
        bool m_dirty;                             //!< A dirty bit to indicate wheter the clients have made changes in the settings since the last update.
    };

    inline ApplicationSettings::ApplicationSettings()
      : m_dirty(false)
    {
    }

    inline
    ApplicationSettings::~ApplicationSettings()
    {
      update();
    }

    inline void ApplicationSettings::setValue(const std::string& key, const std::string& value)
    {
      LockWrite l(this);

      m_settings.put(key, value);
      m_dirty = true;
    }

    inline std::string ApplicationSettings::getValue(const std::string& key)
    {
      LockRead l(this);

      try
      {
        return m_settings.get<std::string>(key);
      }
      catch(...)
      {
        return "";
      }
    }

    inline void ApplicationSettings::load(const std::string& settingsFile)
    {
      LockWrite l(this);

      m_dirty = false;
      m_settings.clear();
      m_file = settingsFile;

      if(boost::filesystem::is_regular_file(m_file))
        boost::property_tree::read_xml(m_file, m_settings, boost::property_tree::xml_parser::trim_whitespace);
    }

    inline void ApplicationSettings::update()
    {
      LockWrite l(this);

      if(m_dirty == false)
        return;

      if(m_settings.empty())
        boost::filesystem::remove(m_file);
      else
      {
#if BOOST_VERSION > 105600
        boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
#else
        boost::property_tree::xml_writer_settings<char> settings('\t', 1);
#endif
        boost::property_tree::write_xml(m_file, m_settings, std::locale(), settings);
      }

      m_dirty = false;
    }

    inline void ApplicationSettings::changed()
    {
      LockWrite l(this);

      m_dirty = true;
    }

    inline const boost::property_tree::ptree& ApplicationSettings::getAllSettings() const
    {
      return m_settings;
    }

    inline boost::property_tree::ptree& ApplicationSettings::getAllSettings()
    {
      return m_settings;
    }

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_APPLICATIONSETTINGS_H

