#include "dmzMitesPlugin.h"
#include <dmzObjectAttributeMasks.h>
#include <dmzObjectModule.h>
#include <dmzObjectModuleGrid.h>
#include <dmzRuntimeConfigToTypesBase.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzTypesConsts.h>
#include <dmzTypesMatrix.h>
#include <stdlib.h>

static const dmz::Vector ForwardVec (0.0, 0.0, -1.0);
static const dmz::Vector UpVec (0.0, 1.0, 0.0);
static const dmz::Vector ChipOffset (0.0, 0.0, -96.0);


dmz::Float64 local_random () { 
   
   dmz::Float64 result = rand () % RAND_MAX;
   return (result / RAND_MAX);
}

dmz::Float64 calc_next_turn_time (const dmz::Float64 Delay) {

   dmz::Float64 result = (local_random () * Delay - 0.5) + 0.5;
   if (result < 0.0) { result = 0.0; }
   return result;
}


dmz::MitesPlugin::MitesPlugin (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      TimeSlice (Info),
      ObjectObserverUtil (Info, local),
      _log (Info),
      _gridMod (0),
      _defaultAttrHandle (0),
      _minAreaAttrHandle (0),
      _maxAreaAttrHandle (0),
      _mitesAttrHandle (0),
      _chipsAttrHandle (0),
      _speedAttrHandle (0),
      _turnAttrHandle (0),
      _turnDelayAttrHandle (0),
      _linkAttrHandle (0),
      _timerAttrHandle (0),
      _waitAttrHandle (0),
      _arenaMin (-3000, 0, -2000),
      _arenaMax (3000, 0, 2000),
      _speed (3000.0),
      _maxTurn (HalfPi64),
      _turnDelay (3.0),
      _wait (1),
      _miteType (),
      _chipType (),
      _miteTable (),
      _chipTable (),
      _volume (Vector (), 256),
      _time (Info.get_context ()) {
   
   _init (local);
}


dmz::MitesPlugin::~MitesPlugin () {

   _miteTable.empty ();
   _chipTable.empty ();
}


// Plugin Interface
void
dmz::MitesPlugin::update_plugin_state (
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
dmz::MitesPlugin::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_gridMod) { _gridMod = ObjectModuleGrid::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_gridMod && (_gridMod == ObjectModuleGrid::cast (PluginPtr))) {
         
         _gridMod = 0;
      }
   }
}


// TimeSlice Interface
void
dmz::MitesPlugin::update_time_slice (const Float64 TimeDelta) {

   const Float64 CurrentTime (_time.get_frame_time ());

   ObjectModule *objMod (get_object_module ());
   
   if (objMod) {

      HashTableHandleIterator it;
      Vector pos;
      Matrix ori;
      
      // update mites
      
      MiteStruct *mite = _miteTable.get_first (it);

      while (mite) {
         
         // move
         objMod->lookup_position (mite->Object, _defaultAttrHandle, pos);
         objMod->lookup_orientation (mite->Object, _defaultAttrHandle, ori);
         
         mite->nextTurn -= TimeDelta;
         
         if (mite->nextTurn <= 0.0) {
            
            Float64 angle = (local_random () - 0.5) * _maxTurn;
            
            ori = Matrix (UpVec, angle) * ori;
            
            mite->nextTurn = calc_next_turn_time (_turnDelay);
         }

         Vector moveVec (ForwardVec);
         ori.transform_vector (moveVec);
         
         pos = pos + (moveVec * TimeDelta * _speed);
         _validate_position (pos);
         
         objMod->store_position (mite->Object, _defaultAttrHandle, pos);
         objMod->store_orientation (mite->Object, _defaultAttrHandle, ori);
         
         // haul
         Float64 timer (CurrentTime);
         objMod->lookup_time_stamp (mite->Object, _timerAttrHandle, timer);
         
         if (timer <= CurrentTime) {
            
            const Handle Chip = _find_nearest_chip (pos);
            
            if (Chip) {
               
               if (mite->link) {
                  
                 objMod->unlink_objects (mite->link);
                 mite->link = 0;
               }
               else {
                  
                  mite->link = objMod->link_objects (_linkAttrHandle, mite->Object, Chip);
               }
               
               objMod->store_time_stamp (mite->Object, _timerAttrHandle, CurrentTime + _wait);
            }
         }
         
         mite = _miteTable.get_next (it);
      }
      
      // update chips
      
      ChipStruct *chip = _chipTable.get_first (it);
      
      while (chip) {
      
         const Handle Mite (_get_mite (chip->Object));
         
         if (Mite) {
            
            objMod->lookup_position (Mite, _defaultAttrHandle, pos);
            objMod->lookup_orientation (Mite, _defaultAttrHandle, ori);
            
            Vector offsetVec (ChipOffset);
            ori.transform_vector (offsetVec);
            
            objMod->store_position (chip->Object, _defaultAttrHandle, pos + offsetVec);
         }
         
         chip = _chipTable.get_next (it);
      }
   }
}


