IF NOT EXIST "%TERRALIB_DEPENDENCIES_DIR%" mkdir %TERRALIB_DEPENDENCIES_DIR% 
IF NOT EXIST "%TERRALIB_DEPENDENCIES_DIR%\include" mkdir %TERRALIB_DEPENDENCIES_DIR%\include 
IF NOT EXIST "%TERRALIB_DEPENDENCIES_DIR%\lib" mkdir %TERRALIB_DEPENDENCIES_DIR%\lib 

::  Setting visual studio environment
::  =================================
echo | set /p="Configuring visual studio... "<nul 

call %VCVARS_FILEPATH%\vcvars64.bat 

echo done.
::  ================================

set PATH=%CMAKE_FILEPATH%;%QMAKE_FILEPATH%;%CD%\icu\bin64;%PATH%
set INCLUDE=%WIN32MAK_FILEPATH%;%INCLUDE%

:: Building libraries
::  ==================


:: BOOST
set B_DIR=%CD%\boost_1_58_0

  echo | set /p="Installing boost... "<nul
  
  cd %B_DIR%

  call bootstrap.bat vc12 setup-amd64 >nul 2>nul
  
  b2 -j4 toolset=msvc-12.0 address-model=64 architecture=x86 variant=debug,release link=shared threading=multi runtime-link=shared --prefix=%TERRALIB_DEPENDENCIES_DIR% --with-chrono --with-date_time --with-filesystem --with-system --with-thread --with-timer --layout=tagged install  >nul 2>nul
  
  echo done.

  cd %B_DIR%\..
:: ====

set CL=/MP

:: BZIP2
::  =========================================
set BZIP2_DIR=%CD%\bzip2-1.0.6
set BZIP2_INCLUDE_DIR=%BZIP2_DIR%
set BZIP2_LIBRARY=%BZIP2_DIR%\lib\libbz2.lib
set BZIP2D_LIBRARY=%BZIP2_DIR%\lib\libbz2d.lib

  cd %BZIP2_DIR%

  echo | set /p="Installing bzip2... "<nul

  nmake /f makefile.msc >nul 2>nul
  
  IF NOT EXIST lib mkdir lib

  xcopy *.lib lib >nul 2>nul 

  nmake /f makefile.msc clean >nul 2>nul

  nmake /f makefile.msc DEBUG=1 >nul 2>nul 

  xcopy *.lib lib >nul 2>nul 

  echo done.

  cd ..
::  ================================

:: ZLIB
::  ====
set ZL_DIR=%CD%\zlib-1.2.8
set ZL_INCLUDE_DIR=%ZL_DIR%;%ZL_DIR%\build
set ZL_LIBRARY=%ZL_DIR%\build\Release\zlib.lib
set ZLD_LIBRARY=%ZL_DIR%\build\Debug\zlibd.lib
set ZL_LIBRARIES=debug;%ZLD_LIBRARY%;optimized;%ZL_LIBRARY%

  echo | set /p="Installing zlib... "<nul

  cd %ZL_DIR%

  IF NOT EXIST build mkdir build

  cd build

  cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_INSTALL_PREFIX=%TERRALIB_DEPENDENCIES_DIR% .. >nul 2>nul

  msbuild /m zlib.sln /p:Configuration=Release >nul 2>nul
  
  msbuild /m zlib.sln /p:Configuration=Debug >nul 2>nul
  
  xcopy %ZL_DIR%\build\Release\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy %ZL_DIR%\build\Debug\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

  echo done.

  cd ..\..
::  ================================

:: ReadLine
set READLINE_DIR=%CD%\readline-32
set READLINE_INCLUDE_DIR=%READLINE_DIR%\readline-32\src
set READLINE_LIBRARY=%READLINE_DIR%\x64\Release\readline-32.lib
set READLINED_LIBRARY=%READLINE_DIR%\x64\Debug\readline-32d.lib
set READLINE_LIBRARIES=debug;%READLINED_LIBRARY%;optimized;%READLINE_LIBRARY%

  echo | set /p="Installing libreadline... "<nul
  
  cd %READLINE_DIR%

  msbuild /m /p:Configuration=Debug readline-32.sln >nul 2>nul
  msbuild /m /p:Configuration=Release readline-32.sln >nul 2>nul
  
  xcopy %READLINE_DIR%\x64\Release\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy %READLINE_DIR%\x64\Debug\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  
  echo done.

  cd ..
::  ========

:: EditLine
::  não tem pra windows
::  ========

