/* This file is modified to serve minimal requirements of iodine */


/*
 * Copyright (c) 1983, 1989, 1993
 *    The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Copyright (c) 2004 by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1996-1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/*
 *	$BINDId: nameser.h,v 8.37 2000/03/30 21:16:49 vixie Exp $
 */

#ifndef _ARPA_NAMESER_H_
#define _ARPA_NAMESER_H_

/*%
 * Revision information.  This is the release date in YYYYMMDD format.
 * It can change every day so the right thing to do with it is use it
 * in preprocessor commands such as "#if (__NAMESER > 19931104)".  Do not
 * compare for equality; rather, use it to determine whether your libbind.a
 * contains a new enough lib/nameser/ to support the feature you need.
 */

#define __NAMESER	19991006	/*%< New interface version stamp. */
/*
 * Define constants based on RFC 883, RFC 1034, RFC 1035
 */
#define NS_PACKETSZ	512	/*%< default UDP packet size */
#define NS_MAXDNAME	1025	/*%< maximum domain name */
#define NS_MAXMSG	65535	/*%< maximum message size */
#define NS_MAXCDNAME	255	/*%< maximum compressed domain name */
#define NS_MAXLABEL	63	/*%< maximum length of domain label */
#define NS_HFIXEDSZ	12	/*%< #/bytes of fixed data in header */
#define NS_QFIXEDSZ	4	/*%< #/bytes of fixed data in query */
#define NS_RRFIXEDSZ	10	/*%< #/bytes of fixed data in r record */
#define NS_INT32SZ	4	/*%< #/bytes of data in a u_int32_t */
#define NS_INT16SZ	2	/*%< #/bytes of data in a u_int16_t */
#define NS_INT8SZ	1	/*%< #/bytes of data in a u_int8_t */
#define NS_INADDRSZ	4	/*%< IPv4 T_A */
#define NS_IN6ADDRSZ	16	/*%< IPv6 T_AAAA */
#define NS_CMPRSFLGS	0xc0	/*%< Flag bits indicating name compression. */
#define NS_DEFAULTPORT	53	/*%< For both TCP and UDP. */
/*
 * These can be expanded with synonyms, just keep ns_parse.c:ns_parserecord()
 * in synch with it.
 */
typedef enum __ns_sect {
	ns_s_qd = 0,		/*%< Query: Question. */
	ns_s_zn = 0,		/*%< Update: Zone. */
	ns_s_an = 1,		/*%< Query: Answer. */
	ns_s_pr = 1,		/*%< Update: Prerequisites. */
	ns_s_ns = 2,		/*%< Query: Name servers. */
	ns_s_ud = 2,		/*%< Update: Update. */
	ns_s_ar = 3,		/*%< Query|Update: Additional records. */
	ns_s_max = 4
} ns_sect;

/*%
 * This is a message handle.  It is caller allocated and has no dynamic data.
 * This structure is intended to be opaque to all but ns_parse.c, thus the
 * leading _'s on the member names.  Use the accessor functions, not the _'s.
 */
typedef struct __ns_msg {
	const u_char	*_msg, *_eom;
	u_int16_t	_id, _flags, _counts[ns_s_max];
	const u_char	*_sections[ns_s_max];
	ns_sect		_sect;
	int		_rrnum;
	const u_char	*_msg_ptr;
} ns_msg;

/* Private data structure - do not use from outside library. */
struct _ns_flagdata {  int mask, shift;  };
extern const struct _ns_flagdata _ns_flagdata[];

/* Accessor macros - this is part of the public interface. */

#define ns_msg_id(handle) ((handle)._id + 0)
#define ns_msg_base(handle) ((handle)._msg + 0)
#define ns_msg_end(handle) ((handle)._eom + 0)
#define ns_msg_size(handle) ((handle)._eom - (handle)._msg)
#define ns_msg_count(handle, section) ((handle)._counts[section] + 0)

/*%
 * This is a parsed record.  It is caller allocated and has no dynamic data.
 */
typedef	struct __ns_rr {
	char		name[NS_MAXDNAME];
	u_int16_t	type;
	u_int16_t	rr_class;
	u_int32_t	ttl;
	u_int16_t	rdlength;
	const u_char *	rdata;
} ns_rr;

