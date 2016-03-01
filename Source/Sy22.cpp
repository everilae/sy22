#include <set>
#include <numeric>

#include <cstring>

#include "Sy22.h"

namespace sy22 {

	const std::set<int> overflow_bytes = {
		// Common
		0x0B, 0x0E, 0x11, 0x13,
		// Element A
		0x16, 0x19, 0x1B, 0x1D, 0x24, 0x26, 0x28,
		// Element B
		0x30, 0x32, 0x35, 0x37, 0x39, 0x3F, 0x43, 0x45, 0x47, 0x4F,
		0x53, 0x55, 0x57,
		// Element C
		0x60, 0x63, 0x65, 0x67, 0x6E, 0x70, 0x72,
		// Element D
		0x7A, 0x7C, 0x7F, 0x81, 0x83, 0x89, 0x8D, 0x8F, 0x91, 0x99,
		0x9D, 0x9F, 0xA1,
		// Vector info overflow bytes
		0xAB, 0xAF, 0xB3, 0xB7, 0xBB, 0xBF, 0xC3, 0xC7, 0xCB, 0xCF,
		0xD3, 0xD7, 0xDB, 0xDF, 0xE3, 0xE7, 0xEB, 0xEF, 0xF3, 0xF7,
		0xFB, 0xFF, 0x103, 0x107, 0x10B, 0x10F, 0x113, 0x117, 0x11B,
		0x11F, 0x123, 0x127, 0x12B, 0x12F, 0x133, 0x137, 0x13B, 0x13F,
		0x143, 0x147, 0x14B, 0x14F, 0x153, 0x157, 0x15B, 0x15F, 0x163,
		0x167, 0x16B, 0x16F, 0x173, 0x177, 0x17B, 0x17F, 0x183, 0x187,
		0x18B, 0x18F, 0x193, 0x197, 0x19B, 0x19F, 0x1A3, 0x1A7, 0x1AB,
		0x1AF, 0x1B3, 0x1B7, 0x1BB, 0x1BF, 0x1C3, 0x1C7, 0x1CB, 0x1CF,
		0x1D3, 0x1D7, 0x1DB, 0x1DF, 0x1E3, 0x1E7, 0x1EB, 0x1EF, 0x1F3,
		0x1F7, 0x1FB, 0x1FF, 0x203, 0x207, 0x20B, 0x20F, 0x213, 0x217,
		0x21B, 0x21F, 0x223, 0x227, 0x22B, 0x22F, 0x233, 0x237
	};

	int byte_sum(const Voice& v) {
		int sum = 0;
		const unsigned char* voice_data_ptr =
			reinterpret_cast<const unsigned char*>(&v);
		for (int i = 0; i < sizeof(Voice); i++) {
			unsigned char b = voice_data_ptr[i];
			// Check if given byte is an overflow byte
			if (overflow_bytes.count(i)) {
				// Overflow bytes are the 8th bit of a byte
				b <<= 7;
			}
			sum += b;
		}
		return sum;
	}

	void Voice::update_checksum() {
		//   The 8-bit checksum is calculated in a very similar way to the 7-bit sysex
		// checksum. When summing all the bytes, the overflows (in brackets) must be
		// treated as the 8th bit of the following byte. The way I think of this is 
		// that all the bytes are summed but the overflow bytes must be multiplied by
		// 128 first. The 8 lowest bits of the sum then give the checksum so that
		//
		//     (-S AND $7F)     goes into byte 23D  (extracting least significant 7 bits)
		//     (-S AND $80)/$80 goes into byte 23C  (extracting 8th bit)
		//
		//   Remember that the first byte in each vector step is an overflow! If you 
		// miss one out then the checksum is wrong.
		checksum = midi::UChar(-byte_sum(*this));
	}

#define SY22_SVD_HEADER "PK  2203AE"

	/**
	 * Create a new Single Voice Dump message and populate with given
	 * Voice data.
	 */
	SingleVoiceDump::SingleVoiceDump(Voice& voice) :
		start_of_sysex(0xF0),
		reserved_0(0x43),
		channel(0),
		reserved_1(0x7E),
		// len(header) + len(voice_data) = 0x248 (0x04 and 0x48 in weird midi bytes)
		count_msb(0x04),
		count_lsb(0x48),
		voice_data(voice),
		eox(0xF7)
	{
		const unsigned char* voice_data_ptr =
			reinterpret_cast<const unsigned char*>(&voice);
		int sum = std::accumulate(
				SY22_SVD_HEADER,
				SY22_SVD_HEADER + sizeof(SY22_SVD_HEADER) - 1,
				0);
		sum += std::accumulate(
				voice_data_ptr,
				voice_data_ptr + sizeof(Voice),
				0);
		checksum = static_cast<unsigned char>(-sum & 0x7F);
		memcpy(header, SY22_SVD_HEADER, sizeof(header));
	}

};
