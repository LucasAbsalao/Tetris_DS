#include"gameMultiplayer.hpp"
#include"gameRemote.hpp"
#include"networkManager.hpp"
#include"network/networkStructures.hpp"

class MatchManager {
    private:
        int savedNormalSpeed;
        int savedFastSpeed;
        Texture2D savedBackground;

        std::shared_ptr<NetworkManager> net;
        GameMultiplayer player1;
        GameRemote *player2;

    public:
        MatchManager(int normalSpeed, int fast_speed, Texture2D background, const std::string& address, int port_host);
        ~MatchManager();
        void init();
        void createRemoteGame(UsernamePacket packet);
        void setLocalPlayerName(const std::string& name);
        void draw();
        void run();
};