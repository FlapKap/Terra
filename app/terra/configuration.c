#include <stdio.h>
#include "configuration.h"
#include "fmt.h"


#ifndef DISABLE_LORA
#include "lorawan.h"
#endif

#define ENABLE_DEBUG 1
#include "debug.h"

#include <flashdb.h>
#include "fal_cfg.h"

#include <mutex.h>

/* KVDB object */
static struct fdb_kvdb kvdb = { 0 };

static mutex_t kvdb_lock;

// Default values
static struct fdb_default_kv_node kv_table[] = {
    {"loop_counter", (CONFIGURATION_LOOP_COUNTER_TYPE) 0, sizeof(CONFIGURATION_LOOP_COUNTER_TYPE)},
    {"raw_message_size", (uint8_t) 0, sizeof(uint8_t)},
    {"raw_message_buffer", (uint8_t[LORAWAN_APP_DATA_MAX_SIZE]){0}, LORAWAN_APP_DATA_MAX_SIZE},
    {"deveui", (uint8_t[LORAMAC_DEVEUI_LEN]){0}, LORAMAC_DEVEUI_LEN},
    {"appeui", (uint8_t[LORAMAC_APPEUI_LEN]){0}, LORAMAC_APPEUI_LEN},
    {"appkey",(uint8_t[LORAMAC_APPKEY_LEN]){0}, LORAMAC_APPKEY_LEN},
    {"uplink_counter", (uint32_t) 0, sizeof(uint32_t)},
};


//locking and unlocking functions. We save the mutex as user data in the db
static void lock(fdb_db_t db){
    mutex_lock(db->user_data);
}

static void unlock(fdb_db_t db){
    mutex_unlock(db->user_data);
}

static char* fdb_error_to_str(fdb_err_t err){
    switch (err)
    {
        case FDB_NO_ERR: return "no error";
        case FDB_ERASE_ERR: return "erase error";
        case FDB_READ_ERR: return "read error";
        case FDB_WRITE_ERR: return "write error";
        case FDB_PART_NOT_FOUND: return "partition not found";
        case FDB_KV_NAME_ERR: return "key/value name error";
        case FDB_KV_NAME_EXIST: return "key/value name exist";
        case FDB_SAVED_FULL: return "saved full";
        case FDB_INIT_FAILED: return "init failed";
    }
    puts("this should never happen");
    return "unknown error";
}
extern void fdb_mtd_init(mtd_dev_t *mtd);


bool configuration_init(void){
    
    struct fdb_default_kv default_kv;
    default_kv.kvs = kv_table;
    default_kv.num = ARRAY_SIZE(kv_table);

    uint32_t sec_size = FAL_MTD->pages_per_sector * FAL_MTD->page_size;
    uint32_t db_size = sec_size*8; //these are just default values from examples. Might need to be tweaked
    
    fdb_mtd_init(FAL_MTD);

    mutex_init(&kvdb_lock);

    fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_LOCK, (void *)(uintptr_t) &lock);
    fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_UNLOCK, (void *)(uintptr_t) &unlock);
    /* set the sector and database max size */
    fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_SEC_SIZE, &sec_size);
    fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_MAX_SIZE, &db_size);

    //initialize database
    fdb_err_t err = fdb_kvdb_init(&kvdb, "conf", FAL_PART0_LABEL, &default_kv, &kvdb_lock);
    DEBUG("[configuration.c] fdb_kvdb_init: %s\n", fdb_error_to_str(err));

    return err == FDB_NO_ERR;
}

