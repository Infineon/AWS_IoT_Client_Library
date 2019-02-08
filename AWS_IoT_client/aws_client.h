/*
 * $ Copyright Cypress Semiconductor $
 */

#ifndef AWSCLIENT_H
#define AWSCLIENT_H

#include "aws_common.h"
#include "NetworkInterface.h"
#include "MQTTClient.h"
#include "MQTTNetwork.h"
#include "MQTTmbed.h"

using namespace MQTT;

/*****************************************************************************/
/**
 *
 * @defgroup wiced_aws          Wiced Amazon Web Service Library
 * @ingroup  ipcoms
 *
 * Communication functions for AWS IoT & AWS Greengrass services
 *
 * AWS IoT provides secure, bi-directional communication between Internet-connected devices such as sensors,
 * actuators, embedded micro-controllers, or smart appliances and the AWS Cloud.
 *
 * AWS Greengrass is software that extends AWS cloud capabilities to local devices(typically Edge or Gateway devices),
 * making it possible for them to collect and analyze data closer to the source of information (Nodes, Wiced Devices, Amazon FreeRTOS devices).
 * With AWS Greengrass, devices securely communicate on a local network and exchange messages with each other
 * without having to connect to the cloud. AWS Greengrass provides a local pub/sub message manager that can
 * intelligently buffer messages if connectivity is lost so that inbound and outbound messages to the cloud are preserved.
 *
 * Wiced AWS library provides application developers an easy-to-use, unified interface for quickly enabling
 * AWS communication in their applications. The library provides a single interface to communicate with AWS
 * using different protocols. Currently, only MQTT & HTTP ( using Client Certificates )
 * are supported. See <https://docs.aws.amazon.com/iot/latest/developerguide/protocols.html> for more details.
 *
 *  @{
 */
/*****************************************************************************/

/*******************************************************
 *                 Type Definitions
 ******************************************************/

typedef struct MQTT::MessageData  aws_iot_message_t;
typedef struct MQTT::Message      aws_message_t;

typedef void (*aws_greengrass_callback)( aws_greengrass_discovery_callback_data_t* );

typedef void (*subscriber_callback)( aws_iot_message_t& );

class AWSIoTEndpoint
{

public:
    /* set the peer common name for the endpoint */
    void set_peer_commonname( char* peer_cn );

    /* get the transport type is set for this endpoint */
    int get_trannsport_type();

    /* get the uri set for this endpoint */
    void get_uri( char* URI );

private:
    aws_iot_transport_type_t transport;
    const char* root_ca;
    uint16_t root_ca_length;
    TLSSocket* socket;
    char* uri;
    uint16_t port;

    friend class AWSIoTClient; /* Now AWSClient can access private members of AWSEndpooint */
};

class AWSIoTClient {

public:

    AWSIoTClient();

    /** Initializes AWS library
     *
     * @param[in] network             : Network interface (WiFI, Ethernet etc ).
     * @param[in] thing_name          : Name of the thing.
     * @param[in] private_key         : Private key of device/thing.
     * @param[in] key_length          : Length of private key of device/thing.
     * @param[in] certificate         : Certificate of device/thing.
     * @param[in] certificate_length  : Length of certificate of device/thing.
     *
     * @return @ref AWS_error
     *
     */
    AWSIoTClient ( NetworkInterface* network, const char* thing_name, const char* private_key, uint16_t key_length, const char* certificate,uint16_t certificate_length );

    /** Create endpoint instance for the information provided to connect to server.
     *
     * @param[in] transport           : AWS transport
     * @param[in] uri                 : URI.
     * @param[in] port                : Port of AWS endpoint.
     * @param[in] root_ca             : RootCA certificate.
     * @param[in] rootca_length       : Length of RootCA certificate.
     *
     * @return @ref AWS_error
     *
     */
    AWSIoTEndpoint* create_endpoint( aws_iot_transport_type_t transport, const char* uri, int port, const char* root_ca, uint16_t root_ca_length);

