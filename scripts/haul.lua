require "const"
local Offset = dmz.vector.new (0, 0, -96)
local UnitMatrix = dmz.matrix.new ()

local local_object_position = dmz.object.position
local local_object_find = dmz.object.find
local local_object_type = dmz.object.type
local local_object_super_links = dmz.object.super_links
local local_object_unlink = dmz.object.unlink
local local_object_link = dmz.object.link
local local_object_time_stamp = dmz.object.time_stamp

local function find_nearest_chip (self, pos)
   local result = nil
   self.volume:set_origin (pos)
   local net = local_object_find (self.volume)
   if net then
      local done = false
      local count = 1
      while not done do
         local object = net[count]
         if object then
            local type = local_object_type (object)
            if type and type:is_of_type (const.ChipType) then
               local links = local_object_super_links (object, const.LinkHandle)
               if not links then
                  result = object
                  done = true
               end
            end
            count = count + 1
         else done = true
         end
      end
   end
   return result
end

local function update_chips (self, time)
   if not self.paused then
      local CTime = dmz.time.frame_time ()
      for object, mite in pairs (self.mites) do
         local Timer = local_object_time_stamp (object, const.TimerHandle)
         if not Timer then Timer = CTime end
         if Timer <= CTime then
            local chip = find_nearest_chip (self, local_object_position (object))
            if chip then
               if mite.link then
                  local_object_unlink (mite.link)
                  mite.link = nil
               else mite.link = local_object_link (const.LinkHandle, object, chip)
               end
               local_object_time_stamp (object, const.TimerHandle, CTime + self.wait)
            end
         end
      end
   end
end

local function create_object (self, object, type)
   if type:is_of_type (const.MiteType) then
      self.mites[object] = {}
   end
end

local function destroy_object (self, object)
   self.mites[object] = nil
end

local function update_wait (self, object, handle, wait)
   self.wait = wait
end

local function update_paused (self, object, handle, value)
   self.paused = value
end

local function start (self)
   self.objObs:register (
      nil,
      { create_object = create_object, destroy_object = destroy_object, },
      self)
   self.objObs:register ("Wait", { update_object_scalar = update_wait, }, self)
   self.objObs:register ("Pause", { update_object_flag = update_paused, }, self)
   self.tsHandle = self.timeSlice:create (update_chips, self, self.name)
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
      mites = {},
      volume = dmz.sphere.new (),
      wait = 1,
      paused = false,
   }

   self.volume:set_radius (256)

   self.log:info ("Creating plugin: " .. name)

   return self
end
