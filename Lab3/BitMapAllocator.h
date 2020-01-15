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
 * File:   BitMapAllocator.h
 * Author: kt & om
 *
 * Created on April 18, 2019, 11:14 AM
 */

#ifndef BITMAPALLOCATOR_H
#define BITMAPALLOCATOR_H

#include <vector>
#include <string>
#include <sstream>


using namespace std;

class BitMapAllocator {
public:
    /**
     * BitMapAllocator - The constructor should accept a single argument,the number of page frames in the memory. 
     * It should resize the memory vector to the number of page frames multiplied by the page frame size. 
     * The constructor should then build the bit map such that page frame 0 is not available (reserved by the BitMapAllocator class), and page frames beyond the end of memory are marked as not available.
     * 
     * @param number - Number of page frames
     */
    BitMapAllocator(uint32_t number);

    //Empty Destructor

    virtual ~BitMapAllocator() {
    }

    //Delete copy and move constructors
    BitMapAllocator(const BitMapAllocator & other) = delete;
    BitMapAllocator(BitMapAllocator && other) = delete;
    BitMapAllocator &operator=(BitMapAllocator && other) = delete;
    BitMapAllocator &operator=(const BitMapAllocator & other) = delete;

    /**
     * GetFrames - Takes as its first argument a count of the number of page frames to allocate from the bit map. 
     * The method should push the addresses of all the allocated page frames onto the back of the vector page_frames specified as the second argument. 
     * The contents of all bytes of allocated page frames should be set to 0. 
     * Must always allocate the page frames starting from the lowest available page frame number. 
     * Page frames allocated need not be contiguous.
     * 
     * @param count - Number of page frames to allocate from the bit map
     * @param page_frames - Allocates data here
     * @return - True if page frames successfully allocated/ false if no page frames allocated
     */
    bool GetFrames(uint32_t count, vector<uint32_t> &page_frames);
    /**
     * FreeFrames - The last count page frame addresses from the vector page_frames should be marked as available. 
     * These page frame addresses should be popped from the back of the page_frames vector as they are returned to the available list. 
     * 
     * @param count - Number of page frames to allocate from the bit map
     * @param page_frames - Allocates data here
     * @return - True if page frames are released, false if no page frames released
     */
    bool FreeFrames(uint32_t count, vector<uint32_t> &page_frames);
    /**
     * get_free_count - Gets current number of unallocated page frames
     * @return - Number of unallocated frames
     */
    uint32_t get_free_count();
    /**
     * get_bit_map_string - Gets string of bitmap
     * @return - String representation of bitmap
     */
    string get_bit_map_string();
    /**
     * first_free_bit - Gets location of the first available frame
     * @return - Index of available bit
     */
    uint32_t first_free_bit();

private:
    //Simulated memory vector
    vector<uint8_t> memory;
};

#endif /* BITMAPALLOCATOR_H */

