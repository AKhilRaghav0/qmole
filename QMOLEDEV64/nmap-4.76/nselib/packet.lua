--- Facilities for manipulating raw packets.
-- @copyright See nmaps COPYING for license
-- @author Marek Majkowski <majek04+nse@gmail.com>

module(... or "packet" ,package.seeall)

require "bit"


----------------------------------------------------------------------------------------------------------------
-- extract number from binary string
function u8(b, i)
        return string.byte(b, i+1)
end
function u16(b, i)
        local b1,b2
        b1, b2 = string.byte(b, i+1), string.byte(b, i+2)
        --        2^8     2^0
        return b1*256 + b2
end
function u32(b,i)
        local b1,b2,b3,b4
        b1, b2 = string.byte(b, i+1), string.byte(b, i+2)
        b3, b4 = string.byte(b, i+3), string.byte(b, i+4)
        --        2^24          2^16       2^8     2^0
        return b1*16777216 + b2*65536 + b3*256 + b4
end

-- insert number to binary string
function set_u8(b, i, num)
	local s = string.char(bit.band(num, 0xff))
	return b:sub(0+1, i+1-1) .. s .. b:sub(i+1+1)
end
function set_u16(b, i, num)
	local s = string.char(bit.band(bit.rshift(num, 8), 0xff)) .. string.char(bit.band(num, 0xff))
	return b:sub(0+1, i+1-1) .. s .. b:sub(i+1+2)
end
function set_u32(b,i, num)
	local s = string.char(bit.band(bit.rshift(num,24), 0xff)) ..
		string.char(bit.band(bit.rshift(num,16), 0xff)) ..
		string.char(bit.band(bit.rshift(num,8), 0xff)) ..
		string.char(bit.band(num, 0xff))
	return b:sub(0+1, i+1-1) .. s .. b:sub(i+1+4)
end


-- Checksum
----  Standard BSD internet checksum routine  check nmap/tcpip.cc
function in_cksum(b)
	local sum = 0
	local c
	local x = b

	while x:len() > 1 do
		c = x:sub(1,2)
		x = x:sub(3)
		sum = sum + u16(c, 0)
	end

	sum = bit.rshift(sum, 16) + bit.band(sum, 0xffff)
	sum = sum + bit.rshift(sum, 16)
	sum = bit.bnot(sum)
	sum = bit.band(sum, 0xffff) -- trunctate to 16 bits
	return sum
end

-- ip protocol field
IPPROTO_IP   = 0               	--  Dummy protocol for TCP
IPPROTO_ICMP = 1             	--  Internet Control Message Protocol
IPPROTO_IGMP = 2             	--  Internet Group Management Protocol
IPPROTO_IPIP = 4             	--  IPIP tunnels (older KA9Q tunnels use 94)
IPPROTO_TCP  = 6              	--  Transmission Control Protocol
IPPROTO_EGP  = 8              	--  Exterior Gateway Protocol
IPPROTO_PUP  = 12             	--  PUP protocol
IPPROTO_UDP  = 17             	--  User Datagram Protocol
IPPROTO_IDP  = 22             	--  XNS IDP protocol
IPPROTO_DCCP = 33            	--  Datagram Congestion Control Protocol
IPPROTO_RSVP = 46            	--  RSVP protocol
IPPROTO_GRE  = 47             	--  Cisco GRE tunnels (rfc 1701,1702)
IPPROTO_IPV6 = 41          	--  IPv6-in-IPv4 tunnelling

IPPROTO_ESP  = 50            	--  Encapsulation Security Payload protocol
IPPROTO_AH   = 51             	--  Authentication Header protocol
IPPROTO_BEETPH  = 94         	--  IP option pseudo header for BEET
IPPROTO_PIM     = 103         	--  Protocol Independent Multicast

IPPROTO_COMP    = 108        	--  Compression Header protocol
IPPROTO_SCTP    = 132         	--  Stream Control Transport Protocol
IPPROTO_UDPLITE = 136        	--  UDP-Lite (RFC 3828)


