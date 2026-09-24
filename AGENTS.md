# Agent Instructions — Unreal MR Utility Kit Sample

Unreal Engine project that demonstrates how to combine the MR Utility Kit (MRUK) Scene API with Unreal's Procedural Content Generation (PCG) framework — a Scene Decorator projects a points grid onto walls/floor/ceiling and spawns static meshes on the hits.

## Source-of-truth files (read these first, do not duplicate their contents in this file)

For setup, build steps, SDK versions, and project layout, read:

- `README.md` — official setup, plug-in install options, and run flow
- `MRUtilityKitSample.uproject` — Unreal engine association and enabled plugins (MRUK / MetaXR live in the engine)
- `Config/` — `DefaultEngine.ini`, `DefaultGame.ini`, etc.
- `Source/MRUtilityKitSample/` — C++ module sources and `.Build.cs`
- `LICENSE` — license terms (Meta License for SDK material; MIT for clearly-marked docs)

## Quest / Horizon-specific notes

- Git LFS is required; run `git lfs install` before cloning.
- The entire sample is **PCG-driven**. To port it into another project, copy `PCG_SceneDecoration`, `PCG_Raycast`, `BP_SceneRayAnchor`, and `BP_CreatePointsGrid` together — they form a single unit.
- The custom `B_SceneRayAnchor` PCG node uses a non-obvious attribute-packing convention: **Position** = ray origin, **Scale** = ray direction, **Bounds Max** = grid extents, **Rotation** = grid rotation. Preserve these meanings when extending graphs.
- If decoration appears mid-air or fails to appear, the user almost certainly has not completed **Space Setup** on the headset, or the requested Scene label has no anchors in the room. MRUK Scene queries are useless without scene capture.
- MRUK Distance Map data is generated at runtime; toggling it changes spawn distributions noticeably and is the intended way to bias decoration near walls.

# Meta Quest tooling

This is a Meta Quest / Horizon OS sample. The bespoke intro above is the source of truth for what this project is and how it's built — use it (and the files it points at) instead of restating facts from memory.

When the user asks anything about Quest device behavior, build / deploy / debug / capture flows, on-device performance, or Horizon OS APIs, reach for these tools instead of generic Unreal answers:

- **`hzdb`** — Quest-aware ADB wrapper (device list, install / launch / stop, logs, screenshots, Perfetto traces, on-device docs search). Already wired up as an MCP server via `.mcp.json`, `.vscode/mcp.json`, and `.cursor/mcp.json`. Also runnable directly: `npx -y @meta-quest/hzdb <subcommand>`.
- **Meta Quest Agentic Tools** — the full skill set, including Unreal-specific skills: <https://github.com/meta-quest/agentic-tools>. Install per your client (Claude Code: `/plugin install meta-vr@meta-quest`; Gemini CLI: `gemini extensions install https://github.com/meta-quest/agentic-tools`; Cursor / VS Code: install the **Meta Horizon** extension from the Marketplace).

A few behavior expectations:

- **Read this repo's files first.** Before answering anything project-specific, read `README.md` and whichever source-of-truth files the intro above points at. Don't restate their contents in chat — quote or link instead.
- **Use `hzdb` for device-side work.** Anything that touches an attached Quest (install, launch, logs, screenshot, capture, manifest inspection) goes through `hzdb`, not raw `adb`.
- **Check live Horizon OS docs before answering API questions.** `hzdb docs search "..."` queries the live docs; training data on Horizon OS APIs goes stale fast.
- **Don't fabricate SDK / engine versions.** If a version isn't visible in this repo's files, say so rather than guessing.
