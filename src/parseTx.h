#pragma once

#include "constants.h"
#include "signTxHash.h"

typedef struct {
    char receiver[FULL_ADDRESS_LENGTH];
    char amount[MAX_AMOUNT_LEN + PRETTY_SIZE];
    uint64_t gas_limit;
    uint64_t gas_price;
    char fee[MAX_AMOUNT_LEN + PRETTY_SIZE];
    char data[MAX_DISPLAY_DATA_SIZE + DATA_SIZE_LEN];
    uint32_t data_size;
    uint8_t signature[64];
} tx_context_t;

extern tx_context_t tx_context;
extern tx_hash_context_t tx_hash_context;

uint16_t parse_data(const uint8_t *dataBuffer, uint16_t dataLength);
