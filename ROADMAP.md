# FreeCAD — Project Anvil

> **Mission:** Describe a part in plain language, watch it appear in FreeCAD as a live parametric model you can tune with sliders and print the same afternoon. (Name "Anvil" is provisional.)
> **Owner:** Pablo / Maximum Effort AI
> **Updated:** 2026-08-30

## Status

| Current Phase | Blockers | Last Action |
|---|---|---|
| Phase 1 — Recipe core | None | 2026-08-30 · Roadmap authored; Phase 0 proof-of-concept complete |

---

## Phase 0 — Proof of concept

**Outcome:** A real, printable part generated headlessly by script, proving the recipe-not-mesh thesis.
**Executor:** Claude Code · `claude-opus-4-7`

- [x] Generate an inverted-T drawer divider via headless `FreeCADCmd` script
      → `~/Projects/3D-Models/drawer-divider/` — valid single solid, STL + FCStd, dims verified against spec
- [x] Confirm `build/debug/bin/FreeCADCmd` (FreeCAD 1.2.0 R45189) runs scripts and exports mesh
- [x] Confirm the GUI opens a script-generated `.FCStd` with the modernized Start page intact

---

## Phase 1 — Recipe core

**Outcome:** A Part Recipe (JSON schema + safe generator) can be executed headlessly to produce a validated solid, with no AI and no GUI involved.
**Executor:** Claude Code · `claude-opus-4-7` (recommended — the sandbox is security-critical)

- [ ] Decide and record distribution model: separate LGPL-compatible addon vs. fork module
      (recommendation: **separate addon** — avoids LGPL-2.1-or-later distribution obligations on a fork, and reaches existing FreeCAD users via Addon Manager)
- [ ] Define the Part Recipe JSON schema: `name`, `intent`, `parameters[]` (id/label/unit/value/min/max), `generator`, `constraints`
- [ ] Implement the constrained geometry DSL — `box`, `cylinder`, `sphere`, `fuse`, `cut`, `fillet`, `chamfer`, `pattern`, `mirror`
- [ ] Implement the `ast`-based guard: whitelist node types; reject imports, attribute access to dunders, `open`, `exec`, `eval`, and all I/O. **Never `exec()` raw model output.**
- [ ] Write the validator: `isValid()`, exactly-one-solid, closed/manifold, bounding box matches declared dimensions within tolerance
- [ ] Write the printability checker: min wall thickness, max bed volume, unsupported-overhang flag
- [ ] Port the drawer divider to a Part Recipe as the first fixture
- [ ] Test suite: ≥10 recipes (valid + deliberately malicious) proving the guard blocks escapes and the validator catches bad geometry

---

## Phase 2 — Live parametric preview

**Outcome:** Load a recipe in FreeCAD, drag a slider, and the model rebuilds in the existing 3D view in under 100 ms. This is the demo that sells the product — and it requires no AI.
**Executor:** Claude Code · `claude-sonnet-4-6` (recommended — Qt wiring, mechanical)
**Gated by:** Phase 1 complete

- [ ] Scaffold a pure-Python workbench at `src/Mod/Anvil/` following the `src/Mod/Plot/` pattern (`Init.py`, `InitGui.py`, `CMakeLists.txt` with `fc_copy_sources`)
- [ ] Add `option(BUILD_ANVIL ...)` to `cMake/FreeCAD_Helpers/InitializeFreeCADBuildOptions.cmake` and the matching `if()` block in `src/Mod/CMakeLists.txt`
- [ ] Build a dock panel that auto-generates a labelled slider/spinbox per recipe parameter
- [ ] Wire parameter change → regenerate shape → update `View3DInventorViewer` without reloading the document
- [ ] Debounce regeneration and confirm sub-100 ms round trip on the divider recipe
- [ ] Add Save Recipe / Load Recipe / Export STL · STEP · 3MF actions
- [ ] Verify the panel is legible in both light and dark themes (per `FreeCAD-Modern.qss`)

---

## Phase 3 — Intelligence layer

**Outcome:** A plain-language prompt produces a working recipe, with the user's own API key, and self-corrects when the geometry fails validation.
**Executor:** Claude Code · `claude-opus-4-7` (recommended — prompt design + credential security)
**Gated by:** Phase 2 complete

- [ ] Build the pluggable provider interface: `providers/anthropic.py`, `providers/openai.py`, `providers/local.py`
- [ ] Implement credential storage via OS keychain (`keyring`); store only a **reference** in `ParameterGrp`. **No plaintext keys, no keys in logs, no keys in saved recipes.**
- [ ] Build the BYO-API-key settings page with a connection test button
- [ ] Author the recipe-generation system prompt: DSL vocabulary, unit discipline, printability constraints, strict JSON output
- [ ] Implement the self-heal loop — validator failure feeds the error back to the model, max 3 attempts, surface only the final result
- [ ] Add the prompt panel to the workbench dock with streaming progress and a cancel button
- [ ] Benchmark ≥25 real prompts; record first-pass and post-self-heal success rates in the repo
- [ ] Rate-limit and cap spend per request; show estimated token cost before sending

---

## Phase 4 — Photos, library, polish

**Outcome:** A user can photograph a space, answer two questions, and get a part that fits.
**Executor:** Claude Code · `claude-sonnet-4-6` (recommended)
**Gated by:** Phase 3 complete

- [ ] Add image input to the prompt panel (drag-drop, paste, file picker)
- [ ] Implement the scale-reference protocol — prompt the user for a coin/ruler/card in frame, or ask for the one or two critical measurements directly
- [ ] Build the clarifying-question flow: model may ask up to 2 questions before generating
- [ ] Ship a starter recipe library (dividers, brackets, hooks, organisers, feet, spacers, cable management) as editable presets
- [ ] Add printer profiles (bed size, nozzle, material) that feed the printability checker
- [ ] Add a first-run tutorial card to the Start page linking into Anvil, reusing `TutorialWidget`
- [ ] Write user docs and publish to the FreeCAD Addon Manager

---

## Phase 5 — Commercial tier

**Outcome:** The same Recipe core running in a browser, with managed keys and a shared library — the version people pay for.
**Executor:** Claude Code · `claude-opus-4-7` (recommended — architecture + deployment)
**Gated by:** Phase 4 complete and ≥100 addon installs

- [ ] Containerize headless `FreeCADCmd` as a recipe execution service
- [ ] Build the web frontend (Next.js + three.js) consuming the identical Part Recipe schema
- [ ] Implement managed-key tier with per-account metering and quota
- [ ] Add account, billing, and a shared/forkable recipe library
- [ ] Deploy per Max Effort AI protocol — `GIT_DIR=/nonexistent vercel --prod`, no GitHub↔Vercel linkage
- [ ] Evaluate print-service fulfilment partnership

---

**Legend:** `[ ]` not started · `[~]` in progress · `[x]` done · `[!]` blocked
