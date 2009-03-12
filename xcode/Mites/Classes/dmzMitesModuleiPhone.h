#ifndef DMZ_MITES_MODULE_IPHONE_DOT_H
#define DMZ_MITES_MODULE_IPHONE_DOT_H

#import <dmziPhoneModuleCanvas.h>
#import <dmzObjectObserverUtil.h>
#import <dmzRuntimeLog.h>
#import <dmzRuntimePlugin.h>
#import <dmzTypesHashTableHandleTemplate.h>

@class RootViewController;


namespace dmz {

   class LuaModule;

   class MitesModuleiPhone :
         public Plugin,
         public ObjectObserverUtil,
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

         // Object Observer Interface
         virtual void update_object_counter (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Int64 Value,
            const Int64 *PreviousValue);
            
         virtual void update_object_scalar (
           const UUID &Identity,
           const Handle ObjectHandle,
           const Handle AttributeHandle,
           const Float64 Value,
           const Float64 *PreviousValue);
            
         // iPhoneModuleCanvas Interface
         virtual UIView *get_view () const;
         virtual Boolean add_item (const Handle ObjectHandle, UIView *item);
         virtual UIView *lookup_item (const Handle ObjectHandle);
         virtual UIView *remove_item (const Handle ObjectHandle);
            
         // Class Interface
         void set_mites (const Int64 Value);
         Int64 get_mites ();
            
         void set_chips (const Int64 Value);
         Int64 get_chips ();
            
         void set_speed (const Float64 Value);
         Float64 get_speed ();
            
         void set_wait (const Float64 Value);
         Float64 get_wait ();
            
         Boolean reset_lua ();
            
      protected:
         static MitesModuleiPhone *_instance;
            
         void _init (Config &local);

         Log _log;
         UINavigationController *_navigationController;
         RootViewController *_rootViewController;
         HashTableHandleTemplate<UIView> _itemTable;
         Handle _arena;
         Handle _mitesHandle;
         Handle _chipsHandle;
         Handle _speedHandle;
         Handle _waitHandle;
         LuaModule *_lua;
            
      private:
         MitesModuleiPhone ();
         MitesModuleiPhone (const MitesModuleiPhone &);
         MitesModuleiPhone &operator= (const MitesModuleiPhone &);
   };
};

#endif // DMZ_MITES_MODULEI_PHONE_DOT_H
