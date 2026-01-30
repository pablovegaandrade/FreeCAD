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

#include "TutorialWidget.h"
#include "FlowLayout.h"

#include <QDesktopServices>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QScrollArea>
#include <QUrl>
#include <QApplication>
#include <QStyle>

#include <App/Application.h>
#include <Gui/Application.h>
#include <Gui/Command.h>
#include <Gui/MainWindow.h>
#include <Gui/BitmapFactory.h>

using namespace StartGui;

// =============================================================================
// TutorialCard Implementation
// =============================================================================

TutorialCard::TutorialCard(const QString& title, 
                           const QString& description, 
                           const QString& iconPath,
                           const QString& difficulty,
                           int estimatedMinutes,
                           QWidget* parent)
    : QFrame(parent)
    , _title(title)
    , _description(description)
    , _iconPath(iconPath)
    , _difficulty(difficulty)
    , _estimatedMinutes(estimatedMinutes)
    , _titleLabel(nullptr)
    , _descriptionLabel(nullptr)
    , _difficultyLabel(nullptr)
    , _timeLabel(nullptr)
    , _iconLabel(nullptr)
{
    setObjectName(QStringLiteral("TutorialCard"));
    setCursor(Qt::PointingHandCursor);
    setFixedSize(280, 160);
    setupUi();
}

void TutorialCard::setupUi()
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(8);

    // Header with icon and title
    auto headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(12);

    _iconLabel = new QLabel(this);
    _iconLabel->setFixedSize(40, 40);
    if (!_iconPath.isEmpty()) {
        QPixmap pixmap(_iconPath);
        if (!pixmap.isNull()) {
            _iconLabel->setPixmap(pixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    headerLayout->addWidget(_iconLabel);

    auto titleContainer = new QVBoxLayout();
    titleContainer->setSpacing(2);
    
    _titleLabel = new QLabel(_title, this);
    _titleLabel->setObjectName(QStringLiteral("TutorialCardTitle"));
    _titleLabel->setWordWrap(true);
    QFont titleFont = _titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 1);
    _titleLabel->setFont(titleFont);
    titleContainer->addWidget(_titleLabel);

    // Difficulty badge
    _difficultyLabel = new QLabel(_difficulty, this);
    _difficultyLabel->setObjectName(QStringLiteral("TutorialCardDifficulty"));
    QFont difficultyFont = _difficultyLabel->font();
    difficultyFont.setPointSize(difficultyFont.pointSize() - 2);
    _difficultyLabel->setFont(difficultyFont);
    titleContainer->addWidget(_difficultyLabel);

    headerLayout->addLayout(titleContainer);
    headerLayout->addStretch();
    mainLayout->addLayout(headerLayout);

    // Description
    _descriptionLabel = new QLabel(_description, this);
    _descriptionLabel->setObjectName(QStringLiteral("TutorialCardDescription"));
    _descriptionLabel->setWordWrap(true);
    _descriptionLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    mainLayout->addWidget(_descriptionLabel, 1);

    // Footer with time estimate
    auto footerLayout = new QHBoxLayout();
    _timeLabel = new QLabel(QString::fromLatin1("⏱ %1 min").arg(_estimatedMinutes), this);
    _timeLabel->setObjectName(QStringLiteral("TutorialCardTime"));
    QFont timeFont = _timeLabel->font();
    timeFont.setPointSize(timeFont.pointSize() - 1);
    _timeLabel->setFont(timeFont);
    footerLayout->addWidget(_timeLabel);
    footerLayout->addStretch();
    
    auto startLabel = new QLabel(tr("Start →"), this);
    startLabel->setObjectName(QStringLiteral("TutorialCardStart"));
    footerLayout->addWidget(startLabel);
    
    mainLayout->addLayout(footerLayout);

    // Style the card
    setStyleSheet(QStringLiteral(
        "QFrame#TutorialCard {"
        "  background-color: palette(base);"
        "  border: 1px solid palette(mid);"
        "  border-radius: 12px;"
        "}"
        "QFrame#TutorialCard:hover {"
        "  border: 2px solid palette(highlight);"
        "  background-color: palette(alternateBase);"
        "}"
        "QLabel#TutorialCardTitle {"
        "  color: palette(text);"
        "}"
        "QLabel#TutorialCardDescription {"
        "  color: palette(text);"
        "}"
        "QLabel#TutorialCardDifficulty {"
        "  color: palette(highlight);"
        "  padding: 2px 6px;"
        "  border-radius: 4px;"
        "}"
        "QLabel#TutorialCardTime {"
        "  color: palette(dark);"
        "}"
        "QLabel#TutorialCardStart {"
        "  color: palette(highlight);"
        "  font-weight: bold;"
        "}"
    ));
}

void TutorialCard::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        Q_EMIT clicked();
    }
    QFrame::mousePressEvent(event);
}

