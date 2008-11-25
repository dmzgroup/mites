#ifndef DMZ_MITES_PLUGIN_MITES_DOT_H
#define DMZ_MITES_PLUGIN_MITES_DOT_H

#include <dmzObjectObserverUtil.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimeObjectType.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeTimeSlice.h>
#include <dmzRuntimeTime.h>
#include <dmzTypesSphere.h>
#include <dmzTypesVector.h>
#include <dmzTypesHashTableHandleTemplate.h>


namespace dmz {

   class ObjectModuleGrid;
   
   class MitesPlugin :
         public Plugin,
         public TimeSlice,
         public ObjectObserverUtil {

      public:
         MitesPlugin (const PluginInfo &Info, Config &local);
         ~MitesPlugin ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // TimeSlice Interface
         virtual void update_time_slice (const Float64 TimeDelta);

         // Object Observer Interface
         virtual void update_object_counter (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Int64 Value,
            const Int64 *PreviousValue);

         virtual void update_object_position (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Vector &Value,
            const Vector *PreviousValue);

         virtual void update_object_scalar (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Float64 Value,
            const Float64 *PreviousValue);

      protected:
         struct MiteStruct {
            
            const Handle Object;
            Float64 nextTurn;
            Handle link;
            
            MiteStruct (const Handle TheObject) :
               Object (TheObject),
               nextTurn (0.0),
               link (0) {;}
         };
         
         struct ChipStruct {
         
            const Handle Object;
            
            ChipStruct (const Handle TheObject) :
               Object (TheObject) {;}
         };
         
         Handle _find_nearest_chip (const Vector &Pos);
         Handle _get_mite (const Handle Object);
         void _validate_position (Vector &pos);
         void _init (Config &local);

         Log _log;
         ObjectModuleGrid *_gridMod;
         Handle _defaultAttrHandle;
         Handle _minAreaAttrHandle;
         Handle _maxAreaAttrHandle;
         Handle _mitesAttrHandle;
         Handle _chipsAttrHandle;
         Handle _speedAttrHandle;
         Handle _turnAttrHandle;
         Handle _turnDelayAttrHandle;
         Handle _linkAttrHandle;
         Handle _timerAttrHandle;
         Handle _waitAttrHandle;
         Vector _arenaMin;
         Vector _arenaMax;
         Float64 _speed;
         Float64 _maxTurn;
         Float64 _turnDelay;
         Float64 _wait;
         ObjectType _miteType;
         ObjectType _chipType;
         HashTableHandleTemplate<MiteStruct> _miteTable;
         HashTableHandleTemplate<ChipStruct> _chipTable;
         Sphere _volume;
         Time _time;
         
      private:
         MitesPlugin ();
         MitesPlugin (const MitesPlugin &);
         MitesPlugin &operator= (const MitesPlugin &);
   };
};

#endif // DMZ_MITES_PLUGIN_MITES_DOT_H
