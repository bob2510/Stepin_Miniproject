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
#include "aes.h"
#include <stdint.h>

/*--------------------------------------------Macros---------------------------------*/

#define CTR 1
#define ADDRESS "tcp://broker.hivemq.com:1883"
#define CLIENTID "PC2"
#define TOPIC "HUMID"
#define END "END"
#define PAYLOAD "Aadreesh:Client2"
#define QOS 1
#define TIMEOUT 10000L

/*--------------------------------------------Main Function--------------------------*/

int main(int argc, char *argv[])
{

/*--------------------------------------------Semaphore Initialization---------------*/    

    sem_unlink("s2");
    sem_t *ps, *qs;
    ps=sem_open("/s1", O_CREAT|O_RDWR, 0666);
    qs=sem_open("/s2", O_CREAT|O_RDWR, 0666, 0);

/*--------------------------------------------MQTT Initialization---------------*/    

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
    
    MQTTClient_create(&client, ADDRESS, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

/*--------------------------------------------MQTT Connect---------------*/
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
        //    pubmsg.payload = PAYLOAD;
        //    pubmsg.payloadlen = strlen(PAYLOAD);


        //AES ENCRYPTION
    sem_wait(ps); 
    sleep(5);   
    for (int i=0;i<10;i++)
    {
        uint8_t int_buf[4096];

/*--------------------------------------------AES Encryption------------------*/               
        struct AES_ctx Enc;
        const uint8_t key[16] = { (uint8_t) 0x2b, (uint8_t) 0x7e, (uint8_t) 0x15, (uint8_t) 0x16, (uint8_t) 0x28, (uint8_t) 0xae, (uint8_t) 0xd2, (uint8_t) 0xa6, (uint8_t) 0xab, (uint8_t) 0xf7, (uint8_t) 0x15, (uint8_t) 0x88, (uint8_t) 0x09, (uint8_t) 0xcf, (uint8_t) 0x4f, (uint8_t) 0x3c };
        uint8_t buf[4096] = PAYLOAD;
        strcat(buf, "\n");
        int len = strlen(PAYLOAD)+1;
        for(int i=0; i<len; i++)
        {
            int_buf[i] = (uint8_t)buf[i]; 
        }
        uint8_t iv[16]  = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };

        AES_init_ctx_iv(&Enc, key,iv);
 
        //AES_init_ctx(&Enc, key);
        AES_CTR_xcrypt_buffer(&Enc, int_buf, len);

        int len = strlen(PAYLOAD)+1;

        pubmsg.payload  = int_buf;
        pubmsg.payloadlen = len;
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        //rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

        printf("Message with delivery token %d delivered\n", token);
        sleep(1);
    }
    
   pubmsg.payload = END;
   pubmsg.payloadlen = strlen(END);

   MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
   MQTTClient_disconnect(client, 10000);
   sem_post(qs); 
   MQTTClient_destroy(&client);

   return rc;
}
 