void TutorialCard::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event)
    // Visual feedback is handled by stylesheet :hover
}

void TutorialCard::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    // Visual feedback is handled by stylesheet
}

// =============================================================================
// TutorialWidget Implementation
// =============================================================================

TutorialWidget::TutorialWidget(QWidget* parent)
    : QWidget(parent)
    , _mainLayout(nullptr)
    , _headerLabel(nullptr)
    , _subheaderLabel(nullptr)
    , _fullGuideButton(nullptr)
{
    setObjectName(QStringLiteral("TutorialWidget"));
    setupUi();
    retranslateUi();
}

void TutorialWidget::setupUi()
{
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(24, 24, 24, 24);
    _mainLayout->setSpacing(20);

    // Header section
    _headerLabel = new QLabel(this);
    _headerLabel->setObjectName(QStringLiteral("TutorialHeader"));
    QFont headerFont = _headerLabel->font();
    headerFont.setPointSize(headerFont.pointSize() + 8);
    headerFont.setBold(true);
    _headerLabel->setFont(headerFont);
    _mainLayout->addWidget(_headerLabel);

    _subheaderLabel = new QLabel(this);
    _subheaderLabel->setObjectName(QStringLiteral("TutorialSubheader"));
    _subheaderLabel->setWordWrap(true);
    _mainLayout->addWidget(_subheaderLabel);

    // Create tutorial cards
    auto gettingStarted = new TutorialCard(
        tr("Getting Started"),
        tr("Learn the basics: navigation, workbenches, and creating your first project"),
        QStringLiteral(":/icons/document-new.svg"),
        tr("Beginner"),
        10,
        this
    );
    connect(gettingStarted, &TutorialCard::clicked, this, &TutorialWidget::onGettingStartedClicked);

    auto sketchingBasics = new TutorialCard(
        tr("Sketching Basics"),
        tr("Master 2D sketches: rectangles, circles, constraints, and dimensions"),
        QStringLiteral(":/icons/Sketcher_NewSketch.svg"),
        tr("Beginner"),
        15,
        this
    );
    connect(sketchingBasics, &TutorialCard::clicked, this, &TutorialWidget::onSketchingBasicsClicked);

    auto modeling3D = new TutorialCard(
        tr("3D Solid Modeling"),
        tr("Create 3D objects using Pad, Pocket, Revolution, and more"),
        QStringLiteral(":/icons/PartDesignWorkbench.svg"),
        tr("Intermediate"),
        20,
        this
    );
    connect(modeling3D, &TutorialCard::clicked, this, &TutorialWidget::on3DModelingClicked);

    auto advanced = new TutorialCard(
        tr("Advanced Techniques"),
        tr("Patterns, mirrors, fillets, chamfers, and parametric editing"),
        QStringLiteral(":/icons/preferences-general.svg"),
        tr("Advanced"),
        25,
        this
    );
    connect(advanced, &TutorialCard::clicked, this, &TutorialWidget::onAdvancedTechniquesClicked);

    auto exporting = new TutorialCard(
        tr("Exporting Models"),
        tr("Export your designs to STEP, STL, and other formats"),
        QStringLiteral(":/icons/document-save-as.svg"),
        tr("Beginner"),
        5,
        this
    );
    connect(exporting, &TutorialCard::clicked, this, &TutorialWidget::onExportingClicked);

    // Tutorial cards in a flow layout
    auto cardsWidget = new QWidget(this);
    auto flowLayout = new FlowLayout(cardsWidget, 16, 16, 16);
    flowLayout->addWidget(gettingStarted);
    flowLayout->addWidget(sketchingBasics);
    flowLayout->addWidget(modeling3D);
    flowLayout->addWidget(advanced);
    flowLayout->addWidget(exporting);
    
    _mainLayout->addWidget(cardsWidget);
    _mainLayout->addStretch();

    // Full guide button at the bottom
    auto buttonLayout = new QHBoxLayout();
    _fullGuideButton = new QPushButton(this);
    _fullGuideButton->setObjectName(QStringLiteral("FullGuideButton"));
    _fullGuideButton->setMinimumHeight(44);
    _fullGuideButton->setMinimumWidth(200);
    _fullGuideButton->setCursor(Qt::PointingHandCursor);
    _fullGuideButton->setStyleSheet(QStringLiteral(
        "QPushButton#FullGuideButton {"
        "  background-color: palette(highlight);"
        "  color: palette(highlighted-text);"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 12px 24px;"
        "  font-weight: bold;"
        "  font-size: 14px;"
        "}"
        "QPushButton#FullGuideButton:hover {"
        "  background-color: palette(dark);"
        "}"
    ));
    connect(_fullGuideButton, &QPushButton::clicked, this, &TutorialWidget::onOpenFullGuideClicked);
    buttonLayout->addStretch();
    buttonLayout->addWidget(_fullGuideButton);
    buttonLayout->addStretch();
    _mainLayout->addLayout(buttonLayout);
}

