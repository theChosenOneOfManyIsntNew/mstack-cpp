#pragma once
#include "layer.hpp"
#include "tuntap.hpp"
#include "ethernet.hpp"
#include <array>
#include <iomanip>
#include <optional>
#include <unordered_map>
#include "arp.hpp"

namespace mstack {

class l2_hook : public base_hook_funcs<l2_packet> {
public:
    arp_hook_t arp_hook;
    template<typename DEV>
    void hook_register_dev(DEV& dev) {
        arp_hook.add(dev);
        return;
    }
    
    virtual void
    hook_register_protocol(protocol_interface<l2_packet>& protocol)
    {
        DLOG(INFO) << "[REGISTER] " << protocol.tag();
        return;
    }

    virtual void
    hook_dispatch(std::optional<l2_packet>& packet)
    {
        return;
    }

    virtual std::optional<raw_packet>
    hook_gather(std::optional<l2_packet> packet)
    {
        DLOG(INFO) << packet.value();
        if(packet->_proto == arp::PROTO){
            return std::move(arp_hook.arp_filter(packet));
        }
        return std::move(std::nullopt);
    }
};


using l2_layer = mstack::layer<raw_packet, l2_packet, l2_hook>;

template<>
std::optional<l2_packet> make_packet(raw_packet in_packet) {
    DLOG(INFO) << "[PACKET: RAW -> L2]";

    uint8_t* ptr = in_packet._payload->get_pointer();

    ethernet_header_t eth_hdr;

    eth_hdr.consume(ptr);

    DLOG(INFO) << "[ETHERNET PACKET] " << eth_hdr;

    in_packet._payload->add_offset(ethernet_header_t::size());

    return std::make_optional<l2_packet>(std::nullopt, eth_hdr.ethernet_type, std::move(in_packet._payload));
}

template <>
std::optional<int> get_proto(std::optional<l2_packet>& packet)
{
    // DLOG(INFO) << "[PROTO L2_PACKET] " << std::setiosflags(std::ios::uppercase) << std::hex << packet->_proto;
    return packet->_proto;
}

};
