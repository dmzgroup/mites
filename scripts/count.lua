require "const"
local Offset = dmz.vector.new (0, 0, -96)
local UnitMatrix = dmz.matrix.new ()

local local_object_find = dmz.object.find
local local_object_type = dmz.object.type
local local_object_counter = dmz.object.counter
local local_object_position = dmz.object.position

local function find_chip_count (self, chips, object, pos)
   local result = {object}
   self.volume:set_origin (pos)
   local net = local_object_find (self.volume)
   if net then
      for _, chip in ipairs (net) do
         if (object ~= chip) and not chips[chip] then 
            local type = local_object_type (chip)
            if type and type:is_of_type (const.ChipType) then
               chips[chip] = true
               result[#result + 1] = chip
            end
         end
      end
   end
   return result
end

local function update_chips (self)
   local chips = {}
   local clusters = {}
   for chip, valid in pairs (self.chips) do
      if not chips[chip] then
         chips[chip] = true
         clusters[#clusters + 1] =
            find_chip_count (self, chips, chip, local_object_position (chip))
      end
   end
   table.sort (clusters, function (c1, c2) return #c1 > #c2 end)
   for index, cluster in ipairs (clusters) do
      for _, chip in ipairs (cluster) do
         local_object_counter (chip, const.CountHandle, index)
      end
   end
end

local function create_object (self, object, type)
   if type:is_of_type (const.ChipType) then
      self.chip = nil
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

   self.volume:set_radius (750) --256)

   self.log:info ("Creating plugin: " .. name)

   return self
end
