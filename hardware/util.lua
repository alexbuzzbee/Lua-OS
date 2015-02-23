local util = {}

local baseDir = "." -- Base directory for all file/directory acceses. Workaround for Lua not having chdir or setenv.

function util.chdir(newDir)
  baseDir = newDir
end

function util.loadfile(filename)
  f, reason = io.open(baseDir .. filename, "r")
  if f == nil then
    return nil, reason
  end
  return f:read("*a")
end

function util.dumpfile(filename, data)
  f, reason = io.open(baseDir .. filename, "w")
  if f == nil then
    return nil, reason
  end
  return f:write(data)
end

function util.clearscr()
  io.write("\027[H\027[2J")
end

return util
