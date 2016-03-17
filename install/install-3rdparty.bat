IF NOT EXIST %TERRALIB_DEPENDENCIES_DIR% mkdir %TERRALIB_DEPENDENCIES_DIR% 
IF NOT EXIST %TERRALIB_DEPENDENCIES_DIR%\include mkdir %TERRALIB_DEPENDENCIES_DIR%\include 
IF NOT EXIST %TERRALIB_DEPENDENCIES_DIR%\lib mkdir %TERRALIB_DEPENDENCIES_DIR%\lib 

del /Q ..\*.log >nul 2>nul

set "ROOT_DIR=%CD%"

set "BUILD_LOG=%ROOT_DIR%\..\build.log"
set "CONFIG_LOG=%ROOT_DIR%\..\config.log"
set "FAILURES_LOG="%ROOT_DIR%\..\failures.log"

::  Setting visual studio environment
::  =================================

IF DEFINED TERRALIB_X64 ( 
set "_config=x64"  
set "_X86=_x64"
set "PATH=%CMAKE_FILEPATH%;%QMAKE_FILEPATH%;%ROOT_DIR%\icu\bin64;%ROOT_DIR%\curl-7.42.1\binaries%_X86%\lib;%PATH%"
set _CMAKE_GENERATOR="Visual Studio 12 2013 Win64" 
) ELSE ( 
set "_config=x86"
set "_X86=_x86"
set "PATH=%CMAKE_FILEPATH%;%QMAKE_FILEPATH%;%ROOT_DIR%\icu\bin;%ROOT_DIR%\curl-7.42.1\binaries%_X86%\lib;%PATH%"
set _CMAKE_GENERATOR="Visual Studio 12 2013" 
)

echo | set /p="Configuring visual studio... "<nul

call "%VCVARS_FILEPATH%"\vcvarsall.bat %_config%

echo done.
echo.

::  ================================

set "INCLUDE=%WIN32MAK_FILEPATH%;%INCLUDE%"

goto begin_libs

:: Building libraries
::  ==================

:append_log_begin
SETLOCAL
echo ********************** >> %CONFIG_LOG% 2>nul
echo %1 >> %CONFIG_LOG% 2>nul
echo ********************** >> %CONFIG_LOG% 2>nul

echo ********************** >> %BUILD_LOG% 2>nul
echo %1 >> %BUILD_LOG% 2>nul
echo ********************** >> %BUILD_LOG% 2>nul
goto :EOF
ENDLOCAL


:append_log_end
SETLOCAL
echo. >> %CONFIG_LOG% 2>nul
echo ********************** >> %CONFIG_LOG% 2>nul
echo. >> %CONFIG_LOG% 2>nul
echo. >> %CONFIG_LOG% 2>nul

echo. >> %BUILD_LOG% 2>nul
echo ********************** >> %BUILD_LOG% 2>nul
echo. >> %BUILD_LOG% 2>nul
echo. >> %BUILD_LOG% 2>nul

goto :EOF
ENDLOCAL

:skip_build
SETLOCAL
echo skip.

goto :EOF
ENDLOCAL

:remove_lib
SETLOCAL
del %TERRALIB_DEPENDENCIES_DIR%\lib\*%1* /S /Q >nul 2>nul

goto :EOF
ENDLOCAL

:: 1-Nome da biblioteca
:: 2-Mensagem de erro simples.
:: 3-Desvio, nome do atalho para a próxima biblioteca

:buildFailLog
SETLOCAL
set FAIL=1
echo fail on %2.
echo %1: fail on %2. >>%FAILURES_LOG%  

goto :EOF
ENDLOCAL

:begin_libs
set LIBS_DIR=%TERRALIB_DEPENDENCIES_DIR%\lib

:: libraries not linked against TerraLib 5 (NOT fully installed)
:: -------------------------------------------------------------
set EXPAT=%LIBS_DIR%\expat.dll
set EAY=%LIBS_DIR%\libeay32.dll
set EAYD=%LIBS_DIR%\libeay32d.dll
set SSL=%LIBS_DIR%\ssleay32.dll
set SSLD=%LIBS_DIR%\ssleay32d.dll
set ZLIB=%LIBS_DIR%\zlib.dll
set ZLIBD=%LIBS_DIR%\zlibd.dll
set READLINE=%LIBS_DIR%\readline-32.dll
set READLINED=%LIBS_DIR%\readline-32d.dll
set PCRE=%LIBS_DIR%\pcre.dll
set PCRED=%LIBS_DIR%\pcred.dll
set FREEXL=%LIBS_DIR%\freexl.dll
set FREEXLD=%LIBS_DIR%\freexld.dll
set PNG=%LIBS_DIR%\libpng15.dll
set HDF4=%LIBS_DIR%\hdfdll.dll
set TIFF=%LIBS_DIR%\libtiff.dll
set GEOTIFF=%LIBS_DIR%\libgeotiff.dll
set CURL=%LIBS_DIR%\libcurl.dll
set ICU=%LIBS_DIR%\icuuc52.dll
set XML2=%LIBS_DIR%\libxml2.dll
set NETCDF=%LIBS_DIR%\netcdf.dll

set APR=%ROOT_DIR%\install%_X86%\bin\libapr-1.dll
set APRD=%ROOT_DIR%\install%_X86%\bin\libapr-1d.dll
set APRUTIL=%ROOT_DIR%\install%_X86%\bin\libaprutil-1.dll
set APRUTILD=%ROOT_DIR%\install%_X86%\bin\libaprutil-1d.dll
set LIBKML=%ROOT_DIR%\libkml-master\build%_X86%\Release\libkml.lib
set BZIP=%ROOT_DIR%\bzip2-1.0.6\lib%_X86%\libbz2.lib
set JPEG=%ROOT_DIR%\jpeg-9a\build%_X86%\libjpeg.lib
set MINIZIP=%ROOT_DIR%\unzip101e\build%_X86%\Release\minizip.lib

set _aux=_x86
IF DEFINED TERRALIB_X64 set "_aux="
set URIPARSER=%ROOT_DIR%\uriparser-0.8.4\win32\build%_aux%\uriparser.lib

:: libraries linked against TerraLib 5 (fully installed)
:: -------------------------------------------------------------
set LOG4CXX=%LIBS_DIR%\log4cxx.lib
set ICONV=%LIBS_DIR%\iconv.lib
set PROJ=%LIBS_DIR%\proj_i.lib
set GEOS=%LIBS_DIR%\geos_i.lib
set XERCES=%LIBS_DIR%\xerces-c_3.lib
set BOOST=%LIBS_DIR%\boost_system-mt.lib
set PGIS=%LIBS_DIR%\libpqdll.lib
set SQLITE=%LIBS_DIR%\sqlite3.lib
set SPATIALITE=%LIBS_DIR%\spatialite_i.lib
set GDAL=%LIBS_DIR%\gdal_i.lib
set PROPERTYBROWSER=%LIBS_DIR%\qt_property_browser.lib
set QWT=%LIBS_DIR%\qwt.lib
set LUA=%LIBS_DIR%\lua.lib
set TERRALIB4=%LIBS_DIR%\terralib.lib

IF NOT EXIST %LIBS_DIR% goto begin_build

goto cppunit_deps

:begin_build

:: CppUnit version 1.12.1
set CPPUNIT_DIR=%ROOT_DIR%\cppunit-1.12.1
set CPPUNIT_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include\cppunit
set CPPUNIT_LIBRARY=%LIBS_DIR%\cppunit_dll.lib
set CPPUNITD_LIBRARY=%LIBS_DIR%\cppunitd_dll.lib

:: Check dependencies
goto end_cppunit_deps
:cppunit_deps
goto iconv_deps
:end_cppunit_deps

echo | set /p="Installing cppunit... "<nul

IF EXIST %CPPUNIT_LIBRARY% call :skip_build && goto iconv 

call :append_log_begin cppunit

:begin_cppunit

IF NOT DEFINED TERRALIB_X64 set "_bits=_x86"

cd %CPPUNIT_DIR%\src >nul 2>nul

