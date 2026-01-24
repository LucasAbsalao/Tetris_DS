#include<string>
class ClientData{
    private:
        int m_id;
        std::string m_username;

    public:
        ClientData(int id);
        void setUsername(std::string username);
        int getId();
        std::string getUsername();

};