::  ====
:: PCRE
set PCRE_DIR=%CD%\pcre-8.37
set PCRE_INCLUDE_DIR= /I%PCRE_DIR% /I%PCRE_DIR%\sljit /I%PCRE_DIR%\build
set PCRE_LIBRARY=%PCRE_DIR%\build\Release\pcre.lib
set PCRED_LIBRARY=%PCRE_DIR%\build\Debug\pcred.lib
set PCRE_LIBRARIES=debug;%PCRED_LIBRARY%;optimized;%PCRE_LIBRARY%

  echo | set /p="Installing libpcre... "<nul
  
  cd %PCRE_DIR%

  IF NOT EXIST build mkdir build

  cd build

  cmake -G "Visual Studio 12 2013 Win64" ^
 -D BZIP2_INCLUDE_DIR="%BZIP2_INCLUDE_DIR%"^
 -D BZIP2_LIBRARY_RELEASE="%BZIP2_LIBRARY%"^
 -D BZIP2_LIBRARY_DEBUG="%BZIP2D_LIBRARY%"^
 -D ZLIB_INCLUDE_DIR="%ZL_INCLUDE_DIR%"^
 -D ZLIB_LIBRARY:STRING="%ZL_LIBRARIES%"^
 -D READLINE_INCLUDE_DIR="%READLINE_INCLUDE_DIR%"^
 -D READLINE_LIBRARY:STRING="%READLINE_LIBRARIES%"^
 -D BUILD_SHARED_LIBS=ON .. >nul 2>nul
   
  msbuild /m /p:Configuration=Release pcre.sln >nul 2>nul
  msbuild /m /p:Configuration=Debug pcre.sln >nul 2>nul
::  cmake --build . --target ALL_BUILD --config Release >nul 2>nul
::  cmake --build . --target ALL_BUILD --config Debug >nul 2>nul

  copy %PCRE_DIR%\build\Release\pcre.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  copy %PCRE_DIR%\build\Debug\pcred.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  
  echo done.

  cd %PCRE_DIR%\..
::  ========

::  SWIG
::  não fizemos
::  ========

::  ====
::  Iconv
set ICONV_DIR=%CD%\iconv
set ICONV_INCLUDE_DIR=%ICONV_DIR%\myIconv\include
set ICONV_LIBRARY=%ICONV_DIR%\x64\Release\iconv.lib
set ICONVD_LIBRARY=%ICONV_DIR%\x64\Debug\iconvd.lib
set ICONV_LIBRARIES=debug;%ICONVD_LIBRARY%;optimized;%ICONV_LIBRARY%

  echo | set /p="Installing iconv... "<nul
  
  cd %ICONV_DIR%

  msbuild /m /p:Configuration=Debug myIconv.sln >nul 2>nul
  msbuild /m /p:Configuration=Release myIconv.sln >nul 2>nul
  
  xcopy %ICONV_DIR%\x64\Release\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy %ICONV_DIR%\x64\Debug\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  
  echo done.

  cd ..
::  ========

::  FreeXL
set FREEXL_DIR=%CD%\freexl-1.0.1
set FREEXL_INCLUDE_DIR=%FREEXL_DIR%\headers
set FREEXL_LIBRARY=%FREEXL_DIR%\lib\freexl.lib
set FREEXLD_LIBRARY=%FREEXL_DIR%\lib\freexld.lib
set FREEXL_LIBRARIES=debug;%FREEXLD_LIBRARY%;optimized;%FREEXL_LIBRARY%

  cd %FREEXL_DIR%

  IF NOT EXIST lib mkdir lib

  echo | set /p="Installing freexl... "<nul

  nmake /f makefile.vc >nul 2>nul
  copy freexl.lib lib >nul
  copy freexl.dll lib >nul
  
  nmake /f makefile.vc clean >nul 2>nul

  nmake /f makefile.vc DEBUG=1 >nul 2>nul 
  copy freexld.lib lib >nul
  copy freexld.dll lib >nul
  
  xcopy lib\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

  echo done.

  cd ..
::  ========

::  OOSP-UUID 
::  Não tem build windows
::  ====

::  Proj4 version 4.9.1 
set PJ_DIR=%CD%\proj-4.9.1
set PJ_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set PJ_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\proj_i.lib
set PJD_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\proj_id.lib
set PJ_LIBRARIES=debug;%PJD_LIBRARY%;optimized;%PJ_LIBRARY%

  echo | set /p="Installing proj4... "<nul
  
  cd %PJ_DIR%

  nmake /f makefile.vc install-all INSTDIR=%TERRALIB_DEPENDENCIES_DIR% >nul 2>nul

  nmake /f makefile.vc clean >nul 2>nul

  nmake /f makefile.vc DEBUG=1  >nul 2>nul
  
  xcopy .\src\*d.lib %TERRALIB_DEPENDENCIES_DIR%\lib >nul
  xcopy .\src\*d.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul

  echo done.

  cd ..
::  ====

