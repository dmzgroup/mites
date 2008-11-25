#include <dmzRuntimeConfig.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimePluginContainer.h>
#include <dmzRuntimePluginInfo.h>

extern "C" {
dmz::Plugin *create_dmzArchiveModuleBasic (const dmz::PluginInfo &Info, dmz::Config &local, dmz::Config &global);
dmz::Plugin *create_dmzArchivePluginAutoLoad (const dmz::PluginInfo &Info, dmz::Config &local, dmz::Config &global);
dmz::Plugin *create_dmzArchivePluginObject (const dmz::PluginInfo &Info, dmz::Config &local, dmz::Config &global);
dmz::Plugin *create_dmzObjectModuleBasic (const dmz::PluginInfo &Info, dmz::Config &local, dmz::Config &global);
dmz::Plugin *create_dmzObjectModuleGridBasic (const dmz::PluginInfo &Info, dmz::Config &local, dmz::Config &global);
dmz::Plugin *create_dmzInputModuleBasic (const dmz::PluginInfo &Info, dmz::Config &local, dmz::Config &global);
dmz::Plugin *create_dmzMitesModuleiPhone (const dmz::PluginInfo &Info, dmz::Config &local, dmz::Config &global);
dmz::Plugin *create_dmziPhonePluginLuaSearchPath (const dmz::PluginInfo &Info, dmz::Config &local, dmz::Config &global);
dmz::Plugin *create_dmziPhonePluginCanvasObject (const dmz::PluginInfo &Info, dmz::Config &local, dmz::Config &global);
dmz::Plugin *create_dmzMitesPlugin (const dmz::PluginInfo &Info, dmz::Config &local, dmz::Config &global);
}

void
dmz_create_plugins (
      dmz::RuntimeContext *context,
      dmz::Config &config,
      dmz::Config &global,
      dmz::PluginContainer &container) {

   dmz::PluginInfo *info (0);
   dmz::Config local;

   info = new dmz::PluginInfo ("dmzArchiveModuleBasic", dmz::PluginDeleteModeDelete, context, 0);
   local.set_config_context (0);
   config.lookup_all_config_merged ("dmzArchiveModuleBasic", local);
   container.add_plugin (info, create_dmzArchiveModuleBasic (*info, local, global));

   info = new dmz::PluginInfo ("dmzArchivePluginAutoLoad", dmz::PluginDeleteModeDelete, context, 0);
   local.set_config_context (0);
   config.lookup_all_config_merged ("dmzArchivePluginAutoLoad", local);
   container.add_plugin (info, create_dmzArchivePluginAutoLoad (*info, local, global));

   info = new dmz::PluginInfo ("dmzArchivePluginObject", dmz::PluginDeleteModeDelete, context, 0);
   local.set_config_context (0);
   config.lookup_all_config_merged ("dmzArchivePluginObject", local);
   container.add_plugin (info, create_dmzArchivePluginObject (*info, local, global));

   info = new dmz::PluginInfo ("dmzObjectModuleBasic", dmz::PluginDeleteModeDelete, context, 0);
   local.set_config_context (0);
   config.lookup_all_config_merged ("dmzObjectModuleBasic", local);
   container.add_plugin (info, create_dmzObjectModuleBasic (*info, local, global));

   info = new dmz::PluginInfo ("dmzObjectModuleGridBasic", dmz::PluginDeleteModeDelete, context, 0);
   local.set_config_context (0);
   config.lookup_all_config_merged ("dmzObjectModuleGridBasic", local);
   container.add_plugin (info, create_dmzObjectModuleGridBasic (*info, local, global));

   info = new dmz::PluginInfo ("dmzInputModuleBasic", dmz::PluginDeleteModeDelete, context, 0);
   local.set_config_context (0);
   config.lookup_all_config_merged ("dmzInputModuleBasic", local);
   container.add_plugin (info, create_dmzInputModuleBasic (*info, local, global));

   info = new dmz::PluginInfo ("dmzMitesModuleiPhone", dmz::PluginDeleteModeDelete, context, 0);
   local.set_config_context (0);
   config.lookup_all_config_merged ("dmzMitesModuleiPhone", local);
   container.add_plugin (info, create_dmzMitesModuleiPhone (*info, local, global));

   info = new dmz::PluginInfo ("dmziPhonePluginLuaSearchPath", dmz::PluginDeleteModeDelete, context, 0);
   local.set_config_context (0);
   config.lookup_all_config_merged ("dmziPhonePluginLuaSearchPath", local);
   container.add_plugin (info, create_dmziPhonePluginLuaSearchPath (*info, local, global));

   info = new dmz::PluginInfo ("dmziPhonePluginCanvasObject", dmz::PluginDeleteModeDelete, context, 0);
   local.set_config_context (0);
   config.lookup_all_config_merged ("dmziPhonePluginCanvasObject", local);
   container.add_plugin (info, create_dmziPhonePluginCanvasObject (*info, local, global));

   info = new dmz::PluginInfo ("dmzMitesPlugin", dmz::PluginDeleteModeDelete, context, 0);
   local.set_config_context (0);
   config.lookup_all_config_merged ("dmzMitesPlugin", local);
   container.add_plugin (info, create_dmzMitesPlugin (*info, local, global));
}
