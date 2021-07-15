#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTAsync.h"
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
 
#if !defined(_WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif
 
#if defined(_WRS_KERNEL)
#include <OsWrapper.h>
#endif
 

#define ADDRESS     "tcp://broker.hivemq.com:1883"
#define CLIENTID    "ExampleClientSub"
//#define TOPIC       "MQTT Examples"
//#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
 
int disc_finished = 0;
int subscribed = 0;
int finished = 0;
int msgg_arrived = 0;
int noofdata = 10;  
int fileptr;
int terminator=0;
sem_t *ps,*qs;
const char* TOPIC = NULL;

const char* topic1 = "TEMPTEST";
const char* topic2 = "HUMID";
const char* topic3 = "MOIST";

void connlost(void *context, char *cause)
{
        MQTTAsync client = (MQTTAsync)context;
        MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
        int rc;
 
        printf("\nConnection lost\n");
        if (cause)
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
 
 
int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
//     printf("Message arrived\n");
//     printf("     topic: %s\n", topicName);
//     printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
    write(fileptr, message->payload,message->payloadlen);
    if(strcmp("END", message->payload) == 0)
    {
            msgg_arrived=1;
            terminator=1;
    }
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    
//     msgg_arrived=1;
    return 1;
}
 
void onDisconnectFailure(void* context, MQTTAsync_failureData* response)
{
        printf("Disconnect failed, rc %d\n", response->code);
        disc_finished = 1;
}
 
void onDisconnect(void* context, MQTTAsync_successData* response)
{
        printf("Successful disconnection\n");
        disc_finished = 1;
}
 
void onSubscribe(void* context, MQTTAsync_successData* response)
{
        printf("Subscribe succeeded\n");
        subscribed = 1;
}
 
void onSubscribeFailure(void* context, MQTTAsync_failureData* response)
{
        printf("Subscribe failed, rc %d\n", response->code);
        finished = 1;
}
 
 
void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
        printf("Connect failed, rc %d\n", response->code);
        finished = 1;
}
 
 
void onConnect(void* context, MQTTAsync_successData* response)
{
        MQTTAsync client = (MQTTAsync)context;
        MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
        int rc;
 
        printf("Successful connection\n");
 
        // printf("Subscribing to topic \nfor client %s using QoS%d\n\n"
        //    "Press Q<Enter> to quit\n\n", CLIENTID, QOS);
        opts.onSuccess = onSubscribe;
        opts.onFailure = onSubscribeFailure;
        opts.context = client;
        if ((rc = MQTTAsync_subscribe(client, TOPIC, QOS, &opts)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to start subscribe, return code %d\n", rc);
                finished = 1;
        }
}
 
 
int make_client()
{
        MQTTAsync client;
        MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
        MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
        int rc;
        int ch;
 
        if ((rc = MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL))
                        != MQTTASYNC_SUCCESS)
        {
                printf("Failed to create client, return code %d\n", rc);
                rc = EXIT_FAILURE;
                goto exit;
        }
 
        if ((rc = MQTTAsync_setCallbacks(client, client, connlost, msgarrvd, NULL)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to set callbacks, return code %d\n", rc);
                rc = EXIT_FAILURE;
                goto destroy_exit;
        }
 
        conn_opts.keepAliveInterval = 5;
        conn_opts.cleansession = 1;
        conn_opts.onSuccess = onConnect;
        conn_opts.onFailure = onConnectFailure;
        conn_opts.context = client;
        if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to start connect, return code %d\n", rc);
                rc = EXIT_FAILURE;
                goto destroy_exit;
        }
 
        while (!subscribed && !finished)
                #if defined(_WIN32)
                        Sleep(100);
                #else
                        usleep(10000L);
                #endif
 
        if (finished)
                goto exit;
        
        
        do
        {
                // continue;
        } while (!(msgg_arrived));
 
        disc_opts.onSuccess = onDisconnect;
        disc_opts.onFailure = onDisconnectFailure;
        if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS)
        {
                printf("Failed to start disconnect, return code %d\n", rc);
                rc = EXIT_FAILURE;
                goto destroy_exit;
        }
        while (!disc_finished)
        {
                #if defined(_WIN32)
                        Sleep(100);
                #else
                        usleep(10000L);
                #endif
        }
 
destroy_exit:
        MQTTAsync_destroy(&client);
exit:
        return rc;
}

void* first_sub(void*pv)
{       
        int fptr = open("t1.txt",  O_CREAT | O_WRONLY | O_APPEND, 0666);
        fileptr=fptr;
        while(!terminator)
        {       TOPIC=topic1;
                make_client();
        }
                close(fptr);
                sem_post(ps);
}
void* second_sub(void*pv)
{       
        sem_wait(ps);
        int fptr = open("t2.txt",  O_CREAT | O_WRONLY | O_APPEND, 0666);
        fileptr=fptr;
        terminator=0;
        TOPIC=topic2;
        while(!terminator)
           {    
                disc_finished = 0;
                subscribed = 0;
                finished = 0;
                msgg_arrived=0;
                make_client();
           }    
        close(fptr);
        sem_post(qs);
        sem_post(ps);
}
void* third_sub(void*pv)
{       
        sem_wait(ps);
        sem_wait(qs);
        int fptr = open("t3.txt",  O_CREAT | O_WRONLY | O_APPEND, 0666);
        fileptr=fptr;
        terminator=0;
        TOPIC = topic3;
        while(!terminator)
        {
                disc_finished = 0;
                subscribed = 0;
                finished = 0;
                msgg_arrived=0;
                make_client();
        }
        close(fptr);
}

int main(int argc, char* argv[])
{
      
    sem_unlink("s1");
    sem_unlink("s2");
        ps=sem_open("/s1",O_CREAT, 0666, 0);
        qs=sem_open("/s2",O_CREAT, 0666, 0);

        pthread_t pt1, pt2, pt3; //thread handle

    
        pthread_create(&pt1, NULL, first_sub, NULL);
        pthread_create(&pt2, NULL, second_sub, NULL);
        pthread_create(&pt3, NULL, third_sub, NULL);
	
        pthread_join(pt1,NULL);
        pthread_join(pt2,NULL);
        pthread_join(pt3,NULL);
   
    sem_unlink("s1");
    sem_unlink("s2");
    return 0;
   
}
