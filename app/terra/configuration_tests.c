#include "configuration_tests.h"
#include "configuration.h"
#include "periph/eeprom.h"

#ifdef APPLICATIOn_RUN_TEST
// useful functions stolen from semtech_loramac.c
static size_t _read_uint32(size_t pos, uint32_t *value)
{
    uint8_t array[4] = { 0 };
    size_t ret = eeprom_read(pos, array, sizeof(uint32_t));
    *value = ((uint32_t)array[0] << 24 | (uint32_t)array[1] << 16 |
              (uint32_t)array[2] << 8 | array[3]);
    return ret;
}

static void tests_SetUp(void)
{
}

static void tests_TearDown(void)
{
    eeprom_erase();
}

static void tests_save(void)
{
    //arrange
    // create message containing 1 query with 1 operation with 2 instructions
    // map that just adds 2 to the stack. 
    TerraProtocol_Message query = TerraProtocol_Message_init_zero;
    query.queries_count = 1;
    query.queries[0].operations_count = 1;
    query.queries[0].operations[0].which_operation = TerraProtocol_Operation_map_tag;
    query.queries[0].operations[0].operation.map.attribute = 1;
    query.queries[0].operations[0].operation.map.function.instructions_count = 2;
    query.queries[0].operations[0].operation.map.function.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
    query.queries[0].operations[0].operation.map.function.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 2};

    semtech_loramac_t loramac = {
        .port = 1,
        .cnf = false,
        .deveui =  {1,2,3,4,5,6,7,8},
        .appeui = {8,7,6,5,4,3,2,1},
        .appkey = {8,7,6,5,4,3,2,1,1,2,3,4,5,6,7,8}, 
    };

    TerraConfiguration config = {
        .loop_counter = 3,
        .query = &query,
        .loramac = &loramac
    };

    //Act
    configuration_save(&config);

    //assert

    char magic[CONFIGURATION_MAGIC_SIZE+1] = { 0 };
    char magic_result[CONFIGURATION_MAGIC_SIZE+1] = "Terra";
    eeprom_read(CONFIGURATION_EEPROM_START, magic, CONFIGURATION_MAGIC_SIZE);
    TEST_ASSERT_EQUAL_STRING(magic_result, magic);
    TerraProtocol_Message read_query = TerraProtocol_Message_init_zero;
    uint32_t loop_counter_read = _read_uint32(CONFIGURATION_EEPROM_START + CONFIGURATION_MAGIC_SIZE, &loop_counter_read);
    eeprom_read(CONFIGURATION_EEPROM_START + CONFIGURATION_MAGIC_SIZE + CONFIGURATION_LOOP_COUNTER_SIZE, &read_query, CONFIGURATION_QUERY_MAX_SIZE);
    TEST_ASSERT_EQUAL_INT(1                                         , read_query.queries_count);
    TEST_ASSERT_EQUAL_INT(1                                         , read_query.queries[0].operations_count);
    TEST_ASSERT_EQUAL_INT(TerraProtocol_Operation_map_tag           , read_query.queries[0].operations[0].which_operation);
    TEST_ASSERT_EQUAL_INT(1                                         , read_query.queries[0].operations[0].operation.map.attribute);
    TEST_ASSERT_EQUAL_INT(TerraProtocol_Expression_instructions_tag , read_query.queries[0].operations[0].operation.map.function.instructions[0].which_data);
    TEST_ASSERT_EQUAL_INT(TerraProtocol_CONST                       , read_query.queries[0].operations[0].operation.map.function.instructions[0].data.instruction);
    TEST_ASSERT_EQUAL_INT(TerraProtocol_Data__int16_tag             , read_query.queries[0].operations[0].operation.map.function.instructions[1].which_data);
    TEST_ASSERT_EQUAL_INT(2                                         , read_query.queries[0].operations[0].operation.map.function.instructions[1].data._int16);

}

static void tests_load(void)
{
    //arrange
    // create message containing 1 query with 1 operation with 2 instructions
    // map that just adds 2 to the stack. 
    TerraProtocol_Message query = TerraProtocol_Message_init_zero;
    query.queries_count = 1;
    query.queries[0].operations_count = 1;
    query.queries[0].operations[0].which_operation = TerraProtocol_Operation_map_tag;
    query.queries[0].operations[0].operation.map.attribute = 1;
    query.queries[0].operations[0].operation.map.function.instructions_count = 2;
    query.queries[0].operations[0].operation.map.function.instructions[0] = (TerraProtocol_Data) {TerraProtocol_Data_instruction_tag, {TerraProtocol_CONST}};
    query.queries[0].operations[0].operation.map.function.instructions[1] = (TerraProtocol_Data) {.which_data = TerraProtocol_Data__int16_tag, .data._int16 = 2};

    semtech_loramac_t loramac = {
        .port = 1,
        .cnf = false,
        .deveui =  {1,2,3,4,5,6,7,8},
        .appeui = {8,7,6,5,4,3,2,1},
        .appkey = {8,7,6,5,4,3,2,1,1,2,3,4,5,6,7,8}, 
    };

    TerraConfiguration config = {
        .loop_counter = 3,
        .query = &query,
        .loramac = &loramac
    };

    //save it in eeprom
    configuration_save(&config);

    //act
    TerraProtocol_Message query_result = TerraProtocol_Message_init_zero;
    semtech_loramac_t loramac_result = {
        .port = 0,
        .cnf = false,
        .deveui =  {0,0,0,0,0,0,0,0},
        .appeui = {0,0,0,0,0,0,0,0},
        .appkey = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
    };
    TerraConfiguration config_result = {
        .loop_counter = 0,
        .query = &query_result,
        .loramac = &loramac_result
    };
    bool res = configuration_load(&config_result);
    
    //assert
    TEST_ASSERT(res);
    TEST_ASSERT_EQUAL_INT(1, config_result.query->queries_count);
    TEST_ASSERT_EQUAL_INT(1, config_result.query->queries[0].operations_count);
    TEST_ASSERT_EQUAL_INT(TerraProtocol_Operation_map_tag, config_result.query->queries[0].operations[0].which_operation);
    TEST_ASSERT_EQUAL_INT(1, config_result.query->queries[0].operations[0].operation.map.attribute);
    TEST_ASSERT_EQUAL_INT(TerraProtocol_Expression_instructions_tag, config_result.query->queries[0].operations[0].operation.map.function.instructions[0].which_data);
    TEST_ASSERT_EQUAL_INT(TerraProtocol_CONST, config_result.query->queries[0].operations[0].operation.map.function.instructions[0].data.instruction);
    TEST_ASSERT_EQUAL_INT(TerraProtocol_Data__int16_tag, config_result.query->queries[0].operations[0].operation.map.function.instructions[1].which_data);
    TEST_ASSERT_EQUAL_INT(2, config_result.query->queries[0].operations[0].operation.map.function.instructions[1].data._int16);

}

TestRef tests_configuration(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) {
        new_TestFixture(tests_save),
        new_TestFixture(tests_load)
    };
    EMB_UNIT_TESTCALLER(configuration_tests, &tests_SetUp, &tests_TearDown, fixtures);
    return (TestRef) &configuration_tests;
}
#endif
