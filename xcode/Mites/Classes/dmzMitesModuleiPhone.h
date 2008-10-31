#ifndef DMZ_MITES_MODULE_IPHONE_DOT_H
#define DMZ_MITES_MODULE_IPHONE_DOT_H

#import <dmziPhoneModuleCanvas.h>
#import <dmzRuntimeLog.h>
#import <dmzRuntimePlugin.h>
#import <dmzTypesHashTableHandleTemplate.h>

@class RootViewController;


namespace dmz {

  
   class MitesModuleiPhone :
         public Plugin,
         public iPhoneModuleCanvas {

      public:
         static MitesModuleiPhone *get_instance ();

         MitesModuleiPhone (const PluginInfo &Info, Config &local);
         ~MitesModuleiPhone ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // iPhoneModuleCanvas Interface
         virtual UIView *get_view () const;
         virtual Boolean add_item (const Handle ObjectHandle, UIView *item);
         virtual UIView *lookup_item (const Handle ObjectHandle);
         virtual UIView *remove_item (const Handle ObjectHandle);
            
      protected:
         static MitesModuleiPhone *_instance;
            
         void _init (Config &local);

         Log _log;
         UINavigationController *_navigationController;
         RootViewController *_rootViewController;
         HashTableHandleTemplate<UIView> _itemTable;
            
      private:
         MitesModuleiPhone ();
         MitesModuleiPhone (const MitesModuleiPhone &);
         MitesModuleiPhone &operator= (const MitesModuleiPhone &);
   };
};

#endif // DMZ_MITES_MODULEI_PHONE_DOT_H
