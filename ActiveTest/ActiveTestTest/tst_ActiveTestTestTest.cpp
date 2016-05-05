#include <QString>
#include <QtTest>

#include "../SMSObjectManager.h"

class ActiveTestTestTest : public QObject
{
  Q_OBJECT

public:
  ActiveTestTestTest();

private Q_SLOTS:
  void testCase1();
  void initSms();
};

ActiveTestTestTest::ActiveTestTestTest()
{
}

void ActiveTestTestTest::testCase1()
{
  QVERIFY2(true, "Failure");
}

void ActiveTestTestTest::initSms()
{
  SMSObjectManager* smsObjMgr = new SMSObjectManager(this);
  bool inited = smsObjMgr->init();
  QCOMPARE(inited, true);
}

QTEST_APPLESS_MAIN(ActiveTestTestTest)

#include "tst_ActiveTestTestTest.moc"