----------------------------------------------------------------------------------------------------------------
-- Packet is a class
Packet = {}
-- Constructor
-- packet          - binary string with packet data
-- packet_len      - packet length, it could be more than string.len(packet)
-- force_continue  - whether error in parsing headers should be fatal or not.
--                   especially usefull at parsing icmp packets, where on small icmp payload
--                   could be tcp header. The problem is that parsing this payload normally
--                   would fail (broken packet, because tcp header is too small)
--                   The basic question is if too short tcp header should be treated as fatal error.
function Packet:new(packet, packet_len, force_continue)
	local o = setmetatable({}, {__index = Packet})
	o.buf		= packet
	o.packet_len	= packet_len
	if not o:ip_parse(force_continue) then
		return nil
	end
	if o.ip_p == IPPROTO_TCP then
		if not o:tcp_parse(force_continue) then
			io.write("Error while parsing TCP packet\n")
		end
	elseif o.ip_p == IPPROTO_ICMP then
		if not o:icmp_parse(force_continue) then
			io.write("Error while parsing ICMP packet\n")
		end
	end
	return o
end

-- Helpers


-- from ip notation as string (like 1.2.3.4) to raw_string(4 bytes long)
function iptobin(str)
	local ret = ""
        for c in string.gmatch(str, "[0-9]+") do
                ret = ret .. string.char(c+0) -- automatic conversion to int
        end
	return ret
end
-- from raw_ip (four bytes string) to dot-notation (like 1.2.3.4)
function toip(raw_ip_addr)
	if not raw_ip_addr then
		return "?.?.?.?"
	end
	return string.format("%i.%i.%i.%i", string.byte(raw_ip_addr,1,4))
end
-- get unsigned byte
function Packet:u8(index)
        return u8(self.buf, index)
end
function Packet:u16(index)
        return u16(self.buf, index)
end
function Packet:u32(index)
        return u32(self.buf, index)
end
function Packet:raw(index, length)
        return string.char(string.byte(self.buf, index+1, index+1+length-1))
end

function Packet:set_u8(index, num)
        self.buf = set_u8(self.buf, index, num)
        return self.buf
end
function Packet:set_u16(index, num)
        self.buf = set_u16(self.buf, index, num)
        return self.buf
end
function Packet:set_u32(index, num)
        self.buf = set_u32(self.buf, index, num)
        return self.buf
end

-- PARSE IP PACKET HEADER
function Packet:ip_parse(force_continue)
	self.ip_offset		= 0
	if    string.len(self.buf) < 20 then 	-- too short
		return false
	end
	self.ip_v		= bit.rshift(bit.band(self:u8(self.ip_offset + 0), 0xF0), 4)
	self.ip_hl		=            bit.band(self:u8(self.ip_offset + 0), 0x0F)		-- header_length or data_offset
	if    self.ip_v ~= 4 then 	-- not ip
		return false
	end
	self.ip = true
	self.ip_tos		= self:u8(self.ip_offset + 1)
	self.ip_len		= self:u16(self.ip_offset + 2)
	self.ip_id		= self:u16(self.ip_offset + 4)
	self.ip_off		= self:u16(self.ip_offset + 6)
	self.ip_rf		= bit.band(self.ip_off, 0x8000)~=0		-- true/false
	self.ip_df		= bit.band(self.ip_off, 0x4000)~=0
	self.ip_mf		= bit.band(self.ip_off, 0x2000)~=0
	self.ip_off		= bit.band(self.ip_off, 0x1FFF)		-- fragment offset
	self.ip_ttl		= self:u8(self.ip_offset + 8)
	self.ip_p		= self:u8(self.ip_offset + 9)
	self.ip_sum		= self:u16(self.ip_offset + 10)
	self.ip_bin_src		= self:raw(self.ip_offset + 12,4)	-- raw 4-bytes string
	self.ip_bin_dst		= self:raw(self.ip_offset + 16,4)
	self.ip_src		= toip(self.ip_bin_src)		-- formatted string
	self.ip_dst		= toip(self.ip_bin_dst)
	self.ip_opt_offset	= self.ip_offset + 20
	self.ip_options		= self:parse_options(self.ip_opt_offset, ((self.ip_hl*4)-20))
	self.ip_data_offset	= self.ip_offset + self.ip_hl*4
	return true