// Object Observer Interface
void
dmz::MitesPlugin::update_object_counter (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Int64 Value,
      const Int64 *PreviousValue) {

   ObjectModule *objMod (get_object_module ());

   if (objMod) {

      const Float64 MinX (_arenaMin.get_x ());
      const Float64 MaxX (_arenaMax.get_x () - MinX);
      const Float64 MinZ (_arenaMin.get_z ());
      const Float64 MaxZ (_arenaMax.get_z () - MinZ);

      if (AttributeHandle == _mitesAttrHandle) {
         
         while (_miteTable.get_count () < Value) {
         
            const Handle TheObject (objMod->create_object (_miteType, ObjectLocal));
         
            MiteStruct *mite = new MiteStruct (TheObject);
            mite->nextTurn = calc_next_turn_time (_turnDelay);
            
            const Float64 StartX ((MaxX * local_random ()) + MinX);
            const Float64 StartZ ((MaxZ * local_random ()) + MinZ);
         
            Vector pos (StartX, 0.0, StartZ);
            Matrix ori (UpVec, local_random () * TwoPi64);
         
            objMod->store_position (mite->Object, _defaultAttrHandle, pos);
            objMod->store_orientation (mite->Object, _defaultAttrHandle, ori);
            
            objMod->store_time_stamp (
               mite->Object, _timerAttrHandle, _time.get_frame_time () + _wait);
         
            objMod->activate_object (mite->Object);
         
            _miteTable.store (mite->Object, mite);
         }
      
         while (_miteTable.get_count () > Value) {
      
            HashTableHandleIterator it;
            MiteStruct *mite = _miteTable.get_first (it);
            if (mite) {
            
               objMod->destroy_object (mite->Object);
               _miteTable.remove (mite->Object);
               delete mite; mite = 0;
            }
         }
      }
      else if (AttributeHandle == _chipsAttrHandle) {
      
         while (_chipTable.get_count () < Value) {
            
            const Handle TheObject (objMod->create_object (_chipType, ObjectLocal));
            
            ChipStruct *chip = new ChipStruct (TheObject);
         
            const Float64 StartX ((MaxX * local_random ()) + MinX);
            const Float64 StartZ ((MaxZ * local_random ()) + MinZ);
         
            Vector pos (StartX, 0.0, StartZ);
            Matrix ori;
         
            objMod->store_position (chip->Object, _defaultAttrHandle, pos);
            objMod->store_orientation (chip->Object, _defaultAttrHandle, ori);
         
            objMod->activate_object (chip->Object);
         
            _chipTable.store (chip->Object, chip);
         }
         
         while (_chipTable.get_count () > Value) {
         
            HashTableHandleIterator it;
            ChipStruct *chip = _chipTable.get_first (it);
            if (chip) {
         
               objMod->destroy_object (chip->Object);
               _chipTable.remove (chip->Object);
               delete chip; chip = 0;
            }
         }
      }
   }
}


void
dmz::MitesPlugin::update_object_position (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Vector &Value,
      const Vector *PreviousValue) {

   if (AttributeHandle == _minAreaAttrHandle) {

      _arenaMin = Value;
   }
   else if (AttributeHandle == _maxAreaAttrHandle) {
      
      _arenaMax = Value;
   }
}


