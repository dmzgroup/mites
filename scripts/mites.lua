
local function start (self)

   self.mite = dmz.object.create ("mite")
   dmz.object.position (self.mite, nil, {0, 0, 0})
   dmz.object.activate (self.mite)

end

function new (config, name)

   local self = {
      start_plugin = start,
      log = dmz.log.new ("lua." .. name),
   }

   self.log:info ("Creating plugin: " .. name)

   return self

end
