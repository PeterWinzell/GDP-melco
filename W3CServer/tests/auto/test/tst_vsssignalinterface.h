#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <VSSSignalinterface/vsssignalinterfaceimpl.h>
#include <QDir>

using namespace testing;

TEST(VSSSignalinterfaceImpl, getVssNode_one_signal)
{
    QString dir = QDir::currentPath();
    QString file = dir + "/vss_rel_1.json";

    VSSSignalInterface* signalInterface = new VSSSignalInterfaceImpl(file);
    QJsonObject obj = signalInterface->getVSSNode("Signal.Drivetrain.InternalCombustionEngine.RPM");

    QJsonValue val1 = obj.value("Signal");
    QJsonValue val2 = val1.toObject().value("children");
    QJsonValue val3 = val2.toObject().value("Drivetrain");
    QJsonValue val4 = val3.toObject().value("children");
    QJsonValue val5 = val4.toObject().value("InternalCombustionEngine");
    QJsonValue val6 = val5.toObject().value("children");
    QJsonValue val7 = val6.toObject().value("RPM");

    QJsonValue val8 = val7.toObject().value("description");
    QJsonValue val9 = val7.toObject().value("type");
    QJsonValue val10 = val7.toObject().value("max");

    ASSERT_THAT(false, val1.isUndefined());
    ASSERT_THAT(false, val2.isUndefined());
    ASSERT_THAT(false, val3.isUndefined());
    ASSERT_THAT(false, val4.isUndefined());
    ASSERT_THAT(false, val5.isUndefined());
    ASSERT_THAT(false, val6.isUndefined());
    ASSERT_THAT(false, val7.isUndefined());
    ASSERT_THAT(false, val8.isUndefined());
    ASSERT_THAT(false, val9.isUndefined());
    ASSERT_THAT(false, val10.isUndefined());

    delete signalInterface;
}

TEST(VSSSignalinterfaceImpl, getVssNode_full_tree)
{
    QString dir = QDir::currentPath();
    QString file = dir + "/vss_rel_1.json";

    VSSSignalInterface* signalInterface = new VSSSignalInterfaceImpl(file);
    QJsonObject obj = signalInterface->getVSSNode("");

    QJsonValue val1 = obj.value("Attribute");
    QJsonValue val2 = obj.value("Signal");
    QJsonValue val3 = obj.value("Private");

    ASSERT_THAT(false, val1.isUndefined());
    ASSERT_THAT(false, val2.isUndefined());
    ASSERT_THAT(false, val3.isUndefined());

    delete signalInterface;
}
