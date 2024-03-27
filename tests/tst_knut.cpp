#include "core/knutcore.h"
#include "core/scriptmanager.h"

#include "common/test_utils.h"
#include "common/testutil.h"

#include <QDir>
#include <QFileInfo>
#include <QQmlEngine>

#include <QSignalSpy>
#include <QTest>

#define KNUT_TEST(name)                                                                                                \
    void tst_##name()                                                                                                  \
    {                                                                                                                  \
        Test::LogSilencer ls;                                                                                          \
        QFileInfo fi(Test::testDataPath() + "/tst_" #name ".qml");                                                     \
        QVERIFY(fi.exists());                                                                                          \
        QStringList arguments {"knut.exe", "-s", fi.absoluteFilePath()};                                               \
        QDir dir(Test::testDataPath() + "/tst_" #name);                                                                \
        if (dir.exists())                                                                                              \
            arguments.append({dir.absolutePath()});                                                                    \
        Core::KnutCore knut;                                                                                           \
        qmlRegisterSingletonType<TestUtil>("Script.Test", 1, 0, "TestUtil", [](QQmlEngine *, QJSEngine *) {            \
            return new TestUtil();                                                                                     \
        });                                                                                                            \
        QSignalSpy finished(Core::ScriptManager::instance(), &Core::ScriptManager::scriptFinished);                    \
        knut.process(arguments);                                                                                       \
        QVERIFY(finished.wait());                                                                                      \
        QCOMPARE(finished.takeFirst().at(0).toInt(), 0);                                                               \
    }

class TestKnut : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        Q_INIT_RESOURCE(core);
        Q_INIT_RESOURCE(rccore);
    }

    KNUT_TEST(mfc_convert_dialog)
    KNUT_TEST(mfc_convert_dataexchange)
    KNUT_TEST(settings)
    KNUT_TEST(dir)
    KNUT_TEST(fileinfo)
    KNUT_TEST(utils)
    KNUT_TEST(rcdocument)
    KNUT_TEST(project)
};

QTEST_MAIN(TestKnut)
#include "tst_knut.moc"