/* Accessor macros - this is part of the public interface. */
#define ns_rr_name(rr)	(((rr).name[0] != '\0') ? (rr).name : ".")
#define ns_rr_type(rr)	((ns_type)((rr).type + 0))
#define ns_rr_class(rr)	((ns_class)((rr).rr_class + 0))
#define ns_rr_ttl(rr)	((rr).ttl + 0)
#define ns_rr_rdlen(rr)	((rr).rdlength + 0)
#define ns_rr_rdata(rr)	((rr).rdata + 0)

/*%
 * These don't have to be in the same order as in the packet flags word,
 * and they can even overlap in some cases, but they will need to be kept
 * in synch with ns_parse.c:ns_flagdata[].
 */
typedef enum __ns_flag {
	ns_f_qr,		/*%< Question/Response. */
	ns_f_opcode,		/*%< Operation code. */
	ns_f_aa,		/*%< Authoritative Answer. */
	ns_f_tc,		/*%< Truncation occurred. */
	ns_f_rd,		/*%< Recursion Desired. */
	ns_f_ra,		/*%< Recursion Available. */
	ns_f_z,			/*%< MBZ. */
	ns_f_ad,		/*%< Authentic Data (DNSSEC). */
	ns_f_cd,		/*%< Checking Disabled (DNSSEC). */
	ns_f_rcode,		/*%< Response code. */
	ns_f_max
} ns_flag;

/*%
 * Currently defined opcodes.
 */
typedef enum __ns_opcode {
	ns_o_query = 0,		/*%< Standard query. */
	ns_o_iquery = 1,	/*%< Inverse query (deprecated/unsupported). */
	ns_o_status = 2,	/*%< Name server status query (unsupported). */
				/* Opcode 3 is undefined/reserved. */
	ns_o_notify = 4,	/*%< Zone change notification. */
	ns_o_update = 5,	/*%< Zone update message. */
	ns_o_max = 6
} ns_opcode;

/*%
 * Currently defined response codes.
 */
typedef	enum __ns_rcode {
	ns_r_noerror = 0,	/*%< No error occurred. */
	ns_r_formerr = 1,	/*%< Format error. */
	ns_r_servfail = 2,	/*%< Server failure. */
	ns_r_nxdomain = 3,	/*%< Name error. */
	ns_r_notimpl = 4,	/*%< Unimplemented. */
	ns_r_refused = 5,	/*%< Operation refused. */
	/* these are for BIND_UPDATE */
	ns_r_yxdomain = 6,	/*%< Name exists */
	ns_r_yxrrset = 7,	/*%< RRset exists */
	ns_r_nxrrset = 8,	/*%< RRset does not exist */
	ns_r_notauth = 9,	/*%< Not authoritative for zone */
	ns_r_notzone = 10,	/*%< Zone of record different from zone section */
	ns_r_max = 11,
	/* The following are EDNS extended rcodes */
	ns_r_badvers = 16,
	/* The following are TSIG errors */
	ns_r_badsig = 16,
	ns_r_badkey = 17,
	ns_r_badtime = 18
} ns_rcode;

/* BIND_UPDATE */
typedef enum __ns_update_operation {
	ns_uop_delete = 0,
	ns_uop_add = 1,
	ns_uop_max = 2
} ns_update_operation;

/*%
 * This structure is used for TSIG authenticated messages
 */
struct ns_tsig_key {
        char name[NS_MAXDNAME], alg[NS_MAXDNAME];
        unsigned char *data;
        int len;
};
typedef struct ns_tsig_key ns_tsig_key;

/*%
 * This structure is used for TSIG authenticated TCP messages
 */
struct ns_tcp_tsig_state {
	int counter;
	struct dst_key *key;
	void *ctx;
	unsigned char sig[NS_PACKETSZ];
	int siglen;
};
typedef struct ns_tcp_tsig_state ns_tcp_tsig_state;

#define NS_TSIG_FUDGE 300
#define NS_TSIG_TCP_COUNT 100
#define NS_TSIG_ALG_HMAC_MD5 "HMAC-MD5.SIG-ALG.REG.INT"

