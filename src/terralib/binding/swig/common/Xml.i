/*
 * File: Xml.i
 *
 * XML module
 */
 
#define TEXMLEXPORT
 
%{
#include "terralib/xml/AbstractWriterFactory.h"
#include "terralib/xml/AbstractWriter.h"
#include "terralib/xml/Enums.h"
#include "terralib/xml/Reader.h"
#include "terralib/xml/ReaderFactory.h"
%}

%nspace te::xml::AbstractWriterFactory;
%nspace te::xml::AbstractWriter;
%nspace te::xml::Reader;
%nspace te::xml::ReaderFactory;

%include "terralib/xml/AbstractWriterFactory.h"
%include "terralib/xml/AbstractWriter.h"
%include "terralib/xml/Enums.h"
%include "terralib/xml/Reader.h"
%include "terralib/xml/ReaderFactory.h"