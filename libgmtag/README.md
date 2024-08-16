# libgmtag

This is one of the reference implementations of the proposed [Game Tag Format (tentative name)](../README.md) for existing sequenced music rips.

This should be preferably compiled alongside your project, or as a static library.

Files to include:

* include/libgmtag.h
* src/libgmtag.cpp
* src/private/*.cpp
* src/private/*.hpp

## Function index
<dl>
    <dt><code><i>GmTagObject</i> <b>tags_from_buffer</b> (<i>const char *</i><b>buff</b>)</code></dt>
    <dd>
    <p>
        Load tags from a string buffer. This buffer is where the <em>main</em> m3u file would be loaded into beforehand.
    </p>
    <p>
        The object returned is opaque and implementation-defined, so it's defined as a <code><i>void*</i></code>. This is so multiple implementations of this library can be supported. You should only use it with these functions&mdash;don't even try to <code>free()</code> it manually!
    </p>
    </dd>
    <dt><code><i>void</i> <b>unset_tags</b> (<i>GmTagObject</i> <b>handle</b>)</code></dt>
    <dd>
    <p>
        Frees a <i>GmTagObject</i> along with its containing objects. This should not affect anything returned from <b>get_*</b> functions.
    </p>
    </dd>
    <dt><code><i>char *</i> <b>get_&#x24e7;</b> (<i>GmTagObject</i> <b>handle</b>, <i>unsigned long</i> <b>subtune</b>)</code></dt>
    <dd>
    <p>
        Where &#x24e7; is one of:
        <ul>
            <li>album</li>
            <li>company</li>
            <li>publisher</li>
            <li>artist</li>
            <li>composer</li>
            <li>sequencer</li>
            <li>arranger</li>
            <li>engineer</li>
            <li>ripper</li>
            <li>tagger</li>
            <li>title</li>
            <li>comment</li>
            <li>copyright</li>
        </ul>
    </p>
    <p>
        These functions will return a <strong>copy</strong> of the entry in question for a given subtune. It is the callers' responsibility to free the string when done.
    </p>
    <p>
        For the <code>comment</code> variant, this may return a string with multiple lines, separated by a new line (<code>\n</code>). Callers should deal with this appropriately.
    </p>
    <p>
        For this and the other functions, subtune 0 is the <strong>global</strong> tag set for this tag object.
    </p>
    </dd>
    <dt><code><i>GmTagDateDef</i> <b>get_date</b> (<i>GmTagObject</i> <b>handle</b>, <i>unsigned long</i> <b>subtune</b>)</code></dt>
    <dd>
    <p>
        Gets the date of a given subtune as the following struct:
        <pre>
typedef struct gmtag_datedef
{
    uint64_t year;
    uint8_t  month;
    uint8_t  day;
} GmTagDateDef;</pre>
    </p>
    </dd>
    <dt><code><i>GmTagDateDef</i> <b>get_&#x24e8;</b> (<i>GmTagObject</i> <b>handle</b>, <i>unsigned long</i> <b>subtune</b>)</code></dt>
    <dd>
    <p>
        Where &#x24e8; is one of:
        <ul>
            <li>date</li>
            <li>length</li>
            <li>fade</li>
        </ul>
    </p>
    <p>
        Gets the field in question as the following struct:
        <pre>
typedef struct gmtag_timedef
{
    uint64_t seconds;
    uint64_t miliseconds;
} GmTagTimeDef;</pre>
    </p>
    </dd>
    <dt><code><i>GmTagOrderDef*</i> <b>get_subtune_order</b> (<i>GmTagObject</i> <b>handle</b>)</code></dt>
    <dd>
    <p>
        Retrieves the subtune ordering from the parsed tags object as the following struct:
        <pre>
typedef struct gmtag_orderdef
{
    uint64_t how_many;
    uint64_t *order;
} GmTagOrderDef;</pre>
    </p>
    <p>
        Please note that <code><b>order</b></code> is a <em>variable length array</em> allocated on the heap, so you may want to free that manually when you're done with it.
    </p>
    <p>
        Example: the M3U defines tags for subtunes 1, 2, 4, 6 in that order. The resulting struct will be:
        <pre>
how_many = 4
order = {
    0, 1, 2, 4, 6
}</pre>
    </p>
    </dd>
</dl>