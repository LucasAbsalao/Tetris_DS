#include"gameMultiplayer.hpp"
#include"gameRemote.hpp"
#include"networkManager.hpp"
#include"network/networkStructures.hpp"

class MatchManager {
    std::shared_ptr<networkManager> net;
    GameMultiplayer player1;
    GameRemote *player2;

public:
    MatchManager(int normalSpeed, int fast_speed, Texture2D background, const std::string& address, int port_host);
    ~MatchManager();
    void createRemoteGame(UsernamePacket packet);
    void draw();
    void update();
};