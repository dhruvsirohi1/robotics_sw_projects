#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <cstdint>

// ---------------------------
// Structures
// ---------------------------
struct Packet {
    uint16_t frame_id;             // 2 bytes
    uint16_t packet_id;            // 2 bytes
    uint8_t flags;                 // 1 byte: SOF/EOF bits
    uint16_t payload_size;         // 2 bytes
    std::vector<uint8_t> payload;  // N bytes
};

struct Frame {
    uint16_t frame_id;
    std::vector<uint8_t> data;
    bool complete = false;
};

// ---------------------------
// Helper Functions
// ---------------------------

// TODO: parse one packet from file
Packet parsePacket(std::ifstream &ifs) {
    Packet pkt{};
    // read header fields (frame_id, packet_id, flags, payload_size)
    if (!ifs.is_open()) return {};
    ifs.read(reinterpret_cast<char *>(&pkt.frame_id), sizeof(pkt.frame_id));
    ifs.read(reinterpret_cast<char *>(&pkt.packet_id), sizeof(pkt.packet_id));
    ifs.read(reinterpret_cast<char *>(&pkt.flags), sizeof(pkt.flags));
    ifs.read(reinterpret_cast<char *>(&pkt.frame_id), sizeof(pkt.frame_id));

    if (!ifs) throw std::runtime_error("Error reading packet: Unexpected EOF...");
    // resize payload vector
    (pkt.payload).resize(pkt.payload_size);
    // read payload into pkt.payload
    ifs.read(reinterpret_cast<char *>(&pkt.payload), sizeof(pkt.payload));

    if (!ifs) throw std::runtime_error("Error reading packet: Unexpected EOF...");

    return pkt;
}

// TODO: save a frame to disk
void saveFrame(const Frame &frame) {
    // open file (e.g., "frame_<id>.bin")
    // write frame.data
    // close file
    std::ofstream ofs;
    std::string filename = "frame_" + std::to_string(frame.frame_id) + ".bin";
    ofs.open(filename, std::ios::out | std::ios::binary);
    ofs.write(reinterpret_cast<const char *>(&frame.data[0]), frame.data.size());
}

// TODO: handle a packet and update corresponding frame
void handlePacket(const Packet &pkt, std::unordered_map<uint16_t, Frame> &frames) {
    // if SOF -> start a new frame
    Frame &frame = frames[pkt.frame_id];
    frame.frame_id = pkt.packet_id;

    if (pkt.flags & 0x1) { // SoF
        frame.data.clear();
        frame.complete = false;
    }
    // append payload to the frame’s data
    frame.data.insert(frame.data.end(), frame.data.begin(), frame.data.end());
    // if EOF -> mark frame complete and save to disk
    if (pkt.flags & 0x2) {
        frame.complete = true;
        saveFrame(frame);
    }
}

// ---------------------------
// Main
// ---------------------------
int main() {
    std::ifstream infile("packets.bin", std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "Failed to open packets.bin" << std::endl;
        return 1;
    }

    std::unordered_map<uint16_t, Frame> frames;

    // loop through file until EOF
    while (infile.peek() != EOF) {
        Packet pkt = parsePacket(infile);
        handlePacket(pkt, frames);
    }

    infile.close();
    return 0;
}
