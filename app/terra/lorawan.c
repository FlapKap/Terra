#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <random.h>
#ifndef BOARD_NATIVE
#ifndef DISABLE_LORA
#include "lorawan.h"

#include "ztimer.h"

#include "thread.h"
#include "msg.h"
#include "fmt.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

#include "log.h"

#include <assert.h>

#define RECV_MSG_QUEUE (4U)

static msg_t _recv_queue[RECV_MSG_QUEUE];
static char _recv_stack[THREAD_STACKSIZE_DEFAULT];
static kernel_pid_t _recv_pid;
static kernel_pid_t _main_pid;
extern semtech_loramac_t loramac;

static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

bool lorawan_send_message(uint8_t *serializedData, uint8_t len)
{   
    assert(len <= LORAWAN_APP_DATA_MAX_SIZE);
    
    LOG_INFO("[lorawan.c] Sending:");
    for (int i = 0; i < len; i++)
    {
        LOG_INFO(" %02x", serializedData[i]);
    }
    LOG_INFO(", len: %d\n", (int) len);
    /* Try to send the message */
    uint8_t ret = semtech_loramac_send(&loramac, serializedData, len);
    if (ret != SEMTECH_LORAMAC_TX_DONE)
    {
        switch (ret)
        {
        case SEMTECH_LORAMAC_NOT_JOINED:
            LOG_ERROR("[lorawan.c] send_message: Not joined\n");
            break;
        case SEMTECH_LORAMAC_BUSY:
            LOG_ERROR("[lorawan.c] send_message: Busy. Join or tx already in progress\n");
            break;
        case SEMTECH_LORAMAC_DUTYCYCLE_RESTRICTED:
            LOG_ERROR("[lorawan.c] send_message: Duty cycle restricted\n");
            break;
        case SEMTECH_LORAMAC_TX_ERROR:
            LOG_ERROR("[lorawan.c] send_message: TX error. Probably invalid paramater given\n");
            break;
        case SEMTECH_LORAMAC_TX_CNF_FAILED:
            LOG_ERROR("[lorawan.c] send_message: Message was transmitted but no ACK was received\n");
            break;
        default:
            LOG_ERROR("[lorawan.c] Cannot send message, ret code: %d\n", ret);
            break;
        }
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
        if (loramac.rx_data.payload_len > 0)
        {
            LOG_DEBUG("[lorawan.c] Data received of length %d on port: %d\n",
                    loramac.rx_data.payload_len, loramac.rx_data.port);
        }
    }
    return NULL;
}

int lorawan_initialize_lorawan(void)
{
    if (!semtech_loramac_get_join_state(&loramac))
    {
        /* Convert identifiers and keys strings to byte arrays */
        assert(fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT) > 0); // if deveui is 0 then it is not set
        assert(fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT) > 0); // if appeui is 0 then it is not set
        assert(fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT) > 0); // if appkey is 0 then it is not set

        semtech_loramac_set_deveui(&loramac, deveui);
        semtech_loramac_set_appeui(&loramac, appeui);
        semtech_loramac_set_appkey(&loramac, appkey);
        //semtech_loramac_set_adr(&loramac, true);
        //semtech_loramac_set_dr(&loramac, LORAMAC_DATA_RATE); //default is set in _init_loramac in semtech_loramac.c
        semtech_loramac_set_rx2_dr(&loramac, CONFIG_LORAMAC_DEFAULT_RX2_DR);
        //semtech_loramac_set_tx_power(&loramac, LORAMAC_TX_PWR_0);
        
        /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
        //semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);
    }
    //TODO: there is a really damned annoying bug somewhere where somehow we can't get confirmed downlinks. The messages are just never received
    // I have tried manually verifying receive windows on TTN and device side. TTN seems to default RX1 window to 5 seconds. Device seems to default to 1 so i changed TTN to 1. Didn't help.
    // gateway sometimes report CRC failed. It seems to be on the acks but i am not sure. 
    // anyway, setting setting the tx mode to uncnf sidesteps the whole issue, but it should be solved at some point.
    // it might also be relevant that both sx12xx drivers and the semtech-loramac code are horribly outdated.
    semtech_loramac_set_tx_mode(&loramac, LORAMAC_TX_UNCNF);
    return 0;
}