#define NS_TSIG_ERROR_NO_TSIG -10
#define NS_TSIG_ERROR_NO_SPACE -11
#define NS_TSIG_ERROR_FORMERR -12

/*%
 * Currently defined type values for resources and queries.
 */
typedef enum __ns_type {
	ns_t_invalid = 0,	/*%< Cookie. */
	ns_t_a = 1,		/*%< Host address. */
	ns_t_ns = 2,		/*%< Authoritative server. */
	ns_t_md = 3,		/*%< Mail destination. */
	ns_t_mf = 4,		/*%< Mail forwarder. */
	ns_t_cname = 5,		/*%< Canonical name. */
	ns_t_soa = 6,		/*%< Start of authority zone. */
	ns_t_mb = 7,		/*%< Mailbox domain name. */
	ns_t_mg = 8,		/*%< Mail group member. */
	ns_t_mr = 9,		/*%< Mail rename name. */
	ns_t_null = 10,		/*%< Null resource record. */
	ns_t_wks = 11,		/*%< Well known service. */
	ns_t_ptr = 12,		/*%< Domain name pointer. */
	ns_t_hinfo = 13,	/*%< Host information. */
	ns_t_minfo = 14,	/*%< Mailbox information. */
	ns_t_mx = 15,		/*%< Mail routing information. */
	ns_t_txt = 16,		/*%< Text strings. */
	ns_t_rp = 17,		/*%< Responsible person. */
	ns_t_afsdb = 18,	/*%< AFS cell database. */
	ns_t_x25 = 19,		/*%< X_25 calling address. */
	ns_t_isdn = 20,		/*%< ISDN calling address. */
	ns_t_rt = 21,		/*%< Router. */
	ns_t_nsap = 22,		/*%< NSAP address. */
	ns_t_nsap_ptr = 23,	/*%< Reverse NSAP lookup (deprecated). */
	ns_t_sig = 24,		/*%< Security signature. */
	ns_t_key = 25,		/*%< Security key. */
	ns_t_px = 26,		/*%< X.400 mail mapping. */
	ns_t_gpos = 27,		/*%< Geographical position (withdrawn). */
	ns_t_aaaa = 28,		/*%< Ip6 Address. */
	ns_t_loc = 29,		/*%< Location Information. */
	ns_t_nxt = 30,		/*%< Next domain (security). */
	ns_t_eid = 31,		/*%< Endpoint identifier. */
	ns_t_nimloc = 32,	/*%< Nimrod Locator. */
	ns_t_srv = 33,		/*%< Server Selection. */
	ns_t_atma = 34,		/*%< ATM Address */
	ns_t_naptr = 35,	/*%< Naming Authority PoinTeR */
	ns_t_kx = 36,		/*%< Key Exchange */
	ns_t_cert = 37,		/*%< Certification record */
	ns_t_a6 = 38,		/*%< IPv6 address (deprecated, use ns_t_aaaa) */
	ns_t_dname = 39,	/*%< Non-terminal DNAME (for IPv6) */
	ns_t_sink = 40,		/*%< Kitchen sink (experimentatl) */
	ns_t_opt = 41,		/*%< EDNS0 option (meta-RR) */
	ns_t_apl = 42,		/*%< Address prefix list (RFC3123) */
	ns_t_tkey = 249,	/*%< Transaction key */
	ns_t_tsig = 250,	/*%< Transaction signature. */
	ns_t_ixfr = 251,	/*%< Incremental zone transfer. */
	ns_t_axfr = 252,	/*%< Transfer zone of authority. */
	ns_t_mailb = 253,	/*%< Transfer mailbox records. */
	ns_t_maila = 254,	/*%< Transfer mail agent records. */
	ns_t_any = 255,		/*%< Wildcard match. */
	ns_t_zxfr = 256,	/*%< BIND-specific, nonstandard. */
	ns_t_max = 65536
} ns_type;

/* Exclusively a QTYPE? (not also an RTYPE) */
#define	ns_t_qt_p(t) (ns_t_xfr_p(t) || (t) == ns_t_any || \
		      (t) == ns_t_mailb || (t) == ns_t_maila)
