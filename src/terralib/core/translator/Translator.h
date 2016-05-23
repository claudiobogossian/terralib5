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
  \file terralib/core/translator/Translator.h

  \brief This class is designed for dealing with multi-language text translation in TerraLib.

  \author Matheus Cavassan Zaglia
  \author Gilberto Ribeiro de Queiroz
*/

#ifndef __TERRALIB_CORE_TRANSLATOR_H__
#define __TERRALIB_CORE_TRANSLATOR_H__

// TerraLib
#include "../../BuildConfig.h"
#include "Config.h"

// STL
#include <vector>
#include <string>

namespace te
{
  namespace core
  {
    /*!
      \class Translator

      \brief This singleton is designed to deal with multi-language text translation in TerraLib.

      The Translator job is to manage "Internationalization" of the TerraLib messages.
      This class is all about Native Language Support.
      For each string you want to have a translation, you have to use
      the special macro TR_TR("string") or TE_TR_PLURAL("string1","string2", int). This macro does nothing with your code;
      its job is just to mark the code fragment that you want to translate and it does
      all the job of calling the translation for you. The translations
      are created with the GNU Gettext Utilities.

      We provide a cmake module that searchs for all the necessary GNU Gettext tools, and adds a macro that creates the files for
      your translations.
      This macro searchs in a given directory for all ".cpp" files and for the given keywords inside the found files,
      then automatically generates the ".po", ".pot" and ".mo" files when you build your project.
      The macro implementation can be found in the file build/cmake/modules/FindGettext.cmake.

      \code
      #include <terralib/core/translator/Translator.h>
      ...
      TE_ADD_TEXT_DOMAIN("terralib_mod_core");
      ...
      std::string msg = TE_TR("My Message");
      ...
      or
      ...
      std::cout << TE_TR_PLURAL("One message", "Two messages", 1);
      ...
      \endcode

      Note that all marked messages will be part of a .pot file from where you can
      create several po documents, one for each idiom. These .po files
      can be used to generate a .mo binary file with the translation.

      \warning It is supposed that PO files are UTF-8 encoded.

      \ingroup core
     */
    class TECOREEXPORT Translator
    {

      public:


        /** @name Translation Methods
         *  Methods that can be used to tranlate texts.
         */
        //@{

        static Translator& getInstance()
        {
          static Translator instance;
          return instance;
        }

        /*!
          \brief It tries to translate the specified text string.

          If no translation is available it will return the
          original message (always in English).

          \param message    The text to be translated.

          \return A string of the translated message. You must not delete the memory pointed by the returned pointer.

          \note The returned message is UTF-8 encoded.
         */
        std::string translate(const std::string& message);

        /*!
          \brief It tries to translate the specified text string.

          If no translation is available it will return the
          original message (always in English).

          \param message    The text to be translated.

          \return A string of the translated message. You must not delete the memory pointed by the returned pointer.

          \note The returned message is UTF-8 encoded.
         */
        std::string translate(const char* message);

        /*!
          \brief It tries to translate the specified text string accounting for plural forms.

          If no translation is available it will return the
          original message (always in English).

          \param msg1       The singular form of the text to be translated.
          \param msg2       The plural form of the text to be translated.
          \param n          This parameter is used to determine the plural form.

          \return A string of the translated message. You must not delete the memory pointed by the returned pointer.

          \note The returned message is UTF-8 encoded.
         */
        std::string translate(const std::string& msg1,
                              const std::string& msg2,
                              unsigned int n);

        /*!
          \brief It tries to translate the specified text string accounting for plural forms.

          If no translation is available it will return the
          original message (always in English).

          \param msg1       The singular form of the text to be translated.
          \param msg2       The plural form of the text to be translated.
          \param n          This parameter is used to determine the plural form.

          \return A string of the translated message. You must not delete the memory pointed by the returned pointer.

          \note The returned message is UTF-8 encoded.
         */
        std::string translate(const char* msg1,
                              const char* msg2,
                              unsigned int n);

        /*!
          \brief It sets the locale for the Translator.

          \param locale A string of the new locale.
         */
        void setLocale(const std::string &locale);

        /*!
          \brief It adds a new text domain (text catalog).

          \param textDomain    A given message domain (just a name). A text domain is the name of the catalog used to translate the message.

          \exception Exception If the text domain already exists it raises an exception. If you are not sure about the existence of a text domain, use the exist() method.
         */
        void addTextDomain(const std::string& textDomain);


        /*!
          \brief It returns true if the text domain (text catalog) exists and false otherwise.

          \param textDomain A given message domain (just a name).
         */
        bool exist(const std::string& textDomain);

        //@}

      private:

        /*! \brief Singleton constructor must be private. */
        Translator(){};

        void operator=(Translator const&);

      private:

        std::string m_locale;
        std::vector<std::string> m_textDomainVector;  //!< A vector from text domains to base directory for the message catalog.

    };

  } // end namespace common
}   // end namespace te


/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{


// Check if the TR macro has already been defined
// by another application... if so, it will output
// an error message and stop compiling!
#ifdef TERRALIB_TRANSLATOR_ENABLED
#ifdef TE_TR
#error "The TE_TR macro has been already defined by another application or code. Please, inform TerraLib Development Team <terralib-team@dpi.inpe.br>, we will be glad to help solving this problem!"
#endif
#endif

/*!
  \def TERRALIB_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib.
 */

//#define TERRALIB_TEXT_DOMAIN "terralib_mod_core"

/*!
  \def TE_ADD_TEXT_DOMAIN

  \brief It adds the given text domain located at domain-dir with the given codeset to the multilingual system.

  \note This macro will check if the domain already exists before doing anyting.
 */
#ifdef TERRALIB_TRANSLATOR_ENABLED
  #define TE_ADD_TEXT_DOMAIN(domain) te::core::Translator::getInstance().addTextDomain(domain)
#else
  #define TE_ADD_TEXT_DOMAIN(domain) ((void)0)
#endif

/*!
  \def TE_TR

  \brief It marks a string in order to get translated.

  Example of usage:
  \code
  std::cout << TE_TR("My message!");

  throw Exception(TE_TR("My other message!"));
  \endcode
 */
#define TE_TR(message) te::core::Translator::getInstance().translate(message).c_str()

/*!
  \def TE_TR_PLURAL

  \brief It marks a string in order to get translated according to plural form.

  Example of usage:
  \code
  int n = f(...);

  std::cout << TE_TR_PLURAL("One Message!", "Two Messages", n);

  throw Exception(TE_TR_PLURAL("One Message!", "Two Messages", n));
  \endcode

  In the above example, the parameter n can be
  a threshold that helps to choose between the first or the second construction.
  If your trabslation file is configured with a theashold of 1,
  indicating that if n > 1 must choose the second construction,
  the plural versin will be choosed, otherwise, it will choose the
  singular form (the fisrt one).
 */
#define TE_TR_PLURAL(message1, message2, n) te::core::Translator::getInstance().translate(message1, message2, n).c_str()

#define TE_TR_LANGUAGE(message) te::core::Translator::getInstance().setLocale(message)

//@}

#endif  // __TERRALIB_CORE_TRANSLATOR_H__

