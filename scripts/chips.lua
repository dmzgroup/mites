require "const"
local Offset = dmz.vector.new (0, 0, -96)
local UnitMatrix = dmz.matrix.new ()

local local_position = dmz.object.position
local local_orientation = dmz.object.orientation
local local_super_links = dmz.object.super_links

local function get_mite (object)
   local result = nil
   local links = local_super_links (object, const.LinkHandle)
   if links then result = links[1] end
   return result
end

local function update_chips (self, time)
   for i, chip in ipairs (self.chips) do
      local mite = get_mite (chip.object)
      if mite then
         local pos = local_position (mite)
         local ori = local_orientation (mite)
         local_position (chip.object, nil, pos + ori:transform (Offset))
      end
   end
end

local function update_chip_count (self, object, handle, count)
   --self.log:error ("Chips:", count)
   local min = dmz.object.position (object, "Minimum_Area")
   local max = dmz.object.position (object, "Maximum_Area")
   local chips = self.chips
   local MinX = min:get_x ()
   local MaxX = max:get_x () - MinX
   local MinZ = min:get_z ()
   local MaxZ = max:get_z () - MinZ
   while #chips < count do
      local chip = {}
      chip.object = dmz.object.create (const.ChipType)
      dmz.object.position (chip.object, nil, {
         (MaxX * math.random ()) + MinX,
         0,
         (MaxZ * math.random ()) + MinZ,
      })
      dmz.object.orientation (chip.object, nil, UnitMatrix)
      dmz.object.activate (chip.object)
      dmz.object.set_temporary (chip.object)
      chips[#chips + 1] = chip
   end
   while #chips > count do
      dmz.object.destroy (chips[#chips].object)
      chips[#chips] = nil
   end
end

local function start (self)
   self.objObs:register ("Chips", { update_object_counter = update_chip_count, }, self)
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
   }

   self.log:info ("Creating plugin: " .. name)

   return self
end