/* Some kind of meta-RR? (not a QTYPE, but also not an RTYPE) */
#define	ns_t_mrr_p(t) ((t) == ns_t_tsig || (t) == ns_t_opt)
/* Exclusively an RTYPE? (not also a QTYPE or a meta-RR) */
#define ns_t_rr_p(t) (!ns_t_qt_p(t) && !ns_t_mrr_p(t))
#define ns_t_udp_p(t) ((t) != ns_t_axfr && (t) != ns_t_zxfr)
#define ns_t_xfr_p(t) ((t) == ns_t_axfr || (t) == ns_t_ixfr || \
		       (t) == ns_t_zxfr)

/*%
 * Values for class field
 */
typedef enum __ns_class {
	ns_c_invalid = 0,	/*%< Cookie. */
	ns_c_in = 1,		/*%< Internet. */
	ns_c_2 = 2,		/*%< unallocated/unsupported. */
	ns_c_chaos = 3,		/*%< MIT Chaos-net. */
	ns_c_hs = 4,		/*%< MIT Hesiod. */
	/* Query class values which do not appear in resource records */
	ns_c_none = 254,	/*%< for prereq. sections in update requests */
	ns_c_any = 255,		/*%< Wildcard match. */
	ns_c_max = 65536
} ns_class;

/* DNSSEC constants. */

typedef enum __ns_key_types {
	ns_kt_rsa = 1,		/*%< key type RSA/MD5 */
	ns_kt_dh  = 2,		/*%< Diffie Hellman */
	ns_kt_dsa = 3,		/*%< Digital Signature Standard (MANDATORY) */
	ns_kt_private = 254	/*%< Private key type starts with OID */
} ns_key_types;

typedef enum __ns_cert_types {
	cert_t_pkix = 1,	/*%< PKIX (X.509v3) */
	cert_t_spki = 2,	/*%< SPKI */
	cert_t_pgp  = 3,	/*%< PGP */
	cert_t_url  = 253,	/*%< URL private type */
	cert_t_oid  = 254	/*%< OID private type */
} ns_cert_types;

/* Flags field of the KEY RR rdata. */
#define	NS_KEY_TYPEMASK		0xC000	/*%< Mask for "type" bits */
#define	NS_KEY_TYPE_AUTH_CONF	0x0000	/*%< Key usable for both */
#define	NS_KEY_TYPE_CONF_ONLY	0x8000	/*%< Key usable for confidentiality */
#define	NS_KEY_TYPE_AUTH_ONLY	0x4000	/*%< Key usable for authentication */
#define	NS_KEY_TYPE_NO_KEY	0xC000	/*%< No key usable for either; no key */
/* The type bits can also be interpreted independently, as single bits: */
#define	NS_KEY_NO_AUTH		0x8000	/*%< Key unusable for authentication */
#define	NS_KEY_NO_CONF		0x4000	/*%< Key unusable for confidentiality */
#define	NS_KEY_RESERVED2	0x2000	/* Security is *mandatory* if bit=0 */
#define	NS_KEY_EXTENDED_FLAGS	0x1000	/*%< reserved - must be zero */
#define	NS_KEY_RESERVED4	0x0800  /*%< reserved - must be zero */
#define	NS_KEY_RESERVED5	0x0400  /*%< reserved - must be zero */
#define	NS_KEY_NAME_TYPE	0x0300	/*%< these bits determine the type */
#define	NS_KEY_NAME_USER	0x0000	/*%< key is assoc. with user */
#define	NS_KEY_NAME_ENTITY	0x0200	/*%< key is assoc. with entity eg host */
#define	NS_KEY_NAME_ZONE	0x0100	/*%< key is zone key */
#define	NS_KEY_NAME_RESERVED	0x0300	/*%< reserved meaning */
#define	NS_KEY_RESERVED8	0x0080  /*%< reserved - must be zero */
#define	NS_KEY_RESERVED9	0x0040  /*%< reserved - must be zero */
#define	NS_KEY_RESERVED10	0x0020  /*%< reserved - must be zero */
#define	NS_KEY_RESERVED11	0x0010  /*%< reserved - must be zero */
#define	NS_KEY_SIGNATORYMASK	0x000F	/*%< key can sign RR's of same name */
#define	NS_KEY_RESERVED_BITMASK ( NS_KEY_RESERVED2 | \
				  NS_KEY_RESERVED4 | \
				  NS_KEY_RESERVED5 | \
				  NS_KEY_RESERVED8 | \
				  NS_KEY_RESERVED9 | \
				  NS_KEY_RESERVED10 | \
				  NS_KEY_RESERVED11 )