end
-- set header length field
function Packet:ip_set_hl(len)
	self:set_u8(self.ip_offset + 0, bit.bor(bit.lshift(self.ip_v, 4), bit.band(len, 0x0F)))
	self.ip_v		= bit.rshift(bit.band(self:u8(self.ip_offset + 0), 0xF0), 4)
	self.ip_hl		=            bit.band(self:u8(self.ip_offset + 0), 0x0F)		-- header_length or data_offset
end
-- set packet length field
function Packet:ip_set_len(len)
	self:set_u16(self.ip_offset + 2, len)
end
-- set ttl
function Packet:ip_set_ttl(ttl)
	self:set_u8(self.ip_offset + 8, ttl)
end
-- set checksum
function Packet:ip_set_checksum(checksum)
	self:set_u16(self.ip_offset + 10, checksum)
end
-- count checksum for packet and save it
function Packet:ip_count_checksum()
	self:ip_set_checksum(0)
	local csum = in_cksum( self.buf:sub(0, self.ip_offset + self.ip_hl*4)  )
	self:ip_set_checksum(csum)
end
-- set source ip
function Packet:ip_set_bin_src(binip)
	nrip = u32(binip, 0)
	self:set_u32(self.ip_offset + 12, nrip)
	self.ip_bin_src		= self:raw(self.ip_offset + 12,4)	-- raw 4-bytes string
end
-- set destination ip
function Packet:ip_set_bin_dst(binip)
	nrip = u32(binip, 0)
	self:set_u32(self.ip_offset + 16, nrip)
	self.ip_bin_dst		= self:raw(self.ip_offset + 16,4)
end
-- set ip options field (and move the data, count new length etc)
function Packet:ip_set_options(ipoptions)
	-- packet = <ip header> + ipoptions + <payload>
	local buf = self.buf:sub(0+1,self.ip_offset + 20) .. ipoptions .. self.buf:sub(self.ip_data_offset+1)
	self.buf = buf
	-- set ip_len
	self:ip_set_len(self.buf:len())
	-- set ip_hl
	self:ip_set_hl(5 + ipoptions:len()/4)
	-- set data offset correctly
	self.ip_options		= self:parse_options(self.ip_opt_offset, ((self.ip_hl*4)-20))
	self.ip_data_offset	= self.ip_offset + self.ip_hl*4
	if self.tcp then
		self.tcp_offset		= self.ip_data_offset
	elseif self.icmp then
		self.icmp_offset	= self.ip_data_offset
	end
end

-- return short information about ip header
function Packet:ip_tostring()
	return string.format(
		"IP %s -> %s",
		self.ip_src,
		self.ip_dst)
end

-- parse ip/tcp options to dict structure
function Packet:parse_options(offset, length)
	local options = {}
	local op = 1
	local opt_ptr = 0
	while opt_ptr < length do
		local t, l, d
		options[op] = {}

		t = self:u8(offset + opt_ptr)
		options[op].type = t
		if t==0 or t==1 then
			l = 1
			d = nil
		else
			l = self:u8(offset + opt_ptr + 1)
			if l > 2 then
			d = self:raw(offset + opt_ptr + 2, l-2)
			end
		end
		options[op].len  = l
		options[op].data = d
		opt_ptr = opt_ptr + l
		op = op + 1
	end
	return options
end

-- print short information about current packet
function Packet:tostring()
	if self.tcp then
		return self:tcp_tostring()
	elseif self.icmp then
		return self:icmp_tostring()
	elseif self.ip then
		return self:ip_tostring()
	end
	return "<no tostring!>"
