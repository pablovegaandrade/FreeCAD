// SPDX-License-Identifier: LGPL-2.1-or-later
/****************************************************************************
 *                                                                          *
 *   Copyright (c) 2024 The FreeCAD Project Association AISBL               *
 *                                                                          *
 *   This file is part of FreeCAD.                                          *
 *                                                                          *
 *   FreeCAD is free software: you can redistribute it and/or modify it     *
 *   under the terms of the GNU Lesser General Public License as            *
 *   published by the Free Software Foundation, either version 2.1 of the   *
 *   License, or (at your option) any later version.                        *
 *                                                                          *
 *   FreeCAD is distributed in the hope that it will be useful, but         *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of             *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU       *
 *   Lesser General Public License for more details.                        *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with FreeCAD. If not, see                                *
 *   <https://www.gnu.org/licenses/>.                                       *
 *                                                                          *
 ***************************************************************************/

#ifndef STARTGUI_TUTORIALWIDGET_H
#define STARTGUI_TUTORIALWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QVector>

namespace StartGui {

/**
 * @brief TutorialCard - A clickable card representing a single tutorial
 */
class TutorialCard : public QFrame
{
    Q_OBJECT

public:
    explicit TutorialCard(const QString& title, 
                          const QString& description, 
                          const QString& iconPath,
                          const QString& difficulty,
                          int estimatedMinutes,
                          QWidget* parent = nullptr);

Q_SIGNALS:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    void setupUi();
    QString _title;
    QString _description;
    QString _iconPath;
    QString _difficulty;
    int _estimatedMinutes;
    QLabel* _titleLabel;
    QLabel* _descriptionLabel;
    QLabel* _difficultyLabel;
    QLabel* _timeLabel;
    QLabel* _iconLabel;
};

/**
 * @brief TutorialWidget - Main widget containing tutorial cards and sections
 */
class TutorialWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TutorialWidget(QWidget* parent = nullptr);
    ~TutorialWidget() override = default;

Q_SIGNALS:
    void tutorialSelected(const QString& tutorialId);

private Q_SLOTS:
    void onGettingStartedClicked();
    void onSketchingBasicsClicked();
    void on3DModelingClicked();
    void onAdvancedTechniquesClicked();
    void onExportingClicked();
    void onOpenFullGuideClicked();

private:
    void setupUi();
    void retranslateUi();
    QWidget* createTutorialSection(const QString& sectionTitle, 
                                    const QVector<TutorialCard*>& cards);
    
    QVBoxLayout* _mainLayout;
    QLabel* _headerLabel;
    QLabel* _subheaderLabel;
    QPushButton* _fullGuideButton;
};

} // namespace StartGui

#endif // STARTGUI_TUTORIALWIDGET_H
