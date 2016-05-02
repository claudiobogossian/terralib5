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
  \file terralib/unittest/core/translator/TsTranslator.cpp

  \brief A test suit for the TerraLib Core Translator Module.

  \author Matheus Cavassan Zaglia.
 */

// STL
// TerraLib
#include <terralib/core/translator/Translator.h>

// Boost
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(translator_test_case)

BOOST_AUTO_TEST_CASE(translator_test)
{

  TE_ADD_TEXT_DOMAIN("terralib_unittest_core");
  BOOST_CHECK_EQUAL(TE_TR("BR-GIS"), "SIG-BR");
  BOOST_CHECK_EQUAL(TE_TR("Wrong cryptographer algorithm type!"), "O tipo de algoritmo de criptografia está errado!");
  BOOST_CHECK_EQUAL(TE_TR("You must specify a logger configuration file!"), "Você deve especificar o arquivo de configuração do logger!");
  BOOST_CHECK_EQUAL(TE_TR("It was not possible to initialize the the logger!"), "Não foi possível inicializar o logger!");
  BOOST_CHECK_EQUAL(TE_TR("Invalid logger configuration type!"), "Tipo de configuração inválido para o logger!");
  BOOST_CHECK_EQUAL(TE_TR("The logger name is empty!"), "O nome do logger está vazio!");
  BOOST_CHECK_EQUAL(TE_TR("TerraLib Core Initialized!"), "O módulo Core do TerraLib foi inicializado!");
  BOOST_CHECK_EQUAL(TE_TR("TerraLib Core Finalized!"), "O módulo Core do TerraLib foi finalizado!");
  BOOST_CHECK_EQUAL(TE_TR("The text domain already exist."), "Text domain já existente.");
  BOOST_CHECK_EQUAL(TE_TR("The text domain doesn't exist."), "O text domain informado não existe.");
// test un-translated messages
  BOOST_CHECK_EQUAL(TE_TR("The unprepared english text will remain the same."), "The unprepared english text will remain the same.");

// test plural messages
  BOOST_CHECK_EQUAL(TE_TR_PLURAL("1 layer is selected.","2 layers are selected.", 2), "2 camadas estão selecionadas.");
  BOOST_CHECK_EQUAL(TE_TR_PLURAL("1 layer is selected.","2 layers are selected.", 1), "1 camada está selecionada.");

    return ;
}
BOOST_AUTO_TEST_SUITE_END()

