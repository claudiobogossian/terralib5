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
  \file terralib/common/Translator.h

  \brief This class is designed for dealing with multi-language text translation in TerraLib.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_TRANSLATOR_H
#define __TERRALIB_COMMON_INTERNAL_TRANSLATOR_H

// TerraLib
#include "Config.h"

#ifdef TERRALIB_TRANSLATOR_ENABLED
#include "Singleton.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace common
  {
    /*!
      \class Translator
      
      \brief This singleton is designed to deal with multi-language text translation in TerraLib.
      
      The Translator job is to manage "Internationalization" of the TerraLib messages.
      This class is all about Native Language Support.
      For each string you want to have a translation, you have to use
      the special macro TR_XXX("string"). This macro does nothing with your code;
      its job is just to mark the code fragment that you want to translate and it does
      all the job of calling the translation for you. Actually, the translation
      is done using GNU Text Utilities. So, you can use commands like:
      \verbatim      
      terralib# xgettext -D ../src/terralib/common -d tlcommon -o tlcommon.pot -f common.txt --keyword=TE_TR --from-code=UTF-8 --msgid-bugs-address=gribeiro.mg@gmail.com -p common
      terralib# msgmerge --directory=common -U tlcommon-pt_BR.po tlcommon.pot
      msgfmt --directory=common -c -v -o common/tlcommon-pt_BR.mo tlcommon-pt_BR.po
      \endverbatim

      Each module must define its translation macro. For example, in the 
      Common module we define the macro TE_TR, and all strings in the Common module are marked like:

      \code
      #include <terralib/common/Translator.h>
      ...
      char* msg = TE_TR("My Message");
      ...
      or
      ...
      std::cout << TE_TR("Other message");
      ...
      \endcode

      Note that all marked messages will be part of a .pot file from where you can
      create several po documents, one for each idiom. These .po files
      can be used to generate a .mo binary file with the translation.

      To create a translation macro, just do the following "define":
      \code
      #define TR_MYMODULE(message) TR(message, MY_MODULE_TEXT_DOMAIN)
      \endcode

      It is recommended that you create a text domain for your module like:
      \code
      #define MY_MODULE_TEXT_DOMAIN "mymodule"
      #define MY_MODULE_TEXT_DOMAIN_DIR "locale"
      \endcode

      Then you can create a initialization routine for registering your module's text domain (i.e. the message catalog of your module):
      \code
      #include <terralib/common/Translator.h>
      ...
      te::common::Translator::getInstance().addTextDomain(MY_MODULE_TEXT_DOMAIN, MY_MODULE_TEXT_DOMAIN_DIR);
      \endcode

      This will register your module text domain, enabling the selection of the domain
      the strings are coming from when you use the macro TR_MYMODULE.

      You can create plurals like:
      \code
      int n = f(...);
      const char* c = TR_PLURAL_COMMON("The module was initialized!", "The modules were initialized!", n);
      \endcode      

      \warning It is supposed that PO files are UTF-8 encoded.

      \todo Investigar o uso da biblioteca de formatacao de mensagens da BOOST para que seja possivel construir mensagens parametrizadas como no caso do printf. Isso pode dar um efeito muito melhor aas mensagens!

      \todo O todo acima deve ser considerado e todo codigo deve ser visto para poder colocar caracteres curinga dentro da mensagem que sejam substituidas, como o printf e cia!

      \todo Investigar o uso de mensagens no plural: http://www.gnu.org/software/hello/manual/gettext/Plural-forms.html#Plural-forms.

      \ingroup common
     */
    class TECOMMONEXPORT Translator : public Singleton<Translator>
    {
      friend class Singleton<Translator>;

      public:        

        /** @name Translation Methods
         *  Methods that can be used to tranlate texts.
         */
        //@{

        /*!
          \brief It tries to translate the specified text string.
          
          If no translation is available it will return the
          original message (always in English).
            
          \param message    The text to be translated.
          \param textDomain The text domain used in translation. A text domain is the name of the catalog used to translate the message.
          
          \return A pointer to the translated message. You must not delete the memory pointed by the returned pointer.

          \note The returned message is UTF-8 encoded.
         */
        const char* translate(const std::string& message,
                              const std::string& textDomain);

        /*!
          \brief It tries to translate the specified text string.
          
          If no translation is available it will return the
          original message (always in English).
            
          \param message    The text to be translated.
          \param textDomain The text domain used in the translation. A text domain is the name of the catalog used to translate the message.
          
          \return A pointer to the translated message. You must not delete the memory pointed by the returned pointer.

          \note The returned message is UTF-8 encoded.
         */
        const char* translate(const char* message,
                              const char* textDomain);

        /*!
          \brief It tries to translate the specified text string accounting for plural forms.
          
          If no translation is available it will return the
          original message (always in English).
          
          \param textDomain The text domain used in the translation. A text domain is the name of the catalog used to translate the message.
          \param msg1       The singular form of the text to be translated.
          \param msg2       The plural form of the text to be translated.
          \param n          This parameter is used to determine the plural form.           
          
          \return A pointer to the translated message. You must not delete the memory pointed by the returned pointer.

          \note The returned message is UTF-8 encoded.
         */
        const char* translate(const std::string& textDomain,
                              const std::string& msg1,
                              const std::string& msg2,
                              unsigned int n);

        /*!
          \brief It tries to translate the specified text string accounting for plural forms.
          
          If no translation is available it will return the
          original message (always in English).
          
          \param textDomain The text domain used in translation. A text domain is the name of the catalog used to translate the message.
          \param msg1       The singular form of the text to be translated.
          \param msg2       The plural form of the text to be translated.
          \param n          This parameter is used to determine the plural form.           
          
          \return A pointer to the translated message. You must not delete the memory pointed by the returned pointer.

          \note The returned message is UTF-8 encoded.
         */
        const char* translate(const char* textDomain,
                              const char* msg1,
                              const char* msg2,
                              unsigned int n);

        /*!
          \brief It adds a new text domain (text catalog) associated to a given directory.
          
          \param textDomain    A given message domain (just a name). A text domain is the name of the catalog used to translate the message.
          \param textDomainDir The base directory for the message catalogs (added with full path).

          \return A NULL string if the domain is not added.
          
          \exception Exception If the text domain already exists it raises an exception. If you are not sure about the existence of a text domain, use the exist() method.
         */
        const char* addTextDomain(const std::string& textDomain, const std::string& textDomainDir);

        /*!
          \brief It can be used to specify the output character set for a given message catalog.

          \param textDomain A given message domain (just a name). A text domain is the name of the catalog used to translate the message.
          \param codeset    If NULL returns the currently selected codeset for the domain. A valid codeset name is, for instance, UTF-8.

          \return A pointer to a string containing the name of the selected codeset. Don't delete the returned pointer!

          \exception Exception If the text domain doesn't exists. If you are not sure about the existence of a text domain, use the exist() method.
         */
        const char* setTextDomainCodeSet(const std::string& textDomain, const std::string& codeset);

        /*!
          \brief It returns true if the text domain (text catalog) exists and false otherwise.
          
          \param textDomain A given message domain (just a name).
         */
        bool exist(const std::string& textDomain);

        /*!
          \brief It returns the base directory for the message catalog identified by the text domain.
          
          \param textDomain A given message domain (just a name).
          
          \return The base directory for the message catalog identified by the text domain.
          
          \note If the domain doesn't exist, it will return an empty string.
         */
        std::string getTextDomainDir(const std::string& textDomain);

        //@}

       protected:

        /*! \brief Singleton constructor must be protected. */
        Translator();


        /* \brief Destructor. */
        ~Translator();

      private:

        std::map<std::string, std::string> m_textDomainMap;  //!< A map from text domains to base directory for the message catalog.
    };

  } // end namespace common
}   // end namespace te

