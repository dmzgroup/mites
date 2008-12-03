#ifndef DMZ_MITES_PLUGIN_CONTROLS_DOT_H
#define DMZ_MITES_PLUGIN_CONTROLS_DOT_H

#include <dmzObjectObserverUtil.h>
#include <dmzQtWidget.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <QtGui/QWidget>
#include <QtGui/QDockWidget>
#include <ui_dmzMitesControls.h>

namespace dmz {

   class QtModuleCanvas;
   class LuaModule;

   class MitesPluginControls :
         public QWidget,
         public Plugin,
         public ObjectObserverUtil,
         public QtWidget {

      Q_OBJECT

      public:
         MitesPluginControls (const PluginInfo &Info, Config &local);
         ~MitesPluginControls ();

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

         // QtWidget Interface
         virtual QWidget *get_qt_widget ();

      protected slots:
         void on_MitesSlider_valueChanged (int value);
         void on_ChipsSlider_valueChanged (int value);
         void on_SpeedSlider_valueChanged (int value);
         void on_WaitSlider_valueChanged (int value);
         void on_TurnSlider_valueChanged (int value);
         void on_TurnDelaySlider_valueChanged (int value);
         void on_ZoomSlider_valueChanged (int value);
         void on_ResetButton_clicked ();
         void slot_scale_changed (qreal value);

      protected:
         void _init (Config &local);

         Log _log;
         Boolean _inUpdate;
         Handle _arena;
         Handle _mitesHandle;
         Handle _chipsHandle;
         Handle _speedHandle;
         Handle _waitHandle;
         Handle _turnHandle;
         Handle _turnDelayHandle;
         Ui::MitesControls _ui;
         LuaModule *_lua;
         QtModuleCanvas *_canvas;

      private:
         MitesPluginControls ();
         MitesPluginControls (const MitesPluginControls &);
         MitesPluginControls &operator= (const MitesPluginControls &);

   };
};

#endif // DMZ_MITES_PLUGIN_CONTROLS_DOT_H
