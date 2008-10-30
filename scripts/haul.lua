require "const"
local Offset = dmz.vector.new (0, 0, -96)
local UnitMatrix = dmz.matrix.new ()

local function find_nearest_chip (self, pos)
   local result = nil
   self.volume:set_origin (pos)
   local net = dmz.object.find (self.volume)
   if net then
      local done = false
      local count = 1
      while not done do
         local object = net[count]
         if object then
            local type = dmz.object.type (object)
            if type and type:is_of_type (const.ChipType) then
               local links = dmz.object.super_links (object, const.LinkHandle)
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
   local CTime = dmz.time.frame_time ()
   for object, mite in pairs (self.mites) do
      local Timer = dmz.object.time_stamp (object, const.TimerHandle)
      if not Timer then Timer = CTime end
      if Timer <= CTime then
         local chip = find_nearest_chip (self, dmz.object.position (object))
         if chip then
            if mite.link then
               dmz.object.unlink (mite.link)
               mite.link = nil
            else mite.link = dmz.object.link (const.LinkHandle, object, chip)
            end
            dmz.object.time_stamp (object, const.TimerHandle, CTime + self.wait)
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

local function start (self)
   self.objObs:register (
      nil,
      { create_object = create_object, destroy_object = destroy_object, },
      self)
   self.objObs:register ("Wait", { update_object_scalar = update_wait, }, self)
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
   }

   self.volume:set_radius (256)

   self.log:info ("Creating plugin: " .. name)

   return self
end
