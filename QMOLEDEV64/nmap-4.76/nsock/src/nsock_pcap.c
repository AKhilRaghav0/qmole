#include "nsock.h"
#include "nsock_internal.h"
#include "nsock_pcap.h"
#include <limits.h>

#if HAVE_PCAP
static int nsock_pcap_get_l3_offset(pcap_t *pt, int *dl);
char * nsock_pcap_set_filter(pcap_t *pt, const char *device, const char *bpf);

/*
 * Convert new nsiod to pcap descriptor. Other parameters have the
 * same meaning as for pcap_open_live in pcap(3).
 * 	device   : pcap-style device name
 * 	snaplen  : size of packet to be copied to hanler
 * 	promisc  : whether to open device in promiscous mode
 * 	bpf_fmt	 : berkeley filter 
 * return value: NULL if everything was okay, or error string if error occurred
 * */
char* nsock_pcap_open(nsock_pool nsp, nsock_iod nsiod, 
		const char *pcap_device, int snaplen, int promisc,
		const char *bpf_fmt, ...)
				
{
	msiod *nsi = (msiod *) nsiod;
	mspool *ms = (mspool *) nsp;
	mspcap *mp = (mspcap *) nsi->pcap;
	static char errorbuf[128];
	
	if(mp) return "nsock-pcap: this nsi already has pcap device opened";
	
	mp = (mspcap *)safe_malloc(sizeof(mspcap));
	nsi->pcap = (void*)mp;
	
	char err0r[PCAP_ERRBUF_SIZE];
	
	#ifdef PCAP_CAN_DO_SELECT
	  #if PCAP_BSD_SELECT_HACK
	    /* MacOsX reports error if to_ms is too big (like INT_MAX) with error
	     *    FAILED. Reported error: BIOCSRTIMEOUT: Invalid argument  
	     * INT_MAX/6 (=357913941) seems to be working...*/
	    int to_ms = 357913941;
	  #else
	    int to_ms = 200;
	  #endif
	#else
	int to_ms = 1;
	#endif

	/* packet filter string */
	char bpf[4096];
	va_list ap;
	
	va_start(ap, bpf_fmt);
	if(Vsnprintf(bpf, sizeof(bpf), bpf_fmt, ap) >= (int) sizeof(bpf)){
		va_end(ap);
		return "nsock-pcap: nsock_pcap_open called with too-large bpf filter arg";
	}
	va_end(ap);
	
	if (ms->tracelevel > 0)
  		nsock_trace(ms, "PCAP requested on device '%s' with berkeley filter '%s' (promisc=%i snaplen=%i to_ms=%i) (IOD #%li)", 
  		pcap_device,bpf, promisc, snaplen, to_ms, nsi->id);

	int failed = 0;
	do {
		mp->pt = pcap_open_live((char*)pcap_device, snaplen, promisc, to_ms, err0r);
		if (mp->pt)	/* okay, opened!*/ 
			break;
		/* sorry, something failed*/
		if (++failed >= 3) {
			fprintf(stderr,
	"Call to pcap_open_live(%s, %d, %d, %d) failed three times. Reported error: %s\n"
	"There are several possible reasons for this, depending on your operating system:\n"
	"LINUX: If you are getting Socket type not supported, try modprobe af_packet or recompile your kernel with SOCK_PACKET enabled.\n"
	"*BSD:  If you are getting device not configured, you need to recompile your kernel with Berkeley Packet Filter support.  If you are getting No such file or directory, try creating the device (eg cd /dev; MAKEDEV <device>; or use mknod).\n"
	"*WINDOWS:  Nmap only supports ethernet interfaces on Windows for most operations because Microsoft disabled raw sockets as of Windows XP SP2.  Depending on the reason for this error, it is possible that the --unprivileged command-line argument will help.\n"
	"SOLARIS:  If you are trying to scan localhost and getting '/dev/lo0: No such file or directory', complain to Sun.  I don't think Solaris can support advanced localhost scans.  You can probably use \"-PN -sT localhost\" though.\n\n", 
				pcap_device, snaplen, promisc, to_ms, err0r);
			return "nsock-pcap: can't open pcap! are you root?";
		}

		fprintf(stderr, "pcap_open_live(%s, %d, %d, %d) FAILED. Reported error: %s.  Will wait %d seconds then retry.\n",
			pcap_device, snaplen, promisc, to_ms, err0r, 4*failed);	
		sleep(4* failed);
	}while(1);

	char *e = nsock_pcap_set_filter(mp->pt, pcap_device, bpf);
	if(e) return e;
	
	
	#ifdef WIN32
	/* We want any responses back ASAP */
	pcap_setmintocopy(mp->pt, 1);
	
	/* If there are no packets left to read exit after to_ms miliseconds*/
	PacketSetReadTimeout(mp->pt->adapter, to_ms);
	#endif
	
	int datalink;
	mp->l3_offset = nsock_pcap_get_l3_offset(mp->pt, &datalink);
	mp->snaplen = snaplen;
	mp->datalink = datalink;
	mp->pcap_device = strdup(pcap_device);
	#ifdef PCAP_CAN_DO_SELECT
	mp->pcap_desc = pcap_get_selectable_fd(mp->pt);
	#else
	mp->pcap_desc = -1;
	#endif

	/* Set device non-blocking */
	if(pcap_setnonblock(mp->pt, 1, err0r) < 0){
		/* I can't do select() on pcap! blockig + no_select is fatal */
		if(mp->pcap_desc < 0){
			Snprintf(errorbuf, sizeof(errorbuf),"nsock-pcap: Failed to set pcap descriptor on device %s to nonblocking state: %s", pcap_device, err0r);
			return errorbuf;
		}
			
		/* When we use bsd hack we also need to set non-blocking */
		#ifdef PCAP_BSD_SELECT_HACK
		Snprintf(errorbuf, sizeof(errorbuf),"nsock-pcap: Failed to set pcap descriptor on device %s to nonblocking state: %s", pcap_device, err0r);
		return errorbuf;
		#endif
		
		/* in other case, we can accept blocking pcap */
		fprintf(stderr, "Failed to set pcap descriptor on device %s to nonblocking state: %s", pcap_device, err0r);
	}
  	
	if (ms->tracelevel > 0)
  		nsock_trace(ms, "PCAP created successfully on device '%s' (pcap_desc=%i bsd_hack=%i to_valid=%i l3_offset=%i) (IOD #%li)", 
  		pcap_device,
  		mp->pcap_desc,
  		#if PCAP_BSD_SELECT_HACK
  		1,
  		#else
  		0,
  		#endif
  		#if PCAP_RECV_TIMEVAL_VALID
  		1,
  		#else
  		0,
  		#endif 
  		mp->l3_offset,
  		nsi->id);
  	return NULL;
}

