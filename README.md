# Proposal for a new tags format for sequenced game sound rips

This document is a proposal to adapt vgmstream's `!tags.m3u` format to older sequenced formats to better standardize sequenced game sound rips.

## Abstract

### From a playback standpoint

Tagging is a long standing issue that has plagued old sequenced rips for over the last 20 years. Typically rips for NSF, GBS, etc. are shared in standalone files. You are forced cycle through the subtunes to find that one song you want to listen to. Additionally, they have a default time like 5 minutes or 3 minutes. There's a few formats like NSFE that address the time issue (but vary in granularity), but each format requires a custom tool, a new format (some of which die an unceremonious death like GBSX), and still don't tackle the issue of subtune cycling.

One solution to this problem was M3U tagging. A fine solution until you see the actual contents of the M3U files. This is very close to a perfect solution with a major problem. Each song has metadata shoved into one line to conform to winamp's (and I would assume others') limitations. When a song it played in winamp from a M3U file, it is only aware of a single line from the M3U file. It's not aware of the comments. Additionally, the individual M3U files for songs are not aware of the metadata in the playlist M3U file so the metadata must be duplicated. Hence when plugin author's resorted to variants of the format you see above and the formats different ever so slightly between plugins. It's a major tech debt item that authors have simply worked around. But in the end, it does the job. However, taggers pay the price because the format is not intuitive.

