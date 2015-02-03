#ifndef _SY22_H_
#define _SY22_H_ 1

#include <midi_data.h>

/**
 * http://worsa.republika.pl/yamaha-sy35/sy35form.txt
 */
namespace sy22 {

	struct VectorStep {
                // step length ($17E:repeat  $17F:end)
                // X (-31 to +31)
                // Y (-31 to +31)
		midi::byte_t len;
		unsigned char x;
		unsigned char y;
	};

	struct VectorInfo {
                // ---------------VECTOR-INFO----------------------------------------------------
                // 
                //     A9                    Vector level rate
                //     AA                    Vector detune rate
                // (AB)AC  0,17F             level step length ($17E:repeat  $17F:end)\
                //     AD  0,3E              level X (-31 to +31)                      > * 50
                //     AE  0,3E              level Y (-31 to +31)                     /
                //     :
                //     :
                // (173)174 0,17F            detune step length  \
                //    175  0,3E              detune X             > * 50
                //    176  0,3E              detune Y            /
                //     :
                //     :
		unsigned char level_rate;
		unsigned char detune_rate;
		VectorStep level[50];
		VectorStep detune[50];
	};

	struct Envelope {
                // (24)25        (L)LLL0RRR  Envelope  L=level scaling  R=rate scaling
                // (26)27        (D)0AAAAAA  D=envelope delay(on/off)  A=Attack rate
                // (28)29  0,17F (P)PDDDDDD  D=Decay rate 1  P=env peak (each of the envelope
                //                             levels has a number: initial(0),attack(1),DL1(2),
                //                             DL2(3). The number of the PEAK level must be put in
                //                             the two bits PP for the envelope to work)
                //     2A  0,3F              Decay rate 2
		//     2B  0,3F              Release rate
                //     2C  0,7F              Initial level (0:max $7F:min)    (The peak level
                //     2D  0,7F              Attack level        "               is the one with
                //     2E  0,7F              Decay level 1       "               lowest value!)
                //     2F  0,7F              Decay level 2       "
		midi::byte_t level_rate_scaling;
		midi::byte_t delay_ar;
		midi::byte_t peak_dr1;
		unsigned char dr2;
		unsigned char rr;
		unsigned char il;
		unsigned char al;
		unsigned char dl1;
		unsigned char dl2;
	};

	struct LFO {
                // (19)1A  0,17F (L)LLSSSSS  L=LFO wave  S=LFO speed
                // (1B)1C  0,17F             LFO Delay
                // (1D)1E  0,17F             LFO Rate
                //     1F  0,1F     001AAAA  A=LFO AM depth
                //     20  0,3F     01PPPPP  P=LFO PM depth
		midi::byte_t wave_speed;
		midi::byte_t delay;
		midi::byte_t rate;
		unsigned char am_depth;
		unsigned char pm_depth;
	};

	struct Wave {
		// ---------------ELEMENT A-----------------------------------------------------
                //     15  0,7F              Wave
                // (16)17  174,0C            Pitch shift (-12 to +12)
                //     18  0,7A     VVVAAAA  V=velocity response  A=after touch response
                // (19)1A  0,17F (L)LLSSSSS  L=LFO wave  S=LFO speed
                // (1B)1C  0,17F             LFO Delay
                // (1D)1E  0,17F             LFO Rate
                //     1F  0,1F     001AAAA  A=LFO AM depth
                //     20  0,3F     01PPPPP  P=LFO PM depth
                //     21           EEE0PPP  E=env type(0:User 7:Organ)  P=pan (0:Right 4:Left)
                //     22  0,7F              Tone volume (0:max $7F:min)
                //     23  0,3F     0TTDDDD  T=temperament  D=detune  (note: these are not
                //                             accessible from the synth but they are only really
                //                             useful for FM elements)
                // (24)25        (L)LLL0RRR  Envelope  L=level scaling  R=rate scaling
                // (26)27        (D)0AAAAAA  D=envelope delay(on/off)  A=Attack rate
                // (28)29  0,17F (P)PDDDDDD  D=Decay rate 1  P=env peak (each of the envelope
                //                             levels has a number: initial(0),attack(1),DL1(2),
                //                             DL2(3). The number of the PEAK level must be put in
                //                             the two bits PP for the envelope to work)
                //     2A  0,3F              Decay rate 2
		//     2B  0,3F              Release rate
                //     2C  0,7F              Initial level (0:max $7F:min)    (The peak level
                //     2D  0,7F              Attack level        "               is the one with
                //     2E  0,7F              Decay level 1       "               lowest value!)
                //     2F  0,7F              Decay level 2       "
		unsigned char wave;
		midi::byte_t pitch_shift;
		unsigned char velocity_after_touch_response;
		LFO lfo;
		unsigned char env_type_pan;
		unsigned char tone_volume;
		unsigned char temperament_detune;
		Envelope env;
	};

