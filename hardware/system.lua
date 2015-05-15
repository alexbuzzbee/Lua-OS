local system = {}

local devices = {}

function system.getDevice(port)
  return devices[port]
end

function system.getDevices()
  local devs
  for i,v in devices do
    devs[i] = v
  end
  return devs
end

function system.addDevice(device)
  table.insert(devices, device)
end

function system.removeDevice(port)
  if devices[port] == nil then
    return false
  end
  devices[port] = nil
  return true
end

return system
