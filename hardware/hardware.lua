local terminal = require("terminal")

local hardware = {}
local devTypes = { -- Device creation functions. Proto device(conf: table): table {type: string, iface: table}
  terminal=terminal.init
}

function hardware.getDevice(port)
  local device = sys.getDevice(port) -- Get device from system.
  if device == nil then -- Error if no such device.
    return nil, "empty port"
  end
  return device.iface -- Return device interface.
end

function hardware.getPortsWithType(type)
  local list = {}
  for i,v in system.getDevices() do
    if v.type == type then
      table.insert(list, v)
    end
  end
  return list
end

function hardware.shutdown()
  local i = 0
  while true do -- Detatch all devices.
    i = i + 1
    if sys.removeDevice(i) == false then
      break
    end
  end
  os.exit()
end

function hardware.init(config)
  for i,v in ipairs(config.devices) do
    if devTypes[v.type] ~= nil then
      local dev = devTypes[v.type]
      sys.addDevice(dev(v.conf))
    else
      print("WARNING: Invalid device type " .. v.type .. ", numbered " .. i .. ".")
    end
  end
end

return hardware