end

----------------------------------------------------------------------------------------------------------------
-- PARSE ICMP PACKET HEADER
function Packet:icmp_parse(force_continue)
	self.icmp_offset	= self.ip_data_offset
	if string.len(self.buf) < self.icmp_offset + 8 then -- let's say 8 bytes minimum
		return false
	end
	self.icmp = true
	self.icmp_type		= self:u8(self.icmp_offset + 0)
	self.icmp_code		= self:u8(self.icmp_offset + 1)
	self.icmp_sum		= self:u16(self.icmp_offset + 2)

	if self.icmp_type == 3 or self.icmp_type == 4 or self.icmp_type == 11 or self.icmp_type == 12 then
		self.icmp_payload = true
		self.icmp_r0	  = self:u32(self.icmp_offset + 4)
		self.icmp_payload_offset = self.icmp_offset + 8
		if string.len(self.buf) < self.icmp_payload_offset + 24 then
			return false
		end
		self.icmp_payload = Packet:new(self.buf:sub(self.icmp_payload_offset+1), self.packet_len - self.icmp_payload_offset, true)
	end
	return true
end
-- return short information about icmp header
function Packet:icmp_tostring()
	return self:ip_tostring() .. " ICMP(" .. self.icmp_payload:tostring() .. ")"
end

----------------------------------------------------------------------------------------------------------------
-- PARSE TCP HEADER FROM PACKET
function Packet:tcp_parse(force_continue)
	self.tcp = true
	self.tcp_offset		= self.ip_data_offset
	if string.len(self.buf) < self.tcp_offset + 4 then
		return false
	end
	self.tcp_sport		= self:u16(self.tcp_offset + 0)
	self.tcp_dport		= self:u16(self.tcp_offset + 2)
	if string.len(self.buf) < self.tcp_offset + 20 then
		if force_continue then
			return true
		else
			return false
		end
	end
	self.tcp_seq		= self:u32(self.tcp_offset + 4)
	self.tcp_ack		= self:u32(self.tcp_offset + 8)
	self.tcp_hl		= bit.rshift(bit.band(self:u8(self.tcp_offset+12), 0xF0), 4)	-- header_length or data_offset
	self.tcp_x2		=            bit.band(self:u8(self.tcp_offset+12), 0x0F)
	self.tcp_flags		= self:u8(self.tcp_offset + 13)
	self.tcp_th_fin		= bit.band(self.tcp_flags, 0x01)~=0		-- true/false
	self.tcp_th_syn		= bit.band(self.tcp_flags, 0x02)~=0
	self.tcp_th_rst		= bit.band(self.tcp_flags, 0x04)~=0
	self.tcp_th_push	= bit.band(self.tcp_flags, 0x08)~=0
	self.tcp_th_ack		= bit.band(self.tcp_flags, 0x10)~=0
	self.tcp_th_urg		= bit.band(self.tcp_flags, 0x20)~=0
	self.tcp_th_ece		= bit.band(self.tcp_flags, 0x40)~=0
	self.tcp_th_cwr		= bit.band(self.tcp_flags, 0x80)~=0
	self.tcp_win		= self:u16(self.tcp_offset + 14)
	self.tcp_sum		= self:u16(self.tcp_offset + 16)
	self.tcp_urp		= self:u16(self.tcp_offset + 18)
	self.tcp_opt_offset	= self.tcp_offset + 20
	self.tcp_options	= self:parse_options(self.tcp_opt_offset, ((self.tcp_hl*4)-20))
	self.tcp_data_offset	= self.tcp_offset + self.tcp_hl*4
	self.tcp_data_length	= self.ip_len - self.tcp_offset - self.tcp_hl*4
        self:tcp_parse_options()
	return true
end

