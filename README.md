# !tags.m3u proposal for retro soundtrack rips

(One-file-to-rule-them-all; if possible)

## Abstract
Original post: https://github.com/libgme/game-music-emu/issues/85

> Tagging is a long standing issue that has plagued old sequenced rips for over the last 20 years. Typically rips for NSF, GBS, etc. are shared in standalone files. You are forced cycle through the subtunes to find that one song you want to listen to. Additionally, they have a default time like 5 minutes or 3 minutes. There's a few formats like NSFE that address the time issue (but vary in granularity), but each format requires a custom tool, a new format (some of which die an unceremonious death like GBSX), and still don't tackle the issue of subtune cycling.

> One solution to this problem was M3U tagging. A fine solution until you see the actual contents of the M3U files. This is very close to a perfect solution with a major problem. Each song has metadata shoved into one line to conform to winamp's (and I would assume others') limitations. When a song it played in winamp from a M3U file, it is only aware of a single line from the M3U file. It's not aware of the comments. Additionally, the individual M3U files for songs are not aware of the metadata in the playlist M3U file so the metadata must be duplicated. Hence when plugin author's resorted to variants of the format you see above and the formats different ever so slightly between plugins. It's a major tech debt item that authors have simply worked around. But in the end, it does the job. However, taggers pay the price because the format is not intuitive.

