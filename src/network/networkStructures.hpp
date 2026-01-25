#pragma once
#include <cstdint>   
#include <enet/enet.h> //TODO: Check if I could use heritage for structs

const int USERNAME_MAX_LENGTH = 31;

enum PacketType : uint32_t {
    Reliable = ENET_PACKET_FLAG_RELIABLE,
    Unreliable = 0
};

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

#pragma pack(push, 1)

struct GamePacket {
    MessageType type;
    std::vector<uint8_t> data;
};

struct UsernamePacket{
    MessageType type = MessageType::USERNAME;
    uint8_t id;
    char username[USERNAME_MAX_LENGTH];
};

struct SetIdPacket{
    MessageType type = MessageType::SET_ID;
    uint8_t id;
};

struct GameOverPacket{
    MessageType type = MessageType::GAME_OVER;
    uint8_t id;
};

struct ReadyPacket{
    MessageType type = MessageType::PLAYER_READY;
    uint8_t id;
    bool ready;
};

struct AttackPacket{
    MessageType type = MessageType::ATTACK;
    uint8_t id;
    uint8_t lines;
};

struct DisconnectPacket{
    MessageType type = MessageType::PLAYER_DISCONNECTED;
    uint8_t id;
};

struct BlockPacket{
    MessageType type = MessageType::BLOCK;
    uint8_t id;
    uint8_t id_block;
    uint8_t rotation;
    uint8_t x;
    uint8_t y;
};

struct StatPacket{
    MessageType type = MessageType::STATS;
    uint8_t id;
    uint8_t level;
    uint32_t score;
};

struct GridPacket{
    MessageType type = MessageType::GRID;
    uint8_t id;
    uint8_t lines;
    uint8_t columns;
    uint8_t grid[24][10];
};

#pragma pack(pop)