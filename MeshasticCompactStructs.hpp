#ifndef MeshtasticCompactStructs_h
#define MeshtasticCompactStructs_h

#include <stdint.h>
#include "meshtastic/mesh.pb.h"

enum class RadioType {
    SX1262,
    SX1261,
    SX1268,
    SX1276
};

struct Radio_PINS {
    uint8_t sck;
    uint8_t miso;
    uint8_t mosi;
    uint8_t cs;
    uint8_t irq;
    uint8_t rst;
    uint8_t gpio;
};

struct LoraConfig {
    float frequency;           // Frequency in MHz
    float bandwidth;           // Bandwidth in kHz
    uint8_t spreading_factor;  // Spreading factor (7-12)
    uint8_t coding_rate;       // Coding rate denominator (5-8)
    uint8_t sync_word;         // Sync word (0x12 for private, 0x34 for public)
    uint16_t preamble_length;  // Preamble length in symbols
    int8_t output_power;       // Output power in dBm
    float tcxo_voltage;        // TCXO voltage in volts
    bool use_regulator_ldo;    // Use LDO regulator (true) or DC-DC regulator (false)
};

typedef enum {
    MC_MESSAGE_TYPE_TEXT = 0,             // Normal text message
    MC_MESSAGE_TYPE_ALERT = 1,            // Alert message
    MC_MESSAGE_TYPE_DETECTOR_SENSOR = 2,  // Detector sensor message
    MC_MESSAGE_TYPE_PING = 3,             // Ping message
    MC_MESSAGE_TYPE_UART = 4,             // UART message
    MC_MESSAGE_TYPE_RANGE_TEST = 5,       // Range test message
} MC_MESSAGE_TYPE;

struct MC_Header {
    uint32_t srcnode;  // source node ID
    uint32_t dstnode;  // destination node ID
    uint32_t packet_id;
    uint8_t hop_limit;
    uint8_t hop_start;
    uint8_t chan_hash;
    bool want_ack;  // true if the sender wants an acknowledgment
    bool via_mqtt;  // true if the packet is sent via MQTT
    float rssi;
    float snr;
    uint32_t request_id;
    uint32_t reply_id;
    uint16_t freq;
};

struct MC_OutQueueEntry {
    MC_Header header;
    meshtastic_Data data;
    uint8_t encType = 0;  // 0 = auto, 1 = aes, 2 = key
    uint8_t* key;
    size_t key_len;  // Length of the key in bytes
};

struct MC_TextMessage {
    std::string text;
    uint8_t chan;
    MC_MESSAGE_TYPE type;
};

struct MC_Position {
    int32_t latitude_i;   // Latitude in degrees
    int32_t longitude_i;  // Longitude in degrees
    int32_t altitude;     // Altitude in meters
    uint32_t ground_speed;
    uint32_t sats_in_view;    // Number of satellites in view
    uint8_t location_source;  // Source of the location data (e.g., GPS, network)
    bool has_latitude_i;
    bool has_longitude_i;
    bool has_altitude;
    bool has_ground_speed;
};

struct MC_NodeInfo {
    uint32_t node_id;         // src
    char id[16];              // Node ID
    char short_name[5];       // Short name of the node
    char long_name[40];       // Long name of the node
    uint8_t hw_model;         // Hardware model
    uint8_t macaddr[6];       // MAC address (not used in this struct)
    uint8_t public_key[32];   // Public key (not used in this struct)
    uint8_t public_key_size;  // Size of the public key
    uint8_t role;             // Role of the node
    uint32_t last_updated;    // Last updated timestamp
};

struct MC_Waypoint {
    uint32_t id;            // Waypoint ID
    char name[30];          // Name of the waypoint
    char description[100];  // Description of the waypoint
    int32_t latitude_i;     // Latitude in degrees
    int32_t longitude_i;    // Longitude in degrees
    uint32_t icon;          // Icon representing the waypoint
    uint32_t expire;        // Expiration time of the waypoint in unix timestamp format
    bool has_latitude_i;
    bool has_longitude_i;
};

struct MC_Telemetry_Device {
    uint32_t battery_level;
    uint32_t uptime_seconds;
    float voltage;
    float channel_utilization;
    bool has_battery_level;
    bool has_uptime_seconds;
    bool has_voltage;
    bool has_channel_utilization;
};

struct MC_Telemetry_Environment {
    float temperature;
    float humidity;
    float pressure;
    float lux;
    bool has_temperature;
    bool has_humidity;
    bool has_pressure;
    bool has_lux;
};

struct MC_RouteDiscovery {
    /* The list of nodenums this packet has visited so far to the destination. */
    pb_size_t route_count;
    uint32_t route[8];
    /* The list of SNRs (in dB, scaled by 4) in the route towards the destination. */
    pb_size_t snr_towards_count;
    int8_t snr_towards[8];
    /* The list of nodenums the packet has visited on the way back from the destination. */
    pb_size_t route_back_count;
    uint32_t route_back[8];
    /* The list of SNRs (in dB, scaled by 4) in the route back from the destination. */
    pb_size_t snr_back_count;
    int8_t snr_back[8];
};

#endif  // MeshtasticCompactStructs_h