_G.sys = require("system")
local util = require("util")
local hardware = require("hardware")
local serial = require("serial")

_G._LUAOSVER = 0.1

local function createSandbox()
  local sandbox = {
    coroutine = coroutine
    string = string
    table = table
    math = math
    bit32 = bit32
    assert = assert
    error = error
    getmetatable = getmetatable
    ipairs = ipairs
    load = load
    next = next
    pcall = pcall
    rawequal = rawequal
    rawget = rawget
    rawlen = rawlen
    rawset = rawset
    select = select
    setmetatable = setmetatable
    tonumber = tonumber
    tostring = tostring
    type = type
    _VERSION = _VERSION
    _HARDWAREVERSION = _LUAOSVER
    xpcall = xpcall
    hardware = hardware
  }
  return sandbox
end

local function main(...)
  local args = {...}
  local firmware = ""
  if args[1] ~= nil then
    util.chdir(args[1])
  end
  print("Loading firmware...") -- Load system firmware.
  local firmware = util.loadfile("firmware.lua")
  print(firmware) -- Debugging.
  local firmwareFunc, reason = load(firmware, "firmware", "t", createSandbox()) -- Load the firmware as text into the sandbox, with the source "firmware" for stack traces.
  if firmwareFunc == nil then
    print("FATAL ERROR: firmware does not compile/load because " .. reason)
    hardware.shutdown()
    os.exit(1)
  end
  print("Loading configuration...") -- Load system configuration.
  local configText = util.loadfile("luaos.cfg")
  local config, reason = serial.unserialize(configText) -- Parse configuration.
  if config == nil then
    print("FATAL ERROR: can't parse configuration because " .. reason)
    hardware.shutdown()
    os.exit(1)
  end
  hardware.init(config) -- Initialize hardware from configuration.
  print("Lua OS version " .. _LUAOSVER .. ", lua version " .. _VERSION .. ".")
  pcall(firmwareFunc) -- Run the firmware; if it returns or errors, print message and shut down.
  util.clearscr()
  print("FATAL ERROR: software crashed or failed to shut down system")
  hardware.shutdown()
  os.exit()
end

main()
