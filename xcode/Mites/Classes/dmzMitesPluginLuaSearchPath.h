#ifndef DMZ_MITES_PLUGIN_LUA_SEARCH_PATH_DOT_H
#define DMZ_MITES_PLUGIN_LUA_SEARCH_PATH_DOT_H

#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>

namespace dmz {

   class MitesPluginLuaSearchPath :
         public Plugin {

      public:
         MitesPluginLuaSearchPath (const PluginInfo &Info, Config &local);
         ~MitesPluginLuaSearchPath ();

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
         MitesPluginLuaSearchPath ();
         MitesPluginLuaSearchPath (const MitesPluginLuaSearchPath &);
         MitesPluginLuaSearchPath &operator= (const MitesPluginLuaSearchPath &);

   };
};

#endif // DMZ_MITES_PLUGIN_LUA_SEARCH_PATH_DOT_H
