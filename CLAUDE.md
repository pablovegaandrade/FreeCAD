# AI Assistant Context for FreeCAD

## What is this?
FreeCAD fork focused on **UX modernization**. Make it look and feel like a 2025 app, not 1980s.

## Quick Facts
- C++ with Qt for UI
- Python for scripting
- CMake build system
- Focus area: `src/Mod/Start/` (Start page & onboarding)

## What We're Doing
Making FreeCAD user-friendly:
1. Modern Start page with tutorials
2. Better first-time user experience
3. Contemporary styling
4. Easy access to learning resources

## Important Files
| File | Purpose |
|------|---------|
| `src/Mod/Start/Gui/StartView.cpp` | Start page |
| `src/Mod/Start/Gui/TutorialWidget.cpp` | Tutorial cards |
| `src/Mod/Start/Gui/FirstStartWidget.cpp` | First-run setup |
| `src/Gui/Stylesheets/FreeCAD-Modern.qss` | Modern theme |
| `FRECAD_Userguide.md` | Tutorial content source |

## When Adding UI Components
1. Create `.h` and `.cpp` in appropriate `Gui/` folder
2. Add to `CMakeLists.txt`
3. Use Qt widgets (QWidget, QLabel, QPushButton, etc.)
4. Follow existing code patterns
5. Support both light and dark themes

## Don't
- Break existing functionality
- Add unnecessary dependencies
- Ignore accessibility
- Forget to update CMakeLists.txt
