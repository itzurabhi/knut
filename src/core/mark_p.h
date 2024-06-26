/*
  This file is part of Knut.

  SPDX-FileCopyrightText: 2024 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#pragma once

#include <QObject>
#include <QPointer>

namespace Core {

class TextDocument;

class MarkPrivate : public QObject
{
    Q_OBJECT

public:
    // Unfortunately this needs to be public, as otherwise std::make_shared can't access it
    explicit MarkPrivate(TextDocument *editor, int pos);

private:
    bool isValid() const;

    int line() const;
    int column() const;

    bool checkEditor() const;

    void update(int from, int charsRemoved, int charsAdded);

    QPointer<TextDocument> m_editor;
    int m_pos = -1;
    friend class Mark;
};

} // namespace Core
