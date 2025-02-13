target('FontEnumeration')
  set_kind('binary')
  add_files("./FontEnumeration.cpp")
  add_defines("UNICODE", "_UNICODE")
  add_links("user32", "Shlwapi", "shcore", "Shell32", "d2d1", "dwrite")
  if is_plat('windows') then
    add_cxflags("/utf-8")
  elseif is_plat('mingw') then
    add_ldflags('-static-libgcc -static-libstdc++ -static', {force=true})
    add_ldflags("-municode", {force = true})
  end
  on_load(function(target)
    target:set("filename", "FontEnumeration."..target:arch()..".exe")
  end)

  after_build(function(target)
    print("copy " .. path.join(target:targetdir(), target:filename()) .. " to $(projectdir)")
    os.trycp(path.join(target:targetdir(), target:filename()), "$(projectdir)")
  end)
