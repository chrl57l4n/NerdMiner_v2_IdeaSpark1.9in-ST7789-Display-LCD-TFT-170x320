#include "motoko_screen.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

static motoko_data sMotoko = {false, 0, 0.0, 0, "", 0, 0, 0, 0, ""};
static unsigned long sLastUpdate = 0;

void motoko_setup(void) {
    Serial.println("[motoko] setup");
    sMotoko.valid = false;
}

motoko_data motoko_getData(void) {
    return sMotoko;
}

void motoko_update(void) {
    unsigned long now = millis();
    // 60s polling — Erst-Call sofort, dann jeden UPDATE_Motoko_min
    if (sLastUpdate != 0 && (now - sLastUpdate) < (UPDATE_Motoko_min * 60UL * 1000UL)) {
        return;
    }
    sLastUpdate = now;

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[motoko] WiFi down, skip");
        return;
    }

    HTTPClient http;
    http.setTimeout(5000);
    if (!http.begin(MOTOKO_API_URL)) {
        Serial.println("[motoko] http.begin failed");
        return;
    }

    int code = http.GET();
    if (code != HTTP_CODE_OK) {
        Serial.printf("[motoko] HTTP %d\n", code);
        http.end();
        return;
    }

    String body = http.getString();
    http.end();

    StaticJsonDocument<512> doc;
    DeserializationError err = deserializeJson(doc, body);
    if (err) {
        Serial.printf("[motoko] JSON parse error: %s\n", err.c_str());
        return;
    }

    sMotoko.btc_usd = doc["btc_usd"] | 0;
    sMotoko.btc_change_24h = doc["btc_change_24h"] | 0.0f;
    sMotoko.fng_value = doc["fng_value"] | 0;
    sMotoko.fng_class = String((const char*)(doc["fng_class"] | ""));
    sMotoko.block_height = doc["block_height"] | 0;
    sMotoko.median_fee_sat_vb = doc["median_fee_sat_vb"] | 0;
    sMotoko.halving_days = doc["halving_days"] | 0;
    sMotoko.pi_lnbits_sats = doc["pi_lnbits_sats"] | 0;
    sMotoko.ts = String((const char*)(doc["ts"] | ""));
    sMotoko.valid = true;

    Serial.printf("[motoko] updated: $%u  fng=%d  block=%u\n",
                  sMotoko.btc_usd, sMotoko.fng_value, sMotoko.block_height);
}