-- return short information about tcp packet
function Packet:tcp_tostring()
	return string.format(
		"TCP %s:%i -> %s:%i",
		self.ip_src, self.tcp_sport,
		self.ip_dst, self.tcp_dport
		)
end

-- parse options for tcp header
function Packet:tcp_parse_options()
        local eoo = false
	for _,opt in ipairs(self.tcp_options) do
                if eoo then
                        self.tcp_opt_after_eol = true
                end

		if      opt.type == 0 then    -- end of options
                        eoo = true
                elseif 	opt.type == 2 then    -- MSS
                        self.tcp_opt_mss = u16(opt.data, 0)
                        self.tcp_opt_mtu = self.tcp_opt_mss + 40
		elseif	opt.type == 3 then     -- widow scaling
                        self.tcp_opt_ws  = u8(opt.data, 0)
		elseif	opt.type == 8 then     -- timestamp
                        self.tcp_opt_t1 = u32(opt.data, 0)
                        self.tcp_opt_t2 = u32(opt.data, 4)
		end
	end
end

function Packet:tcp_set_sport(port)
	self:set_u16(self.tcp_offset + 0, port)
end
function Packet:tcp_set_dport(port)
	self:set_u16(self.tcp_offset + 2, port)
end
-- set tcp sequence field
function Packet:tcp_set_seq(new_seq)
	self:set_u32(self.tcp_offset + 4, new_seq)
end
-- set tcp flags field (like syn, ack, rst)
function Packet:tcp_set_flags(new_flags)
	self:set_u8(self.tcp_offset + 13, new_flags)
end
-- set urgent pointer field
function Packet:tcp_set_urp(urg_ptr)
	self:set_u16(self.tcp_offset + 18, urg_ptr)
end
-- set tcp checksum field
function Packet:tcp_set_checksum(checksum)
	self:set_u16(self.tcp_offset + 16, checksum)
end
-- count and save tcp checksum field
function Packet:tcp_count_checksum()
	self:tcp_set_checksum(0)
	local proto	= self.ip_p
	local length	= self.buf:len() - self.tcp_offset
	local b = self.ip_bin_src ..
		self.ip_bin_dst ..
		string.char(0) ..
		string.char(proto) ..
		set_u16("..", 0, length) ..
		self.buf:sub(self.tcp_offset+1)

	self:tcp_set_checksum(in_cksum(b))
end

