#include <Arduino.h>
#include <MTSimpleBroadcast.hpp>

// Heltec LoRa 32 V3 SPI Pins
// Not actually needed, already defined by SPI library
#define PIN_SCK 9
#define PIN_MOSI 10
#define PIN_MISO 11
#define PIN_CS 8

// LoRa
#define PIN_IRQ 14 // DIO1
#define PIN_RST 12
#define PIN_BUSY 13 // Sometimes called GPIO or DIO2

// MediumFast Meshtastic Config
LoraConfig lora_config_mt = {
    /*.frequency = */ 913.125,  // config
    /*.bandwidth = */ 250,      // config
    /*.spreading_factor = */ 9, // config
    /*.coding_rate = */ 5,      // config
    /*.sync_word = */ 0x2B,
    /*.preamble_length = */ 16,
    /*.output_power = */ 22, // config
    /*.tcxo_voltage = */ 1.8,
    /*.use_regulator_ldo = */ false,
}; //

MTSimpleBroadcast mtsb;

void setup() {
    // Initialize SPI
    SPI.begin();

    // Make sure that this pin is unconnected and floating
    randomSeed(analogRead(A0));

    // Initialize Radio
    mtsb.RadioInit(SPI, PIN_CS, PIN_IRQ, PIN_RST, PIN_BUSY, lora_config_mt);
    mtsb.setSendHopLimit(2);

    // Configure Node Info
    uint32_t node_id = 0xABBABAAB;
    std::string short_name = "hv3t";
    std::string long_name = "heltecV3_test";
    uint8_t hardware_model = 44;
    MTHelpers::NodeInfoBuilder(mtsb.getMyNodeInfo(), node_id, short_name, long_name, hardware_model);

    // Define Channel
    MTSB_ChannelEntry channel("MediumFast", "AQ==");

    // Broadcast Node Info
    mtsb.broadcastMyNodeInfo(channel);
    delay(500);

    // Broadcast Text Message
    std::string test_msg = "Hello World!";
    mtsb.broadcastTextMessage(test_msg, channel);
}

void loop() {}