	struct Operator {
                //          ------MODULATOR-----------------------------------------------------
                // 
                // (3F)40  0,17F (X)WWWFFFF  X=fixed freq mode  W=waveform  F=freq(harmonics)
                //     41  0,7F              FM level (equivalent to FM lvl control on synth)
                //     42  0,3F     0TTDDDD  T=temperament  D=detune  (the temperament makes
                //                             frequency ratios between adjacent notes
                //                             vary over the keyboard- producing bell like
                //                             effects. Detune can be used to create beating
                //                             effects between carrier and modulator.)
                // (43)44                    \
                // (45)46                     \
                // (47)48                      \ modulation envelope- defined in exactly the same
                //     49                      /                      way as element A's
                //     :                      /                       amplitude envelope
                //     4E                    /
                //          -------CARRIER------------------------------------------------------
                // 
                // (4F)50  0,17F (X)WWWFFFF  X=fixed freq mode  W=waveform  F=freq(harmonic)
                //     51  0,7F              Tone Volume
                //     52  0,3F     0TTDDDD  T=temperament  D=detune
                // (53)54                    \
                // (55)56                     \
                // (57)58                      \ carrier envelope (ie ordinary amplitude envelope)
                //     59                      /
                //     :                      /
                //     5E                    /
		midi::byte_t fixed_waveform_freq;
		unsigned char level;
		unsigned char temperament_detune;
		Envelope env;
	};

	struct FM {
                // (30)31  0,17F             Wave (This doesn't set the sound- just the name
                //                             displayed. To change the sound, you have to alter
                //                             the FM parameters. There is no way of selecting
                //                             the presets unless you have all the values stored!)
                // (32)33  174,0C            Pitch shift (-12 to +12)
                //     34  0,7A     VVVAAAA  V=velocity response  A=after touch response
                // (35)36  0,17F (L)LLSSSSS  LFO wave/speed
                // (37)38  0,17F             LFO delay
                // (39)3A  0,17F             LFO rate
                //     3B  0,3F     0CMAAAA  A=LFO AM depth  C=controls carrier &
                //     3C  0,7F     CMPPPPP  P=LFO PM depth  M=controls modulator
                //                             (note: normally AM just creates tremolo effects
                //                             by modulating the carrier, but now you can set
                //                             it to vary the modulation level as well! When
                //                             you change PM on the synth it controls both
                //                             modulator AND carrier. Here you can set it to
                //                             modulate just one of them- creating strange
                //                             beating effects.)
                //     3D           EEE0PPP  E=envelope type  P=pan 
                //     3E  0,7               Feedback
                //          ------MODULATOR-----------------------------------------------------
                // 
                // (3F)40  0,17F (X)WWWFFFF  X=fixed freq mode  W=waveform  F=freq(harmonics)
                //     41  0,7F              FM level (equivalent to FM lvl control on synth)
                //     42  0,3F     0TTDDDD  T=temperament  D=detune  (the temperament makes
                //                             frequency ratios between adjacent notes
                //                             vary over the keyboard- producing bell like
                //                             effects. Detune can be used to create beating
                //                             effects between carrier and modulator.)
                // (43)44                    \
                // (45)46                     \
                // (47)48                      \ modulation envelope- defined in exactly the same
                //     49                      /                      way as element A's
                //     :                      /                       amplitude envelope
                //     4E                    /
                //          -------CARRIER------------------------------------------------------
                // 
                // (4F)50  0,17F (X)WWWFFFF  X=fixed freq mode  W=waveform  F=freq(harmonic)
                //     51  0,7F              Tone Volume
                //     52  0,3F     0TTDDDD  T=temperament  D=detune
                // (53)54                    \
                // (55)56                     \
                // (57)58                      \ carrier envelope (ie ordinary amplitude envelope)
                //     59                      /
                //     :                      /
                //     5E                    /
		midi::byte_t wave;
		midi::byte_t pitch_shift;
		unsigned char velocity_after_touch_response;
		LFO lfo;
		unsigned char env_type_pan;
		unsigned char feedback;
		Operator modulator;
		Operator carrier;
	};