void TutorialWidget::retranslateUi()
{
    _headerLabel->setText(tr("Learn FreeCAD"));
    _subheaderLabel->setText(tr("Interactive tutorials to help you master 3D modeling. "
                                "Start with the basics or jump to advanced techniques."));
    _fullGuideButton->setText(tr("📖 Open Complete User Guide"));
}

QWidget* TutorialWidget::createTutorialSection(const QString& sectionTitle, 
                                                const QVector<TutorialCard*>& cards)
{
    auto sectionWidget = new QWidget(this);
    auto layout = new QVBoxLayout(sectionWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    
    auto titleLabel = new QLabel(sectionTitle, sectionWidget);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

    auto cardsLayout = new FlowLayout();
    for (auto card : cards) {
        cardsLayout->addWidget(card);
    }
    
    auto cardsWidget = new QWidget(sectionWidget);
    cardsWidget->setLayout(cardsLayout);
    layout->addWidget(cardsWidget);

    return sectionWidget;
}

void TutorialWidget::onGettingStartedClicked()
{
    // Open the help browser with Getting Started section
    // For now, show a message about the tutorial
    Q_EMIT tutorialSelected(QStringLiteral("getting-started"));
    
    // Could also open the online documentation:
    QDesktopServices::openUrl(QUrl(QStringLiteral("https://wiki.freecad.org/Getting_started")));
}

void TutorialWidget::onSketchingBasicsClicked()
{
    Q_EMIT tutorialSelected(QStringLiteral("sketching-basics"));
    QDesktopServices::openUrl(QUrl(QStringLiteral("https://wiki.freecad.org/Sketcher_Workbench")));
}

void TutorialWidget::on3DModelingClicked()
{
    Q_EMIT tutorialSelected(QStringLiteral("3d-modeling"));
    QDesktopServices::openUrl(QUrl(QStringLiteral("https://wiki.freecad.org/PartDesign_Workbench")));
}

void TutorialWidget::onAdvancedTechniquesClicked()
{
    Q_EMIT tutorialSelected(QStringLiteral("advanced-techniques"));
    QDesktopServices::openUrl(QUrl(QStringLiteral("https://wiki.freecad.org/Feature_editing")));
}

void TutorialWidget::onExportingClicked()
{
    Q_EMIT tutorialSelected(QStringLiteral("exporting"));
    QDesktopServices::openUrl(QUrl(QStringLiteral("https://wiki.freecad.org/Import_Export")));
}

void TutorialWidget::onOpenFullGuideClicked()
{
    // Open the local user guide markdown file in the default application
    // or open the wiki
    QDesktopServices::openUrl(QUrl(QStringLiteral("https://wiki.freecad.org/User_hub")));
}