char *nsock_pcap_set_filter(pcap_t *pt, const char *device, const char *bpf)
{
	struct bpf_program fcode;
	#ifndef __amigaos__
  	unsigned int localnet, netmask;
	#else
  	bpf_u_int32 localnet, netmask;
	#endif
  	char err0r[PCAP_ERRBUF_SIZE];
  	static char errorbuf[128]; 
  
	// Cast below is becaue OpenBSD apparently has a version that takes a
	// non-const device (hopefully they don't actually write to it).
	if (pcap_lookupnet( (char *) device, &localnet, &netmask, err0r) < 0){
		Snprintf(errorbuf, sizeof(errorbuf), "Failed to lookup subnet/netmask for device (%s): %s", device, err0r);
		return errorbuf;
	}
  
	// log_write(LOG_STDOUT, "Packet capture filter (device %s): %s\n", device, buf);
  
  	if (pcap_compile(pt, &fcode, (char*)bpf, 1, netmask) < 0){
		Snprintf(errorbuf, sizeof(errorbuf), "Error compiling our pcap filter: %s\n", pcap_geterr(pt));
		return errorbuf;
  	}
		
	if (pcap_setfilter(pt, &fcode) < 0 ){
		Snprintf(errorbuf, sizeof(errorbuf),"Failed to set the pcap filter: %s\n", pcap_geterr(pt));
		return errorbuf;
	}
		
  	pcap_freecode(&fcode);
  	return NULL;
}

