#ifndef DMZ_IPHONE_PLUGIN_LUA_SEARCH_PATH_DOT_H
#define DMZ_IPHONE_PLUGIN_LUA_SEARCH_PATH_DOT_H

#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

namespace dmz {

   class iPhonePluginLuaSearchPath :
         public Plugin {

      public:
         iPhonePluginLuaSearchPath (const PluginInfo &Info, Config &local);
         ~iPhonePluginLuaSearchPath ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

      protected:
         void _init (Config &local);

         Log _log;
            

      private:
         iPhonePluginLuaSearchPath ();
         iPhonePluginLuaSearchPath (const iPhonePluginLuaSearchPath &);
         iPhonePluginLuaSearchPath &operator= (const iPhonePluginLuaSearchPath &);

   };
};

#endif // DMZ_IPHONE_PLUGIN_LUA_SEARCH_PATH_DOT_H