::  LibPNG 
set PNG_DIR=%CD%\libpng-1.5.17
set PNG_INCLUDE_DIR=%PNG_DIR%\deploy\include
set PNG_LIBRARY=%PNG_DIR%\deploy\lib\libpng15.lib
set PNGD_LIBRARY=%PNG_DIR%\deploy\lib\libpng15d.lib

  echo | set /p="Installing libpng... "<nul
  
  cd %PNG_DIR%

  IF NOT EXIST build mkdir build

  cd build

  cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_INSTALL_PREFIX="%PNG_DIR%\deploy"^
 -DPNG_STATIC=OFF^
 -DPNG_TESTS=OFF^
 -DZLIB_INCLUDE_DIR:STRING="%ZL_INCLUDE_DIR%"^
 -DZLIB_LIBRARY:STRING="%ZL_LIBRARIES%" .. >nul 2>nul
 
  msbuild /m /p:Configuration=Release INSTALL.vcxproj >nul 2>nul
  msbuild /m INSTALL.vcxproj >nul 2>nul
 
 xcopy %PNG_DIR%\deploy\bin\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

 echo done.

 cd %PNG_DIR%\..
:: ====

::  GEOS 
set G_DIR=%CD%\libgeos-3.5.0
set GEOS_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set GEOS_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\geos_i.lib
set GEOSD_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\geos_id.lib
set GEOSC_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\geos_c_i.lib
set GEOSCD_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\geos_c_id.lib

  echo | set /p="Installing geos... "<nul
  
  cd %G_DIR%

  IF NOT EXIST .\capi\geos_c.h autogen.bat >nul 
  
  IF NOT EXIST "geos_svn_revision.h" echo #define GEOS_SVN_REVISION "1.2.1" > geos_svn_revision.h
 
  nmake /f makefile.vc WIN64=YES >nul 2>nul
  
  IF NOT EXIST lib mkdir lib

  copy src\*.lib lib >nul 2>nul 
  copy src\*.dll lib >nul 2>nul

::  Clean  
  nmake /f makefile.vc WIN64=YES clean >nul 2>nul
  del /s capi\*.obj >nul 2>nul

  nmake /f makefile.vc BUILD_DEBUG=YES WIN64=YES >nul 2>nul

  xcopy src\*.dll .\lib >nul 2>nul
  xcopy src\*.lib .\lib >nul 2>nul
  
::  INSTALL
  IF NOT EXIST %GEOS_INCLUDE_DIR%\geos mkdir %GEOS_INCLUDE_DIR%\geos  
  
  xcopy /S include\geos\*.h %GEOS_INCLUDE_DIR%\geos >nul 2>nul  
  copy capi\geos_c.h %GEOS_INCLUDE_DIR% >nul 2>nul  
  copy include\geos.h %GEOS_INCLUDE_DIR% >nul 2>nul  
  xcopy lib\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul  
  xcopy lib\*i.lib %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul  
  xcopy lib\*id.lib %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul  

  echo done.

  cd ..
::  ====

:: JPEG
set JPG_DIR=%CD%\jpeg-9a
set JPG_INCLUDE_DIR=%JPG_DIR%
set JPG_LIBRARY=%JPG_DIR%\build\libjpeg.lib
set JPGD_LIBRARY=%JPG_DIR%\build\libjpegd.lib

echo | set /p="Installing libjpeg... "<nul

cd %JPG_DIR%

IF NOT EXIST .\build mkdir .\build

nmake /f makefile.vc libjpeg.lib >nul 2>nul

xcopy *.lib build >nul 2>nul

nmake /f makefile.vc clean >nul 2>nul

nmake /f makefile.vc DEBUG=1 libjpeg.lib >nul 2>nul

xcopy *.lib build >nul 2>nul

echo done.

cd %JPG_DIR%\..
::  ====


:: HDF4 version 4.2.9
set HDF4C_DIR=%CD%\hdf-4.2.9
set HDF4_INCLUDE_DIR=%HDF4C_DIR%\binaries\include
set HDF4_LIBRARY=%HDF4C_DIR%\binaries\lib\hdfdll.lib
set HDF4D_LIBRARY=%HDF4C_DIR%\binaries\lib\hdfddll.lib
set MFHDF_LIBRARY=%HDF4C_DIR%\binaries\lib\mfhdfdll.lib
set MFHDFD_LIBRARY=%HDF4C_DIR%\binaries\lib\mfhdfddll.lib

  echo | set /p="Installing hdf4... "<nul
  
  cd %HDF4C_DIR%

  IF NOT EXIST build (
    mkdir build
    mkdir build\Release
    mkdir build\Debug
  )

  cd build\Release

  cmake -G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX=%HDF4C_DIR%\binaries^
 -DCMAKE_BUILD_TYPE=Release^
 -DBUILD_SHARED_LIBS=ON^
 -DHDF4_DISABLE_COMPILER_WARNINGS=ON^
 -DHDF4_ENABLE_COVERAGE=ON^
 -DHDF4_ENABLE_NETCDF=ON^
 -DHDF4_BUILD_FORTRAN=OFF^
 -DJPEG_INCLUDE_DIR="%JPG_INCLUDE_DIR%"^
 -DJPEG_LIBRARY="%JPG_LIBRARY%"^
 -DZLIB_INCLUDE_DIR:STRING="%ZL_INCLUDE_DIR%"^
 -DZLIB_LIBRARY="%ZL_LIBRARY%" ..\.. >nul 2>nul
 
 nmake install >nul 2>nul
 
  cd %HDF4C_DIR%\build\Debug

  cmake -G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX=%HDF4C_DIR%\binaries^
 -DCMAKE_BUILD_TYPE=Debug^
 -DBUILD_SHARED_LIBS=ON^
 -DHDF4_DISABLE_COMPILER_WARNINGS=ON^
 -DHDF4_ENABLE_COVERAGE=ON^
 -DHDF4_ENABLE_NETCDF=ON^
 -DHDF4_BUILD_FORTRAN=OFF^
 -DJPEG_INCLUDE_DIR="%JPG_INCLUDE_DIR%"^
 -DJPEG_LIBRARY="%JPGD_LIBRARY%"^
 -DZLIB_INCLUDE_DIR:STRING="%ZL_INCLUDE_DIR%"^
 -DZLIB_LIBRARY="%ZLD_LIBRARY%" ..\.. >nul 2>nul
 
 nmake install >nul 2>nul
  
  xcopy %HDF4C_DIR%\binaries\bin\hdf* %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy %HDF4C_DIR%\binaries\bin\mfhdf* %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy %HDF4C_DIR%\binaries\bin\xdr* %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

  echo done.

  cd %HDF4C_DIR%\..