#endif  // TERRALIB_TRANSLATOR_ENABLED

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
#define TERRALIB_TEXT_DOMAIN "terralib"

/*!
  \def TERRALIB_TEXT_DOMAIN_DIR
 
  \brief It contains the translation catalog directory.
 */
#define TERRALIB_TEXT_DOMAIN_DIR "locale"


/*!
  \def TE_ADD_TEXT_DOMAIN
 
  \brief It adds the given text domain located at domain-dir with the given codeset to the multilingual system.
 
  \note This macro will check if the domain already exists before doing anyting.
 */
#ifdef TERRALIB_TRANSLATOR_ENABLED
  #define TE_ADD_TEXT_DOMAIN(domain, domaindir, codeset)                         \
  if(!te::common::Translator::getInstance().exist(domain))                       \
  {                                                                              \
    te::common::Translator::getInstance().addTextDomain(domain, domaindir);      \
    te::common::Translator::getInstance().setTextDomainCodeSet(domain, codeset); \
  }                                                                              \
  ((void)0)
#else
  #define TE_ADD_TEXT_DOMAIN(domain, domaindir, codeset) ((void)0)
#endif

/*!
  \def TE_GENERAL_TR
 
  \brief Try to translate the message according to the given domain. See the TE_TR macro for more infomation on how to create a translation mark for your code.
 */
#ifdef TERRALIB_TRANSLATOR_ENABLED
  #define TE_GENERAL_TR(message, domain) te::common::Translator::getInstance().getInstance().translate(message, domain)
#else
  #define TE_GENERAL_TR(message, domain) message
#endif

/*!
  \def TE_GENERAL_TR_PLURAL
 
  \brief Try to translate the message according to the given domain and plural form. See the TE_TR_PLURAL macro for more infomation on how to create a translation mark for your code.
 */
#if TERRALIB_TRANSLATOR_ENABLED
  #define TE_GENERAL_TR_PLURAL(domain, message1, message2, n) te::common::Translator::getInstance().getInstance().translate(domain, message1, message2, n)
#else
  #define TE_GENERAL_TR_PLURAL(domain, message1, message2, n) (n > 1 ? message2 : message1)
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
#define TE_TR(message) TE_GENERAL_TR(message, TERRALIB_TEXT_DOMAIN)

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
#define TE_TR_PLURAL(message1, message2, n) TE_GENERAL_TR_PLURAL(TERRALIB_TEXT_DOMAIN, message1, message2, n)

//@}

#endif  // __TERRALIB_COMMON_INTERNAL_TRANSLATOR_H

