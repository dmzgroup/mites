#import <dmzLuaModule.h>
#import "dmziPhonePluginLuaSearchPath.h"
#import <dmzRuntimePluginFactoryLinkSymbol.h>
#import <dmzRuntimePluginInfo.h>
#import "dmzSystemFileMacOS.h"

dmz::iPhonePluginLuaSearchPath::iPhonePluginLuaSearchPath (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      _log (Info) {

   _init (local);
}


dmz::iPhonePluginLuaSearchPath::~iPhonePluginLuaSearchPath () {

}


// Plugin Interface
void
dmz::iPhonePluginLuaSearchPath::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

   }
}


void
dmz::iPhonePluginLuaSearchPath::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      LuaModule *mod (LuaModule::cast (PluginPtr));
      if (mod) {
         
         String path (dmz_get_main_bundle_directory () + "/?.lua");
         mod->add_lua_path (path);
      }
   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


void
dmz::iPhonePluginLuaSearchPath::_init (Config &local) {

}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmziPhonePluginLuaSearchPath (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::iPhonePluginLuaSearchPath (Info, local);
}

};