:: ====


::  TIFF 
set TIFF_DIR=%CD%\tiff-4.0.3
set TIFF_INCLUDE_DIR=%TIFF_DIR%\libtiff
set TIFF_LIBRARY=%TIFF_DIR%\libtiff\lib\libtiff_i.lib
set TIFFD_LIBRARY=%TIFF_DIR%\libtiff\lib\libtiff_id.lib

echo | set /p="Installing libtiff... "<nul

cd %TIFF_DIR%\libtiff

IF NOT EXIST .\lib mkdir .\lib

nmake /f Makefile.vc >nul 2>nul
xcopy *i.lib lib >nul 2>nul
xcopy *.dll lib >nul 2>nul

nmake /f Makefile.vc clean >nul 2>nul

nmake /f Makefile.vc DEBUG=1 >nul 2>nul
xcopy *id.lib lib >nul 2>nul
xcopy *.dll lib >nul 2>nul

xcopy lib\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

echo done.

cd %TIFF_DIR%\..
::  ====

::  GEOTIFF 
set GTIFF_DIR=%CD%\libgeotiff-1.4.0
set GTIFF_INCLUDE_DIR= /I%GTIFF_DIR% /I%GTIFF_DIR%/libxtiff
set GTIFF_LIBRARY=%GTIFF_DIR%\lib\libgeotiff_i.lib
set GTIFFD_LIBRARY=%GTIFF_DIR%\lib\libgeotiff_id.lib

echo | set /p="Installing libgeotiff... "<nul

cd %GTIFF_DIR%

IF NOT EXIST .\lib mkdir .\lib

nmake /f makefile.vc libgeotiff.dll >nul 2>nul
xcopy *i.lib lib >nul 2>nul
xcopy *.dll lib >nul 2>nul

nmake /f makefile.vc clean >nul 2>nul

nmake /f makefile.vc DEBUG=1 libgeotiffd.dll >nul 2>nul
xcopy *id.lib lib >nul 2>nul
xcopy *.dll lib >nul 2>nul

xcopy lib\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

echo done.

cd %GTIFF_DIR%\..
::  ====

::  OPENSSL 
set SSL_DIR=%CD%\openssl-master
set SSL_INCLUDE_DIR=%SSL_DIR%\inc32
set SSL_LIBRARY=%SSL_DIR%\out32dll\ssleay32.lib
set SSLD_LIBRARY=%SSL_DIR%\out32dll.dbg\ssleay32d.lib
set EAY_LIBRARY=%SSL_DIR%\out32dll\libeay32.lib
set EAYD_LIBRARY=%SSL_DIR%\out32dll.dbg\libeay32d.lib

  echo | set /p="Installing openssl... "<nul  

  cd %SSL_DIR%
  
::  Building release
  copy ms\libeay32.release.def.in ms\libeay32.def >nul 2>nul
  copy ms\ssleay32.release.def.in ms\ssleay32.def >nul 2>nul
  copy ms\nt.release.mak.in ms\nt.mak >nul 2>nul
  copy ms\ntdll.release.mak.in ms\ntdll.mak >nul 2>nul  

  nmake /f ms\ntdll.mak lib >nul 2>nul
  
::  Building debug
  del ms\libeay32.def >nul 2>nul
  del ms\ssleay32.def >nul 2>nul
  del ms\nt.mak >nul 2>nul
  del ms\ntdll.mak >nul 2>nul
    
  copy ms\libeay32.debug.def.in ms\libeay32.def >nul 2>nul
  copy ms\ssleay32.debug.def.in ms\ssleay32.def >nul 2>nul
  copy ms\nt.debug.mak.in ms\nt.mak >nul 2>nul
  copy ms\ntdll.debug.mak.in ms\ntdll.mak >nul 2>nul

  nmake /f ms\ntdll.mak lib >nul 2>nul
  
  copy out32dll\libeay32.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  copy out32dll\ssleay32.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

  copy out32dll.dbg\libeay32d.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  copy out32dll.dbg\ssleay32d.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

  echo done.
  
  cd ..
