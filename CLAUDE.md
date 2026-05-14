# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project purpose

This repo is a deliberate testbed for an agentic coding chain (Claude Code + Mistral Vibe). The "real" software in the repo — Cap'n Mathgnéto — is a 1995 educational clone of Cap'n Magneto (the 1985 Mac game by Al Evans), rewritten in ANSI C / X11 (Xlib, 1-bit black & white, 512×342 window). The game itself is preserved as-is in `archive-1995-2000/` and is the target the agent team works on; improving it is a *byproduct* of building the agent chain, not the primary goal.

Two LinkedIn articles (linked from README.md) explain the motivation — read them before making sweeping changes to the agent workflow.

## Repository layout

- `archive-1995-2000/` — the original 1995 codebase (small ANSI C + Xlib game). All code lives here; the repo root contains only README/LICENSE/this file.
- `archive-1995-2000/sprites/` — `.xbm` (X BitMap) sprite files, included at compile time by render code.
- No build artifacts, no CI, no test suite — the project is intentionally a small, frozen target.

## Building and running

The game is built with the Makefile inside `archive-1995-2000/`:

```bash
cd archive-1995-2000
make                # builds ./mathgneto
make clean          # removes *.o, the binary, and core dumps
./mathgneto         # runs the game; needs an X display
```

Dependencies: `gcc` and `libx11-dev` (Xlib). On Debian/Ubuntu: `apt-get install libx11-dev`. Compile flags assume `/usr/X11R6/{include,lib}` — on modern distros X11 usually resolves through the default include/lib paths but the legacy paths are still respected.

There is **no test suite, no linter, no formatter** configured. "Testing" means compiling cleanly with `-Wall` and running the game.

## Architecture (the "big picture" across modules)

The game is a single-binary X11 app with one big event loop in `main.c` and six clearly split modules. Understanding how they fit together matters because the modules deliberately share global state.

### Event loop and shared state

`main.c` owns:
- The `XPending` / `XNextEvent` loop (handles `Expose`, `ButtonPress`, `KeyPress`, `ConfigureNotify`).
- The `struct joueur joueur` global — defined here, declared `extern` in `types.h`, mutated from `input.c`, `combat.c`, `quiz.c`, and rendered from `render.c`.
- Game-state transitions driven by reading the global `etat_jeu` (defined in `input.c`, declared `extern` in `input.h`). The states are `ST_EXPLORE | ST_COMBAT | ST_QUIZ | ST_DIALOG | ST_GAMEOVER | ST_WIN` (see `types.h`).
- Win/lose detection: `hp <= 0` → `ST_GAMEOVER`; `score >= 100` → `ST_WIN` (the score-100 check is the V1 proxy for "found the Crown of Control").
- Frame pacing via `usleep(33333)` (~30 FPS). Rendering is gated by a local `redraw` flag — modules don't draw themselves, they set the flag and `main.c` decides what to call.

### Module responsibilities

- **`render.c` / `render.h`** — All Xlib calls. Owns the `Display *dpy`, `Window win`, `GC gc`, `XFontStruct *fnt` globals (others read them via `extern`). Also computes and exports the hitbox arrays (`verbe_x/y/w/h`, `compass_x/y/r`, `qcm_x/y/w/h`) so input code can hit-test without re-knowing layout. Each game state has its own `render_*` entry point; `main.c` chooses which to call.
- **`input.c` / `input.h`** — `hit_test(mx,my)` maps a click to a region code (`HIT_NORD`, `HIT_VERBE+i`, `HIT_QCM+i`, `HIT_GAME`, …). `on_click()` consumes that and mutates `joueur` / `etat_jeu`. This is the only place where verbs (`V_WALK`, `V_JUMP`, `V_GET`, …) get acted on.
- **`world.c` / `world.h`** — Tile maps, walkability, entity and object lookup. **Important quirk**: the maps live in `maps.h` as `static char map_outdoor[…]` etc., so each `.c` that includes `maps.h` gets its own private copy. That was a deliberate workaround for linker complaints in 1995 — duplication is intentional, don't "fix" it by moving to globals without understanding the original constraint.
- **`combat.c` / `combat.h`** — Turn-based combat triggered when the player walks into a hostile entity. `combat.h` defines its own `typedef unsigned char Boolean` (legacy from the Think C Mac port) and a `NB_ENNEMIS_MAX` constant that duplicates `world.h`'s `MAX_PNJ` (also intentional historical duplication — see comment in `main.c`).
- **`quiz.c` / `quiz.h` / `questions.h`** — The "lieux de connaissance" variant: temples (`T_TEMPLE`/`'X'` tiles) trigger multiple-choice math questions. Four difficulty tiers (`NIV_COLLEGE | NIV_LYCEE | NIV_PREPA | NIV_INGE`), ~20 questions each, hardcoded as `static struct question` arrays in `questions.h`. The pick algorithm marks seen questions in a bitset and avoids repeats until the level is exhausted. Correct → score + HP bonus; wrong → HP penalty.

### Verbs and input model

The Mac-style verb-based UI is the structural axis of `input.c`: the player picks a verb (Walk / Jump / Fly / Get / Drop / Identify / Status / Z Gate) by clicking the right-hand panel, then clicks the compass or a tile to apply it. `V_STATUS` is special — it overlays a status panel instead of acting on the world (handled in `main.c`'s render branch). Adding a new verb means: add a `V_XXX` constant in `types.h`, bump `NB_VERBES`, lay out the button in `render.c`, and dispatch it in `input.c`'s `on_click`.

## Conventions to respect when modifying the game

This is **legacy code preserved as historical artifact** as much as it is a working program. Before "cleaning up" or "modernizing":

- **Comments and identifiers are in French (Latin-1, no accents in source).** Keep that style — `joueur`, `verbe_actif`, `carte`, `etat_jeu`, etc. The no-accents rule is a deliberate Mac/Unix interop choice from the original author; don't reintroduce accented characters in source code.
- **Pre-ANSI/K&R-era idioms are preserved on purpose**: `int main()` with no `void` parameter, `(void)var;` casts to silence warnings, `typedef unsigned char Boolean` alongside `int`-booleans, `usleep()` rather than something more modern. Comments throughout reference *K&R 2e ed* chapters as the source of techniques — that's part of the artifact.
- **The known bugs in `LISEZMOI.TXT`** (map wrap glitches near doors, NPC stuck-in-wall, startup pixmap leaks, no sound, no title screen, no save) are documented and known. If you fix one, update `LISEZMOI.TXT` to match.
- **Globals are intentional**, not a mistake to refactor away: `joueur`, `etat_jeu`, the Xlib display globals, and the duplicated-static map arrays are load-bearing for the original design. Touch them only with the user's explicit OK.
- **No build system beyond the Makefile, no headers split into pub/priv, no abstraction layers.** Match that.

When the user asks for "improvements", default to surgical, in-style changes — not refactors toward modern C.
