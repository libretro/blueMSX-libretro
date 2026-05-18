blueMSX-libretro
================

source http://sourceforge.net/p/bluemsx/code/HEAD/tree/trunk/blueMSX/Make/blueMSXlite/
svn://svn.code.sf.net/p/bluemsx/code/trunk

this core requires the two folders named Machines/ and Databases/ from standalone blueMSX
copied inside your frontend's system directory.

you can get these from the official windows standalone version of bluemsx (http://www.bluemsx.com/)
or any other binary package.

Virtual Keyboard (On-Screen Keyboard)
--------------------------------------

Machines with a keyboard (MSX, MSX2, MSX2+, MSXturboR, SVI, SC-3000, SF-7000) include
an on-screen keyboard that lets you type without a physical keyboard. The layout follows
the standard MSX international keyboard.

**Controls:**

| Button | Action |
|--------|--------|
| SELECT | Toggle the virtual keyboard on/off |
| D-pad | Navigate between keys |
| A | Press the highlighted key; hold to repeat |
| B | Clear all sticky modifiers |

**Modifier keys** (SHIFT, CTRL, GRAPH, CODE, CAPS) are *sticky*: pressing A on them
toggles them on (shown in green) and they stay active until you press a normal key,
at which point they release automatically. CAPS works as a true toggle (like the
physical key) and is not cleared when a normal key is pressed.

**Shifted labels** are shown at the top of each key where applicable
(e.g. the `1` key shows `!` as its shift label).
