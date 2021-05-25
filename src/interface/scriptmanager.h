#pragma once

#include <QObject>
#include <QStringList>

#include <spdlog/spdlog.h>

#include <functional>
#include <vector>

class QAction;
class QFileSystemWatcher;

namespace Interface {

class ScriptRunner;

/**
 * \brief Manager for scripts.
 *
 * Manage all scripts available to the user, and run them.
 *
 * Scripts directory are watched using a QFileSystemWatcher, to update
 * the list of script in case one is added or deleted.
 */
class ScriptManager : public QObject
{
    Q_OBJECT

public:
    struct Script
    {
        QString name;
        QString fileName;
    };
    using ScriptList = std::vector<Script>;

    explicit ScriptManager(QObject *parent = nullptr);
    ~ScriptManager() override;

    static ScriptManager *instance();

    ScriptList scriptList() const;

    void addDirectory(const QString &path);
    void removeDirectory(const QString &path);

public slots:
    void runScript(const QString &scriptName, bool async = true, bool log = true);

signals:
    void scriptFinished(const QVariant &result);

private:
    void addScript(const QString &fileName);
    void addScriptsFromPath(const QString &path);
    void removeScriptsFromPath(const QString &path);

    void doRunScript(const QString &fileName, std::function<void()> endFunc = {});

    void updateScriptDirectory(const QString &path);

private:
    inline static ScriptManager *m_instance = nullptr;

    std::shared_ptr<spdlog::logger> m_logger;

    QFileSystemWatcher *const m_watcher;
    ScriptRunner *const m_runner;

    ScriptList m_scriptList;
    QStringList m_directories;
};

}