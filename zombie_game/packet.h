#ifndef PACKET_H
#define PACKET_H

#include <SFML/Network.hpp>
#include <iostream>

#include "weapons_enum.h"
#include "player_state_enum.h"
#include "feet_enum.h"
#include "packet_type.h"


class packet {

public:

    packet_type type;
    float x;
    float y;
    float da;
    int hp;
    sf::IpAddress player_ip;

    player_state current_player_state;
    weapons current_equipped_weapon;
    feet current_feet_type;

    packet();
    packet(packet_type type, sf::IpAddress player_ip, float x, float y, float da, int hp, player_state current_player_state, weapons current_equipped_weapon, feet current_feet_type);

    static bool setup_udp_socket(sf::UdpSocket& socket);

    bool send_packet(sf::UdpSocket& socket, sf::IpAddress address);

    bool receive_packet(sf::UdpSocket& socket, sf::IpAddress& sender_ip);

    void operator <<(sf::Packet& packet);

    void operator >>(sf::Packet& packet);

    std::string to_string();

};


#endif // PACKET_H
