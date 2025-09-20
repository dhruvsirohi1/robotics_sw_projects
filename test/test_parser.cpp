//
// Created by Dhruv Sirohi on 9/20/25.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <unordered_map>

#include <gtest/gtest.h>

#include "packet_parser/PacketParser.cpp"

// ---------------------------
// Test Utilities
// ---------------------------

// Helper to write a dummy packets.bin
void createDummyPackets() {
    std::ofstream ofs("./packets.bin", std::ios::binary);

    // Create one frame split into 2 packets
    // Payload = "HELLO_WORLD" (11 bytes)
    std::string payload = "HELLO_FUNNY_WORLD";

    // Packet 1: SOF, frame_id=1, packet_id=0
    uint16_t frame_id = 1;
    uint16_t packet_id = 0;
    uint8_t flags = 0x1; // SOF
    uint16_t chunk1_size = 5;
    ofs.write(reinterpret_cast<char *>(&frame_id), sizeof(frame_id));
    ofs.write(reinterpret_cast<char *>(&packet_id), sizeof(packet_id));
    ofs.write(reinterpret_cast<char *>(&flags), sizeof(flags));
    ofs.write(reinterpret_cast<char *>(&chunk1_size), sizeof(chunk1_size));
    ofs.write(payload.data(), chunk1_size);

    // Packet 2: EOF, frame_id=1, packet_id=1
    packet_id = 1;
    flags = 0x0; // EOF
    uint16_t chunk2_size = 6;
    ofs.write(reinterpret_cast<char *>(&frame_id), sizeof(frame_id));
    ofs.write(reinterpret_cast<char *>(&packet_id), sizeof(packet_id));
    ofs.write(reinterpret_cast<char *>(&flags), sizeof(flags));
    ofs.write(reinterpret_cast<char *>(&chunk2_size), sizeof(chunk2_size));
    ofs.write(payload.data() + chunk1_size, chunk2_size);

    // Packet 3: EOF, frame_id=1, packet_id=1
    packet_id = 2;
    flags = 0x2; // EOF
    uint16_t chunk3_size = payload.size() - chunk1_size - chunk2_size;
    ofs.write(reinterpret_cast<char *>(&frame_id), sizeof(frame_id));
    ofs.write(reinterpret_cast<char *>(&packet_id), sizeof(packet_id));
    ofs.write(reinterpret_cast<char *>(&flags), sizeof(flags));
    ofs.write(reinterpret_cast<char *>(&chunk3_size), sizeof(chunk3_size));
    ofs.write(payload.data() + chunk1_size + chunk2_size, chunk3_size);

    ofs.close();
}

TEST(ParserTest, FrameReassembly) {
    // Step 1: Create dummy packets.bin
    createDummyPackets();

    // Step 2: Run parser
    std::ifstream infile("packets.bin", std::ios::binary);
    std::unordered_map<uint16_t, Frame> frames;

    while (infile.peek() != EOF) {
        Packet pkt = parsePacket(infile);
        handlePacket(pkt, frames);
    }
    infile.close();

    // Step 3: Verify output frame
    Frame &f = frames[1];
    ASSERT_TRUE(f.complete);
    std::string reconstructed(f.data.begin(), f.data.end());
    EXPECT_EQ(reconstructed, "HELLO_FUNNY_WORLD");
}

