# NSFe

[Original link](https://www.nesdev.org/w/index.php?title=NSFe&oldid=20474)

**NSFe** is the **Extended Nintendo Sound Format** created by Disch and
popularized by the [NotSoFatso](https://disch.zophar.net/notsofatso.php)
NSF player. It is based on the original [NSF](NSF "wikilink") file
format, but its data is organized differently and is not backward
compatible with it.

The primary goal of NSFe was to add easily extensible metadata that the
NSF format could not accommodate, such track names and times.

NSFe's chunky metadata has subsequently been incorporated into the
[NSF2](NSF2 "wikilink") format.

All integers use a little-endian format.

## Structure

The NSFe begins with a four byte header, containing the four characters
'NSFE'.

`offset  # of bytes   Function`  
`----------------------------`  
`$0000   4   FOURCC   'N','S','F','E' (header)`

After the header, a series of chunks will appear.

UTF-8 encoding is recommended for all strings used by this format.

## Chunks

All chunks have the same primary structure:

`offset  # of bytes   Function`  
`----------------------------`  
`$0000   4   DWORD    Length of chunk data (n). Does not include the chunk's 8 byte header.`  
`$0004   4   FOURCC   Four character chunk ID.`  
`$0008   n   ----     Chunk data of specified length.`

There are three chunks that are required for a well formed NSFe:

-   'INFO' - Similar to an NSF header, must appear before 'DATA' chunk.
-   'DATA' - Raw ROM data.
-   'NEND' - Last NSFe chunk in file.

Note the 'INFO' chunk must precede the 'DATA' chunk, and the 'NEND'
chunk marks the end of the file; no further chunks should be read past
'NEND'.

If the first byte of a chunk's FourCC ID is a capital letter (i.e. 'A'
to 'Z'), it indicates that this chunk is mandatory, and if the NSFe
player cannot read this type of chunk it should not attempt to play the
file. All other chunks are considered optional, and may be skipped by
the NSFe player if necessary.

The NSFe format does not have a version number. Future revisions of the
format, if necessary, can be accomplished with new mandatory chunk
types.

### INFO

This required chunk describes the music data contained. This is similar
to the NSF header.

`offset  # of bytes   Function`  
`----------------------------`  
`$0000   2   WORD    (lo, hi) load address of data ($8000-FFFF)`  
`$0002   2   WORD    (lo, hi) init address of data ($8000-FFFF)`  
`$0004   2   WORD    (lo, hi) play address of data ($8000-FFFF)`  
`$0006   1   BYTE    PAL/NTSC bits`  
`                bit 0: if clear, this is an NTSC tune`  
`                bit 0: if set, this is a PAL tune`  
`                bit 1: if set, this is a dual PAL/NTSC tune`  
`                bits 2-7: not used. they *must* be 0`  
`$0007   1   BYTE    Extra Sound Chip Support`  
`                bit 0: if set, this song uses `[`VRC6 audio`](VRC6_audio "wikilink")  
`                bit 1: if set, this song uses `[`VRC7 audio`](VRC7_audio "wikilink")  
`                bit 2: if set, this song uses `[`FDS audio`](FDS_audio "wikilink")  
`                bit 3: if set, this song uses `[`MMC5 audio`](MMC5_audio "wikilink")  
`                bit 4: if set, this song uses `[`Namco 163 audio`](Namco_163_audio "wikilink")  
`                bit 5: if set, this song uses `[`Sunsoft 5B audio`](Sunsoft_5B_audio "wikilink")  
`                bit 6: if set, this song uses `[`VT02+ audio`](VT02+_Sound "wikilink")  
`                bit 7: future expansion: *must* be 0`  
`$0008   1   BYTE    Total songs   (1=1 song, 2=2 songs, etc)`  
`$0009   1   BYTE    Starting song (0=1st song, 1=2nd song, etc)`

Note the following differences from NSF:

-   Title, author, and copyright information is not mandatory. See the
    'auth' chunk for details.
-   If bankswitching is required, it is specified in a subsequent 'BANK'
    chunk.
-   The play routine will be called at the NMI rate for the specified
    system (NTSC or PAL) unless a 'RATE' chunk is used.
-   The first song in the NSFe is 0, unlike NSF which begins with 1.

This chunk must be at least 9 bytes long. If starting song is missing, 0
may be assumed. Any extra data in this chunk may be ignored.

This chunk may not be used in [NSF2](NSF2 "wikilink") metadata.

### DATA

This chunk contains the raw ROM data. It will be placed at the load
address specified by the 'INFO' chunk, unless bankswitched (the same as
in NSF). There is no minimum length for this chunk, but data past 1MB is
unusable due to the bank switching mechanisms of the NSF mapper.

This chunk may not be used in [NSF2](NSF2 "wikilink") metadata.

### NEND

This chunk may be any length, but any contained data may be ignored.
Typically it will have 0 length.

Any data following the 'NEND' chunk will be ignored.

### BANK

If the bank chunk is present, this NSFe should use bankswitching, and it
contains 8 bytes specifying the 8 banks to initialize with. See
[NSF](NSF "wikilink") for bankswitching details.

If this chunk is less than 8 bytes, presume 0 for the missing bytes. If
longer, ignore the extra bytes.

This chunk may not be used in [NSF2](NSF2 "wikilink") metadata.

### RATE

`offset  # of bytes   Function`  
`----------------------------`  
`$0000   2   WORD    (lo, hi) Play speed, in 1/1000000th sec ticks, NTSC`  
`$0002   2   WORD    (lo, hi) Play speed, in 1/1000000th sec ticks, PAL (Optional)`  
`$0004   2   WORD    (lo, hi) Play speed, in 1/1000000th sec ticks, Dendy (Optional)`

This chunk has 4 bytes. If present, the NSFe should use a custom rate
for calling the PLAY routine. See equivalent in [NSF](NSF "wikilink")
header at $06E and $078.

If the NSFe uses the default video refresh rates for PLAY, this chunk
should be omitted for compatibility with older players.

If the Dendy speed is omitted, any Dendy playback should use the PAL
speed. (If the PAL speed is omitted, its default rate should be used.)
Use with the **[regn](NSFe#regn "wikilink")** chunk to enable Dendy
playback.

<small>(Added 2018-1-2)</small>

### NSF2

A 1 byte chunk containing the equivalent of the [NSF2](NSF2 "wikilink")
flags byte from its header at offset $7C. This permits the use of NSF2
features from an NSFe file.

This chunk may not be used in [NSF2](NSF2 "wikilink") metadata.

<small>(Added 2019-3-2)</small>

### VRC7

The first byte designates a variant device replacing the VRC7 at the
same register addresses.

-   0 = VRC7
-   1 = YM2413

The next 128 or 152 bytes are optional, and contain a replacement patch
set for the device. The first 8 bytes of this patch set are expected to
be zero, since on the VRC7 patch 0 is custom-only. The larger 152-byte
version of the patch set may additionally customize the rhythm mode
instruments of YM2413 (not accessible on VRC7).

If a replacement patch set is not contained in this chunk, an
appropriate default patch set should be used for the selected device.

<small>(Added 2019-3-12)</small>

### plst

This optional chunk specifies the order in which to play tracks. Each
byte specifies a track from the NSFe (note that the first track in the
NSFe is track 0).

The length of the playlist is specified by the length of this chunk. It
is not required to play all tracks, and they may be duplicated.

A player should normally stop when the playlist is finished. It may be
useful to allow the user to turn off the playlist and select tracks from
the NSFe manually.

### psfx

This optional chunk contains a tracks which are "sound effects" rather
than music. Each byte specifies a track from the NSFe (note that the
first track in the NSFe is track 0).

The length of this list is specified by the length of this chunk.
Duplicate tracks may appear.

A player implementation may treat this as an optional alternate playlist
for sound effects, like the 'plst' chunk above, where the 'plst' is the
music only playlist, and 'psfx' is the sound effects playlist. It can
also be treated as adding a "sound effect" flag to every listed track.
(As a result, the user could be given the option not to listen to sound
effects.)

<small>(Added 2018-8-25)</small>

### time

This optional chunk contains a list of 4 byte signed integers. Each
integer represents the length in milliseconds for the corresponding NSFe
track. Note this is unrelated to the 'plst' chunk; the times are for the
tracks in the NSFe, not the tracks int he playlist. When the track has
played for the specified time, it should begin fading out. A time of 0
is valid, and should begin fadeout immediately. A time of less than 0
represents the "default" time, which should be handled accordingly by
the player.

This chunk should come after the 'INFO' chunk, so that the number of
tracks is already known.

If this chunk is not long enough to specify all tracks in the NSF, a
default time should be assumed for these tracks. Any extra data should
be ignored.

### fade

This optional chunk contains a list of 4 byte signed integers. It is
like the 'time' chunk, but instead specifies a fadeout length in
milliseconds for each track. A fade time of 0 means the track should
immediately end rather than fading out. A fade time of less than 0
represents the default fade time.

This chunk should come after the 'INFO' chunk, so that the number of
tracks is already known.

If this chunk is not long enough to specify all tracks in the NSF, a
default time should be assumed for these tracks. Any extra data should
be ignored.

### tlbl

This optional chunk contains names for each track (tlbl = track label).
This contains a series of null-terminated strings, in track order. (Like
'time' and 'fade', this is not related to the 'plst' chunk's playlist.)

This chunk should come after the 'INFO' chunk, so that the number of
tracks is already known.

If there are not enough strings for every track, subsequent tracks
should receive a default label (or no label) according to the player.
Extra strings can be ignored.

### taut

This optional chunk contains an author name for each track (taut = track
author), useful if several composers contributed to this NSF. This
contains a series of null-terminated strings, in track order. (Not
related to 'plst' order.)

This chunk should come after the 'INFO' chunk, so that the number of
tracks is already known.

If there are not enough strings for every track, subsequent tracks
should receive an empty author. Extra strings can be ignored.

<small>(Added 2018-8-25)</small>

### auth

This optional chunk contains four null-terminated strings describing in
order:

1.  Game title
2.  Artist
3.  Copyright
4.  Ripper

If not all strings are present, a default string (typically "\<?\>") can
be assumed by the player.

### text

This optional chunk contains a single null-terminated string of any
length. This may include a description of the contents, or any message
from the author.

Newlines may either be CR+LF or LF, as commonly found in text files.

<small>(Added 2012-12-16)</small>

### mixe

`offset  # of bytes   Function`  
`----------------------------`  
`$0000   1   BYTE    Specify output device (see below).`  
`$0001   2   WORD    Signed 16-bit integer specifying millibels (1/100 dB) comparison with APU square volume`

This optional chunk contains a list of output devices, and mixing
information for each of them.

The comparison specifies relatively how loud a square wave played on the
device at maximum volume (or other suitable waveform) compared to one at
the same frequency from the built-in APU square channel. Any omitted
device should instead use a default mix.

The primary use of this is for [N163](Namco_163_audio "wikilink") games,
which mix the expansion audio at different levels on a per-game basis.
For other expansion audio there is no evidence of variation on that
basis, so this chunk should not be used if the default mix is
appropriate for the rip.

The secondary use is for homebrew music, especially multi-chip NSF,
where the user may wish to be specific about the mix. For this reason
all expansions are accessible here.

Device byte values:

-   0 - APU Squares - Default: 0
-   1 - APU Triangle / Noise / DPCM - Comparison: Triangle - Default:
    -20
-   2 - VRC6 - Default: 0
-   3 - VRC7 - Comparison: Pseudo-square - Default: 1100
-   4 - FDS - Default: 700
-   5 - MMC5 - Default: 0
-   6 - N163 - Comparison: 1-Channel mode - Default: 1100 or 1900
-   7 - Sunsoft 5B - Comparison: Volume 12 ($C) - Default: -130

Reference test ROMs:
[<https://github.com/bbbradsmith/nes-audio-tests>](https://github.com/bbbradsmith/nes-audio-tests)

Reference test results: \[//forums.nesdev.org/viewtopic.php?f=2&t=17741
Forum: Audio levels survey, expansion audio etc.\]

These comparisons should be done at a matched frequency, close to
A440Hz, using a single channel at its maximum volume, playing a square
wave if possible. Level comparisons, particularly for non-square waves,
are based on RMS amplitude.

-   Device 1 (APU triangle / noise / DPCM) uses a triangle instead of a
    square wave because the nonlinear mix and lack of triangle phase
    control makes it the most deterministic available volume. For this
    comparison, noise is silenced and $4011 is set to 0.
-   Device 3 (VRC7) uses a "pseudo square" FM instrument with the
    modulator at a 2:1 ratio and 50% modulation strength with full
    feedback (instrument data: $22, $21, $20, $07, $F0, $F0, $0F, $0F).
-   Device 6 (N163) is compared in 1-channel mode.
-   Device 7 (5B) is compared using only volume 12 ($C) due to
    nonlinearity of its internal amplifier at higher volumes.

All values are relative to the APU squares at their default volume. If
you set a different volume for device 0, other devices are still
relative to the default volume, not the new one.

<small>(Added 2018-8-6)</small>

### regn

`offset  # of bytes   Function`  
`----------------------------`  
`$0000   1   BYTE    Bitfield of supported regions.`  
`                bit 0: NTSC`  
`                bit 1: PAL`  
`                bit 2: Dendy`  
`                bit 3-7: Reserved (always clear)`  
`$0001   1   BYTE    Specifies preferred region if multiple regions are supported. (Optional)`  
`                0 - NTSC`  
`                1 - PAL`  
`                2 - Dendy`

This chunk adds support for the Dendy region, and the ability to specify
which region is preferred (for players that can support multiple
regions). This chunk should appear after the INFO chunk, and overrides
its region data (byte 6).

If this chunk is present, and Dendy region is chosen by the player, the
X register should be set to 2 for the INIT call. If otherwise
unspecified, Dendy playback will use the PAL rate (see
[RATE](NSFe#RATE "wikilink") chunk above).

<small>(Added 2018-8-21)</small>

## Players

-   [Audio Overload](https://www.bannister.org/software/ao.htm) - Multi
    format player. Windows, Mac Linux.
-   [Game Emu Player](http://foobar2000.org/components/view/foo_gep) -
    Multi format player. Foobar2000 plugin.
-   [Cog](https://cogx.org/) - Multi format player. Macintosh.
-   [VLC Player](https://www.videolan.org/vlc/) - Multi format video and
    music player. Windows, Mac, Linux, Android, iOS.
-   [NSFPlay](https://bbbradsmith.github.io/nsfplay/) - NSF/NSFe player.
    Windows, Winamp plugin.
-   [Mesen](https://www.mesen.ca/) - NES emulator, but can play NSF/NSFe
    files. Windows, Mac, Linux.
-   [NotSoFatso](https://disch.zophar.net/notsofatso.php) - The original
    NSFe player. Winamp plugin.

## References

-   [NSFe specification by Disch, revision 2.
    (9/3/2003)](https://web.archive.org/web/20070317084821/http://slickproductions.org/docs/NSF/nsfespec.txt)
-   [NSFe Archive](https://disch.zophar.net/nsfe.php) - a collection of
    NSFe files by Disch.

[Category:Audio](Category:Audio "wikilink") [Category:File
formats](Category:File_formats "wikilink")