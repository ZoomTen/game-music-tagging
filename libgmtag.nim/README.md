# libgmtag.nim

This is another (probably stabler?) implementation of the proposed [Game Tag Format (tentative name)](../README.md) for existing sequenced music rips. It is written in Nim, however it compiles to a static library that can be linked with your project.

## Function index

Refer to the [function index of the C++ implementation](../libgmtag/README.md).

All return values are a `ptr` (untraced reference) in Nim's point of view so as to not confuse the GC. From the perspective of others, it is just a `void *`.

This library is able to be imported from other Nim applications, however the above should be kept in mind.

Exceptions/defects should not happen in normal operation. When used from Nim, they will be converted into generic `ref CatchableErrors`, but when used from other languages will fall back into ignoring the current operation (`tags_from_buffer`) or returning a sentinel error value `get_*`.

## Building

```sh
nimble build
```

You should end up with a `libgmtag.a` for your platform.