( msbuild /m /t:clean /p:Configuration=Release cppunit\cppunit_dll.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog cppunit  "clean release" && goto iconv

( msbuild /m /t:clean cppunit\cppunit_dll.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog cppunit  "clean debug" && goto iconv

( msbuild /m /p:Configuration=Release cppunit\cppunit_dll.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog cppunit "build release" && goto iconv

( msbuild /m cppunit\cppunit_dll.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog cppunit "build debug" && goto iconv

IF NOT EXIST %CPPUNIT_INCLUDE_DIR% mkdir %CPPUNIT_INCLUDE_DIR%

xcopy %CPPUNIT_DIR%\include\cppunit %CPPUNIT_INCLUDE_DIR% /S /Y >nul 2>nul

xcopy %CPPUNIT_DIR%\src\cppunit\ReleaseDll%_bits%\*.lib %LIBS_DIR% /Y >nul 2>nul

xcopy %CPPUNIT_DIR%\src\cppunit\ReleaseDll%_bits%\*.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %CPPUNIT_DIR%\src\cppunit\DebugDll%_bits%\*.lib %LIBS_DIR% /Y >nul 2>nul

xcopy %CPPUNIT_DIR%\src\cppunit\DebugDll%_bits%\*.dll %LIBS_DIR% /Y >nul 2>nul  

call :append_log_end cppunit

:end_cppunit

echo done.

cd %ROOT_DIR%
:: ====

:iconv

::  ====
::  Iconv
set ICONV_DIR=%ROOT_DIR%\iconv
set ICONV_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set ICONV_LIBRARY=%ICONV%
set ICONVD_LIBRARY=%LIBS_DIR%\iconvd.lib
set ICONV_LIBRARIES=debug;%ICONVD_LIBRARY%;optimized;%ICONV_LIBRARY%

:: Check dependencies
goto end_iconv_deps
:iconv_deps
goto expat_deps
:end_iconv_deps

echo | set /p="Installing iconv... "<nul

IF EXIST %ICONV_LIBRARY% call :skip_build && goto expat 

call :append_log_begin iconv

:begin_iconv

cd %ICONV_DIR% >nul 2>nul

( msbuild /m /t:clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog iconv  "clean debug" && goto expat

( msbuild /m /p:Configuration=Release /t:clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog iconv  "clean release" && goto expat

( msbuild /m  myIconv.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog iconv "build debug" && goto expat

( msbuild /m /p:Configuration=Release myIconv.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog iconv "build release" && goto expat

xcopy %ICONV_DIR%\myIconv\include\iconv.h %ICONV_INCLUDE_DIR% /Y >nul 2>nul

IF DEFINED TERRALIB_X64 ( 
  set "_libF=x64" 
) ELSE set "_libF=x86"

xcopy %ICONV_DIR%\%_libF%\Release\*.lib %LIBS_DIR% /Y  >nul 2>nul
xcopy %ICONV_DIR%\%_libF%\Release\*.dll %LIBS_DIR% /Y  >nul 2>nul

xcopy %ICONV_DIR%\%_libF%\Debug\*.lib %LIBS_DIR% /Y  >nul 2>nul
xcopy %ICONV_DIR%\%_libF%\Debug\*.dll %LIBS_DIR% /Y  >nul 2>nul

call :append_log_end iconv

:end_iconv

echo done.

cd %ROOT_DIR%
::  ========

:expat

:: Expat version 2.1.0
set EXPAT_DIR=%ROOT_DIR%\expat-2.1.0
set EXPAT_INCLUDE_DIR=%EXPAT_DIR%\binaries%_X86%\include
set EXPAT_LIBRARY=%EXPAT_DIR%\binaries%_X86%\lib\expat.lib
set EXPATD_LIBRARY=%EXPAT_DIR%\binaries%_X86%\lib\expatd.lib

:: Check dependencies
goto end_expat_deps
:expat_deps
goto apr_deps
:end_expat_deps

echo | set /p="Installing expat... "<nul

IF EXIST %EXPAT% call :skip_build && goto apr 

call :append_log_begin expat

:begin_expat

cd %EXPAT_DIR%

del build%_X86% /S /Q >nul 2>nul

del binaries%_X86% /S /Q >nul 2>nul

mkdir build%_X86% >nul 2>nul

cd build%_X86% >nul 2>nul

( cmake -G %_CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX=%EXPAT_DIR%\binaries%_X86% ^
 -DCMAKE_DEBUG_POSTFIX=d %EXPAT_DIR% >>%CONFIG_LOG% 2>nul ) || call :buildFailLog expat "configuring" && goto apr

( msbuild /m /p:Configuration=Release INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog expat "build release" && goto apr

( msbuild /m INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog expat "build debug" && goto apr

xcopy %EXPAT_DIR%\binaries%_X86%\bin\*.dll %LIBS_DIR% /Y >nul 2>nul

call :append_log_end expat

:end_expat  

echo done.

cd %ROOT_DIR%
:: ====

:apr

:: APR-1.5.2
::  =========================================
set APACHE_INSTALL_DIR=%ROOT_DIR%\install%_X86%

set APR_DIR=%ROOT_DIR%\apr-1.5.2
set APR_INCLUDE_DIR=%APACHE_INSTALL_DIR%\include
set APR_LIBRARY=%APACHE_INSTALL_DIR%\lib\libapr-1.lib
set APRD_LIBRARY=%APACHE_INSTALL_DIR%\lib\libapr-1d.lib

:: Check dependencies
goto end_apr_deps
:apr_deps
goto zlib_deps
:end_apr_deps

echo | set /p="Installing apr... "<nul

IF EXIST %APR% call :skip_build && goto zlib 

call :append_log_begin apr

:begin_apr

cd %APR_DIR% >nul 2>nul

IF EXIST building%_X86% del /s /Q building%_X86% >nul 2>nul

mkdir building%_X86% >nul 2>nul

cd building%_X86% >nul 2>nul

( cmake -G %_CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX=%APACHE_INSTALL_DIR% -DCMAKE_DEBUG_POSTFIX=d -DINSTALL_PDB=OFF %APR_DIR% >>%CONFIG_LOG% 2>nul ) || call :buildFailLog apr "configuring" && goto zlib 

( msbuild /m INSTALL.vcxproj /p:Configuration=Release >>%BUILD_LOG% 2>nul ) || call :buildFailLog apr "build release" && goto zlib

( msbuild /m INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog apr "build debug" && goto zlib

call :append_log_end apr

:end_apr  

echo done.

cd %ROOT_DIR%
::  ================================

:zlib

:: ZLIB
::  ====
set ZL_DIR=%ROOT_DIR%\zlib-1.2.8
set ZL_INCLUDE_DIR=%ZL_DIR%;%ZL_DIR%\build%_X86%
set ZL_LIBRARY=%ZL_DIR%\build%_X86%\Release\zlib.lib
set ZLD_LIBRARY=%ZL_DIR%\build%_X86%\Debug\zlibd.lib
set ZL_LIBRARIES=debug;%ZLD_LIBRARY%;optimized;%ZL_LIBRARY%

:: Check dependencies
goto end_zlib_deps
:zlib_deps
goto openssl_deps
:end_zlib_deps

echo | set /p="Installing zlib... "<nul

IF EXIST %ZLIB% call :skip_build && goto openssl 

call :append_log_begin zlib

:begin_zlib

cd %ZL_DIR%

del build%_X86% /S /Q >nul 2>nul

mkdir build%_X86% >nul 2>nul

cd build%_X86% >nul 2>nul

( cmake -G %_CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX=%TERRALIB_DEPENDENCIES_DIR% %ZL_DIR% >>%CONFIG_LOG% 2>nul ) || call :buildFailLog  zlib "configuring" && goto openssl

( msbuild /m /p:Configuration=Release zlib.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog zlib "build release" && goto openssl

( msbuild /m zlib.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog zlib "build debug" && goto openssl

xcopy %ZL_DIR%\build%_X86%\Release\*.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %ZL_DIR%\build%_X86%\Debug\*.dll %LIBS_DIR% /Y >nul 2>nul

call :append_log_end zlib

:end_zlib  

echo done.

cd %ROOT_DIR%
::  ================================

:openssl

IF NOT DEFINED TERRALIB_X64 (
set "_x86_=.x86"
set "_out_=32"
) ELSE ( 
set "_out_=64" 
set "_x86_=.x64"
)

::  OPENSSL 
set SSL_DIR=%ROOT_DIR%\openssl-master
set SSL_INCLUDE_DIR=%SSL_DIR%\inc%_out_%
set SSL_LIBRARY=%SSL_DIR%\out%_out_%dll\ssleay32.lib
set SSLD_LIBRARY=%SSL_DIR%\out%_out_%dll.dbg\ssleay32d.lib
set EAY_LIBRARY=%SSL_DIR%\out%_out_%dll\libeay32.lib
set EAYD_LIBRARY=%SSL_DIR%\out%_out_%dll.dbg\libeay32d.lib

:: Check dependencies
goto end_openssl_deps
:openssl_deps
IF NOT EXIST %ZLIB% call :remove_lib %SSL% && goto apr_util_deps
goto apr_util_deps
:end_openssl_deps

echo | set /p="Installing openssl... "<nul

IF EXIST %SSL% call :skip_build && goto apr_util 

call :append_log_begin openssl

:begin_openssl

del %SSL_DIR%\build%_X86% /S /Q >nul 2>nul

mkdir %SSL_DIR%\build%_X86% >nul 2>nul

cd %SSL_DIR%

:: Clear
del %SSL_DIR%\out%_out_%dll\* /S /Q >nul 2>nul

del %SSL_DIR%\out%_out_%dll.dbg\* /S /Q >nul 2>nul

del %SSL_DIR%\tmp%_out_%dll\*.obj /S /Q >nul 2>nul

del %SSL_DIR%\tmp%_out_%dll\*.pdb /S /Q >nul 2>nul

del %SSL_DIR%\tmp%_out_%dll.dbg\*.obj /S /Q >nul 2>nul

del %SSL_DIR%\tmp%_out_%dll.dbg\*.pdb /S /Q >nul 2>nul

::  Building release
copy ms\libeay32.release%_x86_%.def.in ms\libeay32.def /Y >nul 2>nul

copy ms\ssleay32.release%_x86_%.def.in ms\ssleay32.def /Y >nul 2>nul

::copy ms\nt.release%_x86_%.mak.in ms\nt.mak /Y >nul 2>nul

copy ms\ntdll.release%_x86_%.mak.in ms\ntdll.mak /Y >nul 2>nul  

( nmake /f ms\ntdll.mak lib >>%BUILD_LOG% 2>nul ) || call :buildFailLog openssl "build release" && goto apr_util

copy ms\libeay32.debug%_x86_%.def.in ms\libeay32.def /Y >nul 2>nul

copy ms\ssleay32.debug%_x86_%.def.in ms\ssleay32.def /Y >nul 2>nul

::copy ms\nt.debug%_x86_%.mak.in ms\nt.mak /Y >nul 2>nul

copy ms\ntdll.debug%_x86_%.mak.in ms\ntdll.mak /Y >nul 2>nul

( nmake /f ms\ntdll.mak lib >>%BUILD_LOG% 2>nul ) || call :buildFailLog openssl "build debug" && goto apr_util

xcopy out%_out_%dll\ssleay32.dll %LIBS_DIR% /Y >nul 2>nul

xcopy out%_out_%dll\libeay32.dll %LIBS_DIR% /Y >nul 2>nul

xcopy out%_out_%dll.dbg\ssleay32d.dll %LIBS_DIR% /Y >nul 2>nul

xcopy out%_out_%dll.dbg\libeay32d.dll %LIBS_DIR% /Y >nul 2>nul

call :append_log_end openssl

:end_openssl  

echo done.

cd %ROOT_DIR%
::  ====


:apr_util


:: APRUTIL-1.5.4
::  =========================================
set APRUTIL_DIR=%ROOT_DIR%\apr-util-1.5.4
set APRUTIL_INCLUDE_DIR=%APACHE_INSTALL_DIR%\include
set APRUTIL_LIBRARY=%APACHE_INSTALL_DIR%\lib\libaprutil-1.lib
set APRUTILD_LIBRARY=%APACHE_INSTALL_DIR%\lib\libaprutil-1d.lib

:: Check dependencies
goto end_apr_util_deps
:apr_util_deps
IF NOT EXIST %APR% call :remove_lib aprutil && goto log4cxx_deps
IF NOT EXIST %EXPAT% call :remove_lib aprutil && goto log4cxx_deps
goto log4cxx_deps
:end_apr_util_deps

echo | set /p="Installing apr-util... "<nul

IF EXIST %APRUTIL% call :skip_build && goto log4cxx 

call :append_log_begin apr_util

:begin_apr_util

cd %APRUTIL_DIR% >nul 2>nul

del building%_X86% /S /Q >nul 2>nul

mkdir building%_X86% >nul 2>nul

cd building%_X86% >nul 2>nul

( cmake -G %_CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX=%APACHE_INSTALL_DIR% ^
-DCMAKE_DEBUG_POSTFIX=d ^
-DINSTALL_PDB=OFF ^
-D_OPENSSL_VERSION=1.1.0 ^
-DOPENSSL_INCLUDE_DIR=%SSL_INCLUDE_DIR% ^
-DLIB_EAY_LIBRARY_DEBUG=%EAYD_LIBRARY% ^
-DLIB_EAY_LIBRARY_RELEASE=%EAY_LIBRARY% ^
-DSSL_EAY_LIBRARY_DEBUG=%SSLD_LIBRARY% ^
-DSSL_EAY_LIBRARY_RELEASE=%SSL_LIBRARY% ^
-DXMLLIB_LIBRARIES:STRING="debug;%EXPATD_LIBRARY%;optimized;%EXPAT_LIBRARY%" ^
-DXMLLIB_INCLUDE_DIR=%EXPAT_INCLUDE_DIR% ^
-DAPR_INCLUDE_DIR=%APACHE_INSTALL_DIR%/include ^
-DAPR_LIBRARIES=%APR_LIBRARY% %APRUTIL_DIR% >>%CONFIG_LOG% 2>nul ) || call :buildFailLog apr-util "configuring" && goto log4cxx 

( msbuild /m INSTALL.vcxproj /p:Configuration=Release >>%BUILD_LOG% 2>nul ) || call :buildFailLog apr-util "build release" && goto log4cxx

( msbuild /m INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog apr-util "build debug" && goto log4cxx

call :append_log_end apr_util

:end_apr_util  

echo done.

cd %ROOT_DIR%
::  ================================

:log4cxx

:: Log4cxx-0.10.0
::  =========================================
set LOG4CXX_DIR=%ROOT_DIR%\apache-log4cxx-0.10.0
set LOG4CXX_LIBRARY=%LOG4CXX%

:: Check dependencies
goto end_log4cxx_deps
:log4cxx_deps
IF NOT EXIST %APR% call :remove_lib log4cxx && goto bzip_deps
IF NOT EXIST %APRUTIL% call :remove_lib log4cxx && goto bzip_deps
goto bzip_deps
:end_log4cxx_deps

echo | set /p="Installing log4cxx... "<nul

IF EXIST %LOG4CXX_LIBRARY% call :skip_build && goto bzip 

call :append_log_begin log4cxx

:begin_log4cxx

cd %LOG4CXX_DIR%\projects >nul 2>nul

( msbuild /m /p:Configuration=Release /t:clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog log4cxx  "clean release" && goto bzip

( msbuild /m /t:clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog log4cxx  "clean debug" && goto bzip

( msbuild /m /p:Configuration=Release >>%BUILD_LOG% 2>nul ) || call :buildFailLog log4cxx "build release" && goto bzip

( msbuild /m >>%BUILD_LOG% 2>nul ) || call :buildFailLog log4cxx "build debug" && goto bzip

IF NOT EXIST %TERRALIB_DEPENDENCIES_DIR%\include\log4cxx mkdir %TERRALIB_DEPENDENCIES_DIR%\include\log4cxx >nul 2>nul

xcopy ..\src\main\include\log4cxx %TERRALIB_DEPENDENCIES_DIR%\include\log4cxx /S /Y >nul 2>nul

xcopy Debug%_X86%\log4cxxd.lib %LIBS_DIR% /Y >nul 2>nul

xcopy Debug%_X86%\log4cxxd.dll %LIBS_DIR% /Y >nul 2>nul

xcopy Release%_X86%\log4cxx.lib %LIBS_DIR% /Y >nul 2>nul

xcopy Release%_X86%\log4cxx.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %APACHE_INSTALL_DIR%\bin\libapr*.dll %LIBS_DIR% /Y >nul 2>nul

call :append_log_end log4cxx

:end_log4cxx

echo done.

cd %ROOT_DIR%
::  ================================

:bzip

:: BZIP2
::  =========================================
set BZIP2_DIR=%ROOT_DIR%\bzip2-1.0.6
set BZIP2_INCLUDE_DIR=%BZIP2_DIR%
set BZIP2_LIBRARY=%BZIP2_DIR%\lib%_X86%\libbz2.lib
set BZIP2D_LIBRARY=%BZIP2_DIR%\lib%_X86%\libbz2d.lib

:: Check dependencies
goto end_bzip_deps
:bzip_deps
goto readline_deps
:end_bzip_deps

echo | set /p="Installing bzip2... "<nul

IF EXIST %BZIP2_LIBRARY% call :skip_build && goto readline 

call :append_log_begin bzip

:begin_bzip

cd %BZIP2_DIR% >nul 2>nul

del *.lib /Q >nul 2>nul

del *.pdb /Q >nul 2>nul

IF NOT EXIST lib%_X86% mkdir lib%_X86% >nul 2>nul

( nmake /f makefile.msc lib >>%BUILD_LOG% 2>nul ) || call :buildFailLog bzip "build release" && goto readline

xcopy *.lib lib%_X86% /Y >nul 2>nul 

( nmake /f makefile.msc clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog bzip  "clean release" && goto readline

( nmake /f makefile.msc DEBUG=1 lib >>%BUILD_LOG% 2>nul ) || call :buildFailLog bzip "build debug" && goto readline

xcopy *.lib lib%_X86% /Y >nul 2>nul 

( nmake /f makefile.msc DEBUG=1 clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog bzip  "clean debug" && goto readline

call :append_log_end bzip

:end_bzip

echo done.

cd %ROOT_DIR%
::  ================================

:readline

IF NOT DEFINED TERRALIB_X64 (
  set "_x86_=x86" 
) ELSE set "_x86_=x64"

:: ReadLine
set READLINE_DIR=%ROOT_DIR%\readline-32
set READLINE_INCLUDE_DIR=%READLINE_DIR%\readline-32\src
set READLINE_LIBRARY=%READLINE_DIR%\Release\%_x86_%\readline-32.lib
set READLINED_LIBRARY=%READLINE_DIR%\Debug\%_x86_%\readline-32d.lib
set READLINE_LIBRARIES=debug;%READLINED_LIBRARY%;optimized;%READLINE_LIBRARY%

:: Check dependencies
goto end_readline_deps
:readline_deps
goto pcre_deps
:end_readline_deps

echo | set /p="Installing readline... "<nul

IF EXIST %READLINE% call :skip_build && goto pcre 

call :append_log_begin readline

:begin_readline

cd %READLINE_DIR% >nul 2>nul

( msbuild /m /p:Configuration=Release /t:clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog readline  "clean release" && goto pcre

( msbuild /m /t:clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog readline  "clean debug" && goto pcre

( msbuild /m /p:Configuration=Release >>%BUILD_LOG% 2>nul ) || call :buildFailLog readline "build release" && goto pcre

( msbuild /m >>%BUILD_LOG% 2>nul ) || call :buildFailLog readline "build debug" && goto pcre

xcopy %READLINE_DIR%\Release\%_x86_%\*.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %READLINE_DIR%\Debug\%_x86_%\*.dll %LIBS_DIR% /Y >nul 2>nul

call :append_log_end readline

:end_readline

echo done.

cd %ROOT_DIR%
::  ========

:pcre

::  ====
:: PCRE
set PCRE_DIR=%ROOT_DIR%\pcre-8.37
set "PCRE_INCLUDE_DIR= /I%PCRE_DIR% /I%PCRE_DIR%\sljit /I%PCRE_DIR%\build%_X86%"
set PCRE_LIBRARY=%PCRE_DIR%\build%_X86%\Release\pcre.lib
set PCRED_LIBRARY=%PCRE_DIR%\build%_X86%\Debug\pcred.lib
set PCRE_LIBRARIES=debug;%PCRED_LIBRARY%;optimized;%PCRE_LIBRARY%

:: Check dependencies
goto end_pcre_deps
:pcre_deps
IF NOT EXIST %BZIP% call :remove_lib pcre && goto freexl_deps
IF NOT EXIST %ZLIB% call :remove_lib pcre && goto freexl_deps
IF NOT EXIST %READLINE% call :remove_lib pcre && goto freexl_deps
goto freexl_deps
:end_pcre_deps

echo | set /p="Installing pcre... "<nul

IF EXIST %PCRE% call :skip_build && goto freexl 

call :append_log_begin pcre

:begin_pcre

cd %PCRE_DIR% >nul 2>nul

del build%_X86% /S /Q >nul 2>nul

mkdir build%_X86% >nul 2>nul

cd build%_X86% >nul 2>nul

( cmake -G %_CMAKE_GENERATOR% ^
-DBZIP2_INCLUDE_DIR="%BZIP2_INCLUDE_DIR%" ^
-DBZIP2_LIBRARY_RELEASE="%BZIP2_LIBRARY%" ^
-DBZIP2_LIBRARY_DEBUG="%BZIP2D_LIBRARY%" ^
-DZLIB_INCLUDE_DIR="%ZL_INCLUDE_DIR%" ^
-DZLIB_LIBRARY:STRING="%ZL_LIBRARIES%" ^
-DREADLINE_INCLUDE_DIR="%READLINE_INCLUDE_DIR%" ^
-DREADLINE_LIBRARY:STRING="%READLINE_LIBRARIES%" ^
-DBUILD_SHARED_LIBS=ON %PCRE_DIR% >>%CONFIG_LOG% 2>nul ) || call :buildFailLog pcre "configuring" && goto freexl 

( msbuild /m /p:Configuration=Release pcre.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog pcre "build release" && goto freexl

( msbuild /m pcre.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog pcre "build debug" && goto freexl

xcopy %PCRE_DIR%\build%_X86%\Release\pcre.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %PCRE_DIR%\build%_X86%\Debug\pcred.dll %LIBS_DIR% /Y >nul 2>nul

call :append_log_end pcre

:end_pcre

echo done.

cd %ROOT_DIR%
::  ========


:freexl

::  FreeXL
set FREEXL_DIR=%ROOT_DIR%\freexl-1.0.1
set FREEXL_INCLUDE_DIR=%FREEXL_DIR%\headers
set FREEXL_LIBRARY=%FREEXL_DIR%\lib%_X86%\freexl.lib
set FREEXLD_LIBRARY=%FREEXL_DIR%\lib%_X86%\freexld.lib
set FREEXL_LIBRARIES=debug;%FREEXLD_LIBRARY%;optimized;%FREEXL_LIBRARY%

:: Check dependencies
goto end_freexl_deps
:freexl_deps
IF NOT EXIST %ICONV% call :remove_lib freexl && goto proj_deps
goto proj_deps
:end_freexl_deps

echo | set /p="Installing freexl... "<nul

IF EXIST %FREEXL% call :skip_build && goto proj 

call :append_log_begin freexl

:begin_freexl

cd %FREEXL_DIR% >nul 2>nul

del lib%_X86% /S /Q >nul 2>nul

mkdir lib%_X86% >nul 2>nul

del *.ilk /S /Q >nul 2>nul

( nmake /f makefile.vc >>%BUILD_LOG% 2>nul ) || call :buildFailLog freexl "build release" && goto proj

xcopy *.lib lib%_X86% /Y >nul 2>nul

xcopy *.dll lib%_X86% /Y >nul 2>nul

( nmake /f makefile.vc clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog freexl  "clean release" && goto proj

( nmake /f makefile.vc DEBUG=1 >>%BUILD_LOG% 2>nul ) || call :buildFailLog freexl "build debug" && goto proj

xcopy *.lib lib%_X86% /Y >nul 2>nul

xcopy *.dll lib%_X86% /Y >nul 2>nul

xcopy lib%_X86%\*.dll %LIBS_DIR% /Y >nul 2>nul

( nmake /f makefile.vc DEBUG=1 clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog freexl  "clean debug" && goto proj

call :append_log_end freexl

:end_freexl

echo done.

cd %ROOT_DIR%
::  ========

:proj

::  Proj4 version 4.9.1 
set PJ_DIR=%ROOT_DIR%\proj-4.9.1
set PJ_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set PJ_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\proj_i.lib
set PJD_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\proj_id.lib
set PJ_LIBRARIES=debug;%PJD_LIBRARY%;optimized;%PJ_LIBRARY%

:: Check dependencies
goto end_proj_deps
:proj_deps
goto png_deps
:end_proj_deps

echo | set /p="Installing proj... "<nul

IF EXIST %PROJ% call :skip_build && goto png 

call :append_log_begin proj

:begin_proj

cd %PJ_DIR% >nul 2>nul

del *.ilk /S /Q >nul 2>nul

( nmake /f makefile.vc install-all INSTDIR=%TERRALIB_DEPENDENCIES_DIR% >>%BUILD_LOG% 2>nul ) || call :buildFailLog proj "build release" && goto png

( nmake /f makefile.vc clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog proj  "clean release" && goto png

( nmake /f makefile.vc DEBUG=1 >>%BUILD_LOG% 2>nul ) || call :buildFailLog proj "build debug" && goto png

xcopy src\*d.lib %LIBS_DIR% /Y >nul 2>nul

xcopy src\*d.dll %LIBS_DIR% /Y >nul 2>nul

( nmake /f makefile.vc DEBUG=1 clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog proj  "clean debug" && goto png

call :append_log_end proj

:end_proj

echo done.

cd %ROOT_DIR%
::  ====

:png

::  LibPNG 
set PNG_DIR=%ROOT_DIR%\libpng-1.5.17
set PNG_INCLUDE_DIR=%PNG_DIR%\deploy%_X86%\include
set PNG_LIBRARY=%PNG_DIR%\deploy%_X86%\lib\libpng15.lib
set PNGD_LIBRARY=%PNG_DIR%\deploy%_X86%\lib\libpng15d.lib

:: Check dependencies
goto end_png_deps
:png_deps
IF NOT EXIST %ZLIB% call :remove_lib png && goto geos_deps
goto geos_deps
:end_png_deps

echo | set /p="Installing libpng... "<nul

IF EXIST %PNG% call :skip_build && goto geos 

call :append_log_begin png

:begin_png

cd %PNG_DIR% >nul 2>nul

del deploy%_X86% /S /Q >nul 2>nul

del build%_X86% /S /Q >nul 2>nul

mkdir build%_X86% >nul 2>nul

cd build%_X86% >nul 2>nul

( cmake -G %_CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX=%PNG_DIR%\deploy%_X86% ^
-DPNG_STATIC=OFF ^
-DPNG_TESTS=OFF ^
-DZLIB_INCLUDE_DIR:STRING="%ZL_INCLUDE_DIR%" ^
-DZLIB_LIBRARY:STRING="%ZL_LIBRARIES%" %PNG_DIR% >>%CONFIG_LOG% 2>nul ) || call :buildFailLog png "configuring" && goto geos

( msbuild /m /p:Configuration=Release INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog png "build release" && goto geos

( msbuild /m INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog png "build debug" && goto geos

xcopy %PNG_DIR%\deploy%_X86%\bin\*.dll %LIBS_DIR% /Y >nul 2>nul

call :append_log_end png

:end_png

echo done.

cd %ROOT_DIR%
:: ====

:geos

::  GEOS 
set G_DIR=%CD%\libgeos-3.5.0
set GEOS_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set GEOS_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\geos_i.lib
set GEOSD_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\geos_id.lib
set GEOSC_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\geos_c_i.lib
set GEOSCD_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\geos_c_id.lib

:: Check dependencies
goto end_geos_deps
:geos_deps
goto jpeg_deps
:end_geos_deps

echo | set /p="Installing geos... "<nul

IF EXIST %GEOS% call :skip_build && goto jpeg 

call :append_log_begin geos

:begin_geos

cd %G_DIR% >nul 2>nul

IF NOT EXIST capi\geos_c.h autogen.bat >nul 2>nul

IF NOT EXIST geos_svn_revision.h echo #define GEOS_SVN_REVISION "1.2.1" > geos_svn_revision.h 2>nul

IF DEFINED TERRALIB_X64 ( 
  set "_win64=YES"
) ELSE set "_win64="

( nmake /f makefile.vc WIN64=%_win64% >>%BUILD_LOG% 2>nul ) || call :buildFailLog geos "build release" && goto jpeg

del lib%_X86% /S /Q >nul 2>nul 

mkdir lib%_X86% >nul 2>nul

xcopy src\*.lib lib%_X86% /Y >nul 2>nul 

xcopy src\*.dll lib%_X86% /Y >nul 2>nul 

::  Clean  
( nmake /f makefile.vc WIN64=%_win64% clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog geos  "clean release" && goto jpeg

del capi\*.obj /Q >nul 2>nul

( nmake /f makefile.vc BUILD_DEBUG=YES WIN64=%_win64% >>%BUILD_LOG% 2>nul ) || call :buildFailLog geos "build debug" && goto jpeg

xcopy src\*.dll .\lib%_X86% /Y >nul 2>nul

xcopy src\*.lib .\lib%_X86% /Y >nul 2>nul

( nmake /f makefile.vc BUILD_DEBUG=YES WIN64=%_win64% clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog geos  "clean debug" && goto jpeg

del capi\*.obj /Q >nul 2>nul

::  INSTALL
IF NOT EXIST %GEOS_INCLUDE_DIR%\geos mkdir %GEOS_INCLUDE_DIR%\geos  

xcopy include\geos\*.h %GEOS_INCLUDE_DIR%\geos /S /Y >nul 2>nul  

xcopy capi\geos_c.h %GEOS_INCLUDE_DIR% /Y >nul 2>nul  

xcopy include\geos.h %GEOS_INCLUDE_DIR% /Y >nul 2>nul  

xcopy lib%_X86%\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib /Y >nul 2>nul  

xcopy lib%_x86%\*i.lib %TERRALIB_DEPENDENCIES_DIR%\lib /Y >nul 2>nul  

xcopy lib%_x86%\*id.lib %TERRALIB_DEPENDENCIES_DIR%\lib /Y >nul 2>nul  

call :append_log_end geos

:end_geos

echo done.

cd %ROOT_DIR%
::  ====

:jpeg

:: JPEG
set JPG_DIR=%ROOT_DIR%\jpeg-9a
set JPG_INCLUDE_DIR=%JPG_DIR%
set JPG_LIBRARY=%JPG_DIR%\build%_X86%\libjpeg.lib
set JPGD_LIBRARY=%JPG_DIR%\build%_X86%\libjpegd.lib

:: Check dependencies
goto end_jpeg_deps
:jpeg_deps
goto hdf4_deps
:end_jpeg_deps

echo | set /p="Installing jpeg... "<nul

IF EXIST %JPEG% call :skip_build && goto hdf4 

call :append_log_begin jpeg

:begin_jpeg

cd %JPG_DIR% >nul 2>nul

del build%_X86% /S /Q >nul 2>nul

mkdir build%_X86% >nul 2>nul

( nmake /f makefile.vc libjpeg.lib >>%BUILD_LOG% 2>nul ) || call :buildFailLog jpeg "build release" && goto hdf4

xcopy *.lib build%_X86% /Y >nul 2>nul

( nmake /f makefile.vc clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog jpeg  "clean release" && goto hdf4

( nmake /f makefile.vc DEBUG=1 libjpeg.lib >>%BUILD_LOG% 2>nul ) || call :buildFailLog jpeg "build debug" && goto hdf4

xcopy *.lib build%_X86% /Y >nul 2>nul

( nmake /f makefile.vc DEBUG=1 clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog jpeg  "clean debug" && goto hdf4

call :append_log_end jpeg

:end_jpeg

echo done.

cd %ROOT_DIR%
::  ====

:hdf4

:: HDF4 version 4.2.9
set HDF4C_DIR=%ROOT_DIR%\hdf-4.2.9
set HDF4_INCLUDE_DIR=%HDF4C_DIR%\binaries%_X86%\include
set HDF4_LIBRARY=%HDF4C_DIR%\binaries%_X86%\lib\hdfdll.lib
set HDF4D_LIBRARY=%HDF4C_DIR%\binaries%_X86%\lib\hdfddll.lib
set MFHDF_LIBRARY=%HDF4C_DIR%\binaries%_X86%\lib\mfhdfdll.lib
set MFHDFD_LIBRARY=%HDF4C_DIR%\binaries%_X86%\lib\mfhdfddll.lib

:: Check dependencies
goto end_hdf4_deps
:hdf4_deps
IF NOT EXIST %JPEG% call :remove_lib hdf4 && goto tiff_deps
IF NOT EXIST %ZLIB% call :remove_lib hdf4 && goto tiff_deps
goto tiff_deps
:end_hdf4_deps

echo | set /p="Installing hdf4... "<nul

IF EXIST %HDF4% call :skip_build && goto tiff 

call :append_log_begin hdf4

:begin_hdf4

cd %HDF4C_DIR% >nul 2>nul

del binaries%_X86% /S /Q >nul 2>nul

del build%_X86% /S /Q >nul 2>nul

mkdir build%_X86% >nul 2>nul

cd build%_X86% >nul 2>nul

( cmake -G %_CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX=%HDF4C_DIR%\binaries%_X86% ^
-DCMAKE_PREFIX_PATH="%JPG_DIR%;%ZL_DIR%" ^
-DBUILD_SHARED_LIBS=ON ^
-DHDF4_DISABLE_COMPILER_WARNINGS=ON ^
-DHDF4_ENABLE_COVERAGE=ON ^
-DHDF4_ENABLE_NETCDF=OFF ^
-DHDF4_BUILD_FORTRAN=OFF %HDF4C_DIR% >>%CONFIG_LOG% 2>nul ) || call :buildFailLog hdf4 "configuring" && goto tiff

( msbuild /m /p:Configuration=Release INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLoghdf4 "build release" && goto tiff

( msbuild /m INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLoghdf4 "build debug" && goto tiff

xcopy %HDF4C_DIR%\binaries%_X86%\bin\hdf* %LIBS_DIR% /Y >nul 2>nul

xcopy %HDF4C_DIR%\binaries%_X86%\bin\mfhdf* %LIBS_DIR% /Y >nul 2>nul

xcopy %HDF4C_DIR%\binaries%_X86%\bin\xdr* %LIBS_DIR% /Y >nul 2>nul

echo done.

call :append_log_end hdf4

:end_hdf4

cd %ROOT_DIR%
:: ====

:tiff

::  TIFF 
set TIFF_DIR=%ROOT_DIR%\tiff-4.0.3
set TIFF_INCLUDE_DIR=%TIFF_DIR%\libtiff
set TIFF_LIBRARY=%TIFF_DIR%\libtiff\lib%_X86%\libtiff_i.lib
set TIFFD_LIBRARY=%TIFF_DIR%\libtiff\lib%_X86%\libtiff_id.lib

:: Check dependencies
goto end_tiff_deps
:tiff_deps
IF NOT EXIST %JPEG% call :remove_lib tiff && goto geotiff_deps
IF NOT EXIST %ZLIB% call :remove_lib tiff && goto geotiff_deps
goto geotiff_deps
:end_tiff_deps

echo | set /p="Installing tiff... "<nul

IF EXIST %TIFF% call :skip_build && goto geotiff 

call :append_log_begin tiff

:begin_tiff

cd %TIFF_DIR%\libtiff >nul 2>nul

del lib%_X86% /S /Q >nul 2>nul

del *.exp /S /Q >nul 2>nul

del *.ilk /S /Q >nul 2>nul

mkdir lib%_X86% >nul 2>nul

( nmake /f Makefile.vc >>%BUILD_LOG% 2>nul ) || call :buildFailLog tiff "build release" && goto geotiff

xcopy *i.lib lib%_X86% /Y >nul 2>nul

xcopy *.dll lib%_X86% /Y >nul 2>nul

( nmake /f Makefile.vc clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog tiff  "clean release" && goto geotiff

( nmake /f Makefile.vc DEBUG=1 >>%BUILD_LOG% 2>nul ) || call :buildFailLog tiff "build debug" && goto geotiff

xcopy *id.lib lib%_X86% /Y >nul 2>nul

xcopy *.dll lib%_X86% /Y >nul 2>nul

( nmake /f Makefile.vc DEBUG=1 clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog tiff  "clean debug" && goto geotiff

xcopy lib%_X86%\*.dll %LIBS_DIR% /Y >nul 2>nul

copy tif_config.vc.h tif_config.h /Y >nul 2>nul 

copy tiffconf.vc.h tiffconf.h /Y >nul 2>nul 

echo done.

call :append_log_end tiff

:end_tiff

cd %ROOT_DIR%
:: ====

:geotiff

::  GEOTIFF 
set GTIFF_DIR=%ROOT_DIR%\libgeotiff-1.4.0
set GTIFF_INCLUDE_DIR= /I%GTIFF_DIR% /I%GTIFF_DIR%/libxtiff
set GTIFF_LIBRARY=%GTIFF_DIR%\lib%_X86%\libgeotiff_i.lib
set GTIFFD_LIBRARY=%GTIFF_DIR%\lib%_X86%\libgeotiff_id.lib

:: Check dependencies
goto end_geotiff_deps
:geotiff_deps
IF NOT EXIST %PROJ% call :remove_lib geotiff && goto curl_deps
IF NOT EXIST %TIFF% call :remove_lib geotiff && goto curl_deps
goto curl_deps
:end_geotiff_deps

echo | set /p="Installing geotiff... "<nul

IF EXIST %GEOTIFF% call :skip_build && goto curl 

call :append_log_begin geotiff

:begin_geotiff

cd %GTIFF_DIR% >nul 2>nul

del lib%_X86% /S /Q >nul 2>nul

mkdir lib%_X86% >nul 2>nul

( nmake /f makefile.vc libgeotiff.dll >>%BUILD_LOG% 2>nul ) || call :buildFailLog geotiff "build release" && goto curl

xcopy *i.lib lib%_X86% /Y >nul 2>nul

xcopy *.dll lib%_X86% /Y >nul 2>nul

( nmake /f makefile.vc clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog geotiff  "clean release" && goto curl

( nmake /f makefile.vc DEBUG=1 libgeotiffd.dll >>%BUILD_LOG% 2>nul ) || call :buildFailLog geotiff "build debug" && goto curl

xcopy *id.lib lib%_X86% /Y >nul 2>nul

xcopy *.dll lib%_X86% /Y >nul 2>nul

xcopy lib%_X86%\*.dll %LIBS_DIR% /Y >nul 2>nul

( nmake /f makefile.vc DEBUG=1 clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog geotiff  "clean debug" && goto curl

echo done.

call :append_log_end geotiff

:end_geotiff

cd %ROOT_DIR%
:: ====

:curl

set "_bits=i386"

IF DEFINED TERRALIB_X64 set "_bits=x64"

::  CURL 
set CURL_DIR=%ROOT_DIR%\curl-7.42.1
set CURL_INCLUDE_DIR=%CURL_DIR%\binaries%_X86%\include
set CURL_LIBRARY=%CURL_DIR%\binaries%_X86%\lib\libcurl.lib
set CURLD_LIBRARY=%CURL_DIR%\binaries%_X86%\lib\libcurld.lib

:: Check dependencies
goto end_curl_deps
:curl_deps
IF NOT EXIST %ZLIB% call :remove_lib curl && goto icu_deps
IF NOT EXIST %SSL% call :remove_lib curl && goto icu_deps
goto icu_deps
:end_curl_deps

echo | set /p="Installing curl... "<nul

IF EXIST %CURL% call :skip_build && goto icu 

call :append_log_begin curl

:begin_curl

del %CURL_DIR%\builds /S /Q >nul 2>nul

del %CURL_DIR%\binaries%_X86% /S /Q >nul 2>nul

mkdir %CURL_DIR%\binaries%_X86%\lib >nul 2>nul

mkdir %CURL_DIR%\binaries%_X86%\include >nul 2>nul

cd %CURL_DIR%\winbuild >nul 2>nul

( nmake /f Makefile.vc mode=dll VC=12 WITH_SSL=dll WITH_ZLIB=dll DEBUG=no MACHINE=%_bits% >>%BUILD_LOG% 2>nul ) || call :buildFailLog curl "build release" && goto icu

( nmake /f Makefile.vc mode=dll VC=12 WITH_SSL=dll WITH_ZLIB=dll DEBUG=yes MACHINE=%_bits% >>%BUILD_LOG% 2>nul ) || call :buildFailLog curl "build debug" && goto icu

IF NOT DEFINED _bits set "_bits=x86"

xcopy %CURL_DIR%\builds\libcurl-vc12-%_bits%-debug-dll-ssl-dll-zlib-dll-ipv6-sspi\bin\*.dll %CURL_DIR%\binaries%_X86%\lib /Y >nul 2>nul 

xcopy %CURL_DIR%\builds\libcurl-vc12-%_bits%-debug-dll-ssl-dll-zlib-dll-ipv6-sspi\lib\*.lib %CURL_DIR%\binaries%_X86%\lib /Y >nul 2>nul 

xcopy %CURL_DIR%\builds\libcurl-vc12-%_bits%-release-dll-ssl-dll-zlib-dll-ipv6-sspi\bin\*.dll %CURL_DIR%\binaries%_X86%\lib /Y >nul 2>nul 

xcopy %CURL_DIR%\builds\libcurl-vc12-%_bits%-release-dll-ssl-dll-zlib-dll-ipv6-sspi\bin\*.exe %CURL_DIR%\binaries%_X86%\lib /Y >nul 2>nul 

xcopy %CURL_DIR%\builds\libcurl-vc12-%_bits%-release-dll-ssl-dll-zlib-dll-ipv6-sspi\lib\*.lib %CURL_DIR%\binaries%_X86%\lib /Y >nul 2>nul 

xcopy %CURL_DIR%\builds\libcurl-vc12-%_bits%-release-dll-ssl-dll-zlib-dll-ipv6-sspi\include %CURL_DIR%\binaries%_X86%\include /Y /S >nul 2>nul 

xcopy  %CURL_DIR%\binaries%_X86%\lib\*.dll %LIBS_DIR% /Y >nul 2>nul

echo done.

call :append_log_end curl

:end_curl

cd %ROOT_DIR%
:: ====

:icu

set "_libF="

IF DEFINED TERRALIB_X64 set "_libF=64"

::  ICU 
set ICU_DIR=%ROOT_DIR%\icu
set ICU_INCLUDE_DIR=%ICU_DIR%\include
set ICU_LIBRARY=%ICU_DIR%\lib%_libF%\icuuc.lib
set ICUD_LIBRARY=%ICU_DIR%\lib%_libF%\icuucd.lib
set ICUDATA_LIBRARY=%ICU_DIR%\lib%_libF%\icudt.lib
set ICUDATAD_LIBRARY=%ICU_DIR%\lib%_libF%\icudtd.lib
::set ICUIN_LIBRARY=%ICU_DIR%\lib%_libF%\icuin52.lib
::set ICUIND_LIBRARY=%ICU_DIR%\lib%_libF%\icuin52d.lib

:: Check dependencies
goto end_icu_deps
:icu_deps
goto xerces_deps
:end_icu_deps

echo | set /p="Installing icu... "<nul

IF EXIST %ICU% call :skip_build && goto xerces 

call :append_log_begin icu

:begin_icu

set ICUROOT=%ICU_DIR%

cd %ICU_DIR%\source\allinone >nul 2>nul

( msbuild /m /t:clean /p:Configuration=Release allinone.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog icu  "clean release" && goto xerces

( msbuild /m /t:clean allinone.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog icu  "clean debug" && goto xerces

( msbuild /m /t:makedata /p:Configuration=Release allinone.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog icu "build release" && goto xerces

( msbuild /m /t:common allinone.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog icu "build debug" && goto xerces

xcopy %ICU_DIR%\bin%_libF%\icuuc52.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %ICU_DIR%\bin%_libF%\icuuc52d.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %ICU_DIR%\bin%_libF%\icudt52.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %ICU_DIR%\bin%_libF%\icudt52d.dll %LIBS_DIR% /Y >nul 2>nul

echo done.

call :append_log_end icu

:end_icu

cd %ROOT_DIR%
:: ====

:xerces

set "_libF=x86"

IF DEFINED TERRALIB_X64 set "_libF=x64"

::  XERCES 
set XERCES_DIR=%TERRALIB_DEPENDENCIES_DIR%
set XERCES_INCLUDE_DIR=%XERCES_DIR%\include
set XERCES_LIBRARY=%LIBS_DIR%\xerces-c_3.lib
set XERCESD_LIBRARY=%LIBS_DIR%\xerces-c_3D.lib		
set XERCESCROOT=%ROOT_DIR%\xerces-c-3.1.1
set XERCES_ICU_LIBRARY=%XERCESCROOT%\src\xercesc\util\MsgLoaders\ICU\resources\%_libF%\xercesc_messages_3_1.lib

:: Check dependencies
goto end_xerces_deps
:xerces_deps
IF NOT EXIST %ICU% call :remove_lib xerces && goto xml2_deps
goto xml2_deps
:end_xerces_deps

echo | set /p="Installing xerces... "<nul

IF EXIST %XERCES% call :skip_build && goto xml2 

call :append_log_begin xerces

:begin_xerces

cd %XERCESCROOT%\src\xercesc\util\MsgLoaders\ICU\resources >nul 2>nul

:: Generating message files
genrb root.txt 2> %XERCESCROOT%\..\build.log >nul 2>nul

pkgdata --name xercesc_messages_3_1 --mode dll -d . -e xercesc_messages_3_1 res-file-list.txt >nul 2>nul

IF NOT EXIST %_libF% mkdir %_libF% >nul 2>nul

xcopy *.dll %_libF% /Y >nul 2>nul 

xcopy *.lib  %_libF% /Y >nul 2>nul 

del *.dll *.lib *.exp *.obj /Q >nul 2>nul 

cd %XERCESCROOT%\projects\Win32\VC12\xerces-all >nul 2>nul  

( msbuild /m /t:XercesLib /t:clean /p:Configuration="ICU Release" xerces-all.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog xerces  "clean release" && goto xml2 

( msbuild /m /t:XercesLib /t:clean /p:Configuration="ICU Debug" xerces-all.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog xerces  "clean debug" && goto xml2

( msbuild /m /t:XercesLib /p:Configuration="ICU Release" xerces-all.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog xerces "build release" && goto xml2

( msbuild /m /t:XercesLib /p:Configuration="ICU Debug" xerces-all.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog xerces "build debug" && goto xml2 

:: Install  
cd %XERCESCROOT% >nul 2>nul

IF NOT EXIST %TERRALIB_DEPENDENCIES_DIR%\include\xercesc mkdir %TERRALIB_DEPENDENCIES_DIR%\include\xercesc >nul 2>nul 

xcopy %XERCESCROOT%\Build\%_libF%\*.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %XERCESCROOT%\Build\%_libF%\*.lib %LIBS_DIR% /Y >nul 2>nul

xcopy %XERCESCROOT%\src\xercesc\util\MsgLoaders\ICU\resources\%_libF%\*.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %XERCESCROOT%\src\xercesc\*.h* %TERRALIB_DEPENDENCIES_DIR%\include\xercesc /S /Y >nul 2>nul

xcopy %XERCESCROOT%\src\xercesc\util\*.c %TERRALIB_DEPENDENCIES_DIR%\include\xercesc\util /S /Y >nul 2>nul

echo done.

call :append_log_end xerces

:end_xerces

cd %ROOT_DIR%
:: ====

:xml2

::  LibXML2 
set XML2_DIR=%ROOT_DIR%\libxml2-2.9.1
set XML2_INCLUDE_DIR=%XML2_DIR%\include
set XML2_LIBRARY=%XML2_DIR%\win32\bin%_X86%.msvc\libxml2.lib
set XML2D_LIBRARY=%XML2_DIR%\win32\bin%_X86%.dbg.msvc\libxml2d.lib

:: Check dependencies
goto end_xml2_deps
:xml2_deps
IF NOT EXIST %ICONV% call :remove_lib xml2 && goto libboost_deps
IF NOT EXIST %ICU% call :remove_lib xml2 && goto libboost_deps
IF NOT EXIST %ZLIB% call :remove_lib xml2 && goto libboost_deps
goto libboost_deps
:end_xml2_deps

echo | set /p="Installing xml2... "<nul

IF EXIST %XML2% call :skip_build && goto libboost 

call :append_log_begin xml2

:begin_xml2

set "_bits=_x86"
IF DEFINED TERRALIB_X64 ( 
  set "_bits=_x64"
  set "_win64=WIN64=1"
)

cd %XML2_DIR%\win32 >nul 2>nul

del bin%_bits%.msvc /S /Q >nul 2>nul 

del bin%_bits%.dbg.msvc /S /Q >nul 2>nul 

del int%_bits%.msvc /S /Q >nul 2>nul 

del int%_bits%.dbg.msvc /S /Q >nul 2>nul 

( nmake /f Makefile.msvc %_win64% libxml >>%BUILD_LOG% 2>nul ) || call :buildFailLog xml2 "build release" && goto libboost

( nmake /f Makefile.msvc %_win64% DEBUG=1 libxml >>%BUILD_LOG% 2>nul ) || call :buildFailLog xml2 "build debug" && goto libboost

xcopy %XML2_DIR%\win32\bin%_bits%.msvc\*.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %XML2_DIR%\win32\bin%_bits%.dbg.msvc\*.dll %LIBS_DIR% /Y >nul 2>nul

echo done.

call :append_log_end xml2

:end_xml2

cd %ROOT_DIR%
:: ====

:libboost

:: BOOST
set B_DIR=%ROOT_DIR%\boost_1_58_0
set BOOST_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include

:: Check dependencies
goto end_libboost_deps
:libboost_deps
IF NOT EXIST %ICONV% call :remove_lib boost && goto pgis_deps
IF NOT EXIST %BZIP% call :remove_lib boost && goto pgis_deps
IF NOT EXIST %ICU% call :remove_lib boost && goto pgis_deps
goto pgis_deps
:end_libboost_deps

echo | set /p="Installing boost... "<nul

IF EXIST %BOOST% call :skip_build && goto minizip 

call :append_log_begin libboost

:begin_libboost

 set "_am=32"
IF DEFINED TERRALIB_X64 set "_am=64"

cd %B_DIR% >nul 2>nul

( call bootstrap.bat vc12 setup --with-chrono,date_time,filesystem,system,thread,timer,locale >>%CONFIG_LOG% 2>nul ) || goto configFail

( b2 -a toolset=msvc-12.0 address-model=%_am% architecture=x86 variant=debug,release link=shared threading=multi runtime-link=shared --prefix=%TERRALIB_DEPENDENCIES_DIR% --with-chrono --with-date_time --with-filesystem --with-system --with-thread --with-timer --with-locale --layout=tagged -sICU_PATH=%ICUROOT% -sICONV_PATH=%TERRALIB_DEPENDENCIES_DIR% -sBZIP2_INCLUDE=%BZIP2_INCLUDE_DIR% -sBZIP2_LIBPATH=%BZIP2D_LIBRARY%\.. install %J4% >>%BUILD_LOG% 2>nul ) || call :buildFailLog libboost "building" && goto minizip

echo done.

call :append_log_end libboost

:end_libboost

cd %ROOT_DIR%
:: ====

:minizip

:: Minizip
::  ====
set MINIZIP_DIR=%ROOT_DIR%\unzip101e
set MZ_INCLUDE_DIR=%MINIZIP_DIR%
set MINIZIP_LIBRARY=%MINIZIP_DIR%\build%_X86%\Release\minizip.lib
set MINIZIPD_LIBRARY=%MINIZIP_DIR%\build%_X86%\Debug\minizipd.lib
set MINIZIP_LIBRARIES="debug;%MINIZIPD_LIBRARY%;optimized;%MINIZIP_LIBRARY%"

:: Check dependencies
goto end_minizip_deps
:minizip_deps
IF NOT EXIST %ZLIB% del %MINIZIP% /Q >nul 2>nul && goto uriparser_deps
goto uriparser_deps
:end_minizip_deps

echo | set /p="Installing minizip... "<nul

IF EXIST %MINIZIP% call :skip_build && goto uriparser

call :append_log_begin minizip

:begin_minizip

cd %MINIZIP_DIR% >nul 2>nul

del build%_X86% /S /Q >nul 2>nul

mkdir build%_X86% >nul 2>nul

cd build%_X86% >nul 2>nul

( cmake -G %_CMAKE_GENERATOR% -DCMAKE_DEBUG_POSTFIX=d ^
-DZLIB_INCLUDE_DIR="%ZL_INCLUDE_DIR%" ^
-DZLIB_LIBRARY:STRING="%ZL_LIBRARIES%" ^
%MINIZIP_DIR% >>%CONFIG_LOG% 2>nul ) || call :buildFailLog minizip "configuring" && goto uriparser

( msbuild /m /p:Configuration=Release minizip.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog minizip "build release" && goto uriparser

( msbuild /m minizip.sln  >>%BUILD_LOG% 2>nul ) || call :buildFailLog minizip "build debug" && goto uriparser

echo done.

call :append_log_end minizip

:end_minizip  

cd %ROOT_DIR%
:: ====

:uriparser

set "_bits="
IF NOT DEFINED TERRALIB_X64 set "_bits=_x86"

::  URIParser 
set URIPARSER_DIR=%ROOT_DIR%\uriparser-0.8.4
set UR_INCLUDE_DIR=%URIPARSER_DIR%\include
set URIPARSER_LIBRARY=%URIPARSER_DIR%\win32\build%_bits%\uriparser.lib
set URIPARSERD_LIBRARY=%URIPARSER_DIR%\win32\build%_bits%\uriparserd.lib
set URIPARSER_LIBRARIES=debug;%URIPARSERD_LIBRARY%;optimized;%URIPARSER_LIBRARY%

:: Check dependencies
goto end_uriparser_deps
:uriparser_deps
goto libkml_deps
:end_uriparser_deps

echo | set /p="Installing uriparser... "<nul

IF EXIST %URIPARSER% call :skip_build && goto libkml 

call :append_log_begin uriparser

:begin_uriparser

cd %URIPARSER_DIR%\win32\Visual_Studio_2005 >nul 2>nul

( msbuild /m /p:Configuration=Release uriparser.vcxproj /t:clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog uriparser "clean release" && goto libkml

( msbuild /m uriparser.vcxproj /t:clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog uriparser "clean debug" && goto libkml

( msbuild /m /p:Configuration=Release uriparser.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog uriparser "build release" && goto libkml

( msbuild /m uriparser.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog uriparser "build debug" && goto libkml

echo done.

call :append_log_end uriparser

:end_uriparser

cd %ROOT_DIR%
:: ====

:libkml

:: LibKML
::  ====
set LIBKML_DIR=%ROOT_DIR%\libkml-master
set LIBKML_INCLUDE_DIR=%LIBKML_DIR%\src
set LIBKML_LIBRARY=%LIBKML_DIR%\build%_X86%\Release\libkml.lib
set LIBKMLD_LIBRARY=%LIBKML_DIR%\build%_X86%\Debug\libkmld.lib

:: Check dependencies
goto end_libkml_deps
:libkml_deps
IF NOT EXIST %BOOST% del %LIBKML% /Q >nul 2>nul && goto pgis_deps
IF NOT EXIST %EXPAT% del %LIBKML% /Q >nul 2>nul && goto pgis_deps
IF NOT EXIST %MINIZIP% del %LIBKML% /Q >nul 2>nul && goto pgis_deps
IF NOT EXIST %URIPARSER% del %LIBKML% /Q >nul 2>nul && goto pgis_deps
IF NOT EXIST %ZLIB% del %LIBKML% /Q >nul 2>nul && goto pgis_deps
goto pgis_deps
:end_libkml_deps

echo | set /p="Installing libkml... "<nul

IF EXIST %LIBKML% call :skip_build && goto pgis

call :append_log_begin libkml

:begin_libkml

cd %LIBKML_DIR% >nul 2>nul

del build%_X86% /S /Q >nul 2>nul

mkdir build%_X86% >nul 2>nul

cd build%_X86% >nul 2>nul

( cmake -G %_CMAKE_GENERATOR% -DCMAKE_DEBUG_POSTFIX=d ^
-DCMAKE_PREFIX_PATH="%TERRALIB_DEPENDENCIES_DIR%;%ZL_DIR%;%EXPAT_DIR%\binaries%_X86%;%MINIZIP_DIR%;%URIPARSER_DIR%" ^
%LIBKML_DIR% >>%CONFIG_LOG% 2>nul ) || call :buildFailLog libkml "configuring" && goto pgis

( msbuild /m /p:Configuration=Release libkml.sln >>%BUILD_LOG% 2>nul ) || call :buildFailLog libkml "build release" && goto pgis

( msbuild /m libkml.sln  >>%BUILD_LOG% 2>nul ) || call :buildFailLog libkml "build debug" && goto pgis

echo done.

call :append_log_end libkml

:end_libkml  

cd %ROOT_DIR%

:: ====

:: libxslt
:: set XSLT_DIR=%CD%\libxslt-1.1.28
:: set XSLT_INCLUDE_DIR=%XSLT_DIR%\libxslt
:: set XSLT_LIBRARY=%XSLT_DIR%\win32\bin.msvc\libxslt.lib
:: set XSLTD_LIBRARY=%XSLT_DIR%\win32\bin.msvc\libxsltd.lib

::IF NOT EXIST %XSLTD_LIBRARY% (
::  echo | set /p="Installing libxslt... "<nul

::  cd %XSLT_DIR%\win32

::  echo libxslt >> %XSLT_DIR%\..\build.log
::  echo --------------------------- >> %XSLT_DIR%\..\build.log

::  nmake /f Makefile.msvc libxslt >nul 2>%XSLT_DIR%\..\build.log

::  nmake /f Makefile.msvc DEBUG=1 libxslt >nul 2>%XSLT_DIR%\..\build.log 

::  echo done.

::  cd %XSLT_DIR%\..
:: )
::  ====

:pgis

:: PostgreSQL version 9.4.1
set PGis_DIR=%ROOT_DIR%\postgresql-9.4.1
set PG_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set PG_LIBRARY=%PGIS%
set PGD_LIBRARY=%LIBS_DIR%\libpqddll.lib

:: Check dependencies
goto end_pgis_deps
:pgis_deps
IF NOT EXIST %SSL% call :remove_lib pgis && goto netcdf_deps
goto netcdf_deps
:end_pgis_deps

echo | set /p="Installing libpq... "<nul

IF EXIST %PGIS% call :skip_build && goto netcdf 

call :append_log_begin pgis

:begin_pgis

cd %PGis_DIR%\src\interfaces\libpq >nul 2>nul

del Release /S /Q >nul 2>nul

del Debug /S /Q >nul 2>nul

set "_cpu=i386"
set "ARC_DIR=x86"

IF DEFINED TERRALIB_X64 (
set "_cpu=AMD64"
set "ARC_DIR=x64"
)


( nmake /f win32.mak CPU="%_cpu%" USE_SSL=1 >>%BUILD_LOG% 2>nul ) || call :buildFailLog pgis "build release" && goto netcdf

( nmake /f win32.mak CPU="%_cpu%" USE_SSL=1 DEBUG=1 >>%BUILD_LOG% 2>nul ) || call :buildFailLog pgis "build debug" && goto netcdf

xcopy libpq-fe.h %TERRALIB_DEPENDENCIES_DIR%\include /Y >nul 2>nul

xcopy %PGis_DIR%\src\include\pg_config_ext.h %TERRALIB_DEPENDENCIES_DIR%\include /Y >nul 2>nul

xcopy %PGis_DIR%\src\include\postgres_ext.h %TERRALIB_DEPENDENCIES_DIR%\include /Y >nul 2>nul

xcopy %ARC_DIR%\Release\libpqdll.lib %LIBS_DIR% /Y >nul 2>nul

xcopy %ARC_DIR%\Release\*.dll %LIBS_DIR% /Y >nul 2>nul

xcopy %ARC_DIR%\Debug\libpqddll.lib %LIBS_DIR% /Y >nul 2>nul

xcopy %ARC_DIR%\Debug\*.dll %LIBS_DIR% /Y >nul 2>nul

echo done.

call :append_log_end pgis

:end_pgis

cd %ROOT_DIR%
:: ====

:netcdf

:: netcdf version 4.3.3
set NETCDF_DIR=%ROOT_DIR%\netcdf-c-4.3.3
set NETCDF_INCLUDE_DIR=%NETCDF_DIR%\binaries%_X86%\include
set NETCDF_LIBRARY=%NETCDF_DIR%\binaries%_X86%\lib\netcdf.lib
set NETCDFD_LIBRARY=%NETCDF_DIR%\binaries%_X86%\lib\netcdfd.lib

:: Check dependencies
goto end_netcdf_deps
:netcdf_deps
IF NOT EXIST %JPEG% call :remove_lib netcdf && goto sqlite_deps
IF NOT EXIST %CURL% call :remove_lib netcdf && goto sqlite_deps
IF NOT EXIST %HDF4% call :remove_lib netcdf && goto sqlite_deps
goto sqlite_deps
:end_netcdf_deps

echo | set /p="Installing netcdf... "<nul

IF EXIST %NETCDF% call :skip_build && goto sqlite 

call :append_log_begin netcdf

:begin_netcdf

cd %NETCDF_DIR% >nul 2>nul

IF EXIST binaries%_X86% RD /S /Q binaries%_X86% >nul 2>nul

IF EXIST build%_X86% RD /S /Q build%_X86% >nul 2>nul

mkdir build%_X86% >nul 2>nul

cd build%_X86% >nul 2>nul

( cmake -G %_CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX=%NETCDF_DIR%\binaries%_X86% ^
-DCMAKE_PREFIX_PATH="%JPG_DIR%;%ZL_DIR%;%ROOT_DIR%\curl-7.42.1\binaries%_x86%;%HDF4C_DIR%/binaries%_x86%" ^
-DCMAKE_DEBUG_POSTFIX=d ^
-DENABLE_DLL=ON ^
-DBUILD_TESTING=OFF ^
-DBUILD_UTILITIES=OFF ^
-DENABLE_TESTS=OFF ^
-DENABLE_HDF4=ON ^
-DUSE_HDF4=ON ^
-DUSE_HDF5=OFF ^
-DENABLE_NETCDF_4=OFF %NETCDF_DIR% >>%CONFIG_LOG% 2>nul ) || call :buildFailLog netcdf "configuring" && goto sqlite

( msbuild /m /p:Configuration=Release INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog netcdf "build release" && goto sqlite

( msbuild /m INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog netcdf "build debug" && goto sqlite

xcopy %NETCDF_DIR%\binaries%_X86%\bin\net* %LIBS_DIR% /Y >nul 2>nul

echo done.

call :append_log_end netcdf

:end_netcdf

cd %ROOT_DIR%
:: ====

:sqlite

:: SQLite
set SQLITE_DIR=%ROOT_DIR%\sqlite-amalgamation-3090100
set SQLITE_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set SQLITE_LIBRARY=%LIBS_DIR%\sqlite3.lib
set SQLITED_LIBRARY=%LIBS_DIR%\sqlite3d.lib

:: Check dependencies
goto end_sqlite_deps
:sqlite_deps
goto spatialite_deps
:end_sqlite_deps

echo | set /p="Installing sqlite... "<nul

IF EXIST %SQLITE% call :skip_build && goto spatialite 

call :append_log_begin sqlite

:begin_sqlite

cd %SQLITE_DIR% >nul 2>nul

cl sqlite3.c /DSQLITE_API=__declspec(dllexport) -link -dll -out:sqlite3.dll >>%BUILD_LOG% 2>nul || call :buildFailLog sqlite "build release" && goto spatialite

cl sqlite3.c /DSQLITE_API=__declspec(dllexport) /DSQLITE_DEBUG -link -dll -out:sqlite3d.dll >>%BUILD_LOG% 2>nul || call :buildFailLog sqlite "build debug" && goto spatialite

xcopy *.h %TERRALIB_DEPENDENCIES_DIR%\include /Y >nul 2>nul

xcopy *.lib %LIBS_DIR% /Y >nul 2>nul

xcopy *.dll %LIBS_DIR% /Y >nul 2>nul

echo done.

call :append_log_end sqlite

:end_sqlite

cd %ROOT_DIR%
:: ====

:spatialite

:: SpatialLite version 4.3.0a
set SPLITE_DIR=%ROOT_DIR%\libspatialite-4.3.0a
set SPLITE_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set SPLITE_LIBRARY=%SPATIALITE%
set SPLITED_LIBRARY=%LIBS_DIR%\spatialite_id.lib

:: Check dependencies
goto end_spatialite_deps
:spatialite_deps
IF NOT EXIST %PROJ% call :remove_lib spatialite && goto gdal_deps
IF NOT EXIST %GEOS% call :remove_lib spatialite && goto gdal_deps
IF NOT EXIST %ICONV% call :remove_lib spatialite && goto gdal_deps
IF NOT EXIST %SQLITE% call :remove_lib spatialite && goto gdal_deps
IF NOT EXIST %FREEXL% call :remove_lib spatialite && goto gdal_deps
IF NOT EXIST %ZLIB% call :remove_lib spatialite && goto gdal_deps
IF NOT EXIST %XML2% call :remove_lib spatialite && goto gdal_deps
goto gdal_deps
:end_spatialite_deps

echo | set /p="Installing spatialite... "<nul

IF EXIST %SPATIALITE% call :skip_build && goto gdal 

call :append_log_begin spatialite

:begin_spatialite

cd %SPLITE_DIR% >nul 2>nul

( nmake /f makefile.vc install >>%BUILD_LOG% 2>nul ) || call :buildFailLog spatialite "build release" && goto gdal

( nmake /f makefile.vc clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog spatialite  "clean release" && goto gdal

( nmake /f makefile.vc DEBUG=1 install >>%BUILD_LOG% 2>nul ) || call :buildFailLog spatialite "build debug" && goto gdal

( nmake /f makefile.vc DEBUG=1 clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog spatialite  "clean debug" && goto gdal

echo done.

call :append_log_end spatialite

:end_spatialite

cd %ROOT_DIR%
:: ====

:gdal

:: GDAL version 2.0.0
set GDAL_DIR=%ROOT_DIR%\gdal-2.0.0
set GDAL_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set GDAL_LIBRARY=%GDAL%
set GDALD_LIBRARY=%LIBS_DIR%\gdal_id.lib

:: Check dependencies
goto end_gdal_deps
:gdal_deps
IF NOT EXIST %XERCES% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %PROJ% call :remove_lib gdal && goto && goto property_browser_deps
IF NOT EXIST %SQLITE% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %SPATIALITE% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %GEOS% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %ICONV% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %EXPAT% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %HDF4% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %NETCDF% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %GEOTIFF% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %TIFF% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %CURL% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %PNG% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %FREEXL% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %XML2% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %PCRE% call :remove_lib gdal && goto property_browser_deps
IF NOT EXIST %LIBKML% call :remove_lib gdal && goto property_browser_deps
goto property_browser_deps
:end_gdal_deps

echo | set /p="Installing gdal... "<nul

IF EXIST %GDAL% call :skip_build && goto property_browser 

call :append_log_begin gdal

:begin_gdal

cd %GDAL_DIR% >nul 2>nul

del *.lib /S /Q >nul 2>nul  

del *.exp /S /Q >nul 2>nul  

IF DEFINED TERRALIB_X64 (
copy nmake.release.opt.in nmake.opt /Y >nul 2>nul

( nmake -f makefile.vc MSVC_VER=1800 WIN64=1 >>%BUILD_LOG% 2>nul ) || call :buildFailLog gdal "build release" && goto property_browser

( nmake -f makefile.vc MSVC_VER=1800 WIN64=1 devinstall >>%BUILD_LOG% 2>nul ) || call :buildFailLog gdal "install" property_browser

( nmake -f makefile.vc MSVC_VER=1800 WIN64=1 clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog gdal  "clean release" && goto property_browser

copy nmake.debug.opt.in nmake.opt /Y >nul 2>nul

( nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 WIN64=1 >>%BUILD_LOG% 2>nul ) || call :buildFailLog gdal "build debug" && goto property_browser

xcopy *.dll %LIBS_DIR% /Y >nul 2>nul

xcopy gdal_id.lib %LIBS_DIR% /Y >nul 2>nul

( nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 WIN64=1 clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog gdal  "clean debug" && goto property_browser
) else (
copy nmake.release.opt.in nmake.opt /Y >nul 2>nul

( nmake -f makefile.vc MSVC_VER=1800 >>%BUILD_LOG% 2>nul ) || call :buildFailLog gdal "build release" && goto property_browser

( nmake -f makefile.vc MSVC_VER=1800 devinstall >>%BUILD_LOG% 2>nul ) || call :buildFailLog gdal "install" property_browser

( nmake -f makefile.vc MSVC_VER=1800 clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog gdal  "clean release" && goto property_browser

copy nmake.debug.opt.in nmake.opt /Y >nul 2>nul

( nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 >>%BUILD_LOG% 2>nul ) || call :buildFailLog gdal "build debug" && goto property_browser

xcopy *.dll %LIBS_DIR% /Y >nul 2>nul

xcopy gdal_id.lib %LIBS_DIR% /Y >nul 2>nul

( nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog gdal  "clean debug" && goto property_browser
)

echo done.

call :append_log_end gdal

:end_gdal

cd %ROOT_DIR%
:: ====

:property_browser

:: QtPropertyBrowser
set QBROWSER_DIR=%ROOT_DIR%\qtpropertybrowser
set QBROWSER_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include\qtpropertybrowser
set QBROWSER_LIBRARY=%PROPERTYBROWSER%
set QBROWSERD_LIBRARY=%LIBS_DIR%\qt_property_browserd.lib

:: Check dependencies
goto end_property_browser_deps
:property_browser_deps
goto qwt_deps
:end_property_browser_deps

echo | set /p="Installing property_browser... "<nul

IF EXIST %PROPERTYBROWSER% call :skip_build && goto qwt 

call :append_log_begin property_browser

:begin_property_browser

cd %QBROWSER_DIR%\buildlib >nul 2>nul

( qmake "TERRALIB_DIR=%TERRALIB_DEPENDENCIES_DIR%" >>%CONFIG_LOG% 2>nul ) || call :buildFailLog property_browser "configuring" && goto qwt

( nmake clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog property_browser "cleaning" && goto qwt

( nmake >>%BUILD_LOG% 2>nul ) || call :buildFailLog property_browser "building" && goto qwt

( nmake install >>%BUILD_LOG% 2>nul ) || call :buildFailLog property_browser "installing" && goto qwt

echo done.

call :append_log_end property_browser

:end_property_browser

cd %ROOT_DIR%
:: ====

:qwt

:: QWT
set QWT_PATH=%ROOT_DIR%\qwt-6.1.2

:: Check dependencies
goto end_qwt_deps
:qwt_deps
goto lua_deps
:end_qwt_deps

echo | set /p="Installing qwt... "<nul

IF EXIST %QWT% call :skip_build && goto lualib 

call :append_log_begin qwt

:begin_qwt

cd %QWT_PATH% >nul 2>nul

( qmake qwt.pro -r "QWT_INSTALL_PREFIX_TARGET=%TERRALIB_DEPENDENCIES_DIR%" >>%CONFIG_LOG% 2>nul ) || call :buildFailLog qwt "configuring" && goto lua 

( nmake clean >>%BUILD_LOG% 2>nul ) || call :buildFailLog  qwt "cleaning" && goto lua

( nmake >>%BUILD_LOG% 2>nul ) || call :buildFailLog qwt "building" && goto lua 

( nmake install >>%BUILD_LOG% 2>nul ) || call :buildFailLog qwt "installing" && goto lua 

echo done.

call :append_log_end qwt

:end_qwt

cd %ROOT_DIR%
:: ====

:lualib

:: Lua version 5.2.2
set LUAC_DIR=%ROOT_DIR%\lua-5.2.2
set LUAC_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include\lua
set LUAC_LIBRARY=%LUA%
set LUACD_LIBRARY=%LIBS_DIR%\luad.lib

:: Check dependencies
goto end_lua_deps
:lua_deps
IF NOT EXIST %READLINE% call :remove_lib lua && goto terralib4_deps
goto terralib4_deps
:end_lua_deps

echo | set /p="Installing lua... "<nul

IF EXIST %LUA% call :skip_build && goto terralib4 

call :append_log_begin lua

:begin_lua

cd %LUAC_DIR% >nul 2>nul

del build%_X86% /S /Q >nul 2>nul 

mkdir build%_X86% >nul 2>nul

IF NOT EXIST %TERRALIB_DEPENDENCIES_DIR%\include\lua mkdir %TERRALIB_DEPENDENCIES_DIR%\include\lua >nul 2>nul

cd build%_X86% >nul 2>nul

( cmake -G %_CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX=%TERRALIB_DEPENDENCIES_DIR% ^
-DCMAKE_DEBUG_POSTFIX=d ^
-DINSTALL_BIN=%LIBS_DIR% ^
-DREADLINE_INCLUDE_DIR=%READLINE_INCLUDE_DIR% ^
-DREADLINE_LIBRARY:STRING="debug;%READLINED_LIBRARY%;optimized;%READLINE_LIBRARY%" %LUAC_DIR% >>%CONFIG_LOG% 2>nul ) || call :buildFailLog lua  "configuring" && goto terralib4

( msbuild /m /p:Configuration=Release INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog lua "build release" && goto terralib4

( msbuild /m INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog lua "build debug" && goto terralib4

xcopy %LUAC_DIR%\src\lua.h %LUAC_INCLUDE_DIR% /Y >nul 2>nul

xcopy %LUAC_DIR%\src\lualib.h %LUAC_INCLUDE_DIR% /Y >nul 2>nul

xcopy %LUAC_DIR%\src\lauxlib.h %LUAC_INCLUDE_DIR% /Y >nul 2>nul

xcopy %LUAC_DIR%\src\lua.hpp %LUAC_INCLUDE_DIR% /Y >nul 2>nul

xcopy %LUAC_DIR%\build\luaconf.h %LUAC_INCLUDE_DIR% /Y >nul 2>nul

echo done.

call :append_log_end lua

:end_lua

cd %ROOT_DIR%
:: ====

:terralib4

:: TerraLib version 4.2.2
set TERRALIB4_DIR=%ROOT_DIR%\terralib4
set TERRALIB4_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set TERRALIB4_LIBRARY=%LIBS_DIR%\terralib.lib
set TERRALIB4D_LIBRARY=%LIBS_DIR%\terralibd.lib

:: Check dependencies
goto end_terralib4_deps
:terralib4_deps
IF NOT EXIST %TIFF% call :remove_lib terralib && goto begin_build
IF NOT EXIST %GEOTIFF% call :remove_lib terralib && goto begin_build
IF NOT EXIST %ZLIB% call :remove_lib terralib && goto begin_build
IF NOT EXIST %JPEG% call :remove_lib terralib && goto begin_build
goto begin_build
:remove_terralib4  
del %LIBS_DIR%\terralib* /Q >nul 2>nul  
goto begin_build
:end_terralib4_deps

echo | set /p="Installing terralib4... "<nul

IF EXIST %TERRALIB4% call :skip_build && goto clean_third_directory

call :append_log_begin terralib4

:begin_terralib4

cd %TERRALIB4_DIR% >nul 2>nul

del building%_X86% /S /Q >nul 2>nul 

mkdir building%_X86% >nul 2>nul

cd building%_X86% >nul 2>nul

xcopy ..\terralib.conf.cmake . /Y >nul 2>nul

set CL=/MP1

( cmake -G %_CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX="%TERRALIB_DEPENDENCIES_DIR%" ^
-DCMAKE_PREFIX_PATH="%TERRALIB_DEPENDENCIES_DIR%" ^
-DTIFF_INCLUDE_DIR:STRING="%TIFF_INCLUDE_DIR%" ^
-DTIFF_LIBRARY:STRING="debug;%TIFFD_LIBRARY%;optimized;%TIFF_LIBRARY%" ^
-DGeoTIFF_INCLUDE_DIR:STRING="%GTIFF_DIR%;%GTIFF_DIR%/libxtiff" ^
-DGeoTIFF_LIBRARY:STRING="debug;%GTIFFD_LIBRARY%;optimized;%GTIFF_LIBRARY%" ^
-DJPEG_INCLUDE_DIR="%JPG_DIR%" ^
-DJPEG_LIBRARY:STRING="debug;%JPGD_LIBRARY%;optimized;%JPG_LIBRARY%" ^
-DZLIB_INCLUDE_DIR:STRING="%ZL_INCLUDE_DIR%" ^
-DZLIB_LIBRARIES:STRING="%ZL_LIBRARIES%" ^
-C terralib.conf.cmake %TERRALIB4_DIR%\build\cmake >>%CONFIG_LOG% 2>nul ) || call :buildFailLog terralib4 "configuring" && goto clean_third_directory

( msbuild /m /p:Configuration=Release INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog terralib4 "build release" && goto clean_third_directory

( msbuild /m INSTALL.vcxproj >>%BUILD_LOG% 2>nul ) || call :buildFailLog terralib4 "build debug" && goto clean_third_directory

echo done.

call :append_log_end terralib4

:end_terralib4
:: ====

:clean_third_directory

cd %TERRALIB_DEPENDENCIES_DIR% >nul 2>nul

RD /S /Q doc >nul 2>nul

RD /S /Q features >nul 2>nul

RD /S /Q plugins >nul 2>nul

del *.exe /S /Q >nul 2>nul

del *.pdb /S /Q >nul 2>nul

IF DEFINED FAIL exit /b 1

cd %ROOT_DIR%
::exit /b 0
