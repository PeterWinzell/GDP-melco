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

//
//  A handy utility function to add signals to the core data store.
//
static int storeSignal ( vsi_result* result, domain_t domain_id,
                         signal_t signal_id, unsigned char dataValue )
{
    int status = 0;
    printf ( "Storing domain %lu, signal %lu, data %u in the core data store.\n",
             domain_id, signal_id, dataValue );

    result->domainId = domain_id;
    result->signalId = signal_id;
    result->data[0] = dataValue;
    result->dataLength = 1;

    status = vsi_fire_signal ( handle, result );

    result->status = status;

    if ( status != 0 )
    {
        printf ( "Failed to store %lu, %lu! Error code %d.\n", domain_id,
                 signal_id, status );
        return status;
    }

    printf ( "Successfully stored %lu, %lu in the core data store.\n",
             domain_id, signal_id );
    return 0;
}

VSSSignalInterfaceImpl::VSSSignalInterfaceImpl(const QString& vssFile)
{
    m_rpm = "3000";
    m_speed = "70";

    loadJson(vssFile);

    domain_t domain_id;
    signal_t signal_id;
    int      status;
    long     dummyData = 0;
    int i;
    vsi_result  result;
    vsi_result* results = (vsi_result*)malloc ( sizeof(vsi_result) * 10 );




    m_result.data = (char*)malloc ( 1 );
    m_result.dataLength = 1;
    m_result.status = -61;



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
    result.name       = "(empty)";
    result.data       = (char*)&dummyData;
    result.dataLength = sizeof(unsigned char);

    //
    //  Define some signals for the test functions.
    //
    vsi_define_signal_name ( (void*)context, 0, 1, 0, "foo" );
    vsi_define_signal_name ( (void*)context, 0, 2, 0, "bar" );
    vsi_define_signal_name ( (void*)context, 0, 3, 0, "baz" );
    vsi_define_signal_name ( (void*)context, 0, 4, 0, "gen" );
    vsi_define_signal_name ( (void*)context, 0, 5, 0, "ivi" );

    btree_print ( &context->signalNameIndex, printFunction );

    //
    //  Fire a signal into the void to show how generating a signal works.
    //
    printf("\n(1) Firing signal \"bar\".\n");
    status = vsi_name_string_to_id(handle, "bar", &domain_id, &signal_id);
    if (status)
    {
        printf("Failed to find the signal ID for the signal \"bar\"! Error "
               "code %d.\n", status);
        //return status;
    }
    storeSignal ( &result, domain_id, signal_id, 41 );
    printf("Successfully fired signal \"bar\".\n");

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
    //
    //  Store another "bar" signal in the core data store with a larger data
    //  field.  We'll use this and the previous one to test the "get newest"
    //  and "get oldest" signal functions.  The oldest one has a data value of
    //  41 and the newest one has a value of 43.
    //
    storeSignal ( &result, domain_id, signal_id, 42 );
    storeSignal ( &result, domain_id, signal_id, 43 );

    //
    //  Read the latest update from the "bar" signal.
    //
    printf ( "\n(2) Getting newest \"bar\" signal.\n" );
    result.name = "bar";
    status = vsi_get_newest_signal_by_name(handle, &result);
    if (status)
    {
        printf("Failed to get the newest signal data for \"bar\"! Error "
               "code %d.\n", status);
        //return status;
    }
    printf("Successfully read the newest \"bar\" signal and got %d.\n",
           *(int*)result.data);
    //
    //  Read the oldest update from the "bar" signal.
    //
    printf ( "\n(3) Getting oldest \"bar\" signal.\n" );
    status = vsi_get_oldest_signal_by_name(handle, &result);
    if (status)
    {
        printf("Failed to get the oldest signal data for \"bar\"! Error "
               "code %d.\n", status);
        //return status;
    }
    printf("Successfully read the oldest \"bar\" signal and got %d.\n",
           *(int*)result.data);
    //
    //  Read all of the "bar" signals from oldest to newest...
    //
    printf ( "\n(4) Reading the \"bar\" signals, oldest first.\n" );
    do
    {
        result.name = "bar";
        status = vsi_get_oldest_signal_by_name(handle, &result);
        if ( status != 0 )
        {
            if ( status != -ENODATA )
            {
                printf("Failed to get the oldest signal data for \"bar\"! "
                       "Error code %d.\n", status);
                //return status;
            }
        }
        else
        {
            printf("Successfully read the oldest \"bar\" signal and got %d.\n",
                   *(int*)result.data);
        }
    } while (false); //(status != -ENODATA);
    printf("Completed reading the signal.\n");

    //
    //  Go through the flow of creating a group and then listening for that
    //  group to fire.
    //
    printf ( "\n(5) Creating signal group 10 (should pass).\n" );
    status = vsi_create_signal_group(handle, 10);
    if (status)
    {
        printf("Failed to create signal group 10! Error code %d.\n", status);
        //return status;
    }
    dumpGroups ( handle );

    printf("\nCreating signal group 10 again (should fail).\n");
    status = vsi_create_signal_group(handle, 10);
    if (status)
    {
        printf("Failed to create signal group 10! Error code %d.\n", status);
    }
    dumpGroups ( handle );

    printf("\nDeleting signal group 11 (should fail).\n");
    status = vsi_delete_signal_group (handle, 11);
    if (status)
    {
        printf("Failed to delete signal group 11! Error code %d.\n", status);
    }
    dumpGroups ( handle );

    printf("\nDeleting signal group 10 (should pass).\n");
    status = vsi_delete_signal_group (handle, 10);
    if (status)
    {
        printf("Failed to delete signal group 10! Error code %d.\n", status);
    }
    dumpGroups ( handle );

    printf("\nCreated signal group 10 again (should pass).\n");
    status = vsi_create_signal_group(handle, 10);
    if (status)
    {
        printf("Failed to create signal group 10! Error code %d.\n", status);
        //return status;
    }
    dumpGroups ( handle );

    printf ( "\n(6) Adding \"gen\" to the signal group.\n" );
    status = vsi_add_signal_to_group_by_name(handle, "gen", 10);
    if (status)
    {
        printf("Failed to add signal \"gen\" to group 10! Error code %d.\n",
               status);
        //return status;
    }
    dumpGroups ( handle );

    printf ( "\n(7) Adding \"ivi\" to the signal group.\n" );
    status = vsi_add_signal_to_group_by_name(handle, "ivi", 10);
    if (status)
    {
        printf("Failed to add signal \"ivi\" to group 10! Error code %d.\n",
               status);
        //return status;
    }
    printf("Added signals \"gen\" and \"ivi\" to group 10.\n");

    printf ( "\n(7) Adding \"Attribute.Body.BodyType\" to the signal group.\n" );
    status = vsi_add_signal_to_group_by_name(handle, "Attribute.Body.BodyType", 10);
    if (status)
    {
        printf("Failed to add signal \"ivi\" to group 10! Error code %d.\n",
               status);
        //return status;
    }
    printf("Added signals \"gen\", \"ivi\", and \"Attribute.Body.BodyType\" to group 10.\n");

    dumpGroups ( handle );

    //
    //  Initialize the memory for the array of vsi_result structures.
    //
    storeSignal ( &result, 0, 4, 48 );
    storeSignal ( &result, 0, 4, 49 );
    storeSignal ( &result, 0, 5, 50 );
    storeSignal ( &result, 0, 5, 51 );
    memset ( results, 0, sizeof(result) * 10 );
    for (i = 0; i < 10; ++i )
    {
        results[i].data = (char*)malloc ( 1 );
        results[i].dataLength = 1;
        results[i].status = -61;
    }

#if 0
    //
    //  As a pit stop in this example, read the current value of the group to
    //  showcase the functionality.
    //
    printf ( "\n(8) Get the newest items in the group.\n" );
    status = vsi_get_newest_in_group ( handle, 10, results );
    if ( status )
    {
        printf ( "Failed to get the newest group data for group 10! Error code %d.\n",
                 status );
        //return status;
    }
    for (i = 0; i < 10; ++i )
    {
        if ( results[i].status == 0 )
        {
            printf ( "    Newest data for group 10[%d]: domain[%lu], signal[%lu],"
                     " data[%u]\n", i, results[i].domainId, results[i].signalId,
                     results[i].data[0] );
        }
    }
    printf("Successfully read all signals in group 10.\n");
#endif

    //
    //  Now go fetch the oldest signals in our group.
    //
    printf ( "\n(9) Get oldest signals in the group.\n" );

    //
    //  Clear out and reinitialize out results array.
    //
    memset ( results, 0, sizeof(result) * 10 );
    for (i = 0; i < 10; ++i )
    {
        results[i].data = (char*)malloc ( 1 );
        results[i].dataLength = 1;
        results[i].status = -61;
    }

    status = vsi_get_oldest_in_group ( handle, 10, results );
    if (status)
    {
        printf("Failed to get the oldest group data for group 10! Error code %d.\n",
               status);
        //return status;
    }
    for (i = 0; i < 10; ++i )
    {
        if ( results[i].status == 0 )
        {
            printf ( "    Oldest data for group 10[%d]: domain[%lu], signal[%lu],"
                     " data[%u]\n", i, results[i].domainId, results[i].signalId,
                     results[i].data[0] );
        }
    }
    printf ( "Completed reading group 10.\n" );

    //
    //  Clean up the group, since we no longer need it.
    //
    printf ( "\n(10) Cleaning up the group structures.\n" );
    status = vsi_delete_signal_group(handle, 10);
    if (status)
    {
        printf("Failed to delete signal group 10! Error code %d.\n", status);
        //return status;
    }
    free ( results );
    printf("Deleted group 10.\n");

    //
    //  Finally, we close the VSI system because we are finished with it.
    //
    //  Note that we didn't bother to free all of the memory we dynamically
    //  allocated during this test but for neatness, it should normally be
    //  freed.
    //
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
    m_result.data[0] = 0; //value; //TBD variable size of data!
    m_result.dataLength = 1; //TBD variable size of data!

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