static int nsock_pcap_get_l3_offset(pcap_t *pt, int *dl){
	int datalink;
	unsigned int offset = 0;
	
	/* New packet capture device, need to recompute offset */
	if ( (datalink = pcap_datalink(pt)) < 0)
		fatal("Cannot obtain datalink information: %s", pcap_geterr(pt));

	/* NOTE: IF A NEW OFFSET EVER EXCEEDS THE CURRENT MAX (24), ADJUST
	MAX_LINK_HEADERSZ in tcpip.h */
	switch(datalink) {
	case DLT_EN10MB: offset = 14; break;
	case DLT_IEEE802: offset = 22; break;
	#ifdef __amigaos__
	case DLT_MIAMI: offset = 16; break;
	#endif
	#ifdef DLT_LOOP
	case DLT_LOOP:
	#endif
	case DLT_NULL: offset = 4; break;

	case DLT_SLIP:
	#ifdef DLT_SLIP_BSDOS
	case DLT_SLIP_BSDOS:
	#endif
	#if (FREEBSD || OPENBSD || NETBSD || BSDI || MACOSX)
	offset = 16;break;
	#else
	offset = 24;break; /* Anyone use this??? */
	#endif
	
	case DLT_PPP: 
	#ifdef DLT_PPP_BSDOS
	case DLT_PPP_BSDOS:
	#endif
	#ifdef DLT_PPP_SERIAL
	case DLT_PPP_SERIAL:
	#endif
	#ifdef DLT_PPP_ETHER
	case DLT_PPP_ETHER:
	#endif
	#if (FREEBSD || OPENBSD || NETBSD || BSDI || MACOSX)
	offset = 4;break;
	#else
	  #ifdef SOLARIS
	    offset = 8;break;
	  #else
	    offset = 24;break; /* Anyone use this? */
	  #endif /* ifdef solaris */
	#endif /* if freebsd || openbsd || netbsd || bsdi */
	case DLT_RAW: offset = 0; break;
	case DLT_FDDI: offset = 21; break;
	#ifdef DLT_ENC
	case DLT_ENC: offset = 12; break;
	#endif /* DLT_ENC */
	#ifdef DLT_LINUX_SLL
	case DLT_LINUX_SLL: offset = 16; break;
	#endif

	default: /* Sorry, link type is unknown. */
		fatal("Unknown datalink type %d.\n", datalink);
	}
	if(dl)
	  *dl = datalink;
	return(offset);
}


/*
 * Requests exacly one packet to be captured.
 * */
nsock_event_id nsock_pcap_read_packet(nsock_pool nsp, nsock_iod nsiod, 
			 nsock_ev_handler handler, int timeout_msecs,
			 void *userdata)
{
	msiod *nsi = (msiod *) nsiod;
	mspool *ms = (mspool *) nsp;
	msevent *nse;

	nse = msevent_new(ms, NSE_TYPE_PCAP_READ, nsi, timeout_msecs, handler, userdata);
	assert(nse);

	if (ms->tracelevel > 0) {
		nsock_trace(ms, "Pcap read request from IOD #%li  EID %li",  
		  nsi->id, nse->id);
	}
  
	nsp_add_event(ms, nse);
  
	return nse->id;
}

/*
 * Remember that pcap descriptor is in nonblocking state. */
