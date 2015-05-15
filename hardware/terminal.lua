local util = require("util")

local terminal = {}
terminal.template = {}

function terminal.template.print(str)
  io.write(str)
end

function terminal.template.read(length)
  if length == nil then
    return io.read("*line")
  else
    return io.read(length)
  end
end

function terminal.template.clear()
  util.clearscr()
end

function terminal.init(config)
  local interface = {}
  for i,v in ipairs(terminal.template) do
    interface[i] = v
  end
  return {type="terminal", iface=interface}
end

return terminal
