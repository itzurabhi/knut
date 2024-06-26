/*
  This file is part of Knut.

  SPDX-FileCopyrightText: 2024 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include "rctoqrcdialog.h"
#include "core/rcdocument.h"
#include "core/settings.h"
#include "ui_rctoqrcdialog.h"

#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

namespace Gui {

RcToQrcDialog::RcToQrcDialog(Core::RcDocument *document, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RcToQrcDialog)
    , m_document(document)
{
    Q_ASSERT(m_document);
    ui->setupUi(this);
    setWindowTitle(QApplication::applicationName() + ' ' + QApplication::applicationVersion() + " - " + windowTitle());

    auto m_rcFile = &(m_document->file());
    const auto languageList = m_rcFile->data.keys();

    ui->language->clear();
    ui->language->addItems(languageList);
    const int index = ui->language->findText(m_document->language());
    ui->language->setCurrentIndex(index);

    auto assetFlags = DEFAULT_VALUE(Core::RcDocument::ConversionFlags, RcAssetFlags);
    ui->removeUnknown->setChecked(assetFlags & Core::RcDocument::RemoveUnknown);
    ui->convertPng->setChecked(assetFlags & Core::RcDocument::ConvertToPng);
    ui->splitToolbars->setChecked(assetFlags & Core::RcDocument::SplitToolBar);

    auto colorFlags = DEFAULT_VALUE(Core::RcDocument::ConversionFlags, RcAssetColors);
    ui->gray->setChecked(colorFlags & Core::RcDocument::Gray);
    ui->magenta->setChecked(colorFlags & Core::RcDocument::Magenta);
    ui->bottomLeft->setChecked(colorFlags & Core::RcDocument::BottomLeftPixel);

    QString qrcFileName = m_document->fileName();
    QFileInfo fi(qrcFileName);
    qrcFileName.chop(fi.suffix().length());
    qrcFileName += "qrc";
    ui->fileSelector->setFileName(qrcFileName);

    connect(ui->splitToolbars, &QCheckBox::toggled, this, &RcToQrcDialog::updateColorBox);
    connect(ui->convertPng, &QCheckBox::toggled, this, &RcToQrcDialog::updateColorBox);
    connect(ui->language, &QComboBox::currentIndexChanged, this, &RcToQrcDialog::languageChanged);
}

RcToQrcDialog::~RcToQrcDialog() = default;

void RcToQrcDialog::accept()
{
    Core::RcDocument::ConversionFlags flags = Core::RcDocument::NoFlags;
    if (ui->removeUnknown->isChecked())
        flags |= Core::RcDocument::RemoveUnknown;
    if (ui->convertPng->isChecked())
        flags |= Core::RcDocument::ConvertToPng;
    if (ui->splitToolbars->isChecked())
        flags |= Core::RcDocument::SplitToolBar;

    const QString qrcFileName = ui->fileSelector->fileName();
    m_document->convertAssets(flags);
    if (!m_document->writeAssetsToQrc(qrcFileName)) {
        QMessageBox::warning(this, tr("Error"), tr("Unable to write qrc file to file %1.").arg(qrcFileName));
        return;
    }

    if (flags & (Core::RcDocument::ConvertToPng | Core::RcDocument::SplitToolBar)) {
        Core::RcDocument::ConversionFlags colors = Core::RcDocument::NoColors;
        if (ui->gray->isChecked())
            colors |= Core::RcDocument::Gray;
        if (ui->magenta->isChecked())
            colors |= Core::RcDocument::Magenta;
        if (ui->bottomLeft->isChecked())
            colors |= Core::RcDocument::BottomLeftPixel;
        m_document->writeAssetsToImage(colors);
    }

    QDialog::accept();
}

void RcToQrcDialog::updateColorBox()
{
    ui->colorBox->setEnabled(ui->convertPng->isChecked() || ui->splitToolbars->isChecked());
}

void RcToQrcDialog::languageChanged()
{
    const QString language = ui->language->currentText();
    m_document->setLanguage(language);
}

} // namespace Gui
