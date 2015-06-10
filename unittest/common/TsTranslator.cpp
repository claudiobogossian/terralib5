/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file TsTranslator.cpp
 
  \brief Test suite for the Translator framework.
 */

// Unit-Test TerraLib
#include "TsTranslator.h"

// TerraLib
#include <terralib/common.h>

// STL
#include <cstring>

CPPUNIT_TEST_SUITE_REGISTRATION( TsTranslator );

void TsTranslator::setUp()
{
}

void TsTranslator::tearDown()
{
}

void TsTranslator::tcLocalePtBR()
{
// test translated messages
  CPPUNIT_ASSERT(strcmp(TE_TR("BR-GIS"), "SIG-BR") == 0);
  CPPUNIT_ASSERT(strcmp(TE_TR("Wrong cryptographer algorithm type!"), "O tipo de algoritmo de criptografia está errado!") == 0);
  CPPUNIT_ASSERT(strcmp(TE_TR("You must specify a logger configuration file!"), "Você deve especificar o arquivo de configuração do logger!") == 0);
  CPPUNIT_ASSERT(strcmp(TE_TR("It was not possible to initialize the the logger!"), "Não foi possível inicializar o logger!") == 0);
  CPPUNIT_ASSERT(strcmp(TE_TR("Invalid logger configuration type!"), "Tipo de configuração inválido para o logger!") == 0);
  CPPUNIT_ASSERT(strcmp(TE_TR("The logger name is empty!"), "O nome do logger está vazio!") == 0);
  CPPUNIT_ASSERT(strcmp(TE_TR("TerraLib Core Initialized!"), "O módulo Core do TerraLib foi inicializado!") == 0);
  CPPUNIT_ASSERT(strcmp(TE_TR("TerraLib Core Finalized!"), "O módulo Core do TerraLib foi finalizado!") == 0);
  CPPUNIT_ASSERT(strcmp(TE_TR("The text domain already exist."), "Text domain já existente.") == 0);
  CPPUNIT_ASSERT(strcmp(TE_TR("The text domain doesn't exist."), "O text domain informado não existe.") == 0);

// test un-translated messages
  CPPUNIT_ASSERT(strcmp(TE_TR("The unprepared english text will remain the same."), "The unprepared english text will remain the same.") == 0);
  CPPUNIT_ASSERT(strcmp(TE_TR("Wrong cryptographer algorithm type!"), "O tipo de algoritmo de criptografia estã errado!") != 0);

  return;
}