::  ====


::  CURL 
set CURL_DIR=%CD%\curl-7.42.1

IF NOT EXIST %CURL_DIR%\binaries (
  mkdir %CURL_DIR%\binaries
  mkdir %CURL_DIR%\binaries\lib
  mkdir %CURL_DIR%\binaries\include
)

set CURL_INCLUDE_DIR=%CURL_DIR%\binaries\include
set CURL_LIBRARY=%CURL_DIR%\binaries\lib\libcurl.lib
set CURLD_LIBRARY=%CURL_DIR%\binaries\lib\libcurld.lib

  echo|set /p="Installing curl... "<nul
  
  cd %CURL_DIR%\winbuild

  nmake /f Makefile.vc mode=dll VC=12 WITH_SSL=dll WITH_ZLIB=dll DEBUG=no MACHINE=x64 >nul 2>nul

  nmake /f Makefile.vc mode=dll VC=12 WITH_SSL=dll WITH_ZLIB=dll DEBUG=yes MACHINE=x64 >nul 2>nul
  
  xcopy %CURL_DIR%\builds\libcurl-vc12-x64-debug-dll-ssl-dll-zlib-dll-ipv6-sspi\bin\*.dll %CURL_DIR%\binaries\lib >nul 2>nul 
  xcopy %CURL_DIR%\builds\libcurl-vc12-x64-debug-dll-ssl-dll-zlib-dll-ipv6-sspi\lib\*.lib %CURL_DIR%\binaries\lib >nul 2>nul 
  xcopy %CURL_DIR%\builds\libcurl-vc12-x64-release-dll-ssl-dll-zlib-dll-ipv6-sspi\bin\*.dll %CURL_DIR%\binaries\lib >nul 2>nul 
  xcopy %CURL_DIR%\builds\libcurl-vc12-x64-release-dll-ssl-dll-zlib-dll-ipv6-sspi\lib\*.lib %CURL_DIR%\binaries\lib >nul 2>nul 
  xcopy %CURL_DIR%\builds\libcurl-vc12-x64-release-dll-ssl-dll-zlib-dll-ipv6-sspi\include /S %CURL_DIR%\binaries\include >nul 2>nul 
   
  xcopy  %CURL_DIR%\binaries\lib\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
   
  echo done.

  cd ..\..
::  ====

::  ICU 
set ICU_DIR=%CD%\icu
set ICU_INCLUDE_DIR=%ICU_DIR%\include
set ICU_LIBRARY=%ICU_DIR%\lib64\icuuc.lib
set ICUD_LIBRARY=%ICU_DIR%\lib64\icuucd.lib
set ICUDATA_LIBRARY=%ICU_DIR%\lib64\icudt.lib
set ICUDATAD_LIBRARY=%ICU_DIR%\lib64\icudtd.lib
set ICUIN_LIBRARY=%ICU_DIR%\lib64\icuin.lib
set ICUIND_LIBRARY=%ICU_DIR%\lib64\icuind.lib

set ICUROOT=%ICU_DIR%

  echo|set /p="Installing icu... "<nul
  
  cd %ICU_DIR%\source\allinone
  
  msbuild /m /t:makedata /p:Configuration=Release allinone.sln >nul 2>nul
  
  msbuild /m /t:common /p:Configuration=Debug allinone.sln >nul 2>nul
  
  copy %ICU_DIR%\bin64\icuuc52.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  copy %ICU_DIR%\bin64\icuuc52d.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

  copy %ICU_DIR%\bin64\icudt52.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  copy %ICU_DIR%\bin64\icudt52d.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  
  echo done.
  
  cd ..\..\..
::  ====

::  XERCES 
set XERCES_DIR=%TERRALIB_DEPENDENCIES_DIR%
set XERCES_INCLUDE_DIR=%XERCES_DIR%\include
set XERCES_LIBRARY=%XERCES_DIR%\lib\xerces-c_3.lib
set XERCESD_LIBRARY=%XERCES_DIR%\lib\xerces-c_3D.lib		
set XERCESCROOT=%CD%\xerces-c-3.1.1
set XERCES_ICU_LIBRARY=%XERCESCROOT%\src\xercesc\util\MsgLoaders\ICU\resources\xercesc_messages_3_1.lib

  echo | set /p="Installing xerces... "<nul

  cd %XERCESCROOT%\src\xercesc\util\MsgLoaders\ICU\resources

  :: Generating message files
  genrb root.txt 2> %XERCESCROOT%\..\build.log
  pkgdata --name xercesc_messages_3_1 -O %ICUROOT%\bin64 --mode dll -d . -e xercesc_messages_3_1 res-file-list.txt >nul 2>nul
  
  cd %XERCESCROOT%\projects\Win32\VC12\xerces-all  

  msbuild /m /t:XercesLib /p:Configuration="ICU Release" xerces-all.sln >nul 2>nul 
  msbuild /m /t:XercesLib /p:Configuration="ICU Debug" xerces-all.sln >nul 2>nul
  
