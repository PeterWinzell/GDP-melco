#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vsssignalinterfaceimpl.h"

extern "C" {
#include "vsi.h"
}

#include <QJsonObject>
#include <QString>
#include <QMutexLocker>
#include <QJsonDocument>
#include <QPointer>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QJsonArray>



//
//  A few global data items to make life easier...
//
static vsi_handle handle;

static unsigned char data;

extern void btree_print ( btree_t* btree, printFunc printCB );


//
//  This function will be called by the btree code when it has been called to
//  print out the btree nodes.  Each userData entry that is encountered in
//  that process will result in a call to this function to render the data the
//  way the user wants it to be displayed.
//
void printFunction ( char* leader, void* data )
{
    if ( data == NULL )
    {
        printf ( "%s(nil)\n", leader );
        return;
    }
    vsi_id_name_definition* userDataPtr = (vsi_id_name_definition*)data;

    printf ( "%sUserData: domainId: %lu, signalId: %lu, privateId: %lu, name[%s]\n",
             leader, userDataPtr->domainId, userDataPtr->signalId,
             userDataPtr->privateId, userDataPtr->name );
}

VSSSignalInterfaceImpl::VSSSignalInterfaceImpl(const QString& vssFile)
{
    m_rpm = "3000";
    m_speed = "70";

    loadJson(vssFile);

    long     dummyData = 0;


    //
    //  Call into the API to initialize the memory.
    //
    handle = vsi_initialize ( true );
    if (!handle)
    {
        printf("Failed to allocate memory for VSI!\n");
        //return -1;
    }
    vsi_context* context = (vsi_context*)handle;

    printf("Initialized the VSI API.\n");

    //
    //  Initialize the result buffer for data to go into.
    //
    //  Note that we are only going to store a single byte into the shared
    //  memory records.
    //
    m_result.name       = "(empty)";
    m_result.data       = (char*)&dummyData;
    m_result.dataLength = sizeof(unsigned char);

    //
    //  Define some signals for the test functions.
    //
    vsi_define_signal_name ( (void*)context, 0, 1, 0, "foo" );
    vsi_define_signal_name ( (void*)context, 0, 2, 0, "bar" );
    vsi_define_signal_name ( (void*)context, 0, 3, 0, "baz" );
    vsi_define_signal_name ( (void*)context, 0, 4, 0, "gen" );
    vsi_define_signal_name ( (void*)context, 0, 5, 0, "ivi" );

    btree_print ( &context->signalNameIndex, printFunction );
}

VSSSignalInterfaceImpl::~VSSSignalInterfaceImpl()
{
    //
    //  Finally, we close the VSI system because we are finished with it.
    //
    //  Note that we didn't bother to free all of the memory we dynamically
    //  allocated during this test but for neatness, it should normally be
    //  freed.
    //
    int status = 0;
    printf ( "\n(11) Closing the VSI system.\n" );
    status = vsi_destroy ( handle );
    if ( status )
    {
        printf("Failed to free memory used by VSI!\n");
        //return -1;
    }
    printf("Freed the VSI memory.\n");
}

void VSSSignalInterfaceImpl::updateValue(CarSignalType type, QString value)
{
    qDebug()  << "VSSSignalInterfaceImpl::updateValue: " << "\n\ttype: " << type << "\n\tvalue: " << value;
    QMutex mutex;
    QMutexLocker locker(&mutex);

    //
    //  Store signal in the core data store
    //

    switch (type)
    {
        case CarSignalType::RPM:
            m_rpm = value;
            break;
        case CarSignalType::Speed:
            m_speed = value;
            break;

        case CarSignalType::GasPedal:
        case CarSignalType::BrakePedal:
        case CarSignalType::SteerAngle:
        case CarSignalType::Headlights:
        case CarSignalType::EngineRunning:
        case CarSignalType::CurrentFuelConsumption:
        case CarSignalType::FuelTankMax:
        case CarSignalType::FuelTankActual:
        case CarSignalType::PositionLatitude:
        case CarSignalType::PositionLongitude:
        case CarSignalType::PositionAltitude:
        case CarSignalType::Orientation:
        case CarSignalType::Rise:
        case CarSignalType::AccelerationLateral:
        case CarSignalType::Rotation:
        case CarSignalType::AccelerationRotation:
        case CarSignalType::Acceleration:

        default:
            break;
    }

    qDebug()  << "Storing domain :" << m_domainId << " signal: " << m_signalId << " data: " << value;

    m_result.domainId = m_domainId;
    m_result.signalId = m_signalId;

    //m_result.data[0] = 0; //value; //TBD variable size of data!
    //m_result.dataLength = 1; //TBD variable size of data!

    QByteArray ba = value.toLatin1();
    m_result.data = ba.data();
    m_result.dataLength = ba.length();

    int status = 0;
    status = vsi_fire_signal( handle, &m_result );

    if ( 0 == status )
    {
        qDebug()  << "Successfully stored!";
    }
    else
    {
        qDebug()  << "Failed to store! Error code: " << status;
    }

 #if 0
    //
    //  Fire a signal by name.
    //
    #define testSignalName "Attribute.Body.BodyType"

    printf ( "\nStoring domain %d, signal [%s], data [%s] in the core data store.\n",
             0, testSignalName, "name" );

    result.domainId = 0;
    result.name = testSignalName;
    result.data = "name";
    result.dataLength = strlen(result.data);

    status = vsi_fire_signal_by_name ( handle, &result );

    result.status = status;
    if ( status != 0 )
    {
        printf ( "Failed to store %u, [%s]! Error code %d.\n", 0,
                 testSignalName, status );
        //return status;
    }
    result.data = (char*)&dummyData;

    printf ( "Successfully stored %u, [%s] in the core data store.\n",
             0, testSignalName );

#endif
}

