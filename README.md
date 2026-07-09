Fish Fillets - Next Generation Re
=================================

Just a fan project to revive [Fish Fillets NG](http://fillets.sf.net) for modern systems.
Tries to keep things compatible with the old version where possible. Original saves and settings 
should both work without any tweaks.

The original code was ported from SDL1 to SDL2, moved from autotools to
CMake, and updated to build with modern compilers and Lua 5.4+.

Dependencies
============

- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_ttf
- SDL2_gfx
- Lua (5.3+)
- fribidi

Build
=====

    git clone https://github.com/hoppsikCZ/fillets-ng-re
    cd fillets-ng-re
    cmake -B build
    cmake --build build

Run
===

Unlike the original, this repo already includes all the data folders needed to run the game (`images/`, `images-baseline/`, `script/`, `music/`, `sound/` and `font/`). So you should be able to just run the game from it's build location.

    ./build/fillets

Or point to a custom data directory with `systemdir`:

    ./build/fillets systemdir=/path/to/fillets-ng-data

**Note:** The original `fillets-ng-data` package was meant for the SDL1
version and may not work correctly here. The repo bundles its own data.

Controls
========

- Arrow keys — move current fish
- Space — switch fish
- Backspace — restart level
- -/+ — undo/redo
- F1 — help
- F2 — save
- F3 — load
- F5 — move counter
- F6 — subtitles
- F10 — game menu
- F11 — fullscreen
- Shift — fast-forward