void
dmz::MitesPlugin::update_object_scalar (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Float64 Value,
      const Float64 *PreviousValue) {

   if (AttributeHandle == _speedAttrHandle) {
      
      _speed = Value;
   }
   else if (AttributeHandle == _turnAttrHandle) {
      
      _maxTurn = Value;
   }
   else if (AttributeHandle == _turnDelayAttrHandle) {
      
      _turnDelay = Value;
   }
   else if (AttributeHandle == _waitAttrHandle) {
      
      _wait = Value;
   }
}


dmz::Handle
dmz::MitesPlugin::_find_nearest_chip (const Vector &Pos) {

   Handle result (0);
   
   ObjectModule *objMod (get_object_module ());
   
   if (_gridMod && objMod) {
      
      HandleContainer objects;
      
      _volume.set_origin (Pos);
      
      _gridMod->find_objects (_volume, objects);
      
      if (objects.get_count ()) {
         
         Handle object (objects.get_first ());
         
         while (object && !result) {

            ObjectType type = objMod->lookup_object_type (object);
            
            if (type.is_of_type (_chipType)) {

               HandleContainer links;
               
               objMod->lookup_super_links (object, _linkAttrHandle, links);
               
               if (!links.get_count ()) {
               
                  result = object;
               }
            }
            
            object = objects.get_next ();
         }
      }
   }
   
   return result;
}


dmz::Handle
dmz::MitesPlugin::_get_mite (const Handle Object) {
   
   Handle result (0);
   
   ObjectModule *objMod (get_object_module ());

   if (objMod) {
      
      HandleContainer links;
      
      objMod->lookup_super_links (Object, _linkAttrHandle, links);
      
      if (links.get_count ()) {
         
         result = links.get_first ();
      }
   }
   
   return result;
}


void
dmz::MitesPlugin::_validate_position (Vector &pos) {

   if (pos.get_x () > _arenaMax.get_x ()) {
      
      pos.set_x (_arenaMin.get_x () - (_arenaMax.get_x () - pos.get_x ()));
   }
   else if (pos.get_x () < _arenaMin.get_x ()) {
      
      pos.set_x (_arenaMax.get_x () - (_arenaMin.get_x () - pos.get_x ()));
   }
   
   if (pos.get_z () > _arenaMax.get_z ()) {
      
      pos.set_z (_arenaMin.get_z () - (_arenaMax.get_z () - pos.get_z ()));
   }
   else if (pos.get_z () < _arenaMin.get_z ()) {
      
      pos.set_z (_arenaMax.get_z () - (_arenaMin.get_z () - pos.get_z ()));
   }
}


void
dmz::MitesPlugin::_init (Config &local) {

   Definitions defs (get_plugin_runtime_context (), &_log);

   _defaultAttrHandle = defs.create_named_handle (ObjectAttributeDefaultName);
   _minAreaAttrHandle = activate_object_attribute ("Minimum_Area", ObjectPositionMask);
   _maxAreaAttrHandle = activate_object_attribute ("Maximum_Area", ObjectPositionMask);
   _mitesAttrHandle = activate_object_attribute ("Mites", ObjectCounterMask);
   _chipsAttrHandle = activate_object_attribute ("Chips", ObjectCounterMask);
   _speedAttrHandle = activate_object_attribute ("Speed", ObjectScalarMask);
   _turnAttrHandle = activate_object_attribute ("Turn", ObjectScalarMask);
   _waitAttrHandle = activate_object_attribute ("Wait", ObjectScalarMask);
   _turnDelayAttrHandle = activate_object_attribute ("TurnDelay", ObjectScalarMask);

   _linkAttrHandle = defs.create_named_handle ("Chip_Link");
   _timerAttrHandle = defs.create_named_handle ("Mite_Timer");

   defs.lookup_object_type (
      config_to_string ("object_type", local, "mite"), _miteType);
      
   defs.lookup_object_type (
      config_to_string ("object_type", local, "chip"), _chipType);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzMitesPlugin (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::MitesPlugin (Info, local);
}

};