bool configuration_save(TerraConfiguration *config)
{   
    DEBUG("[configuration.c] Saving configuration\n");

    struct fdb_blob blob;
    fdb_err_t err;
    err = fdb_kv_set_blob(&kvdb, "loop_counter", fdb_blob_make(&blob, &config->loop_counter, sizeof(config->loop_counter)));
    if(err != FDB_NO_ERR){
        DEBUG("[configuration.c] saving loop_counter error: %s\n", fdb_error_to_str(err));
        return false;
    }

    err = fdb_kv_set_blob(&kvdb, "raw_message_size", fdb_blob_make(&blob, &config->raw_message_size, sizeof(config->raw_message_size)));
    if(err != FDB_NO_ERR){
        DEBUG("[configuration.c] saving raw_message_size error: %s\n", fdb_error_to_str(err));
        return false;
    }

    err = fdb_kv_set_blob(&kvdb, "raw_message_buffer", fdb_blob_make(&blob, config->raw_message_buffer, config->raw_message_size));
    if(err != FDB_NO_ERR){
        DEBUG("[configuration.c] saving raw_message_buffer error: %s\n", fdb_error_to_str(err));
        return false;
    }
    
    uint8_t deveui[LORAMAC_DEVEUI_LEN];
    assert(fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT) > 0); // if deveui is 0 then it is not set
    err = fdb_kv_set_blob(&kvdb, "deveui", fdb_blob_make(&blob, deveui, LORAMAC_DEVEUI_LEN));
    if(err != FDB_NO_ERR){
        DEBUG("[configuration.c] saving deveui error: %s\n", fdb_error_to_str(err));
        return false;
    }

    uint8_t appeui[LORAMAC_APPEUI_LEN];
    assert(fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT) > 0); // if appeui is 0 then it is not set
    err = fdb_kv_set_blob(&kvdb, "appeui", fdb_blob_make(&blob, appeui, LORAMAC_APPEUI_LEN));
    if (err != FDB_NO_ERR)
    {
        DEBUG("[configuration.c] saving appeui error: %s\n", fdb_error_to_str(err));
        return false;
    }
    
    uint8_t appkey[LORAMAC_APPKEY_LEN];
    assert(fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT) > 0); // if appkey is 0 then it is not set
    err = fdb_kv_set_blob(&kvdb, "appkey", fdb_blob_make(&blob, appkey, LORAMAC_APPKEY_LEN));
    if (err != FDB_NO_ERR)
    {
        DEBUG("[configuration.c] saving appkey error: %s\n", fdb_error_to_str(err));
        return false;
    }

    uint32_t uplink_counter = semtech_loramac_get_uplink_counter(config->loramac);
    err = fdb_kv_set_blob(&kvdb, "uplink_counter", fdb_blob_make(&blob, &uplink_counter, sizeof(uplink_counter)));
    if(err != FDB_NO_ERR){
        DEBUG("[configuration.c] saving uplink_counter error: %s\n", fdb_error_to_str(err));
        return false;
    }

    return true;
}

bool configuration_load( TerraConfiguration* config )
{
    DEBUG("[configuration.c] Loading configuration\n");

    struct fdb_blob blob;
    size_t return_size;
    return_size = fdb_kv_get_blob(&kvdb, "loop_counter", fdb_blob_make(&blob, &config->loop_counter, sizeof(config->loop_counter)));
    if(return_size != sizeof(config->loop_counter)){
        DEBUG("[configuration.c] loading loop_counter error\n");
        return false;
    }

    return_size = fdb_kv_get_blob(&kvdb, "raw_message_size", fdb_blob_make(&blob, &config->raw_message_size, sizeof(config->raw_message_size)));
    if(return_size != sizeof(config->raw_message_size)){
        DEBUG("[configuration.c] loading raw_message_size error\n");
        return false;
    }

    return_size = fdb_kv_get_blob(&kvdb, "raw_message_buffer", fdb_blob_make(&blob, config->raw_message_buffer, config->raw_message_size));
    if(return_size != config->raw_message_size){
        DEBUG("[configuration.c] loading raw_message_buffer error\n");
        return false;
    }

    return_size = fdb_kv_get_blob(&kvdb, "deveui", fdb_blob_make(&blob, config->loramac->deveui, LORAMAC_DEVEUI_LEN));
    if(return_size != sizeof(config->loramac->deveui)){
        DEBUG("[configuration.c] loading deveui error\n");
        return false;
    }

    return_size = fdb_kv_get_blob(&kvdb, "appeui", fdb_blob_make(&blob, config->loramac->appeui, LORAMAC_APPEUI_LEN));
    if(return_size != sizeof(config->loramac->appeui)){
        DEBUG("[configuration.c] loading appeui error\n");
        return false;
    }

    return_size = fdb_kv_get_blob(&kvdb, "appkey", fdb_blob_make(&blob, config->loramac->appkey, LORAMAC_APPKEY_LEN));
    if(return_size != sizeof(config->loramac->appkey)){
        DEBUG("[configuration.c] loading appkey error\n");
        return false;
    }

    uint32_t uplink_counter;
    return_size = fdb_kv_get_blob(&kvdb, "uplink_counter", fdb_blob_make(&blob, &uplink_counter, sizeof(uplink_counter)));
    if(return_size != sizeof(uplink_counter)){
        DEBUG("[configuration.c] loading uplink_counter error\n");
        return false;
    }
    semtech_loramac_set_uplink_counter(config->loramac, uplink_counter);


    
    return true;
}
