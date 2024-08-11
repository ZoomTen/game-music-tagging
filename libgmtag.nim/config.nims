import os

if projectPath() == thisDir() / "src" / "libgmtag.nim" :
  switch("app", "staticlib")
  switch("define", "useMalloc")
  switch("define", "nimPreviewSlimSystem")
  switch("define", "release")
  switch("mm", "arc")
  switch("threads", "off")
  switch("panics", "on")
  switch("nimcache", "/mnt/scrapdisk/aaaa")
else:
  switch("define", "test")
