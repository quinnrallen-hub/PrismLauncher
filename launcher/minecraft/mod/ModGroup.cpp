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

#include "ModGroup.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

QJsonObject ModGroup::toJson() const
{
    QJsonObject obj;
    obj["name"] = name;
    obj["description"] = description;
    obj["is_active"] = is_active;

    QJsonArray mods;
    for (const auto& mod : mod_filenames) {
        mods.append(mod);
    }
    obj["mods"] = mods;

    return obj;
}

ModGroup ModGroup::fromJson(const QJsonObject& obj)
{
    ModGroup group;
    group.name = obj["name"].toString();
    group.description = obj["description"].toString();
    group.is_active = obj["is_active"].toBool();

    QJsonArray mods = obj["mods"].toArray();
    for (const auto& mod : mods) {
        group.mod_filenames.append(mod.toString());
    }

    return group;
}

bool ModGroup::contains(const QString& filename) const
{
    return mod_filenames.contains(filename);
}

void ModGroup::addMod(const QString& filename)
{
    if (!contains(filename)) {
        mod_filenames.append(filename);
    }
}

void ModGroup::removeMod(const QString& filename)
{
    mod_filenames.removeAll(filename);
}

// ModGroupManager implementation

ModGroupManager::ModGroupManager(const QString& instance_dir)
{
    QDir dir(instance_dir);
    m_groups_file = dir.absoluteFilePath("mod_groups.json");
}

bool ModGroupManager::load()
{
    QFile file(m_groups_file);
    if (!file.exists()) {
        // No groups file yet, start with empty list
        m_groups.clear();
        return true;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray groups = root["groups"].toArray();

    m_groups.clear();
    for (const auto& groupVal : groups) {
        if (groupVal.isObject()) {
            m_groups.append(ModGroup::fromJson(groupVal.toObject()));
        }
    }

    return true;
}

bool ModGroupManager::save() const
{
    QJsonObject root;
    QJsonArray groups;

    for (const auto& group : m_groups) {
        groups.append(group.toJson());
    }

    root["groups"] = groups;
    root["version"] = 1;

    QJsonDocument doc(root);

    QFile file(m_groups_file);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(doc.toJson());
    file.close();

    return true;
}

void ModGroupManager::addGroup(const ModGroup& group)
{
    // Check if group with same name exists
    for (int i = 0; i < m_groups.size(); ++i) {
        if (m_groups[i].name == group.name) {
            m_groups[i] = group;
            return;
        }
    }
    m_groups.append(group);
}

void ModGroupManager::removeGroup(const QString& name)
{
    for (int i = 0; i < m_groups.size(); ++i) {
        if (m_groups[i].name == name) {
            m_groups.removeAt(i);
            return;
        }
    }
}

ModGroup* ModGroupManager::getGroup(const QString& name)
{
    for (int i = 0; i < m_groups.size(); ++i) {
        if (m_groups[i].name == name) {
            return &m_groups[i];
        }
    }
    return nullptr;
}

void ModGroupManager::updateGroup(const ModGroup& group)
{
    for (int i = 0; i < m_groups.size(); ++i) {
        if (m_groups[i].name == group.name) {
            m_groups[i] = group;
            return;
        }
    }
}

ModGroup* ModGroupManager::activeGroup()
{
    for (int i = 0; i < m_groups.size(); ++i) {
        if (m_groups[i].is_active) {
            return &m_groups[i];
        }
    }
    return nullptr;
}

void ModGroupManager::setActiveGroup(const QString& name)
{
    // Deactivate all groups first
    for (auto& group : m_groups) {
        group.is_active = false;
    }

    // Activate the specified group
    ModGroup* group = getGroup(name);
    if (group) {
        group->is_active = true;
    }
}

void ModGroupManager::clearActiveGroup()
{
    for (auto& group : m_groups) {
        group.is_active = false;
    }
}
