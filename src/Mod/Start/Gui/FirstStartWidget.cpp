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


#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QWidget>
#include <QFrame>
#include <QDesktopServices>
#include <QUrl>


#include "FirstStartWidget.h"
#include "ThemeSelectorWidget.h"
#include "GeneralSettingsWidget.h"

#include <App/Application.h>
#include <gsl/pointers>

using namespace StartGui;

FirstStartWidget::FirstStartWidget(QWidget* parent)
    : QGroupBox(parent)
    , _themeSelectorWidget {nullptr}
    , _generalSettingsWidget {nullptr}
    , _welcomeLabel {nullptr}
    , _descriptionLabel {nullptr}
    , _doneButton {nullptr}
    , _tutorialsButton {nullptr}
    , _docsButton {nullptr}
    , _quickTipsFrame {nullptr}
    , _quickTipsLabel {nullptr}
{
    setObjectName(QLatin1String("FirstStartWidget"));
    setupUi();
    qApp->installEventFilter(this);
}

void FirstStartWidget::setupUi()
{
    auto outerLayout = gsl::owner<QVBoxLayout*>(new QVBoxLayout(this));
    outerLayout->setAlignment(Qt::AlignCenter);
    outerLayout->setSpacing(16);
    QString application = QString::fromUtf8(App::Application::Config()["ExeName"].c_str());
    _welcomeLabel = gsl::owner<QLabel*>(new QLabel);
    _welcomeLabel->setAlignment(Qt::AlignCenter);
    outerLayout->addWidget(_welcomeLabel);
    _descriptionLabel = gsl::owner<QLabel*>(new QLabel);
    _descriptionLabel->setAlignment(Qt::AlignCenter);
    outerLayout->addWidget(_descriptionLabel);

    // Quick Tips section
    _quickTipsFrame = gsl::owner<QFrame*>(new QFrame(this));
    _quickTipsFrame->setObjectName(QStringLiteral("QuickTipsFrame"));
    _quickTipsFrame->setStyleSheet(QStringLiteral(
        "QFrame#QuickTipsFrame {"
        "  background-color: palette(alternateBase);"
        "  border: 1px solid palette(mid);"
        "  border-radius: 8px;"
        "  padding: 16px;"
        "}"
    ));
    auto tipsLayout = gsl::owner<QVBoxLayout*>(new QVBoxLayout(_quickTipsFrame));
    
    _quickTipsLabel = gsl::owner<QLabel*>(new QLabel(_quickTipsFrame));
    _quickTipsLabel->setWordWrap(true);
    _quickTipsLabel->setTextFormat(Qt::RichText);
    tipsLayout->addWidget(_quickTipsLabel);
    
    outerLayout->addWidget(_quickTipsFrame);

    _themeSelectorWidget = gsl::owner<ThemeSelectorWidget*>(new ThemeSelectorWidget(this));
    _generalSettingsWidget = gsl::owner<GeneralSettingsWidget*>(new GeneralSettingsWidget(this));

    outerLayout->addWidget(_generalSettingsWidget);
    outerLayout->addWidget(_themeSelectorWidget);

    // Learning resources buttons
    auto resourcesLayout = gsl::owner<QHBoxLayout*>(new QHBoxLayout);
    resourcesLayout->setSpacing(12);
    
    _tutorialsButton = gsl::owner<QPushButton*>(new QPushButton);
    _tutorialsButton->setObjectName(QStringLiteral("TutorialsButton"));
    _tutorialsButton->setMinimumHeight(40);
    _tutorialsButton->setCursor(Qt::PointingHandCursor);
    _tutorialsButton->setStyleSheet(QStringLiteral(
        "QPushButton#TutorialsButton {"
        "  background-color: palette(highlight);"
        "  color: palette(highlighted-text);"
        "  border: none;"
        "  border-radius: 6px;"
        "  padding: 8px 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton#TutorialsButton:hover {"
        "  background-color: palette(dark);"
        "}"
    ));
    connect(_tutorialsButton, &QPushButton::clicked, this, &FirstStartWidget::onOpenTutorialsClicked);
    
    _docsButton = gsl::owner<QPushButton*>(new QPushButton);
    _docsButton->setObjectName(QStringLiteral("DocsButton"));
    _docsButton->setMinimumHeight(40);
    _docsButton->setCursor(Qt::PointingHandCursor);
    connect(_docsButton, &QPushButton::clicked, this, &FirstStartWidget::onOpenDocumentationClicked);
    
    resourcesLayout->addStretch();
    resourcesLayout->addWidget(_tutorialsButton);
    resourcesLayout->addWidget(_docsButton);
    resourcesLayout->addStretch();
    outerLayout->addLayout(resourcesLayout);

    _doneButton = gsl::owner<QPushButton*>(new QPushButton);
    _doneButton->setObjectName(QStringLiteral("DoneButton"));
    _doneButton->setMinimumHeight(44);
    _doneButton->setMinimumWidth(120);
    connect(_doneButton, &QPushButton::clicked, this, &FirstStartWidget::dismissed);
    auto buttonBar = gsl::owner<QHBoxLayout*>(new QHBoxLayout);
    buttonBar->setAlignment(Qt::AlignRight);
    buttonBar->addWidget(_doneButton);
    outerLayout->addLayout(buttonBar);

    retranslateUi();
}

QWidget* FirstStartWidget::createQuickTipsWidget()
{
    return _quickTipsFrame;
}

void FirstStartWidget::onOpenTutorialsClicked()
{
    QDesktopServices::openUrl(QUrl(QStringLiteral("https://wiki.freecad.org/Getting_started")));
}

void FirstStartWidget::onOpenDocumentationClicked()
{
    QDesktopServices::openUrl(QUrl(QStringLiteral("https://wiki.freecad.org/User_hub")));
}

bool FirstStartWidget::eventFilter(QObject* object, QEvent* event)
{
    if (object == this && event->type() == QEvent::LanguageChange) {
        this->retranslateUi();
    }
    return QWidget::eventFilter(object, event);
}

void FirstStartWidget::retranslateUi()
{
    _doneButton->setText(tr("Get Started"));
    _tutorialsButton->setText(tr("View Tutorials"));
    _docsButton->setText(tr("Read Documentation"));
    
    QString application = QString::fromUtf8(App::Application::Config()["ExeName"].c_str());
    _welcomeLabel->setText(
        QLatin1String("<h1>") + tr("Welcome to %1").arg(application) + QLatin1String("</h1>")
    );
    _descriptionLabel->setText(
        tr("Your powerful open-source 3D CAD modeling tool.") + QLatin1String(" ")
        + tr("Let's set up your preferences to get started.")
    );
    
    // Quick tips content with formatted HTML
    QString tipsContent = QStringLiteral(
        "<div style='font-size: 13px;'>"
        "<h3 style='margin-bottom: 8px; color: palette(highlight);'>%1</h3>"
        "<ul style='margin: 0; padding-left: 20px;'>"
        "<li><b>%2</b> - %3</li>"
        "<li><b>%4</b> - %5</li>"
        "<li><b>%6</b> - %7</li>"
        "<li><b>%8</b> - %9</li>"
        "</ul>"
        "</div>"
    ).arg(
        tr("Quick Start Tips"),
        tr("Part Design Workbench"),
        tr("Create 3D parametric models from 2D sketches"),
        tr("Sketcher"),
        tr("Draw precise 2D shapes with constraints"),
        tr("Ctrl+Z"),
        tr("Undo any action"),
        tr("Middle Mouse"),
        tr("Rotate the 3D view")
    );
    
    _quickTipsLabel->setText(tipsContent);
}