    /** set command time-out for AWS library commands ex. connect, publish, subscribe and unsubscribe.
     *  Default value set to 5000ms. Use this API to change the time-out value. This API needs to be called after creation of AWS endpoint
     *  and before the MQTT connect operation.
     *
     * @param[in] command_timeout     : timeout in milliseconds
     *
     */
    void set_command_timeout( int command_timeout );

    /** Discovers Greengrass cores(groups) of which this 'Thing' is part of.
     *
     * @param[in] transport           : AWS transport
     * @param[in] uri                 : URI.
     * @param[in] root_ca             : RootCA certificate.
     * @param[in] rootca_length       : Length of RootCA certificate.
     * @param[in] gg_cb               : Greengrass discovery payload callback - Notify application of Greengrass Cores information
     *
     * @return @ref AWS_error
     *
     */
    AWS_error discover( aws_iot_transport_type_t transport, const char* uri, const char* root_ca, uint16_t root_ca_length, aws_greengrass_callback gg_cb );


    /** This API is blocking call and it will only return when CONACK received from server or timeout happened. Establish Connection to an AWS IoT or Greengrass core
     *
     * @param[in] ep              : AWS endpoint object
     * @param[in] conn_params     : Connection parameters
     *
     * @return @ref AWS_error
     *
     */
    AWS_error connect( AWSIoTEndpoint* ep, aws_connect_params conn_params);


    /** This API is blocking call and it will only return when PUBACK received from server or timeout happened.
     *
     *
     * @param[in] ep              : AWS endpoint object
     * @param[in] topic           : Contains the topic on which the message to be published
     * @param[in] data            : Pointer to the message to be published
     * @param[in] length          : Length of the message pointed by 'message' pointer
     * @param[in] pub_params      : publish paramaters
     *
     * @return @ref AWS_error
     */
    AWS_error publish( AWSIoTEndpoint* ep, const char* topic, const char* data, uint32_t length, aws_publish_params pub_params );


    /** This API is blocking call and it will return when SUBACK received from server or timeout happened.
     *
     *
     * @param[in] ep              : AWS endpoint object
     * @param[in] topic           : Contains the topic to be subscribed to
     * @param[in] qos             : QoS level to be used for receiving the message on the given topic
     * @param[in] cb              : subscriber callback for particular topic to receive the messages
     *
     * @return @ref AWS_error
     */
    AWS_error subscribe( AWSIoTEndpoint* ep, const char* topic, aws_iot_qos_level_t qos, subscriber_callback cb );

    /** Unsubscribe the topic
     *
     *
     * @param[in] aws             : Connection handle
     * @param[in] topic           : Contains the topic to be unsubscribed
     *
     * @return @ref AWS_error
     */
    AWS_error unsubscribe( AWSIoTEndpoint* ep, char* topic );

    /** A call to this API must be made within the keepAlive interval to keep the MQTT connection alive
     *  yield can be called if no other MQTT operation is needed.  This will also allow messages to be
     *  received.
     *  @param timeout_ms the time to wait, in milliseconds
     *  @return success code - on failure, this means the client has disconnected
     */
    AWS_error yield( unsigned long timeout_ms = 1000L );

    /** Disconnect from endpoint
     *
     * @param[in] ep              : AWS endpoint object
     * @param[in] command_timeout : Time to wait the CONACK from server
     *
     * @return @ref AWS_error
     *
     */
    AWS_error disconnect( AWSIoTEndpoint* ep );


private:
    NetworkInterface* network;
    const char* thing_name;
    const char* private_key;
    uint16_t key_length;
    const char* certificate;
    uint16_t certificate_length;
    int command_timeout;
    MQTT::Client<MQTTNetwork, Countdown> *mqtt_obj;
    MQTTNetwork *mqttnetwork;
    mqtt_security_flag flag;
};

#endif
