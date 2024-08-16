import os

switch("define", "useMalloc")
switch("mm", "arc")
switch("threads", "off")

if projectPath() == thisDir() / "src" / "libgmtag.nim":
  switch("app", "staticlib")
  switch("define", "nimPreviewSlimSystem")
  switch("define", "danger")
  switch("define", "isLibrary")
  switch("panics", "on")
  switch("nimcache", "/mnt/scrapdisk/aaaa")
else:
  switch("define", "test")
