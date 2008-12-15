require "const"
local Offset = dmz.vector.new (0, 0, -96)
local UnitMatrix = dmz.matrix.new ()

local function find_chip_count (self, object, pos)
   local result = 0
   self.volume:set_origin (pos)
   local net = dmz.object.find (self.volume)
   if net then
      for _, chip in ipairs (net) do
         if object ~= chip then 
            local type = dmz.object.type (chip)
            if type and type:is_of_type (const.ChipType) then
               result = result + 1
            end
         end
      end
   end
   return result
end

local function update_chips (self)
   for chip, _ in pairs (self.chips) do
      local count = find_chip_count (self, chip, dmz.object.position (chip))
      dmz.object.counter (chip, const.CountHandle, count)
   end
end

local function create_object (self, object, type)
   if type:is_of_type (const.ChipType) then
      self.chips[object] = true
   end
end

local function destroy_object (self, object)
   self.chips[object] = nil
end

local function start (self)
   self.objObs:register (
      nil,
      { create_object = create_object, destroy_object = destroy_object, },
      self)
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
      chips = {},
      volume = dmz.sphere.new (),
   }

   self.volume:set_radius (500) --256)

   self.log:info ("Creating plugin: " .. name)

   return self
end
