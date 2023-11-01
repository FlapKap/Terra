#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#ifndef BOARD_NATIVE
#ifndef DISABLE_LORA
#include "lorawan.h"

#include "thread.h"
#include "msg.h"
#include "fmt.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

#include "log.h"

#define RECV_MSG_QUEUE (4U)

static msg_t _recv_queue[RECV_MSG_QUEUE];
static char _recv_stack[THREAD_STACKSIZE_DEFAULT];

extern semtech_loramac_t loramac;

static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

bool lorawan_send_message(uint8_t *serializedData, uint8_t len)
{
    LOG_INFO("Sending: %s\n", serializedData);
    /* Try to send the message */
    uint8_t ret = semtech_loramac_send(&loramac, serializedData, len);
    if (ret != SEMTECH_LORAMAC_TX_DONE)
    {
        LOG_ERROR("Cannot send message, ret code: %d\n", ret);
        return false;
    }
    return true;
}

static void *_recv(void *arg)
{
    msg_init_queue(_recv_queue, RECV_MSG_QUEUE);

    (void)arg; // just to get the compiler to stop complaining
    while (1)
    {
        /* blocks until some data is received */
        semtech_loramac_recv(&loramac);
        loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
        if (loramac.rx_data.payload_len > 0)
        {
            LOG_DEBUG("Data received: %s, port: %d\n length: %d\n",
                      (char *)loramac.rx_data.payload, loramac.rx_data.port, loramac.rx_data.payload_len);
        }
    }
    return NULL;
}

int lorawan_initialize_lorawan(void)
{
    /* Convert identifiers and keys strings to byte arrays */
    size_t deveui_size = fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    if (deveui_size == 0)
    {
        LOG_ERROR("deveui_size 0 indicating no dev eui\n");
        return -1;
    }
    size_t appeui_size = fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    if (appeui_size == 0)
    {
        LOG_ERROR("appeui_size 0 indicating no app eui\n");
        return -1;
    }
    size_t appkey_size = fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);
    if (appkey_size == 0)
    {
        LOG_ERROR("appkey_size 0 indicating no app key\n");
    }
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    return 0;
}

void lorawan_print_connection_info(void)
{   
    // ignoring the bounds-checking errors here. The length of the arrays should be
    // the length of their corresponding bytelengths * 2 + 1 for null termination.
    // This is ensured by the macros.
    char deveui_str[LORAMAC_DEVEUI_LEN*2+1] = ""; /* Flawfinder: ignore */
    char appeui_str[LORAMAC_DEVEUI_LEN*2+1] = ""; /* Flawfinder: ignore */
    char appkey_str[LORAMAC_APPKEY_LEN*2+1] = ""; /* Flawfinder: ignore */
    fmt_bytes_hex(deveui_str, loramac.deveui, LORAMAC_DEVEUI_LEN);
    fmt_bytes_hex(appeui_str, loramac.appeui, LORAMAC_DEVEUI_LEN);
    fmt_bytes_hex(appkey_str, loramac.appkey, LORAMAC_APPKEY_LEN);
    
    printf("LoRaWAN Connection info:\n");
    printf("  Device EUI: %s\n", deveui_str);
    printf("  Application EUI: %s\n", appeui_str);
    printf("  Application Key: %s\n", appkey_str);
}

int lorawan_connect_lorawan(void)
{
    LOG_INFO("trying to join network\n");
    /* Join the network if not already joined */
    while (!semtech_loramac_is_mac_joined(&loramac))
    {
        /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
         * generated device address and to get the network and application session
         * keys.
         */
        LOG_INFO("Starting join procedure\n");
        if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED)
        {
            LOG_WARNING("Join procedure failed. Trying again\n");
        }
    }
    LOG_INFO("Join procedure succeeded");
    // semtech_loramac_save();
    LOG_INFO("creating recv thread");
    thread_create(_recv_stack, sizeof(_recv_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, _recv, NULL, "recv thread");
    // printf("ret: %d\n", ret);
    //  /* trigger the first send */
    //  msg_t msg;
    //  kernel_pid_t sender_pid = thread_getpid();
    //  msg_send(&msg, sender_pid);

    return 0;
}
#endif
#endif
