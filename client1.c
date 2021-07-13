#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define ADDRESS "tcp://broker.hivemq.com:1883"
#define CLIENTID "PC1"
#define TOPIC "TEMP1"
#define END "END"
#define PAYLOAD "Device - HB : Client 1!"
#define QOS 1
#define TIMEOUT 10000L

int main(int argc, char *argv[])
{
    sem_unlink("s1");
    sem_unlink("s2");
    sem_t *ps;
    ps=sem_open("/s1", O_CREAT|O_RDWR, 0666, 1);
    char Data[100] = PAYLOAD;
    time_t rawtime;
    struct tm *timeinfo;

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    sem_wait(ps);
    for (int i = 0; i < 10; i++)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        memset(Data, 0, 100);
        strcat(Data, PAYLOAD);
        sprintf(loop, "loop %d", i);
        strcat(Data, loop);
        //    pubmsg.payload = PAYLOAD;
        //    pubmsg.payloadlen = strlen(PAYLOAD);
        pubmsg.payload = Data;
        pubmsg.payloadlen = strlen(Data);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        printf("Waiting for up to %d seconds for publication of %s\n"
               "on topic %s for client with ClientID: %s\n",
               (int)(TIMEOUT / 1000), PAYLOAD, TOPIC, CLIENTID);
        //rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

        printf("Message with delivery token %d delivered\n", token);
        sleep(1);
    }
    pubmsg.payload = END;
    pubmsg.payloadlen = strlen(END);
    sem_post(ps);

    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return rc;
}