	struct Voice {
		//  Byte   Value
		// Offset  Range    Bit Map            Comments
		// ------------------------------------------------------------------------------
		//     00  01                (I don't know what these bytes are but if you change
		//     01  25                 them, the synth goes quiet!)
		//     02  0,7F     DDDTTTT  Effect  D=depth(0-7)  T=type(0-15)
		//  03-0A                    Name (8 characters)
		// (0B)0C        (T)00PPPPP  T=type(AB or ABCD)  P=pitch bend
		//     0D           LPA00PA  After touch (Level,PM,AM) and Mod Wheel (PM,AM)
		// (0E)0F  174,0C            After touch pitch shift (-12 to +12)
		//     10  0,7F              Envelope delay (same for all envelopes)
		// (11)12  140,3F            Common attack rate (-64 to +63)
		// (13)14  140,3F            Common release rate (-64 to +63)
		// ---------------ELEMENT A-----------------------------------------------------
                //     15  0,7F              Wave
                // (16)17  174,0C            Pitch shift (-12 to +12)
                //     18  0,7A     VVVAAAA  V=velocity response  A=after touch response
                // (19)1A  0,17F (L)LLSSSSS  L=LFO wave  S=LFO speed
                // (1B)1C  0,17F             LFO Delay
                // (1D)1E  0,17F             LFO Rate
                //     1F  0,1F     001AAAA  A=LFO AM depth
                //     20  0,3F     01PPPPP  P=LFO PM depth
                //     21           EEE0PPP  E=env type(0:User 7:Organ)  P=pan (0:Right 4:Left)
                //     22  0,7F              Tone volume (0:max $7F:min)
                //     23  0,3F     0TTDDDD  T=temperament  D=detune  (note: these are not
                //                             accessible from the synth but they are only really
                //                             useful for FM elements)
                // (24)25        (L)LLL0RRR  Envelope  L=level scaling  R=rate scaling
                // (26)27        (D)0AAAAAA  D=envelope delay(on/off)  A=Attack rate
                // (28)29  0,17F (P)PDDDDDD  D=Decay rate 1  P=env peak (each of the envelope
                //                             levels has a number: initial(0),attack(1),DL1(2),
                //                             DL2(3). The number of the PEAK level must be put in
                //                             the two bits PP for the envelope to work)
                //     2A  0,3F              Decay rate 2
                //     2B  0,3F              Release rate
                //     2C  0,7F              Initial level (0:max $7F:min)    (The peak level
                //     2D  0,7F              Attack level        "               is the one with
                //     2E  0,7F              Decay level 1       "               lowest value!)
                //     2F  0,7F              Decay level 2       "
                // ---------------ELEMENT-B-----------------------------------------------------
                // (30)31  0,17F             Wave (This doesn't set the sound- just the name
                //                             displayed. To change the sound, you have to alter
                //                             the FM parameters. There is no way of selecting
                //                             the presets unless you have all the values stored!)
                // (32)33  174,0C            Pitch shift (-12 to +12)
                //     34  0,7A     VVVAAAA  V=velocity response  A=after touch response
                // (35)36  0,17F (L)LLSSSSS  LFO wave/speed
                // (37)38  0,17F             LFO delay
                // (39)3A  0,17F             LFO rate
                //     3B  0,3F     0CMAAAA  A=LFO AM depth  C=controls carrier &
                //     3C  0,7F     CMPPPPP  P=LFO PM depth  M=controls modulator
                //                             (note: normally AM just creates tremolo effects
                //                             by modulating the carrier, but now you can set
                //                             it to vary the modulation level as well! When
                //                             you change PM on the synth it controls both
                //                             modulator AND carrier. Here you can set it to
                //                             modulate just one of them- creating strange
                //                             beating effects.)
                //     3D           EEE0PPP  E=envelope type  P=pan 
                //     3E  0,7               Feedback
                //          ------MODULATOR-----------------------------------------------------
                // 
                // (3F)40  0,17F (X)WWWFFFF  X=fixed freq mode  W=waveform  F=freq(harmonics)
                //     41  0,7F              FM level (equivalent to FM lvl control on synth)
                //     42  0,3F     0TTDDDD  T=temperament  D=detune  (the temperament makes
                //                             frequency ratios between adjacent notes
                //                             vary over the keyboard- producing bell like
                //                             effects. Detune can be used to create beating
                //                             effects between carrier and modulator.)
                // (43)44                    \
                // (45)46                     \
                // (47)48                      \ modulation envelope- defined in exactly the same
                //     49                      /                      way as element A's
                //     :                      /                       amplitude envelope
                //     4E                    /
                //          -------CARRIER------------------------------------------------------
                // 
                // (4F)50  0,17F (X)WWWFFFF  X=fixed freq mode  W=waveform  F=freq(harmonic)
                //     51  0,7F              Tone Volume
                //     52  0,3F     0TTDDDD  T=temperament  D=detune
                // (53)54                    \
                // (55)56                     \
                // (57)58                      \ carrier envelope (ie ordinary amplitude envelope)
                //     59                      /
                //     :                      /
                //     5E                    /
                // ---------------ELEMENT-C------------------------------------------------------
                //     5F  0,7F              Wave
                // (60)61  174,0C            Pitch shift
                //     62  0,7A     VVVAAAA  V=velocity response  A=after touch response
                // (63)64  0,11F (L)LLSSSSS  L=LFO wave  S=LFO speed
                // (65)66  0,17F             LFO Delay
                // (67)68  0,17F             LFO Rate
                //     69  0,1F     001AAAA  A=LFO AM depth
                //     6A  0,3F     01PPPPP  P=LFO PM depth
                //     6B           EEE0PPP  E=env type  P=pan
                //     6C  0,7F              Tone volume
                //     6D  0,3F     0TTDDDD  T=temperament  D=detune
                // (6E)6F                    \
                // (70)71                     \
                // (72)73                      \ amplitude envelope
                //     74                      /
                //     :                      /
                //     79                    /
                // ---------------ELEMENT-D-----------------------------------------------------
                // (7A)7B  0,17F             Wave
                // (7C)7D  174,0C            Pitch shift
                //     7E  0,7A     VVVAAAA  V=velocity response  A=after touch response
                // (7F)80  0,11F (L)LLSSSSS  L=LFO wave  S=LFO speed
                // (81)82  0,17F             LFO delay
                // (83)84  0,17F             LFO rate
                //     85  0,3F     0CMAAAA  A=LFO AM depth  C=carrier  M=modulator
                //     86  0,7F     CMPPPPP  P=LFO PM depth  C=carrier  M=modulator
                //     87           EEE0PPP  E=envelope type  P=pan
                //     88  0,7               Feedback
                //          ------MODULATOR----------------------------------------------------
                // 
                // (89)8A  0,17F (X)WWWFFFF  X=fixed freq mode  W=waveform  F=frequency
                //     8B  0,7F              FM level
                //     8C  0,3F     0TTDDDD  T=temperament  D=detune
                // (8D)8E                    \
                // (8F)90                     \
                // (91)92                      \ modulation envelope
                //     93                      /
                //     :                      /
                //     98                    /
                //          -------CARRIER------------------------------------------------------
                // 
                // (99)9A  0,17F (X)WWWFFFF  X=fixed freq mode  W=waveform  F=freq(harmonic)
                //     9B  0,7F              Tone Volume
                //     9C  0,3F     0TTDDDD  T=temperament  D=detune
                // (9D)9E                    \
                // (9F)A0                     \
                // (A1)A2                      \ carrier envelope
                //     A3                      /
                //     :                      /
                //     A8                    /
                // ---------------VECTOR-INFO----------------------------------------------------
                // 
                //     A9                    Vector level rate
                //     AA                    Vector detune rate
                // (AB)AC  0,17F             level step length ($17E:repeat  $17F:end)\
                //     AD  0,3E              level X (-31 to +31)                      > * 50
                //     AE  0,3E              level Y (-31 to +31)                     /
                //     :
                //     :
                // (173)174 0,17F            detune step length  \
                //    175  0,3E              detune X             > * 50
                //    176  0,3E              detune Y            /
                //     :
                //     :
                // ------------------------------------------------------------------------------
                //    23B  0                 Null
                //    23C  0,1               8-bit checksum
                //    23D  0,7F                    "
		//
		unsigned char reserved_0;
		unsigned char reserved_1;
		unsigned char effect;
		char name[8];
		midi::byte_t configuration_pitch_bend;
		unsigned char after_touch_mod_wheel;
		midi::byte_t after_touch_pitch_shift;
		unsigned char env_delay;
		midi::byte_t common_ar;
		midi::byte_t common_rr;
		Wave A;
		FM B;
		Wave C;
		FM D;
		VectorInfo vector;
		unsigned char null;
		midi::byte_t checksum;

