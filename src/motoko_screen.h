#ifndef MOTOKO_SCREEN_H
#define MOTOKO_SCREEN_H

#include <Arduino.h>

// Default API-URL — override via build_flags -DMOTOKO_API_URL='"http://..."'
#ifndef MOTOKO_API_URL
#define MOTOKO_API_URL "http://192.168.178.86:7000/api/btc-stats.json"
#endif

#define UPDATE_Motoko_min 1  // 60s polling

typedef struct {
    bool valid;
    unsigned int btc_usd;
    float btc_change_24h;
    int fng_value;
    String fng_class;
    unsigned int block_height;
    unsigned int median_fee_sat_vb;
    unsigned int halving_days;
    unsigned int pi_lnbits_sats;
    String ts;
} motoko_data;

void motoko_setup(void);
void motoko_update(void);
motoko_data motoko_getData(void);

// Per-driver screen functions — implementieren in <chip>DisplayDriver.cpp
void tDisplay_MotokoScreen(unsigned long mElapsed);
void tDisplayV1_MotokoScreen(unsigned long mElapsed);
// Bei Bedarf weitere: ttgo_MotokoScreen, amoled_MotokoScreen, etc.

#endif