void lorawan_print_connection_info(void)
{
    // ignoring the bounds-checking errors here. The length of the arrays should be
    // the length of their corresponding bytelengths * 2 + 1 for null termination.
    // This is ensured by the macros.
    char deveui_str[LORAMAC_DEVEUI_LEN * 2 + 1] = ""; /* Flawfinder: ignore */
    char appeui_str[LORAMAC_DEVEUI_LEN * 2 + 1] = ""; /* Flawfinder: ignore */
    char appkey_str[LORAMAC_APPKEY_LEN * 2 + 1] = ""; /* Flawfinder: ignore */
    char devaddr_str[LORAMAC_DEVADDR_LEN * 2 + 1] = ""; /* Flawfinder: ignore */
    char appskey_str[LORAMAC_APPSKEY_LEN * 2 + 1] = ""; /* Flawfinder: ignore */
    char nwkskey_str[LORAMAC_NWKSKEY_LEN * 2 + 1] = ""; /* Flawfinder: ignore */
    fmt_bytes_hex(deveui_str, loramac.deveui, LORAMAC_DEVEUI_LEN);
    fmt_bytes_hex(appeui_str, loramac.appeui, LORAMAC_DEVEUI_LEN);
    fmt_bytes_hex(appkey_str, loramac.appkey, LORAMAC_APPKEY_LEN);


    uint8_t devaddr[LORAMAC_DEVADDR_LEN] = {0}; /* Flawfinder: ignore */
    semtech_loramac_get_devaddr(&loramac, devaddr);
    fmt_bytes_hex(devaddr_str, devaddr, LORAMAC_DEVADDR_LEN);


    uint8_t appskey[LORAMAC_APPSKEY_LEN] = {0}; /* Flawfinder: ignore */
    semtech_loramac_get_appskey(&loramac, appskey);
    fmt_bytes_hex(appskey_str, appskey, LORAMAC_APPSKEY_LEN);


    uint8_t nwkskey[LORAMAC_NWKSKEY_LEN] = {0}; /* Flawfinder: ignore */
    semtech_loramac_get_nwkskey(&loramac, nwkskey);
    fmt_bytes_hex(nwkskey_str, nwkskey, LORAMAC_NWKSKEY_LEN);

    uint8_t rx2_dr = semtech_loramac_get_rx2_dr(&loramac);
    uint32_t rx2_freq = semtech_loramac_get_rx2_freq(&loramac);

    uint32_t uplink_counter = semtech_loramac_get_uplink_counter(&loramac);

    bool joined = semtech_loramac_get_join_state(&loramac);

    loramac_class_t class = semtech_loramac_get_class(&loramac);
    char class_str = (char) class + 65;

    uint8_t dr = semtech_loramac_get_dr(&loramac);

    bool public_nw = semtech_loramac_get_public_network(&loramac);

    uint8_t confirmable = (loramac_tx_mode_t) semtech_loramac_get_tx_mode(&loramac);


    // print connection info. Is done in multiple printf calls because otherwise i think it smashes the stack
    printf("LoRaWAN Connection info: ");
    printf("Joined: %s, ", joined ? "true" : "false");
    printf("Device Class: %c, ", class_str);
    printf("Device EUI: %s, ", deveui_str);
    printf("Application EUI: %s, ", appeui_str);
    printf("Application Key: %s, ", appkey_str);
    printf("Data Rate: %d, ", (int) dr);
    printf("Public Network: %s, ", public_nw ? "true" : "false");
    printf("Confirmable: %s, ", confirmable == LORAMAC_TX_CNF ? "true" : "false");
    printf("AppSKey: %s, ", appskey_str);
    printf("NwkSKey: %s, ", nwkskey_str);
    printf("Device Address: %s, ", devaddr_str);
    printf("RX1 delay: %d, ", CONFIG_LORAMAC_DEFAULT_RX1_DELAY);
    printf("RX2 delay: %d, ", LORAMAC_DEFAULT_RX2_DELAY);
    printf("RX2 data rate: %d, ", (int) rx2_dr);
    printf("RX2 frequency: %" PRIu32 ", ", rx2_freq);
    printf("Uplink counter: %" PRIu32", ", uplink_counter);
    printf("\n");

    
}

bool lorawan_receive(void)
{
    /* blocks until some data is received */
    semtech_loramac_recv(&loramac);
    loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
    if (loramac.rx_data.payload_len > 0)
    {
        LOG_DEBUG("[lorawan.c] Data received of length %d on port: %d\n",
                   loramac.rx_data.payload_len, loramac.rx_data.port);
        return true;
    }
    else
    {
        LOG_DEBUG("[lorawan.c] No data received. I guess this shouldn't happen?\n");
        return false;
    }
}

int lorawan_connect_lorawan(void)
{
    LOG_INFO("[lorawan.c] trying to join network\n");
    /* Join the network if not already joined */
    while (!semtech_loramac_get_join_state(&loramac))
    {
        /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
         * generated device address and to get the network and application session
         * keys.
         */
        LOG_INFO("[lorawan.c] Starting join procedure\n");
        if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED)
        {
            uint32_t retry_sleep_ms = 20000 + random_uint32() % 20000;
            LOG_WARNING("[lorawan.c] Join procedure failed. Trying again after 30s +-10s: %" PRIu32 " ms...\n",retry_sleep_ms);
            ztimer_sleep(ZTIMER_MSEC, retry_sleep_ms);
        }
    }
    LOG_INFO("[lorawan.c] Join procedure succeeded\n");
    // semtech_loramac_save();
    _main_pid = thread_getpid();
    LOG_INFO("[lorawan.c] creating recv thread\n");
    _recv_pid = thread_create(_recv_stack, sizeof(_recv_stack),
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
