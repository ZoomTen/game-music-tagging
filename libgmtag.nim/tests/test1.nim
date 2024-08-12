# This is just an example to get you started. You may wish to put all of your
# tests into a single file, or separate them into multiple `test1`, `test2`
# etc. files (better names are recommended, just make sure the name starts with
# the letter 't').
#
# To run these tests, simply execute `nimble test`.

import unittest2
import libgmtag
import pretty

suite "Katakis 3D m3u":
  setup:
    const m3uStr = staticRead("samples/katakis_3d.m3u")
    var tags: ptr TagContainer

  test "parse and read":
    skip()
    tags = tagsFromBuffer(m3uStr)
  
  teardown:
    tags.unsetTags()

suite "Badly-formatted m3u":
  setup:
    const m3uStr = staticRead("samples/weird.m3u")

  test "parse and read":
    skip()
    #debugEcho m3uStr

suite "Spec check":
  setup:
    const m3uStr = staticRead("samples/spectest.m3u")

  test "parse and read":
    skip()
    #debugEcho m3uStr
