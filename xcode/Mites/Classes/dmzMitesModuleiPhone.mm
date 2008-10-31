#import "dmzAppDelegate.h"
#include "dmzMitesModuleiPhone.h"
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#import "MainViewController.h"
#import "MainView.h"
#import "RootViewController.h"


dmz::MitesModuleiPhone *localInstance (0);
dmz::MitesModuleiPhone *dmz::MitesModuleiPhone::_instance (0);


dmz::MitesModuleiPhone::MitesModuleiPhone (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      iPhoneModuleCanvas (Info),
      _log (Info),
      _navigationController (0),
      _rootViewController (0),
      _itemTable () {

   if (!_instance) {
      
      _instance = this;
      localInstance = _instance;
   }

   _rootViewController = [[RootViewController alloc] 
      initWithNibName:@"RootView" bundle:nil];
         
   _navigationController = [[UINavigationController alloc]
      initWithRootViewController:_rootViewController];

   dmzAppDelegate *app = [dmzAppDelegate shared_dmz_app];
   app.rootController = _navigationController;      

   _navigationController.navigationBarHidden = YES;
         
   _init (local);
}


dmz::MitesModuleiPhone::~MitesModuleiPhone () {

   _itemTable.clear ();
   
   [_rootViewController release];
   [_navigationController release];
   
   if (localInstance) {
      
      _instance = 0;
      localInstance = 0;
   }
}


dmz::MitesModuleiPhone *
dmz::MitesModuleiPhone::get_instance () { return _instance; }


// Plugin Interface
void
dmz::MitesModuleiPhone::update_plugin_state (
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
dmz::MitesModuleiPhone::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}

// iPhoneModuleCanvas Interface
UIView *
dmz::MitesModuleiPhone::get_view () const {
  
//   MainViewController *mainViewController = _rootViewController.mainViewController;
   
   return (UIView *)_rootViewController.mainViewController.mainView.canvas;
}


dmz::Boolean
dmz::MitesModuleiPhone::add_item (const Handle ObjectHandle, UIView *item) {
   
   Boolean retVal (False);
   
   if (item) {
      
      retVal = _itemTable.store (ObjectHandle, item);
      
      if (retVal) {
         
         [item retain];
         
         [_rootViewController.mainViewController.mainView.canvas addSubview:item];
      }
   }
   
   return retVal;
}


UIView *
dmz::MitesModuleiPhone::lookup_item (const Handle ObjectHandle) {
   
   UIView *item (_itemTable.lookup (ObjectHandle));
   return item;
}


UIView *
dmz::MitesModuleiPhone::remove_item (const Handle ObjectHandle) {
   
   UIView *item (_itemTable.remove (ObjectHandle));
   
   if (item) {
      
      [item removeFromSuperview];
      [item release];
   }
   
   return item;
}


// class Interface

void
dmz::MitesModuleiPhone::_init (Config &local) {

}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzMitesModuleiPhone (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::MitesModuleiPhone (Info, local);
}

};
