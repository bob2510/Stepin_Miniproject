#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTAsync.h"
#include <semaphore.h>
#include <fcntl.h>
 
#if !defined(_WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif
 
#if defined(_WRS_KERNEL)
#include <OsWrapper.h>
#endif
 
#define ADDRESS     "tcp://broker.hivemq.com:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "TEMP1"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
 
int finished = 0;
 
void connlost(void *context, char *cause)
{
        MQTTAsync client = (MQTTAsync)context;
        MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
        int rc;
 
        printf("\nConnection lost\n");
        printf("     cause: %s\n", cause);
 
        printf("Reconnecting\n");
        conn_opts.keepAliveInterval = 20;
        conn_opts.cleansession = 1;
        if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to start connect, return code %d\n", rc);
                finished = 1;
        }
}
 
void onDisconnectFailure(void* context, MQTTAsync_failureData* response)
{
        printf("Disconnect failed\n");
        finished = 1;
}
 
void onDisconnect(void* context, MQTTAsync_successData* response)
{
        printf("Successful disconnection\n");
        finished = 1;
}
 
void onSendFailure(void* context, MQTTAsync_failureData* response)
{
        MQTTAsync client = (MQTTAsync)context;
        MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
        int rc;
 
        printf("Message send failed token %d error code %d\n", response->token, response->code);
        opts.onSuccess = onDisconnect;
        opts.onFailure = onDisconnectFailure;
        opts.context = client;
        if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to start disconnect, return code %d\n", rc);
                exit(EXIT_FAILURE);
        }
}
 
void onSend(void* context, MQTTAsync_successData* response)
{
        MQTTAsync client = (MQTTAsync)context;
        MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
        int rc;
 
        printf("Message with token value %d delivery confirmed\n", response->token);
        opts.onSuccess = onDisconnect;
        opts.onFailure = onDisconnectFailure;
        opts.context = client;
        if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to start disconnect, return code %d\n", rc);
                exit(EXIT_FAILURE);
        }
}
 
 
void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
        printf("Connect failed, rc %d\n", response ? response->code : 0);
        finished = 1;
}
 
 
void onConnect(void* context, MQTTAsync_successData* response)
{
        MQTTAsync client = (MQTTAsync)context;
        MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
        MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
        int rc;
 
        printf("Successful connection\n");
        opts.onSuccess = onSend;
        opts.onFailure = onSendFailure;
        opts.context = client;
        pubmsg.payload = PAYLOAD;
        pubmsg.payloadlen = (int)strlen(PAYLOAD);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        if ((rc = MQTTAsync_sendMessage(client, TOPIC, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to start sendMessage, return code %d\n", rc);
                exit(EXIT_FAILURE);
        }
}
 
int messageArrived(void* context, char* topicName, int topicLen, MQTTAsync_message* m)
{
        // not expecting any messages
        return 1;
}
 
int main(int argc, char* argv[])
{
        sem_t *ps;
        ps=sem_open("/s1",O_CREAT, 0666, 1);

        MQTTAsync client;
        MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        MQTTClient_deliveryToken token;
        int rc;
 
        if ((rc = MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to create client object, return code %d\n", rc);
                exit(EXIT_FAILURE);
        }
 
        if ((rc = MQTTAsync_setCallbacks(client, NULL, connlost, messageArrived, NULL)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to set callback, return code %d\n", rc);
                exit(EXIT_FAILURE);
        }
 
        conn_opts.keepAliveInterval = 20;
        conn_opts.cleansession = 1;
        conn_opts.onSuccess = onConnect;
        conn_opts.onFailure = onConnectFailure;
        conn_opts.context = client;
        if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
        {
                sem_wait(ps);
                printf("Failed to start connect, return code %d\n", rc);
                exit(EXIT_FAILURE);
        }
 
        printf("Waiting for publication of %s\n"
         "on topic %s for client with ClientID: %s\n",
         PAYLOAD, TOPIC, CLIENTID);
         for (int i = 0; i < 10; i++)
        {
                memset(Data, 0, 100);
                strcat(Data, PAYLOAD);
                strcat(Data, asctime(timeinfo));

        //    pubmsg.payload = PAYLOAD;
        //    pubmsg.payloadlen = strlen(PAYLOAD);
                pubmsg.payload = Data;
                pubmsg.payloadlen = strlen(Data);
                pubmsg.qos = QOS;
                pubmsg.retained = 0;

                MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
                rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

        printf("Message with delivery token %d delivered\n", token);
        sleep(1);
    } 
        while (!finished)
                #if defined(_WIN32)
                        Sleep(100);
                #else
                        usleep(10000L);
                #endif
        sem_post(ps);
        MQTTAsync_destroy(&client);
        return rc;
}