#define NS_KEY_RESERVED_BITMASK2 0xFFFF /*%< no bits defined here */
/* The Algorithm field of the KEY and SIG RR's is an integer, {1..254} */
#define	NS_ALG_MD5RSA		1	/*%< MD5 with RSA */
#define	NS_ALG_DH               2	/*%< Diffie Hellman KEY */
#define	NS_ALG_DSA              3	/*%< DSA KEY */
#define	NS_ALG_DSS              NS_ALG_DSA
#define	NS_ALG_EXPIRE_ONLY	253	/*%< No alg, no security */
#define	NS_ALG_PRIVATE_OID	254	/*%< Key begins with OID giving alg */
/* Protocol values  */
/* value 0 is reserved */
#define NS_KEY_PROT_TLS         1
#define NS_KEY_PROT_EMAIL       2
#define NS_KEY_PROT_DNSSEC      3
#define NS_KEY_PROT_IPSEC       4
#define NS_KEY_PROT_ANY		255

/* Signatures */
#define	NS_MD5RSA_MIN_BITS	 512	/*%< Size of a mod or exp in bits */
#define	NS_MD5RSA_MAX_BITS	4096
	/* Total of binary mod and exp */
#define	NS_MD5RSA_MAX_BYTES	((NS_MD5RSA_MAX_BITS+7/8)*2+3)
	/* Max length of text sig block */
#define	NS_MD5RSA_MAX_BASE64	(((NS_MD5RSA_MAX_BYTES+2)/3)*4)
#define NS_MD5RSA_MIN_SIZE	((NS_MD5RSA_MIN_BITS+7)/8)
#define NS_MD5RSA_MAX_SIZE	((NS_MD5RSA_MAX_BITS+7)/8)

#define NS_DSA_SIG_SIZE         41
#define NS_DSA_MIN_SIZE         213
#define NS_DSA_MAX_BYTES        405

/* Offsets into SIG record rdata to find various values */
#define	NS_SIG_TYPE	0	/*%< Type flags */
#define	NS_SIG_ALG	2	/*%< Algorithm */
#define	NS_SIG_LABELS	3	/*%< How many labels in name */
#define	NS_SIG_OTTL	4	/*%< Original TTL */
#define	NS_SIG_EXPIR	8	/*%< Expiration time */
#define	NS_SIG_SIGNED	12	/*%< Signature time */
#define	NS_SIG_FOOT	16	/*%< Key footprint */
#define	NS_SIG_SIGNER	18	/*%< Domain name of who signed it */
/* How RR types are represented as bit-flags in NXT records */
#define	NS_NXT_BITS 8
#define	NS_NXT_BIT_SET(  n,p) (p[(n)/NS_NXT_BITS] |=  (0x80>>((n)%NS_NXT_BITS)))
#define	NS_NXT_BIT_CLEAR(n,p) (p[(n)/NS_NXT_BITS] &= ~(0x80>>((n)%NS_NXT_BITS)))
#define	NS_NXT_BIT_ISSET(n,p) (p[(n)/NS_NXT_BITS] &   (0x80>>((n)%NS_NXT_BITS)))
#define NS_NXT_MAX 127

/*%
 * EDNS0 extended flags and option codes, host order.
 */
#define NS_OPT_DNSSEC_OK        0x8000U
#define NS_OPT_NSID		3

/*%
 * Inline versions of get/put short/long.  Pointer is advanced.
 */
#define NS_GET16(s, cp) do { \
	register const u_char *t_cp = (const u_char *)(cp); \
	(s) = ((u_int16_t)t_cp[0] << 8) \
	    | ((u_int16_t)t_cp[1]) \
	    ; \
	(cp) += NS_INT16SZ; \
} while (0)

