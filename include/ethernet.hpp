#include "utils.hpp"
#include <algorithm>
#include <array>
#include <string>
#include <utility>

namespace mstack {
struct mac_addr {
private:
    std::array<uint8_t, 6> mac;

public:
    mac_addr() = default;
    ~mac_addr() = default;
    mac_addr(const mac_addr& other)
    {
        std::copy(std::begin(other.mac), std::end(other.mac), std::begin(mac));
    }
    mac_addr(mac_addr&& other)
    {
        std::swap(mac, other.mac);
    }
    mac_addr& operator=(const mac_addr& other)
    {
        if (this != &other) {
            std::copy(std::begin(other.mac), std::end(other.mac), std::begin(mac));
        }
    }

    mac_addr& operator=(mac_addr&& other)
    {
        std::swap(mac, other.mac);
    }

    mac_addr(std::array<uint8_t, 6> mac_addr)
        : mac(mac_addr){};

    mac_addr(std::string mac_addr)
    {
        for (int i = 0; i < 17; i += 3) {
            std::string tmp = mac_addr.substr(i, 2);
            uint8_t tmp_int = std::stoi(tmp, 0, 16);
            mac[i / 3] = tmp_int;
        }
    };

    friend std::ostream& operator<<(std::ostream& out, mac_addr& m)
    {
        using u = uint32_t;
        out << util::format("%02X:%02X:%02X:%02X:%02X:%02X",
            u(m.mac[0]), u(m.mac[1]), u(m.mac[2]), u(m.mac[3]), u(m.mac[4]), u(m.mac[5]));
        return out;
    }
};
};