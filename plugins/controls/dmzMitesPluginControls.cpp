#include <dmzInputConsts.h>
#include <dmzLuaModule.h>
#include "dmzMitesPluginControls.h"
#include <dmzObjectAttributeMasks.h>
#include <dmzObjectModule.h>
#include <dmzQtModuleCanvas.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzTypesMath.h>

#include <QtGui/QGraphicsView>


dmz::MitesPluginControls::MitesPluginControls (const PluginInfo &Info, Config &local) :
      QFrame (0),
      Plugin (Info),
      ObjectObserverUtil (Info, local),
      QtWidget (Info),
      _log (Info),
      _inUpdate (False),
      _arena (0),
      _mitesHandle (0),
      _chipsHandle (0),
      _speedHandle (0),
      _waitHandle (0),
      _turnHandle (0),
      _turnDelayHandle (0),
      _lua (0),
      _canvas (0) {

   setObjectName (get_plugin_name ().get_buffer ());

   _ui.setupUi (this);

   _init (local);
   
   adjustSize ();
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

      if (!_canvas) {

         _canvas = QtModuleCanvas::cast (PluginPtr);

         if (_canvas) {

            QGraphicsView *view = _canvas->get_view ();

            if (view) {

               connect (
                  view, SIGNAL (scale_changed (qreal)),
                  this, SLOT (slot_scale_changed (qreal)));
            }
         }
      }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_lua && (_lua == dmz::LuaModule::cast (PluginPtr))) { _lua = 0; }

      if (_canvas && (_canvas == dmz::QtModuleCanvas::cast (PluginPtr))) { _canvas = 0; }
   }
}


// Object Observer Interface
void
dmz::MitesPluginControls::update_object_flag (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Boolean Value,
      const Boolean *PreviousValue) {

   if (Value) { _ui.PauseButton->setText ("Start"); }
   else { _ui.PauseButton->setText ("Stop"); }
}


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

         _ui.SpeedSlider->setValue ((int)Value / 100);
      }
      else if (AttributeHandle == _waitHandle) {

         _ui.WaitSlider->setValue ((int)(Value * 10.0));
      }
      else if (AttributeHandle == _turnHandle) {

         _ui.TurnSlider->setValue ((int)to_degrees (Value));
      }
      else if (AttributeHandle == _turnDelayHandle) {

         _ui.TurnDelaySlider->setValue ((int)(Value * 10.0));
      }

      _inUpdate = False;
   }
}


// QtWidget Interface
QWidget *
dmz::MitesPluginControls::get_qt_widget () { return this; }


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

      objMod->store_scalar (_arena, _speedHandle, Float64 (value) * 100.0);
   }
}


void
dmz::MitesPluginControls::on_WaitSlider_valueChanged (int value) {

   ObjectModule *objMod (get_object_module ());

   if (_arena && objMod) {

      objMod->store_scalar (_arena, _waitHandle, Float64 (value) * 0.1);
   }
}


void
dmz::MitesPluginControls::on_TurnSlider_valueChanged (int value) {

   ObjectModule *objMod (get_object_module ());

   if (_arena && objMod) {

      objMod->store_scalar (_arena, _turnHandle, to_radians (Float64 (value)));
   }
}


void
dmz::MitesPluginControls::on_TurnDelaySlider_valueChanged (int value) {

   ObjectModule *objMod (get_object_module ());

   if (_arena && objMod) {

      objMod->store_scalar (_arena, _turnDelayHandle, Float64 (value) * 0.1);
   }
}


void
dmz::MitesPluginControls::on_ResetButton_clicked () {

   if (_lua) { _lua->reset_lua (); }
}


void
dmz::MitesPluginControls::on_PauseButton_clicked () {

   ObjectModule *objMod (get_object_module ());

   if (_arena && objMod) {

      const Boolean Flag = objMod->lookup_flag (_arena, _pauseHandle);

      objMod->store_flag (_arena, _pauseHandle, !Flag);
   }
}


void
dmz::MitesPluginControls::on_ZoomSlider_valueChanged (int value) {

   if (_canvas && !_inUpdate) {

      _inUpdate = True;

      const Float32 ZoomMin (_canvas->get_zoom_min_value ());
      const Float32 ZoomMax (_canvas->get_zoom_max_value ());
      const Float32 ZoomRange (ZoomMax - ZoomMin);
      const Float32 SliderRange (_ui.ZoomSlider->maximum () - _ui.ZoomSlider->minimum ());
      const Float32 SliderValue (value / SliderRange);

      _canvas->set_zoom (ZoomMin + (ZoomRange * SliderValue));

      _inUpdate = False;
   }
}


void
dmz::MitesPluginControls::slot_scale_changed (qreal value) {

   if (_canvas && !_inUpdate) {

      _inUpdate = True;

      const Float32 ZoomMin (_canvas->get_zoom_min_value ());
      const Float32 ZoomMax (_canvas->get_zoom_max_value ());
      const Float32 ZoomRange (ZoomMax - ZoomMin);
      const Float32 SliderRange (_ui.ZoomSlider->maximum () - _ui.ZoomSlider->minimum ());
      const Float32 SliderValue ((value - ZoomMin) / ZoomRange);

      _ui.ZoomSlider->setValue (SliderValue * SliderRange);

      _inUpdate = False;
   }
}


void
dmz::MitesPluginControls::_init (Config &local) {

   _mitesHandle = activate_object_attribute ("Mites", ObjectCounterMask);
   _chipsHandle = activate_object_attribute ("Chips", ObjectCounterMask);
   _speedHandle = activate_object_attribute ("Speed", ObjectScalarMask);
   _waitHandle = activate_object_attribute ("Wait", ObjectScalarMask);
   _turnHandle = activate_object_attribute ("Turn", ObjectScalarMask);
   _turnDelayHandle = activate_object_attribute ("TurnDelay", ObjectScalarMask);
   _pauseHandle = activate_object_attribute ("Pause", ObjectFlagMask);
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
