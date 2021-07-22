/**
 * @file subscriber.h
 * @author Arihant Baid (arihantbaid99@gmail.com)
 * @brief Subscriber for MQTT transmission
 * @version 0.1
 * @date 2021-07-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H_
#include "MQTTAsync.h"
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include "../AES/aes.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>





#if !defined(_WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif
 
#if defined(_WRS_KERNEL)
#include <OsWrapper.h>
#endif


#define CTR 1       /**< AES Encryption Method */
#define ADDRESS     "tcp://broker.hivemq.com:1883" /**< Broker Address */
#define CLIENTID    "ExampleClientSub" /**< Id number of client*/
#define QOS         1    /**< Quality of Servicer of a MQTT server */       
#define TIMEOUT     10000L 
 

/**
 * @brief The subroutine to run while receiving data from the first server
 *
 * @return void* 
 */
void * first_sub();

/**
 * @brief The subroutine to run while receiving the data from the second server
 * 
 * @return void* 
 */
void* second_sub();

/**
 * @brief The subroutine to run while receiving the data from the third server
 * 
 * @return void* 
 */
void *third_sub();

/**
 * @brief This function informs about the lost connection between broker and subscriber
 * 
 * @param context 
 * @param cause 
 */
void connlost(void *context, char *cause);

/**
 * @brief This function decrypts the encrypted message received from the publisher and store it in a text file
 * 
 * @param context 
 * @param topicName 
 * @param topicLen 
 * @param message 
 * @return int 
 */
int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message);

/**
 * @brief This function informs about the failure in disconnection with broker
 * 
 * @param context 
 * @param response 
 */
void onDisconnectFailure(void* context, MQTTAsync_failureData* response);

/**
 * @brief This function informs about successful disconnection
 * 
 * @param context 
 * @param response 
 */
void onDisconnect(void* context, MQTTAsync_successData* response);

/**
 * @brief This function informs about the successful subscription and some other uses
 * 
 * @param context 
 * @param response 
 */
void onSubscribe(void* context, MQTTAsync_successData* response);

/**
 * @brief Informs about the failure in subscribing to the broker
 * 
 * @param context 
 * @param response 
 */
void onSubscribeFailure(void* context, MQTTAsync_failureData* response);

/**
 * @brief Informs about the failure in connecting with server
 * 
 * @param context 
 * @param response 
 */
void onConnectFailure(void* context, MQTTAsync_failureData* response);


/**
 * @brief Function to run after connection and steps to be taken
 * 
 * @param context 
 * @param response 
 */
void onConnect(void* context, MQTTAsync_successData* response);

/**
 * @brief Declares MQTT variables and helps in establishing MQTT connection
 * 
 * @return int 
 */
int make_client();

#endif