:: Install  
  cd %XERCESCROOT%

  IF NOT EXIST %TERRALIB_DEPENDENCIES_DIR%\include\xercesc mkdir %TERRALIB_DEPENDENCIES_DIR%\include\xercesc  
  
  xcopy "%XERCESCROOT%\Build\Win64\VC10\ICU Debug\"*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul 
  xcopy "%XERCESCROOT%\Build\Win64\VC10\ICU Debug\"*.lib %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy "%XERCESCROOT%\Build\Win64\VC10\ICU Release\"*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy "%XERCESCROOT%\Build\Win64\VC10\ICU Release\"*.lib %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy "%XERCESCROOT%\src\xercesc\util\MsgLoaders\ICU\resources\"*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

  xcopy %XERCESCROOT%\src\xercesc\*.h* %TERRALIB_DEPENDENCIES_DIR%\include\xercesc /S /Y >nul 2>nul

  xcopy %XERCESCROOT%\src\xercesc\util\*.c %TERRALIB_DEPENDENCIES_DIR%\include\xercesc\util /Y >nul 2>nul
  
  echo done.

  cd ..
::  ====

::  LibXML2 
set XML2_DIR=%CD%\libxml2-2.9.1
set XML2_INCLUDE_DIR=%XML2_DIR%\include
set XML2_LIBRARY=%XML2_DIR%\win32\bin.msvc\libxml2.lib
set XML2D_LIBRARY=%XML2_DIR%\win32\bin.dbg.msvc\libxml2d.lib

  echo | set /p="Installing libxml2... "<nul
  
  cd %XML2_DIR%\win32

  nmake /f Makefile.msvc libxml >nul 2>nul
  nmake /f Makefile.msvc DEBUG=1 libxml >nul 2>nul

  xcopy %XML2_DIR%\win32\bin.msvc\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy %XML2_DIR%\win32\bin.dbg.msvc\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  
  echo done.

  cd %XML2_DIR%\..
::  ====

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

:: PostgreSQL version 9.4.1
set PGis_DIR=%CD%\postgresql-9.4.1
set PG_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set PG_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\libpqdll.lib
set PGD_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\libpqddll.lib

  echo | set /p="Installing libpq... "<nul
  
  cd %PGis_DIR%\src\interfaces\libpq

  nmake /f win32.mak CPU="AMD64" USE_SSL=1 >nul 2>nul
  nmake /f win32.mak CPU="AMD64" USE_SSL=1 DEBUG=1 >nul 2>nul
  
  copy %PGis_DIR%\src\interfaces\libpq\libpq-fe.h %TERRALIB_DEPENDENCIES_DIR%\include >nul 2>nul
  copy %PGis_DIR%\src\include\pg_config_ext.h %TERRALIB_DEPENDENCIES_DIR%\include >nul 2>nul
  copy %PGis_DIR%\src\include\postgres_ext.h %TERRALIB_DEPENDENCIES_DIR%\include >nul 2>nul
  copy %PGis_DIR%\src\interfaces\libpq\Release\libpqdll.lib %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy %PGis_DIR%\src\interfaces\libpq\Release\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  copy %PGis_DIR%\src\interfaces\libpq\Debug\libpqddll.lib %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy %PGis_DIR%\src\interfaces\libpq\Debug\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

  echo done.

  cd %PGis_DIR%\..
:: ====


:: netcdf version 4.3.3
set NETCDF_DIR=%CD%\netcdf-c-4.3.3
set NETCDF_INCLUDE_DIR=%NETCDF_DIR%\binaries\include
set NETCDF_LIBRARY=%NETCDF_DIR%\binaries\lib\netcdf.lib
set NETCDFD_LIBRARY=%NETCDF_DIR%\binaries\lib\netcdfd.lib

  echo | set /p="Installing netcdf... "<nul
  
  cd %NETCDF_DIR%

  IF NOT EXIST build mkdir build

  cd build

  cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_INSTALL_PREFIX=%NETCDF_DIR%\binaries^
 -DCMAKE_DEBUG_POSTFIX=d^
 -DJPEGLIB_H_INCLUDE_DIR=%JPG_INCLUDE_DIR%^
 -DJPEG_LIB:STRING="debug;%JPGD_LIBRARY%;optimized;%JPG_LIBRARY%"^
 -DMFHDF_H_INCLUDE_DIR=%HDF4_INCLUDE_DIR%\mfhdf^
 -DBUILD_DLL=ON^
 -DCURL_INCLUDE_DIR=%CURL_INCLUDE_DIR%^
 -DCURL_LIBRARY:STRING="debug;%CURLD_LIBRARY%;optimized;%CURL_LIBRARY%"^
 -DHDF4_DF_LIB:STRING="debug;%HDF4D_LIBRARY%;optimized;%HDF4_LIBRARY%"^
 -DHDF4_MFHDF_LIB:STRING="debug;%MFHDFD_LIBRARY%;optimized;%MFHDF_LIBRARY%"^
 -DENABLE_HDF4=ON^
 -DUSE_HDF4=ON^
 -DUSE_HDF5=OFF^
 -DENABLE_NETCDF_4=OFF .. >nul 2>nul

  msbuild /m /p:Configuration=Release INSTALL.vcxproj >nul 2>nul
  msbuild /m INSTALL.vcxproj >nul 2>nul
   
  xcopy %NETCDF_DIR%\binaries\bin\net* %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

  echo done.

  cd %NETCDF_DIR%\..
