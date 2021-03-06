/*
  * Copyright (c) 2014 Clark Cianfarini
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
  * and associated documentation files (the "Software"), to deal in the Software without
  * restriction, including without limitation the rights to use, copy, modify, merge, publish,
  * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
  * Software is furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all copies or
  * substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
  * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
  * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  */

#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

// TODO: Look at supporting constructors with parameters
// TODO: Profile map vs unordered_map

namespace FlakedTuna
{
class PluginRegistry
{
private:
  std::unordered_map<std::type_index, std::vector<std::function<std::shared_ptr<void>()>>> _plugins;

public:
  template<typename T, typename BaseT, typename... Ts> void RegisterPlugin(Ts... rest)
  {
    // If key already exists, throw a duplicate plugin exception
    _plugins[std::type_index(typeid(BaseT))].push_back([this, rest...]() { return std::make_shared<T>(rest...); });
  }

  template<class T> std::vector<std::shared_ptr<T>> ResolvePlugin()
  {
    std::vector<std::shared_ptr<T>> ret = {nullptr};
    if(_plugins.find(std::type_index(typeid(T))) != _plugins.end())
    {
      for(size_t i = 0; i != _plugins[typeid(T)].size(); ++i) ret.push_back(std::static_pointer_cast<T>(_plugins[std::type_index(typeid(T))][i]()));
    }
    return ret;
  }
};
}  // namespace FlakedTuna

#if defined(WIN32) || defined(_WIN32)

  #define FT_EXPORT __declspec(dllexport)

#else

  #define FT_EXPORT __attribute__((visibility("default")))

#endif

// Checks
#define FLAKED_TUNA_DERRIVED(concrete, base) static_assert(std::is_base_of<base, concrete>::value, "ERROR: FLAKED_TUNA_PLUGIN: Registered concrete type must be of base type.");
#define FLAKED_TUNA_DEFAULT_CTOR(concrete)   static_assert((std::is_default_constructible<concrete>::value), "ERROR: FLAKED_TUNA_PLUGIN: Concrete type is not default constructable.");

// Concatenation of checks
#define FLAKED_TUNA_CHECKS(concrete, base) FLAKED_TUNA_DERRIVED(concrete, base) FLAKED_TUNA_DEFAULT_CTOR(concrete)

#define FLAKED_TUNA_EXPORTS_BEGIN                                      \
  FlakedTuna::PluginRegistry* pr = nullptr;                            \
  extern "C" FT_EXPORT FlakedTuna::PluginRegistry* GetPluginRegistry() \
  {                                                                    \
    pr = new FlakedTuna::PluginRegistry();
#define FLAKED_TUNA_PLUGIN(concrete, base) FLAKED_TUNA_CHECKS(concrete, base) pr->RegisterPlugin<concrete, base>();
#define FLAKED_TUNA_EXPORTS_END                   \
  return pr;                                      \
  }                                               \
  extern "C" FT_EXPORT void ClosePluginRegistry() \
  {                                               \
    if(pr != nullptr) { delete pr; }              \
  }

#define FLAKED_TUNA_PLUGIN_VERSION(version) \
  extern "C" FT_EXPORT int GetPluginVersion() { return version; }
