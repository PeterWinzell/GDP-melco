#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <VSSSignalinterface/websocketbroker.h>
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>

using namespace testing;

TEST(VSSSignalinterfaceImpl, getVssNode_one_signal)
{
    WebSocketBroker* signalInterface = new WebSocketBroker(".", "vss_rel_1");
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
    WebSocketBroker* signalInterface = new WebSocketBroker(".", "vss_rel_1");
    QJsonObject obj = signalInterface->getVSSNode("");

    QJsonValue val1 = obj.value("Attribute");
    QJsonValue val2 = obj.value("Signal");
    QJsonValue val3 = obj.value("Private");

    ASSERT_THAT(false, val1.isUndefined());
    ASSERT_THAT(false, val2.isUndefined());
    ASSERT_THAT(false, val3.isUndefined());

    delete signalInterface;
}

TEST(VSSSignalinterfaceImpl, getVssNode_branch)
{
    WebSocketBroker* signalInterface = new WebSocketBroker(".", "vss_rel_1");
    QJsonObject obj = signalInterface->getVSSNode("Signal.Drivetrain");

    // check that we get any signal from Signal.Drivetrain

    QJsonValue val1 = obj.value("Signal");
    QJsonValue val2 = val1.toObject().value("children");
    QJsonValue val3 = val2.toObject().value("Drivetrain");
    QJsonValue val_Drivetrain_children = val3.toObject().value("children");
    QJsonValue val5 = val_Drivetrain_children.toObject().value("InternalCombustionEngine");
    QJsonValue val6 = val5.toObject().value("children");
    QJsonValue val7 = val6.toObject().value("RPM");

    QJsonValue val8 = val7.toObject().value("description");
    QJsonValue val9 = val7.toObject().value("type");
    QJsonValue val10 = val7.toObject().value("max");

    // check one more from Signal.Drivetrain

    QJsonValue val11 = val_Drivetrain_children.toObject().value("Transmission");
    QJsonValue val12 = val11.toObject().value("children");
    QJsonValue val13 = val12.toObject().value("PerformanceMode");

    QJsonValue val14 = val13.toObject().value("description");
    QJsonValue val15 = val13.toObject().value("type");
    QJsonValue val16 = val13.toObject().value("enum");


    ASSERT_THAT(false, val1.isUndefined());
    ASSERT_THAT(false, val2.isUndefined());
    ASSERT_THAT(false, val3.isUndefined());
    ASSERT_THAT(false, val_Drivetrain_children.isUndefined());
    ASSERT_THAT(false, val5.isUndefined());
    ASSERT_THAT(false, val6.isUndefined());
    ASSERT_THAT(false, val7.isUndefined());
    ASSERT_THAT(false, val8.isUndefined());
    ASSERT_THAT(false, val9.isUndefined());
    ASSERT_THAT(false, val10.isUndefined());

    ASSERT_THAT(false, val11.isUndefined());
    ASSERT_THAT(false, val12.isUndefined());
    ASSERT_THAT(false, val13.isUndefined());
    ASSERT_THAT(false, val14.isUndefined());
    ASSERT_THAT(false, val15.isUndefined());
    ASSERT_THAT(false, val16.isUndefined());

    delete signalInterface;
}