#define NS_GET32(l, cp) do { \
	register const u_char *t_cp = (const u_char *)(cp); \
	(l) = ((u_int32_t)t_cp[0] << 24) \
	    | ((u_int32_t)t_cp[1] << 16) \
	    | ((u_int32_t)t_cp[2] << 8) \
	    | ((u_int32_t)t_cp[3]) \
	    ; \
	(cp) += NS_INT32SZ; \
} while (0)

#define NS_PUT16(s, cp) do { \
	register u_int16_t t_s = (u_int16_t)(s); \
	register u_char *t_cp = (u_char *)(cp); \
	*t_cp++ = t_s >> 8; \
	*t_cp   = t_s; \
	(cp) += NS_INT16SZ; \
} while (0)

#define NS_PUT32(l, cp) do { \
	register u_int32_t t_l = (u_int32_t)(l); \
	register u_char *t_cp = (u_char *)(cp); \
	*t_cp++ = t_l >> 24; \
	*t_cp++ = t_l >> 16; \
	*t_cp++ = t_l >> 8; \
	*t_cp   = t_l; \
	(cp) += NS_INT32SZ; \
} while (0)

#endif /* !_ARPA_NAMESER_H_ */
/*! \file */



/*%
 *      from nameser.h	8.1 (Berkeley) 6/2/93
 *	$BINDId: nameser_compat.h,v 8.11 1999/01/02 08:00:58 vixie Exp $
 */

#ifndef _ARPA_NAMESER_COMPAT_
#define	_ARPA_NAMESER_COMPAT_

#define	__BIND		19950621	/*%< (DEAD) interface version stamp. */

#include <endian.h>

/*%
 * Structure for query header.  The order of the fields is machine- and
 * compiler-dependent, depending on the byte/bit order and the layout
 * of bit fields.  We use bit fields only in int variables, as this
 * is all ANSI requires.  This requires a somewhat confusing rearrangement.
 */

typedef struct {
	unsigned	id :16;		/*%< query identification number */
#if BYTE_ORDER == BIG_ENDIAN
			/* fields in third byte */
	unsigned	qr: 1;		/*%< response flag */
	unsigned	opcode: 4;	/*%< purpose of message */
	unsigned	aa: 1;		/*%< authoritive answer */
	unsigned	tc: 1;		/*%< truncated message */
	unsigned	rd: 1;		/*%< recursion desired */
			/* fields in fourth byte */
	unsigned	ra: 1;		/*%< recursion available */
	unsigned	unused :1;	/*%< unused bits (MBZ as of 4.9.3a3) */
	unsigned	ad: 1;		/*%< authentic data from named */
	unsigned	cd: 1;		/*%< checking disabled by resolver */
	unsigned	rcode :4;	/*%< response code */
#endif
#if BYTE_ORDER == LITTLE_ENDIAN || BYTE_ORDER == PDP_ENDIAN
			/* fields in third byte */
	unsigned	rd :1;		/*%< recursion desired */
	unsigned	tc :1;		/*%< truncated message */
	unsigned	aa :1;		/*%< authoritive answer */
	unsigned	opcode :4;	/*%< purpose of message */
	unsigned	qr :1;		/*%< response flag */
			/* fields in fourth byte */
	unsigned	rcode :4;	/*%< response code */
	unsigned	cd: 1;		/*%< checking disabled by resolver */
	unsigned	ad: 1;		/*%< authentic data from named */
	unsigned	unused :1;	/*%< unused bits (MBZ as of 4.9.3a3) */
	unsigned	ra :1;		/*%< recursion available */
#endif
			/* remaining bytes */
	unsigned	qdcount :16;	/*%< number of question entries */
	unsigned	ancount :16;	/*%< number of answer entries */
	unsigned	nscount :16;	/*%< number of authority entries */
	unsigned	arcount :16;	/*%< number of resource entries */
} HEADER;

