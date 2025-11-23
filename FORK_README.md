# Prism Launcher Fork - Enhanced Mod Management

This is a fork of [Prism Launcher](https://github.com/PrismLauncher/PrismLauncher) with enhanced mod management features.

## New Features

### Mod Groups/Collections
Easily create and manage collections of mods for different playstyles:
- Create named mod groups (e.g., "PvP Build", "Adventure Mode", "Performance")
- Switch between mod sets with one click
- All mods in the active group are enabled, all others disabled
- Groups are saved per-instance

See [MOD_MANAGEMENT_IMPROVEMENTS.md](MOD_MANAGEMENT_IMPROVEMENTS.md) for detailed documentation.

## Building from Source

### Prerequisites

**Linux:**
```bash
# Debian/Ubuntu
sudo apt install build-essential cmake git qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools libqt5svg5-dev qttools5-dev

# Arch Linux
sudo pacman -S base-devel cmake git qt5-base qt5-svg qt5-tools

# Fedora
sudo dnf install cmake gcc-c++ git qt5-qtbase-devel qt5-qtsvg-devel qt5-qttools-devel

# openSUSE
sudo zypper install cmake gcc-c++ git libqt5-qtbase-devel libqt5-qtsvg-devel libqt5-qttools-devel
```

**Additional dependencies may be required.** See the [official Prism Launcher build instructions](https://prismlauncher.org/wiki/development/build-instructions/) for complete dependency lists.

### Build Instructions

1. **Clone this repository:**
   ```bash
   git clone https://github.com/quinnrallen-hub/PrismLauncher.git
   cd PrismLauncher
   ```

2. **Initialize submodules:**
   ```bash
   git submodule init
   git submodule update
   ```

3. **Create build directory and configure:**
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

4. **Build:**
   ```bash
   make -j$(nproc)
   ```

   Or for faster builds on systems with many cores:
   ```bash
   cmake --build . -j$(nproc)
   ```

5. **Install (optional):**
   ```bash
   sudo make install
   ```

   Or run directly from the build directory:
   ```bash
   ./PrismLauncher
   ```

### CMake Build Options

You can customize the build with CMake options:

```bash
# Build with debugging symbols
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build for release (optimized)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Install to a custom location
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local

# Use system libraries instead of bundled ones
cmake .. -DLauncher_FORCE_BUNDLED_LIBS=OFF
```

### Quick Build Script

For convenience, you can use this one-liner to build:

```bash
git clone https://github.com/quinnrallen-hub/PrismLauncher.git && \
cd PrismLauncher && \
git submodule init && \
git submodule update && \
mkdir build && cd build && \
cmake .. && \
make -j$(nproc)
```

## Using Mod Groups

1. Launch Prism Launcher
2. Select a Minecraft instance
3. Go to the "Mods" page
4. Click the **"Manage Groups"** button in the toolbar
5. Create a new group and add mods to it
6. Click **"Activate"** to enable that group's mods

Groups are automatically saved and will persist between launches.

## Differences from Upstream

This fork adds:
- `launcher/minecraft/mod/ModGroup.h` - Mod group data structures
- `launcher/minecraft/mod/ModGroup.cpp` - Mod group implementation
- `launcher/ui/dialogs/ModGroupDialog.h` - Mod groups UI dialog
- `launcher/ui/dialogs/ModGroupDialog.cpp` - Dialog implementation
- `launcher/ui/dialogs/ModGroupDialog.ui` - Dialog UI layout
- Modified `launcher/ui/pages/instance/ModFolderPage.*` - Integration point
- Updated `launcher/CMakeLists.txt` - Build system changes

## Contributing

This is a personal fork. If you'd like to contribute these features to the main Prism Launcher project, please submit a PR to the [upstream repository](https://github.com/PrismLauncher/PrismLauncher).

## License

This fork maintains the same license as Prism Launcher (GPL-3.0-only). See [LICENSE](LICENSE) for details.

## Credits

- Original Prism Launcher: https://github.com/PrismLauncher/PrismLauncher
- Mod Groups feature: Custom addition for easier mod management

## Troubleshooting

### Build fails with "Qt5 not found"
Make sure you have Qt5 development packages installed. See Prerequisites section above.

### Build fails with submodule errors
Run:
```bash
git submodule update --init --recursive
```

### Application crashes on launch
Try building with debug symbols and running through gdb:
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
gdb ./PrismLauncher
```

For more help, consult the [official Prism Launcher build documentation](https://prismlauncher.org/wiki/development/build-instructions/).
