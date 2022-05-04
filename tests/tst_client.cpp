#include "lsp/client.h"
#include "lsp/notifications.h"
#include "lsp/requests.h"

#include "common/test_utils.h"

#include <QFile>
#include <QTest>
#include <QTextStream>

class TestClient : public QObject
{
    Q_OBJECT

private slots:
    void initializeAndShutdown()
    {
        CHECK_CLANGD;

        Lsp::Client client("cpp", "clangd", {"--log=verbose", "--pretty"});
        auto logs = Test::LogSilencers {"cpp_client", "cpp_server", "cpp_messages"};

        QVERIFY(client.initialize(Test::testDataPath()));
        QCOMPARE(client.state(), Lsp::Client::Initialized);
        QVERIFY(client.shutdown());
        QCOMPARE(client.state(), Lsp::Client::Shutdown);
    }

    void openClose()
    {
        CHECK_CLANGD;

        Lsp::Client client("cpp", "clangd", {"--log=verbose", "--pretty"});
        auto logs = Test::LogSilencers {"cpp_client", "cpp_server", "cpp_messages"};

        client.initialize(Test::testDataPath() + "/cppdocument");

        Lsp::DidOpenTextDocumentParams openParams;
        openParams.textDocument.uri = Lsp::Client::toUri(Test::testDataPath() + "/cppdocument/test/hello.cpp");
        openParams.textDocument.version = 1;
        openParams.textDocument.text = "";
        openParams.textDocument.languageId = "cpp";
        client.didOpen(std::move(openParams));

        Lsp::DidCloseTextDocumentParams closeParams;
        closeParams.textDocument.uri = Lsp::Client::toUri(Test::testDataPath() + "/cppdocument/test/hello.cpp");
        client.didClose(std::move(closeParams));

        client.shutdown();
    }

    void documentSymbol()
    {
        CHECK_CLANGD;

        Lsp::Client client("cpp", "clangd", {"--log=verbose", "--pretty"});
        auto logs = Test::LogSilencers {"cpp_client", "cpp_server", "cpp_messages"};

        client.initialize(Test::testDataPath() + "/cpp-project");

        QFile file(Test::testDataPath() + "/cpp-project/myobject.cpp");
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            Lsp::DidOpenTextDocumentParams openParams;
            openParams.textDocument.uri = Lsp::Client::toUri(Test::testDataPath() + "/cpp-project/myobject.cpp");
            openParams.textDocument.version = 1;
            openParams.textDocument.text = stream.readAll().toStdString();
            openParams.textDocument.languageId = "cpp";
            client.didOpen(std::move(openParams));

            Lsp::DocumentSymbolParams params;
            params.textDocument.uri = Lsp::Client::toUri(Test::testDataPath() + "/cpp-project/myobject.cpp");
            auto result = client.documentSymbol(std::move(params));
            QVERIFY(std::holds_alternative<std::vector<Lsp::DocumentSymbol>>(result.value()));
            auto symbols = std::get<std::vector<Lsp::DocumentSymbol>>(result.value());
            QVERIFY(!symbols.empty());
            auto lastSymbol = symbols.back();
            QCOMPARE(lastSymbol.name, "MyObject::sayMessage");
            QCOMPARE(lastSymbol.range.start.line, 16);
            QCOMPARE(lastSymbol.range.end.line, 18);
        }

        client.shutdown();
    }
};

QTEST_MAIN(TestClient)
#include "tst_client.moc"
