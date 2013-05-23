#ifndef __buffer_h

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
#define __buffer_h

#include <system/types.h>
/*
 * Linked list of buffers
 * used to do zero-copy network send by stacking
 * the payload from multiple protocols:
 *
 * Example: EthernetPreamble + IP Packet + UDP Datagram + RTP Header + RTP Payload + EthernetCRC
 */
class SegmentedBuffer;

class SegmentedBuffer {
 protected:
	const char * _data;
	size_t _size;
	SegmentedBuffer * _next;
 public:

	SegmentedBuffer(const char * data,size_t size,SegmentedBuffer * next = 0) :
		_data(data), _size(size), _next(next) {}
	SegmentedBuffer(void * data,size_t size,SegmentedBuffer * next = 0) :
			_data((const char *)data), _size(size), _next(next) {}

	void append(SegmentedBuffer * next) { _next = next; }

	const char * data() { return _data; }
	size_t size() { return _size; }
	SegmentedBuffer * next() { return _next; }

	size_t total_size() {
		return _size + (_next ? _next->total_size() : 0);
	}

	// method for final delivery (like copy to a DMA ring buffer)
	size_t copy_to(char * dst,size_t maxsize) {
		if (!_data)
			return 0;

		if (_size < maxsize) {
			memcpy(dst,_data,_size);
			return _size + (_next ? (_next->copy_to(dst + _size,maxsize - _size)) : 0);

		} else {
			memcpy(dst,_data,maxsize);
			return maxsize;
		}
	}
};

#endif
