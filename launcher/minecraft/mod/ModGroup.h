// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2025
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <QJsonObject>
#include <QList>
#include <QString>
#include <QStringList>

/**
 * Represents a named collection of mods that can be enabled/disabled as a group.
 * This allows users to easily switch between different mod sets (e.g., "PvP", "Adventure", "Performance").
 */
struct ModGroup {
    QString name;              // Display name of the group
    QString description;       // Optional description
    QStringList mod_filenames; // List of mod filenames in this group (just the filename, not full path)
    bool is_active = false;    // Whether this group is currently active

    // Serialize to JSON for storage
    [[nodiscard]] QJsonObject toJson() const;

    // Deserialize from JSON
    [[nodiscard]] static ModGroup fromJson(const QJsonObject& obj);

    // Check if this group contains a specific mod
    [[nodiscard]] bool contains(const QString& filename) const;

    // Add a mod to this group
    void addMod(const QString& filename);

    // Remove a mod from this group
    void removeMod(const QString& filename);
};

/**
 * Manages a collection of ModGroups, providing save/load functionality.
 */
class ModGroupManager {
   public:
    ModGroupManager(const QString& instance_dir);

    // Load groups from file
    bool load();

    // Save groups to file
    bool save() const;

    // Get all groups
    [[nodiscard]] const QList<ModGroup>& groups() const { return m_groups; }

    // Add a new group
    void addGroup(const ModGroup& group);

    // Remove a group by name
    void removeGroup(const QString& name);

    // Get a group by name
    [[nodiscard]] ModGroup* getGroup(const QString& name);

    // Update an existing group
    void updateGroup(const ModGroup& group);

    // Get the currently active group (if any)
    [[nodiscard]] ModGroup* activeGroup();

    // Set active group (deactivates others)
    void setActiveGroup(const QString& name);

    // Clear active group (no group active)
    void clearActiveGroup();

   private:
    QString m_groups_file;
    QList<ModGroup> m_groups;
};
