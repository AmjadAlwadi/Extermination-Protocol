#include "packet.h"
#include "constants.h"


packet::packet()
{
    this->type = packet_type::NOTHING;
    x = 0;
    y = 0;
    da = 0;
    hp = 0;
    current_player_state = player_state::idle;
    current_equipped_weapon = weapons::knife;
    current_feet_type = feet::idle;
    player_ip = sf::IpAddress();
}



packet::packet(packet_type type,sf::IpAddress player_ip, float x, float y, float da, int hp, player_state current_player_state, weapons current_equipped_weapon, feet current_feet_type) : type(type), player_ip(player_ip), x(x), y(y), da(da), hp(hp),
current_equipped_weapon(current_equipped_weapon), current_feet_type(current_feet_type), current_player_state(current_player_state) {}





// setup as server
bool packet::setup_udp_socket(sf::UdpSocket& socket)
{
    if (socket.bind(PORT) != sf::Socket::Done) {
        // Handle bind error (e.g., port already in use)
        std::cerr << "socket failed" << std::endl;
        return false;
    }

    return true;
}




bool packet::send_packet(sf::UdpSocket& socket, sf::IpAddress address)
{

    sf::Packet packet;
    *this >> packet;

    if (socket.send(packet,address,PORT) != sf::Socket::Done) {
        // Handle send error (e.g., network issue)
        std::cerr << "could not send data " << this->to_string() << std::endl;
        return false;
    }

    //std::cout << "sent data successfully to " << address.toString() << " Payload : " << this->to_string() << std::endl;
    return true;

}


bool packet::receive_packet(sf::UdpSocket& socket, sf::IpAddress& sender_ip)
{

    sf::Packet packet;
    unsigned short sender_port;

    sf::Socket::Status status = socket.receive(packet, sender_ip, sender_port);


    if (status == sf::Socket::Done) {
        *this << packet; // Unpack data from the packet
        //std::cout << "received successfully from " << sender_ip << " payload : " << this->to_string() << std::endl;
        return true;
    } else if (status == sf::Socket::NotReady) {
        // No data received yet, may need to check again later
        //std::cout << "could not receive from : " << sender_ip << std::endl;
        return false;
    } else {
        // Handle other receive errors
        std::cerr << "error receiving" << std::endl;
        return false;
    }

}






void packet::operator >>(sf::Packet& packet)
{
    packet << static_cast<uint8_t>(type) << this->player_ip.toInteger() << this->x << this->y << this->da << this->hp << static_cast<uint8_t>((this->current_player_state)) << static_cast<uint8_t>(this->current_equipped_weapon) << static_cast<uint8_t>(this->current_feet_type);
}



void packet::operator <<(sf::Packet& packet)
{
    uint8_t type_int;

    packet >> type_int;

    this->type = static_cast<packet_type>(type_int);

    sf::Uint32 player_ip_int;

    packet >> player_ip_int;

    this->player_ip = sf::IpAddress(player_ip_int);

    packet >> this->x >> this->y >> this->da >> this->hp;

    uint8_t t1, t2, t3;

    packet >> t1 >> t2 >> t3;

    this->current_player_state = static_cast<player_state>(t1);
    this->current_equipped_weapon = static_cast<weapons>(t2);
    this->current_feet_type = static_cast<feet>(t3);

}




std::string packet::to_string()
{
    return std::string(std::to_string(static_cast<uint8_t>(type)) + ", " + player_ip.toString() + ", " + std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->da) + ", " + std::to_string(this->hp) + ", " +
        std::to_string(static_cast<uint8_t>((this->current_player_state))) + ", " + std::to_string(static_cast<uint8_t>(this->current_equipped_weapon))
        + ", " + std::to_string(static_cast<uint8_t>(this->current_feet_type)));
}


