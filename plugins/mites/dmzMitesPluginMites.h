#ifndef DMZ_MITES_PLUGIN_MITES_DOT_H
#define DMZ_MITES_PLUGIN_MITES_DOT_H

#include <dmzObjectObserverUtil.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimeObjectType.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeTimeSlice.h>
#include <dmzTypesVector.h>
#include <dmzTypesHashTableHandleTemplate.h>


namespace dmz {

   class MitesPluginMites :
         public Plugin,
         public TimeSlice,
         public ObjectObserverUtil {

      public:
         MitesPluginMites (const PluginInfo &Info, Config &local);
         ~MitesPluginMites ();

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
            
            MiteStruct (const Handle TheObject) :
               Object (TheObject),
               nextTurn (0.0) {;}
         };
         
         void _validate_position (Vector &pos);
         void _init (Config &local);

         Log _log;
         Handle _defaultAttrHandle;
         Handle _minAreaAttrHandle;
         Handle _maxAreaAttrHandle;
         Handle _mitesAttrHandle;
         Handle _speedAttrHandle;
         Handle _turnAttrHandle;
         Handle _turnDelayAttrHandle;
         Vector _arenaMin;
         Vector _arenaMax;
         Float64 _speed;
         Float64 _maxTurn;
         Float64 _turnDelay;
         ObjectType _miteType;
         HashTableHandleTemplate<MiteStruct> _miteTable;

      private:
         MitesPluginMites ();
         MitesPluginMites (const MitesPluginMites &);
         MitesPluginMites &operator= (const MitesPluginMites &);

   };
};

#endif // DMZ_MITES_PLUGIN_MITES_DOT_H