		void update_checksum() {
			int sum = reserved_0 + reserved_1 + effect;
			for (auto c: name) {
				sum += c;
			}
			sum += midi::UChar(configuration_pitch_bend);
			sum += after_touch_mod_wheel;
			sum += midi::SChar(after_touch_pitch_shift);
			sum += env_delay;
			sum += midi::SChar(common_ar);
			sum += midi::SChar(common_rr);
			sum += A.get_sum();
			sum += B.get_sum();
			sum += C.get_sum();
			sum += D.get_sum();
			sum += vector.get_sum();

			checksum = midi::UChar(-sum)
		}
	};

	Voice make_voice() {
		return { 0x01, 0x25, 0x00 };
	}

	struct SingleVoiceDump {
		const unsigned char start_of_sysex;
		const unsigned char reserved_0;
		unsigned char channel;
		const unsigned char reserved_1;
		const unsigned char count_msb;
		const unsigned char count_lsb;
		const char header[10];
		const Voice voice_data;
		unsigned char checksum;
		const unsigned char eox;
	};

	SingleVoiceDump make_single_voice_dump(const Voice& voice) {
		SingleVoiceDump svd = {
			.start_of_sysex = 0xF0,
			.reserved_0 = 0x43,
			.channel = 0,
			.reserved_1 = 0x7E,
			.count_msb = 0x04,
			.count_lsb = 0x48,
			.header = {'P', 'K', ' ', ' ', '2', '2', '0', '3', 'A', 'E'},
			.voice_data = voice,
			.checksum = 0,
			.eox = 0xF7,
		};

		// header stuff included in sum
		int sum = 'P' + 'K' + ' ' + ' ' + '2' + '2' + '0' + '3' + 'A' + 'E';
		for (int i = 0; i < sizeof(Voice); i++) {
			// checksum is 2's complement of sum of all bytes in
			// voice data block and header, (-S & 0x7F)
			sum += reinterpret_cast<const char*>(&voice)[i];
		}

		svd.checksum = -sum & 0x7F;
		return svd;
	}

};

#endif
