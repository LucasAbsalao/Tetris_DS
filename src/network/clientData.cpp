#include"clientData.hpp"
ClientData::ClientData(int id): m_id(id) {}

void ClientData::setUsername(std::string username) {m_username = username;}

int ClientData::getId(){return m_id;}
std::string ClientData::getUsername(){return m_username;}
