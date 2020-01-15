/*
 * Class manages the allocation and deallocation of the page frames.
 * The set of available page frames should be maintained as a bit map. The bit map:

 * Must allow allocation of up to 256 (0x100) page frames. Bits must be packed 8 per byte, so this will require 32 (0x20) bytes.
 * 
 * Must be kept in the simulated system memory vector along with the page frames being allocated. 
 * Keep the bit map in the first page frame (0), which will be reserved for the memory allocator.
 * 
 * Each page frame number n is represented in the byte in the bit map at offset n/8. 
 * Within that byte, the page frame is represented by bit n%8 (% is the modulus operator), where bit 0 is the least significant bit. 
 * A 0 bit indicates that the corresponding page frame has been allocated. A 1 bit indicates that the corresponding page frame is available.
 * 
 */

/* 
 * File:   BitMapAllocator.cpp
 * Author: kt & om
 * 
 * Created on April 18, 2019, 11:14 AM
 */

//Page Frame Size (256 bytes)

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "BitMapAllocator.h"

using namespace std;
static int frameSize = 0x400;

BitMapAllocator::BitMapAllocator(uint32_t nPages) {
    //Max page frames allowed
    if (nPages <= 256) {
        memory.resize(nPages*frameSize, (uint8_t) 0x0);
        for (int i = 1; i < nPages; i++) {
            memory[i/8] |= 0x1 << i % 8;
        }
    }
}

bool BitMapAllocator::GetFrames(uint32_t count, vector<uint32_t> &page_frames) {
    if (count > get_free_count()) {
        return false;
    }
    //first bit in memory that is available
    uint32_t bit = first_free_bit();
    for (int i = bit; i < count+bit; i++) {
        uint8_t mask = (memory[(i / 8)] >> (i) % 8) & 0x1;
        //Handles case for non-consecutive available memory
        while (mask != 0x1) {
            i++;
            bit++;
            mask = (memory[(i / 8)] >> (i) % 8) & 0x1;
        }
        if (mask == 0x1) {
            memory[i / 8] ^= (mask << i % 8);
            page_frames.push_back(i * frameSize);
        }
    }
    return true;
}

bool BitMapAllocator::FreeFrames(uint32_t count, vector<uint32_t> &page_frames) {
    if (count > page_frames.size()) {
        return false;
    }
    for (int i = 0; i < count; i++) {
        //Finds where the specific page frame is located inside of the bitmap
        uint32_t bitmapLocal = page_frames[page_frames.size() - 1] / frameSize;
        uint8_t mask = 0x1 << (bitmapLocal % 8);
        memory[bitmapLocal / 8] |= mask;
        page_frames.pop_back();
    }
    return true;
}

uint32_t BitMapAllocator::get_free_count() {
    uint32_t totalFrames = (memory.size() / frameSize);
    uint32_t count = 0;
    for (int i = 1; i < totalFrames; i++) {
        uint8_t mask = (memory[i / 8] >> i % 8) & 0x1;
        if (mask == 0x1) {
            count++;
        }
    }
    return count;
}

string BitMapAllocator::get_bit_map_string() {
    stringstream ss;
    for (int i = 0; i < 0x20; i++) {
        ss << setfill('0') << setw(2) << hex << (int) memory[i] << " ";
    }
    return ss.str();
}

uint32_t BitMapAllocator::first_free_bit() {
    uint32_t totalFrames = (memory.size() / frameSize);
    uint32_t bit = 0;
    //Run through all frames
    for (int i = 1; i < totalFrames; i++) {
        uint8_t mask = ((memory[i / 8] >> i % 8) | 0x1);
        //bit is marked as available
        if (mask == (memory[i / 8] >> i % 8)) {
            //free bit 
            if (bit < i) {
                bit = i;
                i = totalFrames;
            }
        }
    }
    return bit;
}