-- small database, mtu to link type string. Stolen from p0f.
function Packet:tcp_lookup_link()
        local mtu_def = {
            {["mtu"]=256,   ["txt"]= "radio modem"},
            {["mtu"]=386,   ["txt"]= "ethernut"},
            {["mtu"]=552,   ["txt"]= "SLIP line / encap ppp"},
            {["mtu"]=576,   ["txt"]= "sometimes modem"},
            {["mtu"]=1280,  ["txt"]= "gif tunnel"},
            {["mtu"]=1300,  ["txt"]= "PIX, SMC, sometimes wireless"},
            {["mtu"]=1362,  ["txt"]= "sometimes DSL (1)"},
            {["mtu"]=1372,  ["txt"]= "cable modem"},
            {["mtu"]=1400,  ["txt"]= "(Google/AOL)"},
            {["mtu"]=1415,  ["txt"]= "sometimes wireless"},
            {["mtu"]=1420,  ["txt"]= "GPRS, T1, FreeS/WAN"},
            {["mtu"]=1423,  ["txt"]= "sometimes cable"},
            {["mtu"]=1440,  ["txt"]= "sometimes DSL (2)"},
            {["mtu"]=1442,  ["txt"]= "IPIP tunnel"},
            {["mtu"]=1450,  ["txt"]= "vtun"},
            {["mtu"]=1452,  ["txt"]= "sometimes DSL (3)"},
            {["mtu"]=1454,  ["txt"]= "sometimes DSL (4)"},
            {["mtu"]=1456,  ["txt"]= "ISDN ppp"},
            {["mtu"]=1458,  ["txt"]= "BT DSL (?)"},
            {["mtu"]=1462,  ["txt"]= "sometimes DSL (5)"},
            {["mtu"]=1470,  ["txt"]= "(Google 2)"},
            {["mtu"]=1476,  ["txt"]= "IPSec/GRE"},
            {["mtu"]=1480,  ["txt"]= "IPv6/IPIP"},
            {["mtu"]=1492,  ["txt"]= "pppoe (DSL)"},
            {["mtu"]=1496,  ["txt"]= "vLAN"},
            {["mtu"]=1500,  ["txt"]= "ethernet/modem"},
            {["mtu"]=1656,  ["txt"]= "Ericsson HIS"},
            {["mtu"]=2024,  ["txt"]= "wireless/IrDA"},
            {["mtu"]=2048,  ["txt"]= "Cyclom X.25 WAN"},
            {["mtu"]=2250,  ["txt"]= "AiroNet wireless"},
            {["mtu"]=3924,  ["txt"]= "loopback"},
            {["mtu"]=4056,  ["txt"]= "token ring (1)"},
            {["mtu"]=4096,  ["txt"]= "Sangoma X.25 WAN"},
            {["mtu"]=4352,  ["txt"]= "FDDI"},
            {["mtu"]=4500,  ["txt"]= "token ring (2)"},
            {["mtu"]=9180,  ["txt"]= "FORE ATM"},
            {["mtu"]=16384, ["txt"]= "sometimes loopback (1)"},
            {["mtu"]=16436, ["txt"]= "sometimes loopback (2)"},
            {["mtu"]=18000, ["txt"]= "token ring x4"},
            }
        if not self.tcp_opt_mss or self.tcp_opt_mss==0 then
                return "unspecified"
        end
        for _,x in ipairs(mtu_def) do
                local mtu = x["mtu"]
                local txt = x["txt"]
                if self.tcp_opt_mtu == mtu then
                        return txt
                end
                if self.tcp_opt_mtu < mtu then
                        return string.format("unknown-%i", self.tcp_opt_mtu)
                end
        end
        return string.format("unknown-%i", self.tcp_opt_mtu)
end

----------------------------------------------------------------------------------------------------------------

-- UTILS
-- get binary string  as hex string
function bintohex(str)
        local b = ""
        if not str then -- nil
        	return ""
        end
        for c in string.gmatch(str, ".") do
                b = string.format('%s%02x',b, string.byte(c))
        end
        return b
end



-- Parse specifically printed hex string as binary
-- Only bytes [a-f0-9A-F] from input are interpreted. The rest is ignored.
-- Number of interpreted bytes _must_ be even. *The input is interpreted in pairs*.
-- hextobin("20 20 20")		-> "   "
-- hextobin("414243")		-> "ABC"
-- hextobin("\\41\\42\\43")	-> "ABC"
-- hextobin("   41   42    43  ")-> "ABC"
function hextobin(str)
        local ret = ""
        local a,b
        if not str then -- nil
        	return ""
        end
        for c in string.gmatch(str, "[0-9a-fA-F][0-9a-fA-F]") do
        	a = string.byte(c:sub(1,1))
        	b = string.byte(c:sub(2,2))
        	if     a >= string.byte('a') then -- 97>a-f
        		a = a - string.byte('a') + 10
        	elseif a >= string.byte('A') then -- 65>A-F
        		a = a - string.byte('A') + 10
        	else -- 48> 0-9
        		a = a - string.byte('0')
        	end
        	if     b >= string.byte('a') then -- 97>a-f
        		b = b - string.byte('a') + 10
        	elseif b >= string.byte('A') then -- 65>A-F
        		b = b - string.byte('A') + 10
        	else -- 48> 0-9
        		b = b - string.byte('0')
        	end
        	--io.write(string.format(">%s %i %i\n",c, a, b))
                ret = ret .. string.char(a*16 + b)
        end

        --io.write(string.format(">%s|%s<\n",bintohex(ret), str))
        return ret
end



