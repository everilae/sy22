#ifndef _MIDI_DATA_H_
#define _MIDI_DATA_H_ 1

namespace midi {

	template <class Int> class Byte;

	struct byte_t {
		unsigned char msb;
		unsigned char lsb;

		template <class Int>
		Int operator +(const Int& rhs) const {
			return Byte<Int>(*this) + rhs;
		}
	};

	template <class Int>
	class Byte {
		const Int value;

		public:
		Byte(const Int i) : value(i) {}
		Byte(const byte_t& b) : value((b.msb << 7) | (b.lsb & 0x7F)) {}

		operator Int() const {
			return value;
		}
		Byte operator +(const Byte& rhs) const {
			return Byte(value + rhs.value);
		}
		Byte operator +(const Int& rhs) const {
			return Byte(value + rhs);
		}
		operator byte_t() const {
			return {
				// effin integer promotions and narrowing casts (C++14 or some such)
				static_cast<unsigned char>(static_cast<unsigned char>(value) >> 7),
				static_cast<unsigned char>(static_cast<unsigned char>(value) & 0x7F)
			};
		}
	};

	using UChar = Byte<unsigned char>;
	using SChar = Byte<char>;

};

#endif
