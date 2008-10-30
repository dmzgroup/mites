require "const"
local Forward = dmz.math.forward ()
local Up = dmz.math.up ()

local function calc_next_turn_time ()
   return (math.random () * 2.5) + 0.5
end

local function validate_position (self, pos)
   if pos:get_x () > self.arena.max:get_x () then
      pos:set_x (self.arena.min:get_x () + (self.arena.max:get_x () - pos:get_x ()))
   elseif pos:get_x () < self.arena.min:get_x () then
      pos:set_x (self.arena.max:get_x () - (self.arena.min:get_x () - pos:get_x ()))
   end
   if pos:get_z () > self.arena.max:get_z () then
      pos:set_z (self.arena.min:get_z () + (self.arena.max:get_z () - pos:get_z ()))
   elseif pos:get_z () < self.arena.min:get_z () then
      pos:set_z (self.arena.max:get_z () - (self.arena.min:get_z () - pos:get_z ()))
   end
end

local function update_mites (self, time)
   for i, m in ipairs (self.mites) do
      local pos = dmz.object.position (m.object)
      local ori = dmz.object.orientation (m.object)
      m.nextTurn = m.nextTurn - time
      if m.nextTurn <= 0 then
         ori = dmz.matrix.new (Up, (math.random () - 0.5) * dmz.math.HalfPi) * ori
         m.nextTurn = calc_next_turn_time ()
      end
      pos = pos + (ori:transform (Forward) * time * self.speed)
      validate_position (self, pos)
      dmz.object.position (m.object, nil, pos)
      dmz.object.orientation (m.object, nil, ori)
   end
end

local function update_mite_count (self, object, handle, count)
   --self.log:error ("Mites:", count)
   self.arena.min = dmz.object.position (object, "Minimum_Area")
   self.arena.max = dmz.object.position (object, "Maximum_Area")
   local mites = self.mites
   local MinX = self.arena.min:get_x ()
   local MaxX = self.arena.max:get_x () - MinX
   local MinZ = self.arena.min:get_z ()
   local MaxZ = self.arena.max:get_z () - MinZ
   while #mites < count do
      local m = {}
      m.object = dmz.object.create (const.MiteType)
      dmz.object.position (m.object, nil, {
         (MaxX * math.random ()) + MinX,
         0,
         (MaxZ * math.random ()) + MinZ,
      })
      dmz.object.orientation (
         m.object,
         nil,
         dmz.matrix.new (Up, math.random () * dmz.math.TwoPi))
      dmz.object.activate (m.object)
      dmz.object.set_temporary (m.object)
      mites[#mites + 1] = m
      m.nextTurn = calc_next_turn_time ()
   end
   while #mites > count do
      dmz.object.destroy (mites[#mites].object)
      mites[#mites] = nil
   end
end

local function update_area_minimum (self, object, handle, min)
   self.log:error ("Setting arena min:", min)
   self.arena.min = min
end

local function update_area_maximum (self, object, handle, max)
   self.log:error ("Setting arena max:", max)
   self.arena.max = max
end

local function update_mite_speed (self, object, handle, speed)
   self.log:error ("speed:", speed)
   self.speed = speed
end

local function start (self)
   local callbacks = { update_object_position = update_area_minimum, }
   self.objObs:register ("Minimum_Area", callbacks, self)
   callbacks = { update_object_position = update_area_maximum, }
   self.objObs:register ("Maximum_Area", callbacks, self)
   callbacks = { update_object_counter = update_mite_count, }
   self.objObs:register ("Mites", callbacks, self)
   callbacks = { update_object_scalar = update_mite_speed, }
   self.objObs:register ("Speed", callbacks, self)
   self.tsHandle = self.timeSlice:create (update_mites, self, self.name)
end

local function stop (self)
   if self.tsHandle then self.timeSlice:destroy (self.tsHandle) self.tsHandle = nil end
end

function new (config, name)
   local self = {
      start_plugin = start,
      stop_plugin = stop,
      timeSlice = dmz.time_slice.new (),
      log = dmz.log.new ("lua." .. name),
      objObs = dmz.object_observer.new (),
      arena = {
         min = dmz.vector.new (-3000, 0, -2000),
         max = dmz.vector.new (3000, 0, 2000),
      },
      speed = 3000,
      mites = {},
   }

   self.log:info ("Creating plugin: " .. name)

   return self
end
