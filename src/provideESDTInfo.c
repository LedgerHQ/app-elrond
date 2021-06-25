#include "provideESDTInfo.h"
#include "constants.h"
#include <string.h>
#include <ux.h>
#include <lcx_ecdsa.h>
#include <lcx_sha256.h>
#include "globals.h"

esdt_info_t esdt_info;

uint16_t handleProvideESDTInfo(uint8_t *dataBuffer, uint16_t dataLength) {
    uint8_t last_required_len = 0;
    uint8_t required_len = 1;
    uint8_t hash[32];
    cx_sha256_t sha256;
    cx_ecfp_public_key_t tokenKey;

    // read ticker len
    if (dataLength < required_len) {
        return ERR_MESSAGE_INCOMPLETE;
    }
    esdt_info.ticker_len = dataBuffer[last_required_len];
    
    // read ticker
    last_required_len = required_len;
    required_len += esdt_info.ticker_len;
    if (dataLength < required_len) {
        return ERR_MESSAGE_INCOMPLETE;
    }
    memmove(esdt_info.ticker, dataBuffer + last_required_len, esdt_info.ticker_len);
    esdt_info.ticker[esdt_info.ticker_len] = '\0';
    
    // read identifier len
    last_required_len = required_len;
    required_len++;
    if (dataLength < required_len) {
        return ERR_MESSAGE_INCOMPLETE;
    }
    esdt_info.identifier_len = dataBuffer[last_required_len];

    // read identifier
    last_required_len = required_len;
    required_len += esdt_info.identifier_len;
    if (dataLength < required_len) {
        return ERR_MESSAGE_INCOMPLETE;
    }
    memmove(esdt_info.identifier, dataBuffer + last_required_len, esdt_info.identifier_len);
    esdt_info.identifier[esdt_info.identifier_len] = '\0';

    // read decimals
    last_required_len = required_len;
    required_len++;
    if (dataLength < required_len) {
        return ERR_MESSAGE_INCOMPLETE;
    }
    esdt_info.decimals = dataBuffer[last_required_len];

    // read chain id len
    last_required_len = required_len;
    required_len++;
    if (dataLength < required_len) {
        return ERR_MESSAGE_INCOMPLETE;
    }
    esdt_info.chain_id_len = dataBuffer[last_required_len];

    // read chain id
    last_required_len = required_len;
    required_len += esdt_info.chain_id_len;
    if (dataLength < required_len) {
        return ERR_MESSAGE_INCOMPLETE;
    }
    memmove(esdt_info.chain_id, dataBuffer + last_required_len, esdt_info.chain_id_len);
    esdt_info.chain_id[esdt_info.chain_id_len] = '\0';

    // compute hash and verify signature
    if (dataLength < required_len + ESDT_SIGNATURE_LEN) {
        return ERR_MESSAGE_INCOMPLETE;
    }

    cx_sha256_init(&sha256);
    cx_hash((cx_hash_t *) &sha256, CX_LAST, dataBuffer, required_len, hash, 32);

    cx_ecfp_init_public_key(CX_CURVE_256K1,
                            LEDGER_SIGNATURE_PUBLIC_KEY,
                            sizeof(LEDGER_SIGNATURE_PUBLIC_KEY),
                            &tokenKey);

    if (!cx_ecdsa_verify(&tokenKey,
                         CX_LAST,
                         CX_SHA256,
                         hash,
                         32,
                         dataBuffer + required_len,
                         ESDT_SIGNATURE_LEN)) {
        return ERR_INVALID_ESDT_SIGNATURE;
    }

    return MSG_OK;
}