#define PACKETSZ	NS_PACKETSZ
#define MAXDNAME	NS_MAXDNAME
#define MAXCDNAME	NS_MAXCDNAME
#define MAXLABEL	NS_MAXLABEL
#define	HFIXEDSZ	NS_HFIXEDSZ
#define QFIXEDSZ	NS_QFIXEDSZ
#define RRFIXEDSZ	NS_RRFIXEDSZ
#define	INT32SZ		NS_INT32SZ
#define	INT16SZ		NS_INT16SZ
#define INT8SZ		NS_INT8SZ
#define	INADDRSZ	NS_INADDRSZ
#define	IN6ADDRSZ	NS_IN6ADDRSZ
#define	INDIR_MASK	NS_CMPRSFLGS
#define NAMESERVER_PORT	NS_DEFAULTPORT

#define S_ZONE		ns_s_zn
#define S_PREREQ	ns_s_pr
#define S_UPDATE	ns_s_ud
#define S_ADDT		ns_s_ar

#define QUERY		ns_o_query
#define IQUERY		ns_o_iquery
#define STATUS		ns_o_status
#define	NS_NOTIFY_OP	ns_o_notify
#define	NS_UPDATE_OP	ns_o_update

#define NOERROR		ns_r_noerror
#define FORMERR		ns_r_formerr
#define SERVFAIL	ns_r_servfail
#define NXDOMAIN	ns_r_nxdomain
#define NOTIMP		ns_r_notimpl
#define REFUSED		ns_r_refused
#define YXDOMAIN	ns_r_yxdomain
#define YXRRSET		ns_r_yxrrset
#define NXRRSET		ns_r_nxrrset
#define NOTAUTH		ns_r_notauth
#define NOTZONE		ns_r_notzone
/*#define BADSIG		ns_r_badsig*/
/*#define BADKEY		ns_r_badkey*/
/*#define BADTIME		ns_r_badtime*/


#define DELETE		ns_uop_delete
#define ADD		ns_uop_add

#define T_A		ns_t_a
#define T_NS		ns_t_ns
#define T_MD		ns_t_md
#define T_MF		ns_t_mf
#define T_CNAME		ns_t_cname
#define T_SOA		ns_t_soa
#define T_MB		ns_t_mb
#define T_MG		ns_t_mg
#define T_MR		ns_t_mr
#define T_NULL		ns_t_null
#define T_WKS		ns_t_wks
#define T_PTR		ns_t_ptr
#define T_HINFO		ns_t_hinfo
#define T_MINFO		ns_t_minfo
#define T_MX		ns_t_mx
#define T_TXT		ns_t_txt
#define	T_RP		ns_t_rp
#define T_AFSDB		ns_t_afsdb
#define T_X25		ns_t_x25
#define T_ISDN		ns_t_isdn
#define T_RT		ns_t_rt
#define T_NSAP		ns_t_nsap
#define T_NSAP_PTR	ns_t_nsap_ptr
#define	T_SIG		ns_t_sig
#define	T_KEY		ns_t_key
#define	T_PX		ns_t_px
#define	T_GPOS		ns_t_gpos
#define	T_AAAA		ns_t_aaaa
#define	T_LOC		ns_t_loc
#define	T_NXT		ns_t_nxt
#define	T_EID		ns_t_eid
#define	T_NIMLOC	ns_t_nimloc
#define	T_SRV		ns_t_srv
#define T_ATMA		ns_t_atma
#define T_NAPTR		ns_t_naptr
#define T_A6		ns_t_a6
#define T_DNAME		ns_t_dname
#define	T_TSIG		ns_t_tsig
#define	T_IXFR		ns_t_ixfr
#define T_AXFR		ns_t_axfr
#define T_MAILB		ns_t_mailb
#define T_MAILA		ns_t_maila
#define T_ANY		ns_t_any

#define C_IN		ns_c_in
#define C_CHAOS		ns_c_chaos
#define C_HS		ns_c_hs
/* BIND_UPDATE */
#define C_NONE		ns_c_none
#define C_ANY		ns_c_any

#define	GETSHORT		NS_GET16
#define	GETLONG			NS_GET32
#define	PUTSHORT		NS_PUT16
#define	PUTLONG			NS_PUT32

#endif /* _ARPA_NAMESER_COMPAT_ */
/*! \file */