> `!tags.m3u` is a format from the [vgmstream](https://github.com/vgmstream/vgmstream/blob/master/doc/USAGE.md#tagging) library, the definitive streamed music library. There's a plethora of streamed formats so most stream rips resort to `!tags.m3u` and `.txtp` for extra metadata for all streamed formats. `!tags.m3u` is a static filename that is located at the base of the rip and contains metadata for all the rips in one file. 

This document is a proposal to adapt vgmstream's `!tags.m3u` format to older sequenced formats to better standardize sound rips.

## Existing extended tagging formats

### Embedded

<dl>
  <dt>VGM / VGZ (multi-system, usually Sega)</dt>
  <dd>GD3 format, analogous to MP3’s ID3; see https://vgmrips.net/wiki/GD3_Specification<br>One file each track.</dd>
  
  <dt>SPC / RSN (SNES)</dt>
  <dd>ID666; see https://wiki.superfamicom.org/spc-and-rsn-file-format<br>One file each track.</dd>
  
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

<dl>
  <dt>GBS (Game Boy 8-bit)</dt>
  <dd>No intended standard (and no room for it, either); existing HCS64 rips use Knurek’s comments format in conjunction with the NEZPlug++ tagging format; see https://forums.bannister.org/ubbthreads.php?ubb=showflat&Number=78196#Post78196 and https://nezplug.sourceforge.net/in_nez.txt<br>One of two formats parsed by Game_Music_Emu.</dd>
  
  <dt>HES</dt>

  <dt>SID (Commodore 64)</dt>
  <dd>Refers to the STIL (SID Tune Information List): https://www.hvsc.c64.org/download/C64Music/DOCUMENTS/STIL.txt
Refers to Songlength.md5; see https://www.hvsc.c64.org/download/C64Music/DOCUMENTS/Songlength.md5</dd>
</dl>

## M3U sidecar formats supported by Game_Music_Emu

GME sidecar formats take on the same name as the file it's meant for; e.g. `Pictionary (1990-07)(Software Creations)(LJN).m3u` is read for `Pictionary (1990-07)(Software Creation)(LJN).nsf`.

### Knurek / HCS64

(the rip on HCS at the time of writing doesn't follow this format…)

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

### not sure what this one's "called"

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

## Approximate list of tags supported by vgmplay plugins

Basic tags:

* `ALBUM`
* `ARTIST`
* `COMMENT`
* `GENRE`
* `TITLE`

Player-specific tags:

* `ALBUMARTIST` (audacious, foobar)
* `COMPOSER` (audacious)
* `COPYRIGHT` (audacious)
* `CUESHEET` (xmplay)
* `DATE` (foobar, xmplay)
* `DISCNUMBER` (foobar)
* `FAMILY` (winamp)
* `FILETYPE` (xmplay)
* `GAIN` (winamp)
* `PERFORMER` (audacious)
* `PUBLISHER` (audacious, winamp)
* `LENGTH` (winamp)
* `TRACK` (audacious, winamp, xmplay)
* `TOTALTRACKS` (foobar)
* `TRACKNUMBER` (foobar, xmplay)
* `YEAR` (audacious, winamp)

Replaygain tags:

* `REPLAYGAIN_ALBUM_GAIN`
* `REPLAYGAIN_ALBUM_PEAK`
* `REPLAYGAIN_TRACK_GAIN`
* `REPLAYGAIN_TRACK_PEAK`

## Mapping of legacy GME tags to !tags.m3u tags

<details><summary>
<code># Game: xxx</code> / <code># @TITLE xxx</code>
</summary>
<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The name of the game the file is a soundtrack rip of.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
            <li><code>@album</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        This tag does not specify track titles, but the
        title of the game getting ripped.
    </dd>
</dl>
</details>

<details><summary>
<code># Artist: xxx</code> / <code># @ARTIST xxx</code>
</summary>
<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The companies who developed and published the game.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>@artist</code>, if no sound team information is available.</li>
          <li><code>@company</code> / <code>@publisher</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        For some reason, "Artist" represents the corporate entity who owns the music,
        and not… uh, the people who produced it. Something to do with the copyright fields?
    </dd>
</dl>
</details>

<details><summary>
<code># @DATE xxx</code>
</summary>
<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The companies who developed and published the game. This usually
        follows YYYY-MM-DD.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>@date</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        I think the existing vgmstream
        implementations just have a <code>@year</code>. Ideally, just <code>@date</code> should be
        filled in and then the year can be derived from it. But if you <i>really</i> want compatibility, fill both in. But make sure <code>@year</code> is an integer!
    </dd>
</dl>
</details>

<details><summary>
<code># Composer: xxx</code> / <code># @COMPOSER xxx</code>
</summary>
<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The musician who wrote the score to the track.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>@artist</code></li>
          <li><code>@composer</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        -
    </dd>
</dl>
</details>

<details><summary>
<code># @SEQUENCER xxx</code>
</summary>
<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The programmer responsible for transferring the composer's score to
        the in-game format.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>@sequencer</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        No such tag may have conventionally existed, but this tag can
        be reserved for players that can display such specific information,
        since parsing comments are hard.
    </dd>
</dl>
</details>

<details><summary>
<code># Engineer: xxx</code> / <code># @ENGINEER xxx</code>
</summary>
<dl>
    <dt>Legacy purpose</dt>
    <dd>
        Not much examples I can find, but I suspect this could
        be the person writing the sound engine, for example.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>@engineer</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        See Sequencer.
    </dd>
</dl>
</details>

<details><summary>
<code># Ripping: xxx</code> / <code># @RIPPER xxx</code>
</summary>
<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The person who ripped the soundtrack into a playable file.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>@ripper</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        See Sequencer.
    </dd>
</dl>
</details>

<details><summary>
<code># Tagging: xxx</code> / <code># @TAGGER xxx</code>
</summary>
<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The person who identified and tagged an existing rip.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>@tagger</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        See Sequencer.
    </dd>
</dl>
</details>

<details><summary>
<code># Copyright: xxx</code>
</summary>
<dl>
    <dt>Legacy purpose</dt>
    <dd>
        The game's copyright information.
    </dd>
    <dt>Should map to</dt>
    <dd>
        <ul>
          <li><code>@copyright</code></li>
        </ul>
    </dd>
    <dt>Rationale</dt>
    <dd>
        -
    </dd>
</dl>
</details>

## Global commands

* `$autotrack` - When specified, track numbers don't need to be specified manually.
* `$autoalbum` - When specified, the directory name is taken as an album name.

## Potential differences to vgmstream

* Only tags without spaces should be supported.
* TXTP files isn't necessary.

## `!tags.m3u` format proposal for sequenced foramts

### Tags
* `album`
  * The title of the game.
* `company`
  * The company which produced the game.
  * Players that only have a `Publisher` field should instead append its contents into that field.
* `publisher`
  * The company which published the game, if it differs from the company producing it.
* `artist`
* `year`
* `ripper`
  * Can be ignored by the player.
* `tagger`
  * Can be ignored by the player.
* `source`
* `title`
  * The title of the track.
  * This should be its' composer given name, or if unavailable, the part of the game it plays on.
  * Consider: https://www.youtube.com/watch?v=AbBpQTkTFF4
* `subtune`
  * Id of song within rip file
* `length` 
* `fade`

### Example
```
# @album	Das Geheimnis der Happy Hippo-Insel
# @company	Kritzelkratz 3000, Infogrames
# @artist	Stello Doussis
# @year		2000-05-17
# @ripper	DevEd
# @tagger	DevEd
# @source	CGB-BHOD-GER.gbs

# %title	Title Screen
# %subtune	0
# %length	0:02:03.000
# %fade		0:00:10.000
01 Title Screen.m3u
```
