#include <dmzInputConsts.h>
#include <dmzLuaModule.h>
#include "dmzMitesPluginControls.h"
#include <dmzObjectAttributeMasks.h>
#include <dmzObjectModule.h>
#include <dmzQtModuleMainWindow.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>


dmz::MitesPluginControls::MitesPluginControls (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      ObjectObserverUtil (Info, local),
      _log (Info),
      _inUpdate (False),
      _arena (0),
      _channel (0),
      _mitesHandle (0),
      _chipsHandle (0),
      _speedHandle (0),
      _waitHandle (0),
      _lua (0),
      _window (0),
      _dock (0) {

   setObjectName (get_plugin_name ().get_buffer ());

   _ui.setupUi (this);

   _init (local);
}


dmz::MitesPluginControls::~MitesPluginControls () {

}


// Plugin Interface
void
dmz::MitesPluginControls::update_plugin_state (
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
dmz::MitesPluginControls::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_lua) { _lua = dmz::LuaModule::cast (PluginPtr); }

      if (!_window) {

         _window = QtModuleMainWindow::cast (PluginPtr);

         if (_window) {

            _dock = new QDockWidget ("Controls", this);
            _dock->setObjectName (get_plugin_name ().get_buffer ());
            _dock->setAllowedAreas (Qt::AllDockWidgetAreas);

            _dock->setFeatures (QDockWidget::NoDockWidgetFeatures);
   //            QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

            _window->add_dock_widget (
               _channel,
               Qt::RightDockWidgetArea,
               _dock);

            _dock->setWidget (this);
         }
      }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_lua && (_lua == dmz::LuaModule::cast (PluginPtr))) { _lua = 0; }

      if (_window && (_window == QtModuleMainWindow::cast (PluginPtr))) {

         _window->remove_dock_widget (_channel, _dock);
         _window = 0;
      }
   }
}


// Object Observer Interface
void
dmz::MitesPluginControls::update_object_counter (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Int64 Value,
      const Int64 *PreviousValue) {

   _arena = ObjectHandle;

   if (!_inUpdate) {

      _inUpdate = True;

      if (AttributeHandle == _mitesHandle) {

         _ui.MitesSlider->setValue ((int)Value);
      }
      else if (AttributeHandle == _chipsHandle) {

         _ui.ChipsSlider->setValue ((int)Value);
      }
     
      _inUpdate = False;
   }
}


void
dmz::MitesPluginControls::update_object_scalar (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Float64 Value,
      const Float64 *PreviousValue) {

   _arena = ObjectHandle;

   if (!_inUpdate) {

      _inUpdate = True;

      if (AttributeHandle == _speedHandle) {

         _ui.SpeedSlider->setValue ((int)Value);
      }
      else if (AttributeHandle == _waitHandle) {

         _ui.WaitSlider->setValue ((int)Value);
      }

      _inUpdate = False;
   }
}


void
dmz::MitesPluginControls::on_MitesSlider_valueChanged (int value) {

   ObjectModule *objMod (get_object_module ());

   if (_arena && objMod) {

      objMod->store_counter (_arena, _mitesHandle, Int64 (value));
   }
}


void
dmz::MitesPluginControls::on_ChipsSlider_valueChanged (int value) {

   ObjectModule *objMod (get_object_module ());

   if (_arena && objMod) {

      objMod->store_counter (_arena, _chipsHandle, Int64 (value));
   }
}


void
dmz::MitesPluginControls::on_SpeedSlider_valueChanged (int value) {

   ObjectModule *objMod (get_object_module ());

   if (_arena && objMod) {

      objMod->store_scalar (_arena, _speedHandle, Float64 (value));
   }
}


void
dmz::MitesPluginControls::on_WaitSlider_valueChanged (int value) {

   ObjectModule *objMod (get_object_module ());

   if (_arena && objMod) {

      objMod->store_scalar (_arena, _waitHandle, Float64 (value));
   }
}


void
dmz::MitesPluginControls::on_ResetButton_clicked () {

   if (_lua) { _lua->reset_lua (); }
}


void
dmz::MitesPluginControls::_init (Config &local) {

   _channel = Definitions (get_plugin_runtime_context ()).create_named_handle (
      InputChannelDefaultName);

   _mitesHandle = activate_object_attribute ("Mites", ObjectCounterMask);
   _chipsHandle = activate_object_attribute ("Chips", ObjectCounterMask);
   _speedHandle = activate_object_attribute ("Speed", ObjectScalarMask);
   _waitHandle = activate_object_attribute ("Wait", ObjectScalarMask);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzMitesPluginControls (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::MitesPluginControls (Info, local);
}

};
