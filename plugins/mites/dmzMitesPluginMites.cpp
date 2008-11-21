#include "dmzMitesPluginMites.h"
#include <dmzObjectAttributeMasks.h>
#include <dmzObjectModule.h>
#include <dmzRuntimeConfigToTypesBase.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzTypesConsts.h>
#include <dmzTypesMatrix.h>
#include <stdlib.h>

static const dmz::Vector ForwardVec (0.0, 0.0, -1.0);
static const dmz::Vector UpVec (0.0, 1.0, 0.0);

dmz::Float64 local_random () { return ((rand () % RAND_MAX) / RAND_MAX); }

dmz::Float64 calc_next_turn_time (const dmz::Float64 Delay) {

   dmz::Float64 result = (local_random () * Delay - 0.5) + 0.5;
   if (result < 0.0) { result = 0.0; }
   return result;
}


dmz::MitesPluginMites::MitesPluginMites (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      TimeSlice (Info),
      ObjectObserverUtil (Info, local),
      _log (Info),
      _defaultAttrHandle (0),
      _minAreaAttrHandle (0),
      _maxAreaAttrHandle (0),
      _mitesAttrHandle (0),
      _speedAttrHandle (0),
      _turnAttrHandle (0),
      _turnDelayAttrHandle (0),
      _arenaMin (-3000, 0, -2000),
      _arenaMax (3000, 0, 2000),
      _speed (3000.0),
      _maxTurn (HalfPi64),
      _turnDelay (3.0),
      _miteType (),
      _miteTable () {

   _init (local);
}


dmz::MitesPluginMites::~MitesPluginMites () {

   _miteTable.empty ();
}


// Plugin Interface
void
dmz::MitesPluginMites::update_plugin_state (
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
dmz::MitesPluginMites::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


// TimeSlice Interface
void
dmz::MitesPluginMites::update_time_slice (const Float64 TimeDelta) {

   ObjectModule *objMod (get_object_module ());
   
   if (objMod) {

      HashTableHandleIterator it;
      
      MiteStruct *mite = _miteTable.get_first (it);

      while (mite) {

         Vector pos;
         Matrix ori;
         
         objMod->lookup_position (mite->Object, _defaultAttrHandle, pos);
         objMod->lookup_orientation (mite->Object, _defaultAttrHandle, ori);
         
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
         
         mite = _miteTable.get_next (it);
      }
   }
}


// Object Observer Interface
void
dmz::MitesPluginMites::update_object_counter (
      const UUID &Identity,
      const Handle ObjectHandle,
      const Handle AttributeHandle,
      const Int64 Value,
      const Int64 *PreviousValue) {

   if (AttributeHandle == _mitesAttrHandle) {
      
      ObjectModule *objMod (get_object_module ());

      if (objMod) {
      
         objMod->lookup_position (ObjectHandle, _minAreaAttrHandle, _arenaMin);
         objMod->lookup_position (ObjectHandle, _maxAreaAttrHandle, _arenaMax);
         
         const Float64 MinX (_arenaMin.get_x ());
         const Float64 MaxX (_arenaMax.get_x ());
         const Float64 MinZ (_arenaMin.get_z ());
         const Float64 MaxZ (_arenaMax.get_z ());
         
         while (_miteTable.get_count () < Value) {

            const Handle TheObject (objMod->create_object (_miteType, ObjectLocal));
            
            MiteStruct *mite = new MiteStruct (TheObject);
            mite->nextTurn = calc_next_turn_time (_turnDelay);
            
            Vector pos (
               (MaxX * local_random ()) + MinX,
               0.0,
               (MaxZ * local_random ()) + MinZ);
            
            Matrix ori (UpVec, local_random () * TwoPi);
            
            objMod->store_position (mite->Object, _defaultAttrHandle, pos);
            objMod->store_orientation (mite->Object, _defaultAttrHandle, ori);
            
            objMod->active_object (mite->Object);
         }
      }
   }
}


void
dmz::MitesPluginMites::update_object_position (
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
dmz::MitesPluginMites::update_object_scalar (
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
}


void
dmz::MitesPluginMites::_validate_position (Vector &pos) {

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
dmz::MitesPluginMites::_init (Config &local) {

   Definitions defs (get_plugin_runtime_context (), &_log);

   _defaultAttrHandle = defs.create_named_handle (ObjectAttributeDefaultName);
   _minAreaAttrHandle = activate_object_attribute ("Minimum_Area", ObjectPositionMask);
   _maxAreaAttrHandle = activate_object_attribute ("Maximum_Area", ObjectPositionMask);
   _mitesAttrHandle = activate_object_attribute ("Mites", ObjectCounterMask);
   _speedAttrHandle = activate_object_attribute ("Speed", ObjectScalarMask);
   _turnAttrHandle = activate_object_attribute ("Turn", ObjectScalarMask);
   _turnDelayAttrHandle = activate_object_attribute ("TurnDelay", ObjectScalarMask);
   
   defs.lookup_object_type (
      config_to_string ("object_type", local, "mite"), _miteType);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzMitesPluginMites (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::MitesPluginMites (Info, local);
}

};