`!tags.m3u` is a format from the [vgmstream](https://github.com/vgmstream/vgmstream/blob/master/doc/USAGE.md#tagging) library, the definitive streamed music library. There's a plethora of streamed formats so most stream rips resort to `!tags.m3u` and `.txtp` for extra metadata for all streamed formats. `!tags.m3u` is a static filename that is located at the base of the rip and contains metadata for all the rips in one file.

(Original post: https://github.com/libgme/game-music-emu/issues/85)

### From a preservation standpoint

Many sequenced sound rip formats (NSF, GBS, HES, ...) have suffered from rag-tag tagging and limited facilities from the formats themselves. The only other option being to give up and just use VGM, especially as the VGM format itself over time adds support for more and more sound chips.

Sequenced sound rips are advantageous over the VGM format for many reasons, one of which being preservation of **raw** sound data potentially including accurate tempo information and sound engine code, lending itself well to uses beyond simple playback. Being a simple register log at a specified sample rate, it is difficult to do the same with VGM. Thus VGM is only perfectly suited for playback, and not manipulation. Given VGM's more complete tagging format and the relative ease in which they can be generated, it incentivizes more rips done in this format rather than the native format of the game. This proposal wants to fix one of those two problems, and to fill the void left open by vgmstream's design:

> vgmstream's internals are tailored to play streams so, in other words, it's not possible to add support for sequenced audio unless massive changes were done, basically becoming another program entirely. There are other projects better suited for playing sequences.

## Review of existing extended tagging formats

### Embedded

<dl>
  <dt>VGM / VGZ (multi-system, usually Sega)</dt>
  <dd>GD3 format, analogous to MP3’s ID3; see https://vgmrips.net/wiki/GD3_Specification<br>One file each track.</dd>

<dt>SPC / RSN (SNES)</dt>
  <dd>ID666 and XID6; see https://wiki.superfamicom.org/spc-and-rsn-file-format<br>One file each track.</dd>

<dt>SNSF (SNES)</dt>
  <dd>Implements PSF-style tagging; see http://snsf.caitsith2.net/snsf%20spec.txt and https://gist.githubusercontent.com/SaxxonPike/a0b47f8579aad703b842001b24d40c00/raw/a6fa28b44fb598b8874923dbffe932459f6a61b9/psf_format.txt</dd>

<dt>NSFE (NES)</dt>
  <dd>RIFF with embedded NSF; see https://www.nesdev.org/wiki/NSFe</dd>

<dt>NSF2 (NES)</dt>
  <dd>NSF with embedded RIFF; see https://www.nesdev.org/wiki/NSF2</dd>

<dt>GBSX (Game Boy 8-bit)</dt>
  <dd>Discontinued format, can either be embedded or as sidecar; see https://raw.githubusercontent.com/mmitch/gbsplay/05b68b8ff0b1faa6a2f1c987a34c564b28cde1fe/gbsformat.txt</dd>
</dl>

### Requires sidecar

"Sidecar" meaning, the tags are stored in an external file, and they are referenced by the rip.

<dl>
  <dt>GBS (Game Boy 8-bit)</dt>
  <dd>No intended standard (and no room for it, either); existing HCS64 rips use Knurek’s comments format in conjunction with the NEZPlug++ tagging format; see https://forums.bannister.org/ubbthreads.php?ubb=showflat&Number=78196#Post78196 and https://nezplug.sourceforge.net/in_nez.txt<br>One of two formats parsed by Game_Music_Emu.</dd>

<dt>HES (PC Engine)</dt>
  <dd>Same as GBS, except 4 bytes are at least reserved in the format.</dd>

<dt>NSF (NES)</dt>
  <dd>Same as GBS, although NSF2 at least is an improvement.</dd>

<dt>SID (Commodore 64)</dt>
  <dd>Refers to the STIL (SID Tune Information List): https://www.hvsc.c64.org/download/C64Music/DOCUMENTS/STIL.txt<br>
Refers to Songlength.md5; see https://www.hvsc.c64.org/download/C64Music/DOCUMENTS/Songlengths.md5 and https://www.hvsc.c64.org/download/C64Music/DOCUMENTS/Songlengths.faq</dd>
</dl>

### Comparison

Notes:

* This does not consider hardware-related information such as clock speeds and patch sets.
* Release dates may be *implied* by the copyright string even if they do not appear in the table.
* `(time)` means a HH:MM:SS.MMM format.
* In the HCS64 m3u format, notes *may* be added, but they are simply ignored by players, as there is no way of reliably parsing these.
* Table takes into account the format the tags are extending (e.g. GBS for GBSX, STIL for SID, etc.)

<table>
    <tr>
        <th>Quality\Format</th>
        <th scope="column">GD3</th>
        <th scope="column">ID666 + XID6</th>
        <th scope="column">PSF</th>
        <th scope="column">NSF{E,2}</th>
        <th scope="column">GBSX</th>
        <th scope="column">HCS64.m3u</th>
        <th scope="column">STIL</th>
    </tr>
    <tr>
        <th scope="row">Track name</th>
        <td>Yes ×2</td>
        <td>Yes</td>
        <td>Yes</td>
        <td>Yes [NSFE/2]</td>
        <td>Yes [GBSX]</td>
        <td>Yes</td>
        <td>Yes [STIL]</td>
    </tr>
    <tr>
        <th scope="row">Game name</th>
        <td>Yes ×2</td>
        <td>Yes</td>
        <td>Yes</td>
        <td>Yes</td>
        <td>Yes</td>
        <td>Yes</td>
        <td>Yes</td>
    </tr>
    <tr>
        <th scope="row">Release date</th>
        <td>Month and date optional</td>
        <td>Year [XID6]</td>
        <td>Year</td>
        <td></td>
        <td></td>
        <td>Month and date optional</td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">Track credits</th>
        <td>Author</td>
        <td><ul><li>Author</li><li>Publisher [XID6]</li></ul></td>
        <td><ul><li>Author</li><li>Copyright string</li></ul></td>
        <td><ul><li>Author</li><li>Copyright string</li></ul></td>
        <td><ul><li>Author</li><li>Copyright string</li></ul></td>
        <td><ul><li>Composer</li><li>Arranger</li><li>Sequencer</li><li>Engineer</li><li>Company/Publisher</li></ul></td>
        <td><ul><li>Author [STIL]</li><li>Copyright string</li></ul></td>
    </tr>
    <tr>
        <th scope="row">Rip credits</th>
        <td>Logger</td>
        <td><ul><li>Dumper</li><li>Date of Dump</li><li>Emulator (enumerated)</li></ul></td>
        <td>PSF creator</td>
        <td>Ripper</td>
        <td></td>
        <td><ul><li>Ripper</li><li>tagger</li></ul></td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">Playback metadata</th>
        <td><ul><li>Duration (samples)</li><li>Loop begin (samples)</li><li>Loop length (samples)</li></ul></td>
        <td><ul><li>Duration (seconds)</li><li>fade (miliseconds)</li><li>Channel masks</li><li>Loop data (tick) [XID6]</li><li>Preamp level [XID6]</li></ul></td>
        <td><ul><li>Preamp level</li><li>Duration (time)</li><li>Fade (time)</li></ul></td>
        <td>All NSFE/2:<ul><li>Duration (miliseconds)</li><li>fade (miliseconds)</li><li>Playlist order, Music/SFX differentiation</li><li>Per-channel mixing levels</li></ul></td>
        <td>Duration (ticks) [GBSX]</td>
        <td><ul><li>Duration (time)</li><li>Loop (time)</li><li>Fade (time)</li><li>Loop count (time)</li></ul></td>
        <td>Duration (time)</td>
    </tr>
    <tr>
        <th scope="row">Tag defined for</th>
        <td>One track = one file</td>
        <td>One track = one file</td>
        <td>One track = one file</td>
        <td>Many tracks = one file; each track may have unique info [NSFE/2]</td>
        <td>Many tracks = one file; each track may have unique info [GBSX]</td>
        <td>Many tracks = one file; one track = one file.</td>
        <td>Many tracks = one file; each track may have unique info [STIL, Songlength]</td>
    </tr>
    <tr>
        <th scope="row">Tag placement</th>
        <td>Embedded</td>
        <td>Embedded</td>
        <td>Embedded</td>
        <td>Embedded</td>
        <td>Embedded, or sidecar [GBSX]</td>
        <td>Sidecar</td>
        <td>Embedded, or sidecar [STIL, Songlength]</td>
    </tr>
    <tr>
        <th scope="row">Other metadata</th>
        <td>Notes</td>
        <td><ul><li>Notes</li><li>OST name [XID6]</li><li>OST track number [XID6]</li></ul></td>
        <td><ul><li>Notes</li><li>Genre</li></ul></td>
        <td>Notes</td>
        <td>Notes</td>
        <td></td>
        <td>Notes</td>
    </tr>
    <tr>
        <th scope="row">System name</th>
        <td>Yes</td>
        <td></td>
        <td>Enumerated</td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
</table>

## M3U sidecar formats supported by Game_Music_Emu

GME sidecar formats take on the same name as the file it's meant for; e.g. `Pictionary (1990-07)(Software Creations)(LJN).m3u` is read for `Pictionary (1990-07)(Software Creations)(LJN).nsf`.

### "Knurek / HCS64" format

(the Pictionary rip on HCS itself at the time of writing doesn't follow this format…)

```python
# @TITLE     Pictionary
# @ARTIST    Software Creations
# @DATE      1990-07
# @COMPOSER  Tim Follin
# @SEQUENCER Tim Follin
# @ENGINEER  Stephen Ruddy
# @RIPPER    Gil Galad
# @TAGGER    wrldwzrd89

Pictionary.nsf::NSF,1,Title Screen,1:34,-,10,
# .. more entries here
```

### "GME" format

```python
# Game:      Pictionary
# Artist:    Software Creations
# Copyright: 1990 LJN
# Composer:  Tim Follin
# Engineer:  Stephen Ruddy
# Ripping:   Gil Galad
# Tagging:   wrldwzrd89

Pictionary.nsf::NSF,1,Title Screen,1:34,-,10,
# .. more entries here
```

## Approximate list of tags already supported by vgmplay plugins

Cursory code inspection, they might not be 100% correct.

<table>
<thead>
    <th scope="column">Tag</th>
    <th scope="column">Audacious</th>
    <th scope="column">Foobar2000</th>
    <th scope="column">XMPlay</th>
    <th scope="column">Winamp</th>
</thead>
<tbody>
    <tr>
        <th scope="row">ALBUM</th>
        <td>✔</td>
        <td>✔</td>
        <td>✔</td>
        <td>✔</td>
    </tr>
    <tr>
        <th scope="row">ARTIST</th>
        <td>✔</td>
        <td>✔</td>
        <td>✔</td>
        <td>✔</td>
    </tr>
    <tr>
        <th scope="row">COMMENT</th>
        <td>✔</td>
        <td>✔</td>
        <td>✔</td>
        <td>✔</td>
    </tr>
    <tr>
        <th scope="row">GENRE</th>
        <td>✔</td>
        <td>✔</td>
        <td>✔</td>
        <td>✔</td>
    </tr>
    <tr>
        <th scope="row">TITLE</th>
        <td>✔</td>
        <td>✔</td>
        <td>✔</td>
        <td>✔</td>
    </tr>
    <tr>
        <th scope="row">ALBUMARTIST</th>
        <td>✔</td>
        <td>✔</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">COMPOSER</th>
        <td>✔</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">COPYRIGHT</th>
        <td>✔</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">CUESHEET</th>
        <td></td>
        <td></td>
        <td>✔</td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">DATE</th>
        <td></td>
        <td>✔</td>
        <td>✔</td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">DISCNUMBER</th>
        <td></td>
        <td>✔</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">FAMILY</th>
        <td></td>
        <td></td>
        <td></td>
        <td>✔</td>
    </tr>
    <tr>
        <th scope="row">FILETYPE</th>
        <td></td>
        <td></td>
        <td>✔</td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">GAIN</th>
        <td></td>
        <td></td>
        <td></td>
        <td>✔</td>
    </tr>
    <tr>
        <th scope="row">PERFORMER</th>
        <td>✔</td>
        <td></td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">PUBLISHER</th>
        <td>✔</td>
        <td></td>
        <td></td>
        <td>✔</td>
    </tr>
    <tr>
        <th scope="row">LENGTH</th>
        <td></td>
        <td></td>
        <td></td>
        <td>✔</td>
    </tr>
    <tr>
        <th scope="row">TRACK</th>
        <td>✔</td>
        <td></td>
        <td>✔</td>
        <td>✔</td>
    </tr>
    <tr>
        <th scope="row">TOTALTRACKS</th>
        <td></td>
        <td>✔</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">TRACKNUMBER</th>
        <td></td>
        <td>✔</td>
        <td>✔</td>
        <td></td>
    </tr>
    <tr>
        <th scope="row">YEAR</th>
        <td>✔</td>
        <td></td>
        <td></td>
        <td>✔</td>
    </tr>
</tbody>
</table>

Replaygain tags:

* `REPLAYGAIN_ALBUM_GAIN`
* `REPLAYGAIN_ALBUM_PEAK`
* `REPLAYGAIN_TRACK_GAIN`
* `REPLAYGAIN_TRACK_PEAK`

## `!tags.m3u` format proposal for sequenced formats

### Example

`!tags.m3u`
```python
# @album     Das Geheimnis der Happy Hippo-Insel
# @company   Kritzelkratz 3000
# @publisher Infogrames
# @artist    Stello Doussis
# @date      2000-05-17
# @ripper    DevEd
# @tagger    DevEd

# %title     Title Screen
# %length    0:02:03.000
# %fade      0:00:10.000
CGB-BHOD-GER.gbs?1
```

`01 Title Screen.m3u`
```python
CGB-BHOD-GER.gbs?1
```

### Definitions

<dl>
<dt>Player</dt>
<dd>
A media player with support for playing sequenced
rips, either built-in or through an extension.
</dd>
<dt>Player field</dt>
<dd>
An information field that a player shows in its UI
or injected when exporting from it, e.g. as ID3 tags
in an MP3.
</dd>
<dt>Tag</dt>
<dd>
Key:value information defined inside
<code>!tags.m3u</code> that describes a particular
quality of the sequenced rip.
</dd>
</dl>

### General information

All tags follow the same rules as in the vgmstream specification (including case-insensitivity), with the general tag format being: `# <Sigil><Tag> <Value>`. Or, in terms of [cryptic regex-runes](https://regex101.com/): `#\s*([\$@%])(\w+)\s+(.+)$|#\s*\$(\w+)\s*$`. The sigils are:

* `$` - representing a [global command](#global-commands).
* `@` - representing a global tag, which applies to all tracks, unless overridden by a local tag.
* `%` - representing a local tag, which applies to the track directly underneath the containing comment block.

Local tags are **not** to be appended to the existing global tag, but should replace it. For example, if the global `artist` tag defines `Junichi Masuda`, but a local tag for a track defines `Go Ichinose`, then the resulting `artist` tag should be `Go Ichinose`, not `Go Ichinose, Junichi Masuda`.

### Global commands

These commands do not have a value.

* `$autoalbum` - When specified, the directory name is taken as an album name.

### File structure

The rip should be a directory, compressed with 7z for distribution purposes. This directory shall consist of at least two files: the rip; and `!tags.m3u`.

```
Game (2024)(Publisher)[PCE].7z
├─ Game (2024)(Publisher)[PCE].hes
└─ !tags.m3u
```

Additionally, individual tracks may be split into one .m3u file each.

```
Game (2024)(Publisher)[PCE].7z
├─ Game (2024)(Publisher)[PCE].hes
├─ !tags.m3u
├─ 01 Title Screen.m3u
├─ 02 Options Menu.m3u
└─ 03 Level Begin.m3u
```

### File format information

`!tags.m3u` is a standard M3U playlist. It is plain text that should either be encoded as ANSI or UTF-8, and should support all types of line breaks.

It contains "tags" encoded as M3U comments, each defined as a line beginning with a hash/pound symbol `#`. See [General information](#general-information) for more details.

Non-comment lines must be a reference to a valid file, usually the rip inside the folder. When the rip contains subtunes, the file name must be suffixed with `?N`, where `N` is a **1-indexed** subtune number.

Split M3U files should only contain the aforementioned non-comment lines. Tags in these files are ignored, as `!tags.m3u` is considered the only source of truth.

When a rip file is loaded by the player, the player will look for a `!tags.m3u` in the directory it's contained in. If not present, the player must fall back to default or tags already embedded in the file, however limited it is.

When multiple rip files are referenced in `!tags.m3u`, e.g.

```python
# ...tags A...
A.nsf?1

# ...tags B...
B.nsf?3

# ...tags C...
A.nsf?2

# ...tags D...
A.nsf?4
```

If A.nsf is loaded, the player should only load tags A, C, and D. The resulting playlist would render subtune 3 lacking any tags.

Likewise with B.nsf, where subtunes 1 and 2 will not have any tags.

If multiple tags are referenced for the same file and subsong:

```python
# ...tags A...
A.nsf?1

# ...tags B...
A.nsf?1
```

…the resulting tag for it would be a combination of tags A and B, with tags B taking priority as it is defined after tags A.

### List of proposed tags

#### Common

Considered to be the lowest common denominator, these tags may be supported on most players, particularly when it comes to on-screen display and exporting to various audio formats.

* `album`
  * The title of the game.
  * Players should use this for its `Album` field.
* `artist`
  * Players should use this for its `Artist` field.
  * When defined as a global tag, players should use it as the `Album Artist` field as well, when available.
* `title`
  * The title of the track. This should be its composer given name, or if unavailable, the part of the game it plays on.
  * Players should use this for its `Title` field.
  * Consider the following: https://www.youtube.com/watch?v=AbBpQTkTFF4
* `comment`
  * Notes or trivia concerning this track or the game rip.
  * Can be defined more than once; subsequent definitions will be appended to (with a newline joining each definition) e.g. `#%comment Hello` and `#%comment World` will result in `Hello\nWorld`.
* `track`
  * The track number to manually assign to this track.
  * Normally, this isn't needed as it is set automatically based on the order defined in `!tags.m3u`.

#### Playback

These tags control how the track is played by the backend, and should be also used to determine its duration.

* `length`
  * Recommended length of the song.
  * This is usually the time it takes for the song to loop once or twice, this is left to the tagger's discretion.
  * See [Time format](#time-format) for allowed values. 
* `fade`
  * How long to fade out the song.
  * See [Time format](#time-format) for allowed values. 
  * Songs that don't loop should have a fade of 0.
  * When determining duration, players must keep into account `length` + `fade`.

#### Metadata

Some players support these more specific types of metadata.

* `company`
  * The company which produced the game.
  * Players that only have a `Publisher` field should instead append its contents into that field.
  * If no `Publisher` field is available, and no `artist` tags is defined either, this should go into the `Artist` field as a fallback.
* `publisher`
  * The company which published the game, if it differs from the company producing it.
  * Supporting players can insert this into their `Publisher` field.
* `composer`
  * This is the person who writes the game's score.
  * When available, players should use this for its `Composer` field.
* `arranger`
  * This is the person who tweaks the composer's score or to adjust it to fit the game's limitations.
  * When available, players should use this for its `Arranger` field.
* `sequencer`
  * This is the person who transcribes the composer's score into the game's format.
  * When available, players should use this for its `Performer` field.
* `engineer`
  * The engineer for either a single track (local tag) or assumed engineer for all tracks (global tag) unless specified otherwise. I assume this is the person who wrote the sound driver.
  * May be shown for informational purposes (e.g. in a `Comments` field), otherwise can be safely ignored by the player.
* `date`
  * Accepts only YYYY-MM-DD, YYYY-MM, or YYYY formats. The separating character MUST be a dash `-`.
  * Supporting players can just insert this into their `Date` field.
  * The year field can be extracted and put into the player's `Year` field. This goes for both players that support `Date` and otherwise.
* `ripper`
  * May be shown for informational purposes (e.g. in a `Comments` field), otherwise can be safely ignored by the player.
* `tagger`
  * May be shown for informational purposes (e.g. in a `Comments` field), otherwise can be safely ignored by the player.
* `copyright`
  * The applicable copyright string, its format similar to existing formats, e.g. `1995 Nintendo`.
  * Supporting players can just insert this into their `Copyright` field.

#### Additional notes

* When defined multiple times for a track, local tags *excluding* `comment` should overwrite the previous definition.
* When multiple entities are involved in one aspect of the track(s), e.g. more than one composer should be listed in a `composer` field, all entities must be listed as that tag's value in one line, with *at least* a comma `,` separating each value.

### Time format

One of:

* SS
  * e.g. `30`
* SS.mmm
  * e.g. `17.104`
* MM:SS
  * e.g. `6:05`
* MM:SS.mmm
  * e.g. `4:07.04`
* HH:MM:SS
  * e.g. `1:02:00`
* HH:MM:SS.mmm
  * e.g. `00:47:16.140`

Where:

* **HH** = hours
* **MM** = minutes
* **SS** = seconds
* **mmm** = miliseconds

Each part may be zero-padded, or not.

### Mapping of legacy GME tags to !tags.m3u tags


#### `# Game: xxx` / `# @TITLE xxx`

<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The name of the game the file is a soundtrack rip of.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
            <li><code>album</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        This tag does not specify track titles, but the
        title of the game getting ripped.
    </dd>
</dl>


#### `# Artist: xxx` / `# @ARTIST xxx`

<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The companies who developed and published the game.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>company</code></li>
          <li><code>publisher</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        For some reason, "Artist" represents the corporate entity who owns the music,
        and not… uh, the people who produced it. Something to do with the copyright fields?
    </dd>
</dl>

#### `# @DATE xxx`

<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The game's release date. This usually
        follows YYYY-MM-DD.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>date</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        -
    </dd>
</dl>

#### `# Composer: xxx` / `# @COMPOSER xxx`

<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The musician who wrote the score to the track.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>artist</code></li>
          <li><code>composer</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        -
    </dd>
</dl>

#### `# @SEQUENCER xxx`

<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The programmer responsible for transferring the composer's score to
        the in-game format.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>sequencer</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        No such tag may have conventionally existed, but this tag can
        be reserved for players that can display such specific information,
        since parsing comments are hard.
    </dd>
</dl>

#### `# Engineer: xxx` / `# @ENGINEER xxx`

<dl>
    <dt>Legacy purpose</dt>
    <dd>
        Not much examples I can find, but I suspect this could
        be the person writing the sound engine, for example.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>engineer</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        See Sequencer.
    </dd>
</dl>

#### `# Ripping: xxx` / `# @RIPPER xxx`

<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The person who ripped the soundtrack into a playable file.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>ripper</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        See Sequencer.
    </dd>
</dl>
</details>

#### `# Tagging: xxx` / `# @TAGGER xxx`

<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The person who identified and tagged an existing rip.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>tagger</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        See Sequencer.
    </dd>
</dl>

#### `# Copyright: xxx`

<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The game's copyright information.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>copyright</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        -
    </dd>
</dl>

### Differences to vgmstream

* Only tags without spaces is to be supported.
* As sequenced rips are usually self-contained, TXTP files aren't necessary—in its place are separate m3u files for each track.
* `autotrack` is implicitly assumed; the `track` tag can be used to manually assign track numbers if necessary.
* The tags are standardized, so it doesn't assume a certain player is to be used. Players implementing this should instead adapt tags defined here to player fields themselves as appropriate.

### Additional considerations

* Audacious **does not support** nested M3U's, while VLC **does**. Winamp, XMPlay, Foobar2000 support currently unknown to the author.
* Some players like Audacious supports URLs with subtunes in them, e.g. suffixed with `?1`, `?2`.
* libgme has 1-indexed subtunes.
* vgmstream's design seems to be intentionally player-specific.
  * **Pro**: there little parsing work to be done, the design kept relatively simple
  * **Con**: a risk of redundant data being entered when a player supports more granular fields (like `Composer` and `Date`), while players with less granular fields (like `Artist` and `Year`) is to also be supported. Either go with the lowest-common denominator, or input redundant data. Can be automated somewhat, but overall a hassle.
* The M3U entries should be made as compatible as possible for existing players, and ideally both `!tags.m3u` and the rip itself should provide the same outcome: proper tags and track separation.
* Encoding loop start/length information may not be necessary; the player should provide an option to play tracks endlessly instead.

## Proof of concepts

Like this document, all of them are works in progress. They may not yet conform to this spec.

* [Fork of NEZPlug++ Winamp plugin with !tags.m3u support](https://github.com/romh-acking/nezplug-m3u-tags)
* [Fork of Audacious Console Music plugin with !tags.m3u support](https://github.com/ZoomTen/audacious-plugin-gme/tree/tags-m3u)
  * Added on top of the updates to libgme.
* [Support in Zumi's gbstools](https://gitgud.io/zumi-gbs/hcs/gbstools/-/blob/master/gbsdist.py)

## Reference implementation

[This repository](https://github.com/ZoomTen/game-music-tagging) currently contains two reference implementations for reading files that conform to this spec:

1. **C++**: Can be compiled alongside a C++ project that wants to add support for this spec, or compiled as a (static or dynamic) library to link with any project. This implementation targets **C++11**.
2. **Nim**: Can be imported as a module in a Nim project that wants to add support for this spec (with caveats), or compiled as a (static or dynamic) library to link with any project. This implementation is currently written and tested for Nim **v2.0.4**.