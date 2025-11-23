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

#include "ModGroupDialog.h"
#include "ui_ModGroupDialog.h"

#include <QInputDialog>
#include <QMessageBox>

#include "minecraft/mod/ModFolderModel.h"

ModGroupDialog::ModGroupDialog(ModGroupManager* manager, ModFolderModel* model, QWidget* parent)
    : QDialog(parent), ui(new Ui::ModGroupDialog), m_manager(manager), m_model(model)
{
    ui->setupUi(this);

    refreshGroupsList();

    connect(ui->createGroupBtn, &QPushButton::clicked, this, &ModGroupDialog::on_createGroupBtn_clicked);
    connect(ui->deleteGroupBtn, &QPushButton::clicked, this, &ModGroupDialog::on_deleteGroupBtn_clicked);
    connect(ui->activateGroupBtn, &QPushButton::clicked, this, &ModGroupDialog::on_activateGroupBtn_clicked);
    connect(ui->addModsBtn, &QPushButton::clicked, this, &ModGroupDialog::on_addModsBtn_clicked);
    connect(ui->removeModsBtn, &QPushButton::clicked, this, &ModGroupDialog::on_removeModsBtn_clicked);
    connect(ui->groupsList, &QListWidget::currentRowChanged, this, &ModGroupDialog::on_groupsList_currentRowChanged);
}

ModGroupDialog::~ModGroupDialog()
{
    delete ui;
}

void ModGroupDialog::refreshGroupsList()
{
    ui->groupsList->clear();
    const auto& groups = m_manager->groups();
    for (const auto& group : groups) {
        QString label = group.name;
        if (group.is_active) {
            label += " (Active)";
        }
        ui->groupsList->addItem(label);
    }
}

void ModGroupDialog::refreshModsList()
{
    ui->groupModsList->clear();
    if (m_current_group_name.isEmpty())
        return;

    ModGroup* group = m_manager->getGroup(m_current_group_name);
    if (!group)
        return;

    for (const auto& filename : group->mod_filenames) {
        ui->groupModsList->addItem(filename);
    }
}

void ModGroupDialog::refreshAvailableModsList()
{
    ui->availableModsList->clear();

    // Get all mods from the model
    auto all_mods = m_model->allResources();

    // Filter out mods already in the current group
    ModGroup* group = nullptr;
    if (!m_current_group_name.isEmpty()) {
        group = m_manager->getGroup(m_current_group_name);
    }

    for (const auto& mod : all_mods) {
        QString filename = mod->fileinfo().fileName();
        if (!group || !group->contains(filename)) {
            ui->availableModsList->addItem(filename);
        }
    }
}

void ModGroupDialog::on_createGroupBtn_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("Create Mod Group"), tr("Group name:"), QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        ModGroup group;
        group.name = name;
        group.description = "";
        group.is_active = false;

        m_manager->addGroup(group);
        m_manager->save();
        refreshGroupsList();
    }
}

void ModGroupDialog::on_deleteGroupBtn_clicked()
{
    if (m_current_group_name.isEmpty()) {
        QMessageBox::information(this, tr("No Group Selected"), tr("Please select a group to delete."));
        return;
    }

    auto reply = QMessageBox::question(this, tr("Delete Group"), tr("Are you sure you want to delete the group '%1'?").arg(m_current_group_name),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_manager->removeGroup(m_current_group_name);
        m_manager->save();
        m_current_group_name.clear();
        refreshGroupsList();
        refreshModsList();
        refreshAvailableModsList();
    }
}

void ModGroupDialog::on_activateGroupBtn_clicked()
{
    if (m_current_group_name.isEmpty()) {
        QMessageBox::information(this, tr("No Group Selected"), tr("Please select a group to activate."));
        return;
    }

    ModGroup* group = m_manager->getGroup(m_current_group_name);
    if (!group)
        return;

    // Disable all mods first
    auto all_mods = m_model->allResources();
    for (const auto& mod : all_mods) {
        m_model->setResourceEnabled({m_model->index(mod)}, ModFolderModel::Disable);
    }

    // Enable only mods in this group
    for (const auto& filename : group->mod_filenames) {
        for (int i = 0; i < m_model->size(); ++i) {
            auto& mod = m_model->at(i);
            if (mod.fileinfo().fileName() == filename) {
                m_model->setResourceEnabled({m_model->index(i, 0)}, ModFolderModel::Enable);
                break;
            }
        }
    }

    m_manager->setActiveGroup(m_current_group_name);
    m_manager->save();
    refreshGroupsList();

    QMessageBox::information(this, tr("Group Activated"), tr("Group '%1' has been activated!").arg(m_current_group_name));
}

void ModGroupDialog::on_addModsBtn_clicked()
{
    if (m_current_group_name.isEmpty()) {
        QMessageBox::information(this, tr("No Group Selected"), tr("Please select a group first."));
        return;
    }

    auto selected = ui->availableModsList->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, tr("No Mods Selected"), tr("Please select mods to add."));
        return;
    }

    ModGroup* group = m_manager->getGroup(m_current_group_name);
    if (!group)
        return;

    for (const auto& item : selected) {
        group->addMod(item->text());
    }

    m_manager->updateGroup(*group);
    m_manager->save();

    refreshModsList();
    refreshAvailableModsList();
}

void ModGroupDialog::on_removeModsBtn_clicked()
{
    if (m_current_group_name.isEmpty()) {
        QMessageBox::information(this, tr("No Group Selected"), tr("Please select a group first."));
        return;
    }

    auto selected = ui->groupModsList->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, tr("No Mods Selected"), tr("Please select mods to remove."));
        return;
    }

    ModGroup* group = m_manager->getGroup(m_current_group_name);
    if (!group)
        return;

    for (const auto& item : selected) {
        group->removeMod(item->text());
    }

    m_manager->updateGroup(*group);
    m_manager->save();

    refreshModsList();
    refreshAvailableModsList();
}

void ModGroupDialog::on_groupsList_currentRowChanged(int row)
{
    if (row >= 0 && row < m_manager->groups().size()) {
        m_current_group_name = m_manager->groups()[row].name;
        refreshModsList();
        refreshAvailableModsList();
    } else {
        m_current_group_name.clear();
    }
}
