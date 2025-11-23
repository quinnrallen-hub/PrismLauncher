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

#include <QDialog>
#include <memory>

#include "minecraft/mod/ModGroup.h"

class ModFolderModel;

namespace Ui {
class ModGroupDialog;
}

class ModGroupDialog : public QDialog {
    Q_OBJECT

   public:
    explicit ModGroupDialog(ModGroupManager* manager, ModFolderModel* model, QWidget* parent = nullptr);
    ~ModGroupDialog() override;

   private slots:
    void on_createGroupBtn_clicked();
    void on_deleteGroupBtn_clicked();
    void on_activateGroupBtn_clicked();
    void on_addModsBtn_clicked();
    void on_removeModsBtn_clicked();
    void on_groupsList_currentRowChanged(int row);

   private:
    void refreshGroupsList();
    void refreshModsList();
    void refreshAvailableModsList();

    Ui::ModGroupDialog* ui;
    ModGroupManager* m_manager;
    ModFolderModel* m_model;
    QString m_current_group_name;
};
