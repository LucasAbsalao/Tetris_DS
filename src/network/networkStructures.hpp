#pragma once
#include <cstdint>   
#include <enet/enet.h>

const int USERNAME_MAX_LENGTH = 31;

// Wrapper for ENet packet flags to distinguish between critical and optional data
enum PacketType : uint32_t {
    Reliable = ENET_PACKET_FLAG_RELIABLE, // TCP-like: Guaranteed delivery (Stats, Game Over, Chat)
    Unreliable = 0                        // UDP-like: Fast, fire-and-forget (Movement, Rotation)
};

// Protocol identifiers used as the first byte of every payload
// Allows the receiver to cast the raw bytes into the correct struct
enum MessageType : uint8_t {
    USERNAME = 0,
    GRID = 1,
    STATS = 2,
    BLOCK = 3,
    PLAYER_READY = 4,
    PLAYER_DISCONNECTED = 5,
    SET_ID = 6,
    GAME_OVER = 7,
    ATTACK = 8
};

// Forces the compiler to disable memory alignment padding.
// This ensures that sizeof(Struct) equals the exact sum of its members,
// allowing safe binary serialization/deserialization across different systems.
#pragma pack(push, 1)

// Internal wrapper used in the Client/Server Queues. It is used to store the raw data after reception.
struct GamePacket {
    MessageType type;
    std::vector<uint8_t> data;
};

// Payload sent during the handshake phase to identify a player
struct UsernamePacket {
    MessageType type = MessageType::USERNAME;
    uint8_t id;
    char username[USERNAME_MAX_LENGTH]; // Fixed buffer to ensure constant packet size
};

// Sent by the Server to the Client immediately after connection
// Assigns the  Network ID to the client
struct SetIdPacket {
    MessageType type = MessageType::SET_ID;
    uint8_t id;
};

// Sent when a player loses
struct GameOverPacket {
    MessageType type = MessageType::GAME_OVER;
    uint8_t id; // ID of the player who lost
};

// Lobby synchronization packet. Toggles the "Ready" state before the match begins
struct ReadyPacket {
    MessageType type = MessageType::PLAYER_READY;
    uint8_t id;
    bool ready;
};

// Sent when a player clears lines, instructing the opponent to add garbage lines
struct AttackPacket {
    MessageType type = MessageType::ATTACK;
    uint8_t id;    // Attacker ID
    uint8_t lines; // Number of lines to add to the opponent
};

// Notification packet when a peer drops connection
struct DisconnectPacket {
    MessageType type = MessageType::PLAYER_DISCONNECTED;
    uint8_t id;
};

// Real-time synchronization of the currently falling block
struct BlockPacket {
    MessageType type = MessageType::BLOCK;
    uint8_t id;
    uint8_t id_block; // Tetromino Type (I, O, T, etc.)
    uint8_t rotation; // Current rotation state (0-3)
    uint8_t x;        // Grid X position
    uint8_t y;        // Grid Y position
};

// Metadata synchronization for the HUD (Score and Level)
struct StatPacket {
    MessageType type = MessageType::STATS;
    uint8_t id;
    uint8_t level;
    uint32_t score; // 32-bit integer to support high scores
};

// Sent when a block locks into place. Heavy state synchronization representing the static matrix (pile)
struct GridPacket {
    MessageType type = MessageType::GRID;
    uint8_t id;
    uint8_t lines;   // Grid height
    uint8_t columns; // Grid width
    uint8_t grid[24][10]; // Flattened 2D array representing the board state
};

// Restores the default memory alignment settings
#pragma pack(pop)