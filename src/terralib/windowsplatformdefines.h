#ifndef __TERRALIB_INTERNAL_WINDOWSPLATFORMDEFINES_H 
#define __TERRALIB_INTERNAL_WINDOWSPLATFORMDEFINES_H

/** @name Dependency Libraries
 *  Flags for Enabling/Disabling Dependency Libraries in TerraLib.
 */
//@{

/*!
  \def TERRALIB_STATIC_BUILD

  \brief If set to 1 this will tells TerraLib that it is statically build.
 */
#ifndef TE_TERRALIB_STATIC_BUILD
  #define TE_TERRALIB_STATIC_BUILD 0
#endif

/*!
  \def TE_LOGGER_ENABLED

  \brief If you want to disable all kind of logging in the TerraLib code just turn off this macro (set it to 0).
 */
#ifndef TE_LOGGER_ENABLED
  #define TE_LOGGER_ENABLED 1
#endif

/*!
  \def TE_LOGGER_DO_AUTOMATIC_INITIALIZATION

  \brief If this flag is set to 1, the logger will be automatically initialized by the TerraLib Common Runtime module.

  If you want to make the initialization in your application, feel free to change this macro to 0.
 */
#define TE_LOGGER_DO_AUTOMATIC_INITIALIZATION 1

/*!
  \def TE_LOGGER_DO_STATIC_INITIALIZATION

  \brief If you want to start log during static initialization turn on this macro (set it to 1) otherwise turn it off (set it to 0).

  If you want to make the initialization in your application, feel free to change this macro to 0.
 */
#define TE_LOGGER_DO_STATIC_INITIALIZATION 0

/*!
  \def TE_USE_APACHE_LOG4CXX

  \brief If this flag is set to 1 the Apache log4cxx library will be used for log support.
 */
#ifndef TE_USE_APACHE_LOG4CXX
  #define TE_USE_APACHE_LOG4CXX 1
#endif

/*!
  \def TE_TRANSLATOR_ENABLED

  \brief If you want to disable all kind of translation support in the TerraLib code just turn off this macro (set it to 0).
 */
#ifndef TE_TRANSLATOR_ENABLED
  #define TE_TRANSLATOR_ENABLED 1
#endif

/*!  
  \def TE_USE_GNU_TEXT_UTILITIES

  \brief If you want to have international support you must set this flag to 1, otherwise set it to 0 (zero).
 */
#ifndef TE_USE_GNU_TEXT_UTILITIES
  #define TE_USE_GNU_TEXT_UTILITIES 1
#endif

/*!
  \def TE_CHARENCODING_ENABLED

  \brief If you want to disable all kind of character encoding conversion in the TerraLib code just turn off this macro (set it to 0).
 */
#ifndef TE_CHARENCODING_ENABLED
  #define TE_CHARENCODING_ENABLED 1
#endif

/*!  
  \def TE_USE_GNU_ICONV

  \brief If you want to have character encoding support you must set this flag to 1, otherwise set it to 0 (zero).
 */
#ifndef TE_USE_GNU_ICONV
  #define TE_USE_GNU_ICONV 1
#endif

/*!
  \def TE_USE_GDAL

  \brief If this flag is set to 1 the GDAL driver support will be enabled.
 */
#ifndef TE_USE_GDAL
  #define TE_USE_GDAL 1
#endif

/*!
  \def TE_USE_OGR

  \brief If this flag is set to 1 the OGR driver support will be enabled.
 */
#ifndef TE_USE_OGR
  #define TE_USE_OGR 1
#endif

/*!
  \def TE_USE_PROJ4

  \brief If this flag is set to 1 the PROJ4 driver support will be enabled.
 */
#ifndef TE_USE_PROJ4
  #define TE_USE_PROJ4 1
#endif

/*!
  \def TE_USE_PGIS

  \brief If this flag is set to 1 the POSTGIS driver support will be enabled.
 */
#ifndef TE_USE_PGIS
  #define TE_USE_PGIS 1
#endif

/*!
  \def TE_USE_GEOS

  \brief If this flag is set to 1 the GEOS library can be used in the Vector Geometry Model of TerraLib.

  This will give you the possibility to convert between geometry models and use all the functionalities
  in GEOS. This library is available at: http://trac.osgeo.org/geos.
 */
#ifndef TE_USE_GEOS
  #define TE_USE_GEOS 1
#endif

/*!
  \def TE_USE_CGAL

  \brief If this flag is set to 1 the CGAL library can be used in the Vector Geometry Model of TerraLib.

  This will give you the possibility to convert between geometry models and use all the functionalities
  in CGAL. This library is available at: http://www.cgal.org/.
 */
#ifndef TE_USE_CGAL
  #define TE_USE_CGAL 0
#endif

/*!
  \def TE_USE_CGAL

  \brief If this flag is set to 1 the Generic Geometry Library can be used in the Vector Geometry Model of TerraLib.

  This will give you the possibility to convert between geometry models and use all the functionalities
  in GGL. This library is available at: http://geometrylibrary.geodan.nl/.
 */
#ifndef TE_USE_GGL
  #define TE_USE_GGL 0
#endif

/*!
  \def TE_USE_WYKOBI

  \brief If this flag is set to 1 the Wykobi library can be used in the Vector Geometry Model of TerraLib.

  This will give you the possibility to convert between geometry models and use all the functionalities
  in Wykobi. This library is available at: http://www.wykobi.com/.
 */
#ifndef TE_USE_WYKOBI
  #define TE_USE_WYKOBI 0
#endif

/*!
  \def TE_USE_FASTGEO

  \brief If this flag is set to 1 the FastGEO library can be used in the Vector Geometry Model of TerraLib.

  This will give you the possibility to convert between geometry models and use all the functionalities
  in FastGEO. This library is available at: http://www.partow.net/projects/fastgeo/index.html.
 */
#ifndef TE_USE_FASTGEO
  #define TE_USE_FASTGEO 0
#endif

//@}

#endif /*__TERRALIB_INTERNAL_WINDOWSPLATFORMDEFINES_H */
