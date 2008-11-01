#ifndef DMZ_IPHONE_PLUGIN_CANVAS_OBJECT_DOT_H
#define DMZ_IPHONE_PLUGIN_CANVAS_OBJECT_DOT_H

#include <dmzObjectObserverUtil.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimeObjectType.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHashTableHandleTemplate.h>

@class UIImageView;


namespace dmz {

   class iPhoneModuleCanvas;

   class iPhonePluginCanvasObject :
         public Plugin,
         public ObjectObserverUtil {

      public:
         iPhonePluginCanvasObject (const PluginInfo &Info, Config &local);
         ~iPhonePluginCanvasObject ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // Object Observer Interface
         virtual void create_object (
            const UUID &Identity,
            const Handle ObjectHandle,
            const ObjectType &Type,
            const ObjectLocalityEnum Locality);

         virtual void destroy_object (const UUID &Identity, const Handle ObjectHandle);

         virtual void update_object_position (
            const UUID &Identity,
            const Handle ObjectHandle,
            const Handle AttributeHandle,
            const Vector &Value,
            const Vector *PreviousValue);
            
         virtual void update_object_orientation (
             const UUID &Identity,
             const Handle ObjectHandle,
             const Handle AttributeHandle,
             const Matrix &Value,
             const Matrix *PreviousValue);

      protected:
         Boolean _find_config_from_type (Config &local, ObjectType &objType);
         
         void _init (Config &local);

         Log _log;
         Definitions _defs;
         iPhoneModuleCanvas *_canvasModule;
         String _canvasModuleName;
         Handle _defaultAttrHandle;
         HashTableHandleTemplate<UIImageView> _objectTable;

      private:
         iPhonePluginCanvasObject ();
         iPhonePluginCanvasObject (const iPhonePluginCanvasObject &);
         iPhonePluginCanvasObject &operator= (const iPhonePluginCanvasObject &);
   };
};


#endif // DMZ_IPHONE_PLUGIN_CANVAS_OBJECT_DOT_H

