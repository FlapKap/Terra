#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifndef BOARD_NATIVE

#include "lora.h"

#include "thread.h"
#include "msg.h"
#include "fmt.h"
#include "net/loramac.h"
#include "semtech_loramac.h"

#define RECV_MSG_QUEUE (4U)

static msg_t _recv_queue[RECV_MSG_QUEUE];
static char _recv_stack[THREAD_STACKSIZE_DEFAULT];

extern semtech_loramac_t loramac;

static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

uint8_t send_message(uint8_t* serializedData, uint8_t len)
{
    printf("Sending: %s\n", serializedData);
    /* Try to send the message */
    uint8_t ret = semtech_loramac_send(&loramac,serializedData, len);
    if (ret != SEMTECH_LORAMAC_TX_DONE) {
        printf("Cannot send message, ret code: %d\n", ret);
    }
    return ret;
}

static void *_recv(void *arg)
{
    msg_init_queue(_recv_queue, RECV_MSG_QUEUE);
 
    (void)arg; // just to get the compiler to stop complaining
    while (1) {
        /* blocks until some data is received */
        semtech_loramac_recv(&loramac);
        loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
        printf("Data received: %s, port: %d\n length: %d\n",
               (char *)loramac.rx_data.payload, loramac.rx_data.port, loramac.rx_data.payload_len);
    }
    return NULL;
}

int init_lorawan(void) {
    /* Convert identifiers and keys strings to byte arrays */
    size_t deveui_size = fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    size_t appeui_size = fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    size_t appkey_size = fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    // if any of deveui_size, appeui_size, or appkey_size is zero, return -1
    if (deveui_size == 0 || appeui_size == 0 || appkey_size == 0) {
        return -1;
    } else {
        return 0;
    }
}


int connect_lorawan(void)
{
    puts("trying to join network\n");
    /* Join the network if not already joined */
    while (!semtech_loramac_is_mac_joined(&loramac)) {
        /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
         * generated device address and to get the network and application session
         * keys.
         */
        printf("Starting join procedure\n");
        if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
            printf("Join procedure failed. Trying again\n");
            
        }
    }
    puts("Join procedure succeeded");
    //semtech_loramac_save();
    puts("creating recv thread");
    int ret = thread_create(_recv_stack, sizeof(_recv_stack),
              THREAD_PRIORITY_MAIN - 1, 0, _recv, NULL, "recv thread");
    printf("ret: %d\n", ret);
    // /* trigger the first send */
    // msg_t msg;
    // kernel_pid_t sender_pid = thread_getpid();
    // msg_send(&msg, sender_pid);

    return 0;
}
#endif