; ---------------------------------------------------------------------------
; TeDependencies
; ---------------------------------------------------------------------------
;
; Description:
; ------------
; Script for TerraLib dependencies manipulation. Contains macros that make
; the insertion of all TerraLib dependencies in the installer.
;
; ---------------------------------------------------------------------------

!ifndef TE_DEPENDENCIES
!define TE_DEPENDENCIES

!macro TE_INSERT_DEPENDENCIES_DEBUG_32
  
  SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"
 
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\boost_date_time-vc100-mt-gd-1_47.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\boost_filesystem-vc100-mt-gd-1_47.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\boost_system-vc100-mt-gd-1_47.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\boost_thread-vc100-mt-gd-1_47.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\comerr32.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\gdal18.dll"
	File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\geos.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\iconv_d.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\intl_d.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\krb5_32.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libeay32.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libecpg.dll"
	File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libexpat.dll"
	File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libexpatw.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libiconv-2.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libintl-2.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libpgtypes.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libpq.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\log4cxx_d.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\proj.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\QtCored4.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\QtGuid4.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\QtSvgd4.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\ssleay32.dll"

!macroend

!macro TE_INSERT_DEPENDENCIES_RELEASE_32

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"

    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\boost_date_time-vc100-mt-1_47.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\boost_filesystem-vc100-mt-1_47.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\boost_system-vc100-mt-1_47.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\boost_thread-vc100-mt-1_47.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\comerr32.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\gdal18.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\geos.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\iconv.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\intl.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\krb5_32.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libeay32.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libecpg.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libexpat.dll"
	File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libexpatw.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libiconv-2.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libintl-2.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libpgtypes.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\libpq.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\log4cxx.dll"
	File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\proj.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\QtCore4.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\QtGui4.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\QtSvg4.dll"
    File /nonfatal "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\ssleay32.dll"
  
!macroend

!macro TE_INSERT_DEPENDENCIES_DEBUG_64

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"

    File "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\dependencia64Debug.dll"

!macroend

!macro TE_INSERT_DEPENDENCIES_RELEASE_64

    SetOutPath "$INSTDIR\bin${PLATFORM}\${GENERATOR}"

    File "${TERRALIB_DIR}\bin${PLATFORM}\${GENERATOR}\dependencia64Release.dll"

!macroend

!endif ; TE_DEPENDENCIES