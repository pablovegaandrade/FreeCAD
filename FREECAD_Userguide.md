# FreeCAD Basic 3D Modeling User Guide

A comprehensive guide to creating 3D solids in FreeCAD, covering essential operations, sketching, and parametric modeling.

---

## Table of Contents

1. [Introduction to FreeCAD](#introduction-to-freecad)
2. [Getting Started](#getting-started)
3. [Navigation](#navigation)
4. [Creating Sketches](#creating-sketches)
5. [Constraints and Dimensions](#constraints-and-dimensions)
6. [3D Solid Operations](#3d-solid-operations)
7. [Modifying Models](#modifying-models)
8. [Advanced Operations](#advanced-operations)
9. [Exporting Models](#exporting-models)

---

## Introduction to FreeCAD

FreeCAD is a **completely free, open-source 3D CAD system** developed by a community of users.  Key features include:

- ✅ Free for both hobby and commercial projects
- ✅ Can be installed on any number of computers
- ✅ Works locally (no internet/cloud required)
- ✅ Modular system with different workbenches (WB)

### Workbenches

FreeCAD uses a modular system where modules are called **workbenches**:

| Workbench | Purpose |
|-----------|---------|
| **Part Design** | Primary workbench for creating 3D parts (parametric modeling) |
| **Part** | Alternative for 3D model creation |
| **Sketcher** | Creating 2D sketches (integrated with Part Design) |

---

## Getting Started

### Starting a New Project

1. Select the **Part Design** workbench
2. Click the **New** button or click on **Parametric Part**
3. A new project opens with a **Body** already created

> **Important:** A Body represents a single solid.  One project can have multiple Bodies, but each Body must contain only one continuous solid (not separate pieces).

### Initial Setup

1. **Navigation Style:** Set to **Blender** navigation style for optimal control
2. **Toolbar Customization:** 
   - Right-click on the toolbar area
   - Enable desired toolbar options
   - Drag toolbars to preferred positions

---

## Navigation

### Mouse Controls (Blender Style)

| Action | Control |
|--------|---------|
| **Rotate view** | Press mouse wheel |
| **Pan view** | Shift + mouse wheel press |
| **Zoom in/out** | Scroll mouse wheel |

> **Tip:** Hover your cursor over the navigation area to see a tooltip with navigation hints for your selected style.

---

## Creating Sketches

### Starting a Sketch

1. Click the **Create Sketch** icon
2. Choose a sketch plane: 
   - **XY Plane** - Horizontal plane
   - **XZ Plane** - Vertical front plane
   - **YZ Plane** - Vertical side plane
   - **Model Face** - Select any face on an existing solid

### Drawing Tools

#### Rectangle
```
1. Select rectangle tool
2. Click to define first corner
3. Click to define opposite corner
   - OR enter dimensions (e.g., 50 x 30 mm)
```

#### Centered Rectangle
```
1. Select centered rectangle tool
2. Click to define center point
3. Enter dimensions
4. Options:  Rounded corners, Frame (offset rectangle)
```

#### Circle
```
1. Select circle tool
2. Click to define center point
3. Enter diameter or click to set size
```

#### Line/Polyline
```
1. Select line or polyline tool
2. Click to define start point
3. Click to define end point(s)
4. Right-click to finish
```

#### Slot
```
1. Select slot tool
2. Click to define first point
3. Enter length and angle
4. Enter width
```

#### Polygon (Hexagon)
```
1. Select polygon tool (e.g., hexagon)
2. Click to define center
3. Enter circumscribed circle radius
4. Enter angle for orientation
```

### Construction Geometry

Construction geometry is **auxiliary geometry** that helps position other elements but doesn't become part of the 3D solid.

**To create construction geometry:**
1. Draw the geometry normally
2. Select the geometry
3. Click **Toggle Construction Geometry**
4. Geometry changes color/style to indicate it's auxiliary

### External Geometry

Use external geometry to **link a sketch to existing solid edges**: 

1. Select **Create External Geometry** command
2. Click on edges of existing solids
3. These edges become reference lines in your sketch

---

## Constraints and Dimensions

### Understanding Constraints

Sketches should be **fully constrained** (all positions and dimensions defined). FreeCAD shows: 
- **Degrees of Freedom** - Number of undefined parameters
- **Green geometry** - Fully constrained
- **Orange/White geometry** - Under-constrained

### Adding Dimensions

**Method 1: While Drawing**
- Enter dimensions directly when prompted during geometry creation

**Method 2: After Drawing**
1. Select geometry (line, circle, etc.)
2. Press **D** or choose dimension command
3. Enter the value

### Common Constraints

| Constraint | Shortcut | Description |
|------------|----------|-------------|
| **Coincident** | - | Links two points or a point to a line |
| **Equal** | E | Makes two elements the same size |
| **Symmetry** | - | Places elements symmetrically about a point/line |
| **Horizontal/Vertical** | H/V | Forces line orientation |
| **Parallel** | - | Makes lines parallel |

### Automatic Constraints

Enable **Automatic Constraints** to automatically apply constraints while drawing: 
- Hovering over a line highlights it
- Clicking creates a coincident constraint automatically

### Using Symmetry Constraints

**For symmetric placement:**
1. Select first point
2. Select second point
3. Select axis/line of symmetry
4. Apply Symmetry constraint

---

## 3D Solid Operations

### Pad (Extrusion)

Creates a 3D solid by **extruding a 2D sketch**. 

```
1. Close the sketch
2. Select Pad operation
3. Enter extrusion length
4. Click OK
```

### Pocket

**Removes material** from a solid based on a 2D sketch. 

```
1. Create sketch on solid face
2. Close sketch
3. Select Pocket operation
4. Define depth: 
   - Specific value (e.g., 5 mm)
   - "Up to First" - cuts to next face
   - With offset if needed
5. Click OK
```

### Revolution

Creates a solid by **rotating a sketch around an axis**.

```
1. Create cross-section sketch
2. Close sketch
3. Select Revolution operation
4. Choose rotation axis (Z-axis, Y-axis, or reference)
5. Set angle (default 360°)
6. Click OK
```

> **Note:** Use "Symmetric to plane" option to extrude equally in both directions.

### Groove

**Removes material** by rotating a sketch around an axis (opposite of Revolution).

```
1. Create cross-section sketch
2. Close sketch
3. Select Groove operation
4. Choose axis and angle
5. Click OK
```

### Chamfer

Adds angled edges to corners. 

```
1. Select edge(s) - use Ctrl for multiple
   - OR select a face (applies to all edges)
2. Choose Chamfer operation
3. Enter chamfer size
4. Click OK
```

### Fillet

Adds rounded edges to corners.

```
1. Select edge(s) - use Ctrl for multiple
   - OR select a face
2. Choose Fillet operation
3. Enter radius value
4. Click OK
```

### Thickness (Shell)

Creates a hollow box with specified wall thickness.

```
1. Create a solid
2. Select face(s) to remove (Ctrl for multiple)
3. Choose Thickness operation
4. Enter wall thickness
5. Choose direction (inward/outward)
6. Click OK
```

---

## Modifying Models

### Parametric Editing

FreeCAD uses **parametric modeling** - you can modify any previous step:

#### Editing Sketches
1.  Expand the operation in the Model Tree
2. Right-click on the sketch
3. Select **Edit Sketch**
4. Modify dimensions or geometry
5. Close sketch - changes propagate automatically

#### Editing Operations
1. Double-click or right-click the operation
2. Select **Edit [Operation Name]**
3. Modify parameters
4. Click OK

### Deleting Geometry

**In Sketcher:**
- Select geometry (click or drag selection window)
- Press **Delete** key

---

## Advanced Operations

### Mirror

Creates a **symmetric copy** of features. 

```
1. Select operation(s) to mirror (Ctrl for multiple)
2. Choose Mirror operation
3. Select mirror plane (XY, XZ, YZ, or reference)
4. Click OK
```

> **Tip:** Changes to the original feature automatically update the mirrored copy.

### Linear Pattern

Creates **copies along a straight line**.

```
1. Select operation to pattern
2. Choose Linear Pattern
3. Define direction (axis or edge)
4. Set number of instances
5. Set pattern length (first to last element distance)
6. Click OK
```

### Polar (Circular) Pattern

Creates **copies around a circular path**.

```
1. Select operation to pattern
2. Choose Polar Pattern
3. Select rotation axis
4. Set number of instances
5. Set angle (default 360°)
6. Click OK
```

---

## Exporting Models

### Changing Appearance

1. Right-click on Body
2. Select **Appearance**
3. Choose material or custom color

### Export Formats

| Format | Best For | Notes |
|--------|----------|-------|
| **STEP** | 3D printing, CAD exchange | High accuracy, preserves curves |
| **STL** | 3D printing | Mesh format, curved surfaces become segments |

### Export Process

```
1. Select what to export
2. File → Export
3. Choose location and filename
4. Select format (STEP recommended)
5. Click Save
6. Confirm settings → OK
```

---

## Quick Reference

### Keyboard Shortcuts

| Key | Action |
|-----|--------|
| D | Add dimension |
| E | Equal constraint |
| H | Horizontal constraint |
| V | Vertical constraint |
| Delete | Remove selected geometry |
| Ctrl+Z | Undo |
| Ctrl | Multi-select |

### Best Practices

1. **Always fully constrain sketches** - Green geometry = fully defined
2. **One Body = One Solid** - Keep separate parts in separate Bodies
3. **Use constraints over dimensions** when elements should stay linked
4. **Use construction geometry** for positioning references
5. **Plan your modeling approach** before starting complex parts
6. **Save in STEP format** for best compatibility and accuracy

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| Fillet/Chamfer not working | This is a known FreeCAD limitation; try different edge order or smaller values |
| Sketch not fully constrained | Add dimensions or constraints until Degrees of Freedom = 0 |
| Cannot create solid from sketch | Ensure sketch has closed profiles with no gaps |
| Pattern extends beyond solid | Adjust pattern length to account for feature offset from edge |

---

*This guide covers the fundamentals of FreeCAD 3D modeling. Practice with simple projects and gradually increase complexity as you become comfortable with the tools.*