:: ====

:: SQLite
set SQLITE_DIR=%CD%\"sqlite-amalgamation-3090100"
set SQLITE_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set SQLITE_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\sqlite3.lib
set SQLITED_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\sqlite3d.lib

  echo | set /p="Installing sqlite... "<nul
  
  cd %SQLITE_DIR%

  cl sqlite3.c /DSQLITE_API=__declspec(dllexport) -link -dll -out:sqlite3.dll >nul 2>nul

  cl sqlite3.c /DSQLITE_API=__declspec(dllexport) /DSQLITE_DEBUG -link -dll -out:sqlite3d.dll >nul 2>nul
  
  xcopy *.h %TERRALIB_DEPENDENCIES_DIR%\include >nul 2>nul
  xcopy *.lib %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  xcopy *.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

  echo done.

  cd %SQLITE_DIR%\..
:: ====

:: SpatialLite version 4.3.0a
set SPLITE_DIR=%CD%\libspatialite-4.3.0a
set SPLITE_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set SPLITE_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\spatialite_i.lib
set SPLITED_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\spatialite_id.lib

  echo | set /p="Installing spatiallite... "<nul
  
  cd %SPLITE_DIR%

  nmake /f makefile.vc install >nul 2>nul
  
  nmake /f makefile.vc clean >nul 2>nul

  nmake /f makefile.vc DEBUG=1 install >nul 2>nul 
  
  echo done.

  cd %SPLITE_DIR%\..
:: ====

:: Expat version 2.1.0
set EXPAT_DIR=%CD%\expat-2.1.0
set EXPAT_INCLUDE_DIR=%EXPAT_DIR%\binaries\include
set EXPAT_LIBRARY=%EXPAT_DIR%\binaries\lib\expat.lib
set EXPATD_LIBRARY=%EXPAT_DIR%\binaries\lib\expatd.lib

  echo | set /p="Installing expat... "<nul
  
  cd %EXPAT_DIR%

  IF NOT EXIST build mkdir build

  cd build

  cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_INSTALL_PREFIX=%EXPAT_DIR%\binaries^
 -DCMAKE_INSTALL_PREFIX=%EXPAT_DIR%\binaries^
 -DCMAKE_DEBUG_POSTFIX=d .. >nul 2>nul

  msbuild /m /p:Configuration=Release INSTALL.vcxproj >nul 2>nul
  msbuild /m INSTALL.vcxproj >nul 2>nul
  
  xcopy %EXPAT_DIR%\binaries\bin\*.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul

  echo done.

  cd %EXPAT_DIR%\..
:: ====

:: GDAL version 2.0.0
set GDAL_DIR=%CD%\gdal-2.0.0
set GDAL_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set GDAL_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\gdal_i.lib
set GDALD_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\gdal_id.lib

  echo | set /p="Installing gdal... "<nul
  
  cd %GDAL_DIR%

  IF EXIST nmake.opt del /s nmake.opt >nul 2>nul  
  copy nmake.release.opt.in nmake.opt >nul 2>nul
  
  nmake -f makefile.vc MSVC_VER=1800 >nul 2>nul
  
  nmake -f makefile.vc MSVC_VER=1800 devinstall >nul 2>nul

  nmake /f makefile.vc clean >nul 2>nul

  del /s nmake.opt >nul 2>nul  
  copy nmake.debug.opt.in nmake.opt >nul 2>nul
  
  nmake -f makefile.vc MSVC_VER=1800 DEBUG=1 >nul 2>nul
  
  xcopy *.dll %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
  copy gdal_id.lib %TERRALIB_DEPENDENCIES_DIR%\lib >nul 2>nul
    
  echo done.

  cd %GDAL_DIR%\..
:: ====

:: QtPropertyBrowser
set QBROWSER_DIR=%CD%\qtpropertybrowser
set QBROWSER_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include\qtpropertybrowser
set QBROWSER_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\qtpropertybrowser.lib
set QBROWSERD_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\qtpropertybrowserd.lib

  echo | set /p="Installing qtpropertybrowser... "<nul
  
  cd %QBROWSER_DIR%

  set TERRALIB_DIR=%TERRALIB_DEPENDENCIES_DIR%
  
  qmake >nul 2>nul
  
  nmake >nul 2>nul

  nmake install >nul 2>nul

  echo done.

  cd %QBROWSER_DIR%\..
