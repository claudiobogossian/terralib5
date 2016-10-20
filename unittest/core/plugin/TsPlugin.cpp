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

std::vector<te::core::PluginInfo> LoadPluginsInfo()
{
  std::vector<te::core::PluginInfo> v_pInfo;

  v_pInfo.push_back(te::core::JSONPluginInfoSerializer(
      te::core::FindInTerraLibPath("unittest/plugins/plugin1.teplg.json")));
  v_pInfo.push_back(te::core::JSONPluginInfoSerializer(
      te::core::FindInTerraLibPath("unittest/plugins/plugin2.teplg.json")));
  v_pInfo.push_back(te::core::JSONPluginInfoSerializer(
      te::core::FindInTerraLibPath("unittest/plugins/plugin3.teplg.json")));
  v_pInfo.push_back(te::core::JSONPluginInfoSerializer(
      te::core::FindInTerraLibPath("unittest/plugins/plugin4.teplg.json")));

  return v_pInfo;
}

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

  BOOST_CHECK_NO_THROW(te::core::plugin::InitializePluginSystem());
  BOOST_CHECK_NO_THROW(te::core::plugin::FinalizePluginSystem());
}

BOOST_AUTO_TEST_CASE(plugin_serializer_test)
{
  BOOST_CHECK_THROW(
      te::core::JSONPluginInfoSerializer("plugin/plugin1.teplg.json"),
      te::InvalidArgumentException);

  std::string p_manifest =
      te::core::FindInTerraLibPath("unittest/plugins/plugin1.teplg.json");
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
      te::core::FindInTerraLibPath("unittest/plugins/plugin1.teplg.json");

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
      te::core::FindInTerraLibPath("unittest/plugins/plugin1.teplg.json");

  te::core::PluginInfo p_info = te::core::JSONPluginInfoSerializer(p_manifest);

  std::unique_ptr<te::core::AbstractPlugin> plugin(plugin_engine.load(p_info));

  BOOST_CHECK_NO_THROW(plugin->info());

  BOOST_CHECK_NO_THROW(plugin->initialized());

  BOOST_CHECK_NO_THROW(plugin->startup());

  BOOST_CHECK_NO_THROW(plugin->shutdown());

  BOOST_CHECK_NO_THROW(plugin_engine.unload(std::move(plugin)));

  te::core::PluginEngineManager::instance().remove("C++");
}

BOOST_AUTO_TEST_CASE(plugin_sort_test)
{
  te::core::PluginInfo pInfo1;
  te::core::PluginInfo pInfo2;
  te::core::PluginInfo pInfo3;
  te::core::PluginInfo pInfo4;

  pInfo1.name = "plugin1";
  pInfo2.name = "plugin2";
  pInfo3.name = "plugin3";
  pInfo4.name = "plugin4";
                                               //  1<------+4
  pInfo2.dependencies.push_back(pInfo3.name);  //  ^        +
  pInfo3.dependencies.push_back(pInfo1.name);  //  |        |
  pInfo4.dependencies.push_back(pInfo1.name);  //  |        |
  pInfo4.dependencies.push_back(pInfo2.name);  //  +        v
                                               //  3<------+2
  std::vector<te::core::PluginInfo> v_pInfo;
  v_pInfo.push_back(pInfo1);
  v_pInfo.push_back(pInfo2);
  v_pInfo.push_back(pInfo3);
  v_pInfo.push_back(pInfo4);

  BOOST_CHECK_NO_THROW(v_pInfo = te::core::plugin::TopologicalSort(v_pInfo));

  BOOST_CHECK(v_pInfo[0].name == "plugin1");

  BOOST_CHECK(v_pInfo[1].name == "plugin3");

  BOOST_CHECK(v_pInfo[2].name == "plugin2");

  BOOST_CHECK(v_pInfo[3].name == "plugin4");
}

BOOST_AUTO_TEST_CASE(plugin_cyclic_dependency_test)
{
  te::core::PluginInfo pInfo1;
  te::core::PluginInfo pInfo2;

  pInfo1.name = "plugin1";
  pInfo2.name = "plugin2";

  pInfo1.dependencies.push_back(pInfo2.name);
  pInfo2.dependencies.push_back(pInfo1.name);

  std::vector<te::core::PluginInfo> v_pInfo;
  v_pInfo.push_back(pInfo1);
  v_pInfo.push_back(pInfo2);

  BOOST_CHECK_THROW(te::core::plugin::TopologicalSort(v_pInfo),
                    te::core::PluginCyclicDependencyException);
}

BOOST_AUTO_TEST_CASE(plugin_dependency_test)
{
  BOOST_CHECK_NO_THROW(te::core::plugin::InitializePluginSystem());

  std::vector<te::core::PluginInfo> v_pInfo = LoadPluginsInfo();

  BOOST_CHECK_NO_THROW(v_pInfo = te::core::plugin::TopologicalSort(v_pInfo));

  for(const te::core::PluginInfo& pinfo : v_pInfo)
  {
    BOOST_CHECK_NO_THROW(te::core::PluginManager::instance().insert(pinfo));
    BOOST_CHECK_NO_THROW(te::core::PluginManager::instance().load(pinfo.name));
  }
  std::vector<te::core::PluginInfo> pVec =
      te::core::PluginManager::instance().getLoadedPlugins();

  for(auto plugin = pVec.rbegin(); plugin != pVec.rend(); ++plugin)
  {
    BOOST_CHECK_NO_THROW(
        te::core::PluginManager::instance().stop(plugin->name));
    BOOST_CHECK_NO_THROW(
        te::core::PluginManager::instance().unload(plugin->name));
  }
  BOOST_CHECK_NO_THROW(te::core::PluginManager::instance().clear());
  BOOST_CHECK_NO_THROW(te::core::plugin::FinalizePluginSystem());
}
BOOST_AUTO_TEST_CASE(plugin_missing_dependency_test)
{
  BOOST_CHECK_NO_THROW(te::core::plugin::InitializePluginSystem());

  std::vector<te::core::PluginInfo> v_pInfo = LoadPluginsInfo();

  // plugin 3 depends of plugin 1
  BOOST_CHECK_NO_THROW(te::core::PluginManager::instance().insert(v_pInfo[2]));
  BOOST_CHECK_THROW(te::core::PluginManager::instance().load(v_pInfo[2].name),
                    te::core::PluginLoadException);

  te::core::PluginManager::instance().clear();
  te::core::plugin::FinalizePluginSystem();
}
BOOST_AUTO_TEST_CASE(plugin_unload_with_dependent_test)
{
  te::core::plugin::InitializePluginSystem();

  std::vector<te::core::PluginInfo> v_pInfo = LoadPluginsInfo();

  v_pInfo = te::core::plugin::TopologicalSort(v_pInfo);

  for(const te::core::PluginInfo& pinfo : v_pInfo)
  {
    te::core::PluginManager::instance().insert(pinfo);
    te::core::PluginManager::instance().load(pinfo.name);
  }
  // plugin 1 has plugin 4 and plugin 2 as dependents
  BOOST_CHECK_THROW(te::core::PluginManager::instance().unload(v_pInfo[0].name),
                    te::core::PluginUnloadException);

  te::core::PluginManager::instance().clear();
  te::core::plugin::FinalizePluginSystem();
}

BOOST_AUTO_TEST_SUITE_END()