void VSSSignalInterfaceImpl::loadJson(const QString &fileName)
{
    QFile jsonFile(fileName);
    QJsonDocument doc;

    if (jsonFile.exists())
    {
        jsonFile.open(QFile::ReadOnly);
        doc = QJsonDocument().fromJson(jsonFile.readAll());
    }
    else
    {
        qWarning() << "Vss file " << fileName << " not found!";
    }

    m_vssTree = doc.object();
}

QString VSSSignalInterfaceImpl::getSignalValue(const QString& path)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    QString result = "not implemented";

    if(path == "Signal.Drivetrain.InternalCombustionEngine.RPM")
    {
        result = m_rpm;
    }
    else if (path == "Signal.Drivetrain.Transmission.Speed")
    {
        result = m_speed;
    }

    return result;
}

qint8 VSSSignalInterfaceImpl::setSignalValue(const QString& path, QVariant value)
{
    QMutex mutex;
    QMutexLocker locker(&mutex);

    qint8 result = 0;

    qDebug() << "setSignalValue: path = " << path;

    if(path == "Signal.Drivetrain.InternalCombustionEngine.RPM")
    {
        m_rpm = value.toString();

        qDebug() << "m_rpm = " << m_rpm;
    }
    else if (path == "Signal.Drivetrain.Transmission.Speed")
    {
        m_speed = value.toString();

        qDebug() << "m_speed = " << m_speed;
    }

    return result;
}

QJsonObject VSSSignalInterfaceImpl::getVSSNode(const QString& path)
{

    // clear old
    m_vssTreeNode = QJsonObject();

    if (m_vssTree.empty())
    {
        qDebug() << "Empty vss tree! Returning empty node.";
        return m_vssTreeNode;
    }
    else if (path == "")
    {
        qDebug() << "Empty path, returning full tree.";
        return m_vssTree;
    }
    else
    {
        QStringList tokens = path.split('.');

        // Make a copy of the full tree
        QJsonObject tree = m_vssTree;

        QVector<JsonNode> nodes;
        getTreeNodes(tree, tokens, nodes);

        QJsonObject obj;
        createJsonVssTree(nodes, obj);

        return m_vssTreeNode;
    }
}

void VSSSignalInterfaceImpl::getTreeNodes(QJsonObject& tree, QStringList& path, QVector<JsonNode>& nodes)
{
    if (path.length() > 0)
    {
        QString key = path[0];
        JsonNode node;
        node.isBranch = path.length() > 1;
        node.key = key;

        if (node.isBranch)
        {
            removeAllKeysButOne(tree, key);
        }

        tree = tree.value(key).toObject();

        QJsonObject treeCopy = tree;

        if (node.isBranch)
        {
            removeOne(treeCopy, "children");
            tree = tree.value("children").toObject();
        }

        node.json = treeCopy;

        nodes.push_front(node);

        path.removeFirst();
        getTreeNodes(tree, path, nodes);
    }
}

void VSSSignalInterfaceImpl::createJsonVssTree(QVector<JsonNode>& nodes, QJsonObject& json)
{

    if (nodes.size() > 0)
    {
        JsonNode node = nodes.first();
        QJsonObject obj;

        if (node.isBranch)
        {
            node.json.insert("children", json);
            obj.insert(node.key, node.json);
        }
        else
        {
            obj.insert(node.key, node.json);
        }

        nodes.removeFirst();
        createJsonVssTree(nodes, obj);
    }
    else
    {
        m_vssTreeNode = json;
    }
}

void VSSSignalInterfaceImpl::removeAllKeysButOne(QJsonObject& json, const QString& filter)
{
    foreach (QString key, json.keys())
    {
        if (filter != key)
        {
            json.remove(key);
        }
    }
}

void VSSSignalInterfaceImpl::removeOne(QJsonObject& json, const QString& filter)
{
    foreach (QString key, json.keys())
    {
        if (filter == key)
        {
            json.remove(key);
        }
    }
}

QJsonObject VSSSignalInterfaceImpl::getVSSTree(const QString& path)
{
    QString p = path;
    QJsonObject tree;
    return tree;
}