:: ====

:: QWT
 set QWT_PATH=%CD%\qwt-6.1.2

  echo | set /p="Installing qwt... "<nul
  
  cd %QWT_PATH%

  qmake qwt.pro -r -spec win32-msvc2013 "QWT_INSTALL_PREFIX_TARGET=%TERRALIB_DEPENDENCIES_DIR%" >nul 2>nul 

  nmake >nul 2>nul

  nmake install >nul 2>nul

  echo done.

  cd %QWT_PATH%\..
:: ====

:: Lua version 5.2.2
set LUAC_DIR=%CD%\lua-5.2.2
set LUAC_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include\lua
set LUAC_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\lua.lib
set LUACD_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\luad.lib

  echo | set /p="Installing lua... "<nul
  
  cd %LUAC_DIR%

  IF NOT EXIST build mkdir build
  IF NOT EXIST %TERRALIB_DEPENDENCIES_DIR%\include\lua mkdir %TERRALIB_DEPENDENCIES_DIR%\include\lua

  cd build

  cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_INSTALL_PREFIX=%TERRALIB_DEPENDENCIES_DIR%^
 -DCMAKE_DEBUG_POSTFIX=d^
 -DINSTALL_BIN=%TERRALIB_DEPENDENCIES_DIR%\lib^
 -DREADLINE_INCLUDE_DIR=%READLINE_INCLUDE_DIR%^
 -DREADLINE_LIBRARY:STRING="debug;%READLINED_LIBRARY%;optimized;%READLINE_LIBRARY%" .. >nul 2>nul
  
  msbuild /m /p:Configuration=Release INSTALL.vcxproj >nul 2>nul
  msbuild /m INSTALL.vcxproj >nul 2>nul
  
  copy %LUAC_DIR%\src\lua.h %LUAC_INCLUDE_DIR% >nul 2>nul
  copy %LUAC_DIR%\src\lualib.h %LUAC_INCLUDE_DIR% >nul 2>nul
  copy %LUAC_DIR%\src\lauxlib.h %LUAC_INCLUDE_DIR% >nul 2>nul
  copy %LUAC_DIR%\src\lua.hpp %LUAC_INCLUDE_DIR% >nul 2>nul
  copy %LUAC_DIR%\build\luaconf.h %LUAC_INCLUDE_DIR% >nul 2>nul
  
  echo done.

  cd %LUAC_DIR%\..
:: ====

:: TerraLib version 4.2.2
set TERRALIB4_DIR=%CD%\terralib4
set TERRALIB4_INCLUDE_DIR=%TERRALIB_DEPENDENCIES_DIR%\include
set TERRALIB4_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\terralib4.lib
set TERRALIB4D_LIBRARY=%TERRALIB_DEPENDENCIES_DIR%\lib\terralib4d.lib

echo | set /p="Installing terralib4... "<nul

cd %TERRALIB4_DIR%

IF NOT EXIST building mkdir building

cd .\building

copy ..\terralib.conf.cmake . >nul 2>nul

set CL=/MP1

cmake -G "Visual Studio 12 2013 Win64" -DCMAKE_INSTALL_PREFIX=%TERRALIB_DEPENDENCIES_DIR%^
 -DCMAKE_PREFIX_PATH="%TERRALIB_DEPENDENCIES_DIR%"^
 -DTIFF_INCLUDE_DIR:STRING="%TIFF_INCLUDE_DIR%"^
 -DTIFF_LIBRARY:STRING="debug;%TIFFD_LIBRARY%;optimized;%TIFF_LIBRARY%"^
 -DGeoTIFF_INCLUDE_DIR:STRING="%GTIFF_DIR%;%GTIFF_DIR%/libxtiff"^
 -DGeoTIFF_LIBRARY:STRING="debug;%GTIFFD_LIBRARY%;optimized;%GTIFF_LIBRARY%"^
 -DJPEG_INCLUDE_DIR="%JPG_DIR%"^
 -DJPEG_LIBRARY:STRING="debug;%JPGD_LIBRARY%;optimized;%JPG_LIBRARY%"^
 -DZLIB_INCLUDE_DIR:STRING="%ZL_INCLUDE_DIR%"^
 -DZLIB_LIBRARIES:STRING="%ZL_LIBRARIES%"^
 -C terralib.conf.cmake %TERRALIB4_DIR%\build\cmake >nul 2>nul

  msbuild /p:Configuration=Release INSTALL.vcxproj >nul 2>nul
  
  msbuild INSTALL.vcxproj >nul 2>nul
 
echo done.

cd %TERRALIB4_DIR%\..
:: ====

del /s %TERRALIB_DEPENDENCIES_DIR%\lib\*.exe >nul 2>nul

echo.
echo ----------------------------------
echo Dependencies builded successfully!
echo ----------------------------------
echo.

pause