# Mod Management Improvements for Prism Launcher

This document describes the improvements made to Prism Launcher's mod management system to make adding and removing mods easier.

## Summary of Changes

### 1. Mod Groups/Collections Feature

**Purpose**: Allow users to create named collections of mods and quickly switch between them (e.g., "PvP Build", "Adventure Mode", "Performance").

**Files Added**:
- `launcher/minecraft/mod/ModGroup.h` - Data structures for mod groups
- `launcher/minecraft/mod/ModGroup.cpp` - Implementation of mod group management
- `launcher/ui/dialogs/ModGroupDialog.h` - Dialog UI header
- `launcher/ui/dialogs/ModGroupDialog.cpp` - Dialog implementation
- `launcher/ui/dialogs/ModGroupDialog.ui` - Qt Designer UI layout

**Files Modified**:
- `launcher/ui/pages/instance/ModFolderPage.h` - Added mod group manager and dialog slot
- `launcher/ui/pages/instance/ModFolderPage.cpp` - Integrated mod groups UI button
- `launcher/CMakeLists.txt` - Added new files to build system

#### How It Works:

**ModGroup Structure**:
- Stores group name, description, and list of mod filenames
- Tracks whether the group is currently active
- Serializes to/from JSON for persistent storage

**ModGroupManager**:
- Manages collection of groups
- Saves groups to `<instance_dir>/mod_groups.json`
- Provides add/remove/update operations
- Handles active group selection

**ModGroupDialog UI**:
```
┌─────────────────────────────────────────────────────────┐
│  Manage Mod Groups                                      │
├─────────────────────────────────────────────────────────┤
│  Groups          │  Mods in Group  │  Available Mods   │
│  ┌─────────┐    │  ┌────────────┐ │  ┌──────────────┐ │
│  │ PvP     │    │  │ mod1.jar   │ │  │ mod4.jar     │ │
│  │ Adventure│   │  │ mod2.jar   │ │  │ mod5.jar     │ │
│  │ Creative│    │  │            │ │  │              │ │
│  └─────────┘    │  └────────────┘ │  └──────────────┘ │
│  [Create]       │  [Remove Sel]   │  [Add Selected]   │
│  [Delete]       │                 │                    │
│  [Activate]     │                 │                    │
└─────────────────────────────────────────────────────────┘
```

**User Workflow**:
1. Click "Manage Groups" button in mod folder page toolbar
2. Create a new group with "Create" button
3. Select mods from "Available Mods" list
4. Click "Add Selected" to add them to the group
5. Click "Activate" to enable only mods in this group (disables all others)
6. Switch between groups by selecting and clicking "Activate"

#### Storage Format:
```json
{
  "version": 1,
  "groups": [
    {
      "name": "PvP Build",
      "description": "Optimized for PvP",
      "is_active": true,
      "mods": ["mod1.jar", "mod2.jar", "mod3.jar"]
    }
  ]
}
```

### 2. Integration Points

**ModFolderPage Constructor**:
- Initializes `ModGroupManager` with instance directory
- Loads existing groups from JSON
- Adds "Manage Groups" button to toolbar
- Positioned after "Export Metadata" action

**ModFolderPage Destructor**:
- Automatically saves groups on page close

**New Toolbar Button**:
- Label: "Manage Groups"
- Tooltip: "Create and manage mod groups for easy switching"
- Opens ModGroupDialog when clicked

## Implementation Details

### Key Classes

**ModGroup** (`ModGroup.h`):
```cpp
struct ModGroup {
    QString name;              // Group display name
    QString description;       // Optional description
    QStringList mod_filenames; // Mod files in group
    bool is_active;           // Currently active flag

    QJsonObject toJson() const;
    static ModGroup fromJson(const QJsonObject& obj);
    bool contains(const QString& filename) const;
    void addMod(const QString& filename);
    void removeMod(const QString& filename);
};
```

**ModGroupManager** (`ModGroup.h`):
```cpp
class ModGroupManager {
public:
    ModGroupManager(const QString& instance_dir);
    bool load();
    bool save() const;
    const QList<ModGroup>& groups() const;
    void addGroup(const ModGroup& group);
    void removeGroup(const QString& name);
    ModGroup* getGroup(const QString& name);
    void updateGroup(const ModGroup& group);
    ModGroup* activeGroup();
    void setActiveGroup(const QString& name);
    void clearActiveGroup();
};
```

**ModGroupDialog** (`ModGroupDialog.h`):
```cpp
class ModGroupDialog : public QDialog {
public:
    ModGroupDialog(ModGroupManager* manager,
                   ModFolderModel* model,
                   QWidget* parent = nullptr);

private slots:
    void on_createGroupBtn_clicked();
    void on_deleteGroupBtn_clicked();
    void on_activateGroupBtn_clicked();
    void on_addModsBtn_clicked();
    void on_removeModsBtn_clicked();
    void on_groupsList_currentRowChanged(int row);
};
```

## Benefits

1. **Easy Mod Set Switching**: Users can quickly switch between different mod configurations without manually enabling/disabling individual mods

2. **Organization**: Keep mods organized by purpose (PvP, adventure, creative, performance, etc.)

3. **Experimentation**: Try different mod combinations without losing track of working configurations

4. **Multiple Playstyles**: Maintain separate mod sets for different server types or gameplay modes

5. **Simplified Management**: Instead of remembering which mods to enable/disable, just activate the appropriate group

## Future Enhancements (Not Implemented)

Additional features that could be added:

1. **Quick Switch Dropdown**: Add a dropdown in the toolbar to switch groups without opening the dialog

2. **Auto-Dependencies**: Automatically include dependency mods when activating a group

3. **Import/Export Groups**: Share group configurations with other users

4. **Mod Conflict Detection**: Warn about conflicting mods in a group

5. **Per-Row Update Buttons**: Add update button next to each mod in the list view (requires custom delegate implementation)

6. **Keyboard Shortcuts**: Add shortcuts for common operations (Ctrl+G for manage groups, etc.)

## Testing

To test these changes:

1. Install required build dependencies (CMake, Qt5, etc.)
2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
3. Run Prism Launcher
4. Navigate to an instance's Mods page
5. Click "Manage Groups" button
6. Create a group, add some mods, and activate it
7. Verify that only mods in the group are enabled
8. Switch to another group and verify mods update correctly
9. Restart launcher and verify groups are persisted

## Notes

- Groups are stored per-instance in `<instance_root>/mod_groups.json`
- Activating a group disables ALL mods first, then enables only group mods
- Groups track filenames only, not full paths
- If a mod file is deleted, it remains in the group definition but won't cause errors
- The dialog uses multi-selection lists for easy bulk operations

## Code Quality

- Follows existing Prism Launcher code style and patterns
- Uses existing Qt infrastructure (QDialog, QListWidget, etc.)
- Integrates cleanly with existing ModFolderModel
- Minimal changes to existing code
- All new code properly licensed (GPL-3.0-only)
