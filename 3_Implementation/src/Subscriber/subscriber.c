#include<stdio.h>

#ifndef SUBSCRIBER_H_
#define SUBSCRIBER_H_
#include "subscriber.h"
#endif

int disc_finished = 0;
int subscribed = 0;
int finished = 0;
int msgg_arrived = 0;
// int noofdata = 10;  
int fileptr;
int terminator=0;
const char* TOPIC = NULL;
char* topic1 = "TEMPTEST";
char* topic2 = "HUMID";
char* topic3 = "MOIST";



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
        uint8_t  enc_buff [4096];
        char char_buf[4096];
        struct AES_ctx Enc;
        const uint8_t key[16] = { (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2, (uint8_t) 0xa6, (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };
        uint8_t iv[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };

        if(strcmp("END", message->payload) == 0)
        {
            msgg_arrived=1;
            terminator=1;
            return 1;
        }
        
        AES_init_ctx_iv(&Enc, key,iv);
        memcpy(enc_buff,message->payload, message->payloadlen);

        

        AES_CTR_xcrypt_buffer(&Enc, enc_buff, message->payloadlen);
        
        for(int i=0; i < message->payloadlen; i++)
        {
            char_buf[i] = (char)enc_buff[i]; 
        } 
        

        write(fileptr, char_buf,message->payloadlen);
        

        MQTTAsync_freeMessage(&message);
        MQTTAsync_free(topicName);
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
        printf("here\n");
        MQTTAsync_destroy(&client);
exit:
        return rc;
}

void* first_sub()
{       
        fileptr = open("t1.txt",  O_CREAT | O_WRONLY | O_APPEND, 0666);
        while(!terminator)
        {       TOPIC=topic1;
                make_client();
        }
                close(fileptr);
                
}
void* second_sub()
{       
        fileptr = open("t2.txt",  O_CREAT | O_WRONLY | O_APPEND, 0666);
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
        close(fileptr);
        
}
void* third_sub()
{       
        
        fileptr = open("t3.txt",  O_CREAT | O_WRONLY | O_APPEND, 0666);
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
        close(fileptr);
}