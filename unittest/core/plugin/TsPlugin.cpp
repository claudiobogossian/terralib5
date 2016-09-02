/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

  This file is part of the TerraLib - a Framework for building GIS enabled
  applications.

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
  \file terralib/unittest/core/plugin/TsPlugin.cpp

  \brief A test suit for the TerraLib Core Plugin Module.

  \author Matheus Cavassan Zaglia.
 */

// TerraLib
#include <terralib/core/plugin.h>
#include <terralib/core/utils.h>

// Boost
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(plugin_test_case)

BOOST_AUTO_TEST_CASE(plugin_engine_test)
{
  std::unique_ptr<te::core::AbstractPluginEngine> cppengine(nullptr);
  BOOST_CHECK_NO_THROW(cppengine.reset(new te::core::CppPluginEngine()));

  BOOST_CHECK_NO_THROW(
      te::core::PluginEngineManager::instance().insert(std::move(cppengine)));
  BOOST_CHECK_THROW(
      te::core::PluginEngineManager::instance().insert(std::move(cppengine)),
      te::InvalidArgumentException);

  BOOST_CHECK_NO_THROW(te::core::PluginEngineManager::instance().get("C++"));
  BOOST_CHECK_THROW(te::core::PluginEngineManager::instance().get("++C"),
                    te::OutOfRangeException);

  te::core::AbstractPluginEngine& plugin_engine =
      te::core::PluginEngineManager::instance().get("C++");

  BOOST_CHECK(plugin_engine.id() == "C++");
  BOOST_CHECK(plugin_engine.name() == "Engine for plugins written in C++");

  BOOST_CHECK_NO_THROW(te::core::PluginEngineManager::instance().remove("C++"));
  BOOST_CHECK_THROW(te::core::PluginEngineManager::instance().remove("C++"),
                    te::OutOfRangeException);
}

BOOST_AUTO_TEST_CASE(plugin_serializer_test)
{
  BOOST_CHECK_THROW(
      te::core::JSONPluginInfoSerializer("plugin/plugin.teplg.json"),
      te::InvalidArgumentException);

  std::string p_manifest =
      te::core::FindInTerraLibPath("unittest/plugins/plugin.teplg.json");
  BOOST_CHECK_NO_THROW(te::core::JSONPluginInfoSerializer(p_manifest));
}

BOOST_AUTO_TEST_CASE(plugin_load_test)
{
  std::unique_ptr<te::core::AbstractPluginEngine> cppengine(
      new te::core::CppPluginEngine());
  te::core::PluginEngineManager::instance().insert(std::move(cppengine));

  te::core::AbstractPluginEngine& plugin_engine =
      te::core::PluginEngineManager::instance().get("C++");

  std::string p_manifest =
      te::core::FindInTerraLibPath("unittest/plugins/plugin.teplg.json");

  te::core::PluginInfo p_info = te::core::JSONPluginInfoSerializer(p_manifest);

  BOOST_CHECK_NO_THROW(plugin_engine.load(p_info));

  te::core::PluginEngineManager::instance().remove("C++");
}

BOOST_AUTO_TEST_CASE(plugin_usage_test)
{
  std::unique_ptr<te::core::AbstractPluginEngine> cppengine(
      new te::core::CppPluginEngine());

  te::core::PluginEngineManager::instance().insert(std::move(cppengine));

  te::core::AbstractPluginEngine& plugin_engine =
      te::core::PluginEngineManager::instance().get("C++");

  std::string p_manifest =
      te::core::FindInTerraLibPath("unittest/plugins/plugin.teplg.json");

  te::core::PluginInfo p_info = te::core::JSONPluginInfoSerializer(p_manifest);

  std::unique_ptr<te::core::AbstractPlugin> plugin(plugin_engine.load(p_info));

  BOOST_CHECK_NO_THROW(plugin->info());

  BOOST_CHECK_NO_THROW(plugin->initialized());

  BOOST_CHECK_NO_THROW(plugin->startup());

  BOOST_CHECK_NO_THROW(plugin->shutdown());

  BOOST_CHECK_NO_THROW(plugin_engine.unload(std::move(plugin)));

  te::core::PluginEngineManager::instance().remove("C++");
}

BOOST_AUTO_TEST_SUITE_END()
