#include "binpac.h"

#include "Net.h"
#include "Conn.h"
#include "Sessions.h"
#include "analyzer/Analyzer.h"
#include "analyzer/Manager.h"
#include "analyzer/protocol/pia/PIA.h"
#include "analyzer/protocol/tcp/TCP.h"

#include "FuzzBuffer.h"
#include "fuzzer-setup.h"

#include "pcap/dlt.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
	{

	zeek::FuzzBuffer fb{data, size};

	if ( ! fb.Valid() )
		return 0;

	for ( ; ;  )
		{
		auto chunk = fb.Next();

		if ( ! chunk )
			break;

		Packet pkt;
		auto timestamp = current_time(true);
		pkt_timeval ts = {long(timestamp), 0};
		pkt.Init(DLT_RAW, &ts, chunk->size, chunk->size, chunk->data.get(), false, "");

		try
			{
			sessions->NextPacket(timestamp, &pkt);
			}
		catch ( binpac::Exception const &e )
			{
			}

		chunk = {};
		mgr.Drain();
		}

	zeek::fuzz_cleanup_one_input();
	return 0;
	}
