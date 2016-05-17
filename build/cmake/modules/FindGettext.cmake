#
#  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of the TerraLib - a Framework for building GIS enabled applications.
#
#  TerraLib is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License,
#  or (at your option) any later version.
#
#  TerraLib is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with TerraLib. See COPYING. If not, write to
#  TerraLib Team at <terralib-team@terralib.org>.
#
#
#  Description: Find Gettext tools and macros for creating translations and templates
#
#  Author: Matheus Cavassan Zaglia
#          Frederico Bedê
#
#  Notes:
#
#    There are three files created with the macros provided in this module:
#
#    POT File -> Template for translations, do not translate in this file.
#    PO  File -> Created from POT file, translations are made in this file.
#    MO  File -> Binary file of the PO file, this file will be used to translate your application.
#
#    The following macros use the GNU Gettext.
#    The GNU Gettext is available for Windows, Linux and MAC OS X at: https://www.gnu.org/software/gettext/
#
#  Usage:
#
#    GETTEXT_CREATE_TRANSLATIONS(
#     terralib_unittest_core                        <--- Name of the translation
#     "TE_TR"                                       <--- Keyword that will be searched
#     "TE_TR_PLURAL"                                <--- Keyword for plural texts
#     "pt_BR"                                       <--- Translation locale
#     "${TERRALIB_ABSOLUTE_ROOT_DIR}/unittest/core" <--- Where the macro will search for the keyword)


FIND_PROGRAM(GETTEXT_MSGMERGE_EXECUTABLE msgmerge)
FIND_PROGRAM(GETTEXT_MSGFMT_EXECUTABLE msgfmt)
FIND_PROGRAM(GETTEXT_MSGINIT_EXECUTABLE msginit)
FIND_PROGRAM(XGETTEXT_EXECUTABLE xgettext)

IF (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE AND GETTEXT_MSGINIT_EXECUTABLE)
  SET(GETTEXT_FOUND TRUE)
ELSE (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE AND GETTEXT_MSGINIT_EXECUTABLE)
  SET(GETTEXT_FOUND FALSE)
  IF (Gettext_REQUIRED)
    MESSAGE(FATAL_ERROR "GetText not found")
  ENDIF (Gettext_REQUIRED)
ENDIF (GETTEXT_MSGMERGE_EXECUTABLE AND GETTEXT_MSGFMT_EXECUTABLE AND GETTEXT_MSGINIT_EXECUTABLE)
IF(XGETTEXT_EXECUTABLE)
  SET(XGETTEXT_FOUND TRUE)
ELSE(XGETTEXT_EXECUTABLE)
  MESSAGE(STATUS "xgettext not found.")
  SET(XGETTTEXT_FOUND FALSE)
ENDIF(XGETTEXT_EXECUTABLE)
IF(XGETTEXT_FOUND)
IF (WIN32)
   STRING(REGEX REPLACE "/" "\\\\" XGETTEXT_EXECUTABLE ${XGETTEXT_EXECUTABLE})
   STRING(REGEX REPLACE "/" "\\\\" GETTEXT_MSGFMT_EXECUTABLE ${GETTEXT_MSGFMT_EXECUTABLE})
   STRING(REGEX REPLACE "/" "\\\\" GETTEXT_MSGMERGE_EXECUTABLE ${GETTEXT_MSGMERGE_EXECUTABLE})
   STRING(REGEX REPLACE "/" "\\\\" GETTEXT_MSGINIT_EXECUTABLE ${GETTEXT_MSGINIT_EXECUTABLE})
ENDIF (WIN32)
ENDIF(XGETTEXT_FOUND)

include(terralib_macros.cmake)
