_G.sys = require("system")
util = require("util")
hardware = require("hardware")
serial = require("serial")

_G._LUAOSVER = 0.1

local function createSandbox()
  local sandbox = {}
  sandbox.coroutine = coroutine
  sandbox.string = string
  sandbox.table = table
  sandbox.math = math
  sandbox.bit32 = bit32
  sandbox.assert = assert
  sandbox.error = error
  sandbox.getmetatable = getmetatable
  sandbox.ipairs = ipairs
  sandbox.load = load
  sandbox.next = next
  sandbox.pcall = pcall
  sandbox.rawequal = rawequal
  sandbox.rawget = rawget
  sandbox.rawlen = rawlen
  sandbox.rawset = rawset
  sandbox.select = select
  sandbox.setmetatable = setmetatable
  sandbox.tonumber = tonumber
  sandbox.tostring = tostring
  sandbox.type = type
  sandbox._VERSION = _VERSION
  sandbox.xpcall = xpcall
  sandbox.hardware = hardware
  return sandbox
end

local function main(...)
  local args = {...}
  local firmware = ""
  if args[1] ~= nil then
    util.chdir(args[1])
  end
  sys.init()
  print("Loading firmware...") -- Load system firmware.
  local firmware, reason = util.loadfile("firmware.lua")
  if firmware == "" then
    print("FATAL ERROR: couldn't load firmware because " .. reason)
    hardware.shutdown()
    os.exit(1)
  end
  local firmwareFunc, reason = load(firmware, "firmware", "t", createSandbox()) -- Load the firmware as text into the sandbox, with the source "firmware" for stack traces.
  if firmwareFunc == nil then
    print("FATAL ERROR: firmware does not compile because " .. reason)
    hardware.shutdown()
    os.exit(1)
  end
  print("Loading configuration...") -- Load system configuration.
  local config, reason = serial.unserialize(util.loadfile("luaos.cfg")) -- Parse configuration.
  if config == nil then
    print("FATAL ERROR: can't parse configuration because " .. reason)
    hardware.shutdown()
    os.exit(1)
  end
  hardware.init(config) -- Initialize hardware from configuration.
  print("Lua OS version" .. _LUAOSVER .. ", lua version " .. _VERSION .. ".")
  pcall(firmwareFunc()) -- Run the firmware; if it returns or errors, print message and shut down.
  util.clearscr()
  print("FATAL ERROR: software crashed or failed to shut down system")
  hardware.shutdown()
  os.exit()
end

main()