int do_actual_pcap_read(msevent *nse)
{
	msiod *iod = nse->iod;
	mspcap *mp = (mspcap *) iod->pcap;
	
	nsock_pcap npp;
	nsock_pcap *n;
	memset(&npp, 0, sizeof(nsock_pcap));
	
	if (nse->iod->nsp->tracelevel > 2)
  		nsock_trace(nse->iod->nsp, "PCAP do_actual_pcap_read TEST (IOD #%li) (EID #%li)",
  		nse->iod->id, nse->id);

	assert( FILESPACE_LENGTH(&(nse->iobuf)) == 0 );
	
	struct pcap_pkthdr *pkt_header;
        const unsigned char *pkt_data = NULL;
	int rc = pcap_next_ex(mp->pt, &pkt_header, &pkt_data);
	switch(rc){
	case 1: /* read good packet  */
		#ifdef PCAP_RECV_TIMEVAL_VALID
		npp.ts     = pkt_header->ts;
		#else
		/* on these platforms time received from pcap is invalid. It's better to set current time */
		memcpy(&npp.ts, nsock_gettimeofday(), sizeof(struct timeval));
		#endif
		npp.len    = pkt_header->len;
		npp.caplen = pkt_header->caplen;
		npp.packet = pkt_data;
		fscat(&(nse->iobuf), (char*)&npp, sizeof(npp));
		fscat(&(nse->iobuf), (char*)pkt_data, npp.caplen);
		n = (nsock_pcap *) FILESPACE_STR(&(nse->iobuf));
		n->packet = (unsigned char*)FILESPACE_STR(&(nse->iobuf))+sizeof(npp);
		if (nse->iod->nsp->tracelevel > 2)
	  		nsock_trace(nse->iod->nsp, "PCAP do_actual_pcap_read READ (IOD #%li) (EID #%li) size=%i",
	  		nse->iod->id, nse->id, pkt_header->caplen);
		return(1);
	case 0: /* timeout */
		return(0);
	case -1: /* error */
		fatal("pcap_next_ex() fatal error while reading from pcap: %s\n", pcap_geterr(mp->pt));
		break;
	case -2: /* no more packets in savefile (if reading from one) */
	default:
		assert(0);
	}
  	return 0;
}

void nse_readpcap(nsock_event nsee,
	const unsigned char **l2_data, size_t *l2_len,
	const unsigned char **l3_data, size_t *l3_len,
	size_t *packet_len, struct timeval *ts) 
{
	msevent *nse = (msevent *)nsee;
	msiod  *iod = nse->iod;
	mspcap *mp = (mspcap *) iod->pcap;
	
	nsock_pcap *n = (nsock_pcap *) FILESPACE_STR(&(nse->iobuf));
	if(FILESPACE_LENGTH(&(nse->iobuf)) < sizeof(nsock_pcap)){
		if(l2_data) *l2_data = NULL;
		if(l2_len ) *l2_len  = 0;
		if(l3_data) *l3_data = NULL;
		if(l3_len ) *l3_len  = 0;
		if(packet_len) *packet_len = 0;
		return;
	}
	
	size_t l2l = MIN(mp->l3_offset, n->caplen);
	size_t l3l = MAX(0, n->caplen-mp->l3_offset);
	
	if(l2_data) *l2_data = n->packet;
	if(l2_len ) *l2_len  = l2l;
	if(l3_data) *l3_data = l3l>0? n->packet+l2l : NULL;
	if(l3_len ) *l3_len  = l3l;
	if(packet_len) *packet_len = n->len;
	if(ts) *ts = n->ts;
	return;
}

int nsi_pcap_linktype(nsock_iod nsiod){
	msiod *nsi = (msiod *) nsiod;
	mspcap *mp = (mspcap *) nsi->pcap;
	assert(mp);
	return(mp->datalink);
}

int nsi_is_pcap(nsock_iod nsiod){
	msiod *nsi = (msiod *) nsiod;
	mspcap *mp = (mspcap *) nsi->pcap;
	return(mp!=NULL);
}


#endif // HAVE_PCAP
	
