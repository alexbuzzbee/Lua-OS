system = {}

local devices = {}

function system.getDevice(port)
  return devices[port]
end

function system.addDevice(device)
  table.insert(devices, device)
end

function system.removeDevice(port)
  devices[port] = nil
end
