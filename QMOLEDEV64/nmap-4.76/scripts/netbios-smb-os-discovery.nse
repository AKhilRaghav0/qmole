--- This script probes a target for its operating system version.
-- It sends traffic via UDP port 137 and TCP port 139/445.\n\n
-- == Implementation Information ==\n
-- First, we need to
-- elicit the NetBIOS share name associated with a workstation share.
-- Once we have that, we need to encode the name into the "mangled"
-- equivalent and send TCP 139/445 traffic to connect to the host and
-- in an attempt to elicit the OS version name from an SMB Setup AndX
-- response.\n\n
--
-- Thanks to Michail Prokopyev and xSharez Scanner for required 
-- traffic to generate for OS version detection.
--
--@usage
-- sudo nmap -sU -sS --script netbios-smb-os-discovery.nse -p U:137,T:139 127.0.0.1
-----------------------------------------------------------------------

id = "Discover OS Version over NetBIOS and SMB"
description = "Attempt to elicit OS version from host running NetBIOS/SMB"
author = "Judy Novak"
copyright = "Sourcefire Inc, (C) 2006-2007"
license = "Same as Nmap--See http://nmap.org/book/man-legal.html"
categories = {"version"}

require 'bit'
 
hostrule = function(host)

    -- This script should run under two different conditions:
    -- a) port tcp/445 is open (allowing us to make a raw connection)
    -- b) ports tcp/139 and udp/137 are open (137 may not be known)

   local port_u137 = nmap.get_port_state(host, {number=137, protocol="udp"})
   local port_t139 = nmap.get_port_state(host, {number=139, protocol="tcp"})
   local port_t445 = nmap.get_port_state(host, {number=445, protocol="tcp"})

   if(port_t445 ~= nil and port_t445.state == "open") then
       -- tcp/445 is open, we're good
       return true
   end

   if(port_t139 ~= nil and port_t139.state == "open") then
       -- tcp/139 is open, check uf udp/137 is open or unknown
       if(port_u137 == nil or port_u137.state == "open" or port_u137.state == "open|filtered") then
           return true
       end
   end

   return false
end

action = function(host)
   local sharename, message, osversion, currenttime, gen_msg, gen_msg_time, x

   sharename = 0
   osversion = ""
   gen_msg = "OS version cannot be determined.\n"
   gen_msg_time = "System time cannot be determined.\n"

   -- Decide whether to use raw SMB (port 445) or SMB over NetBIOS (139). 
   -- Raw is better, because it uses one less packet and doesn't require a 
   -- name to be known. 
   local port_t445 = nmap.get_port_state(host, {number=445, protocol="tcp"})

   local use_raw = (port_t445 ~= nil and port_t445.state == "open")

   if(not use_raw) then
      sharename, message = udp_query(host)
   end

   local ret = ""

   if (use_raw or sharename ~= 0)  then
       osversion, currenttime, message = tcp_session(sharename, host, use_raw)
       if (osversion ~= 0) then
           ret = ret .. osversion
           if(currenttime ~= 0) then
              ret = ret .. "\n" ..  "Discover system time over SMB: " .. currenttime
           else
              ret = ret .. "\n" .. gen_msg_time .. message
           end
       else
           ret = ret .. gen_msg .. message
       end
   else
      ret = ret .. gen_msg .. "TCP/445 closed and couldn't determine NetBIOS name"
   end

   return ret

end

-----------------------------------------------------------------------
-- A NetBIOS wildcard query is sent to a host in an attempt to discover
-- any NetBIOS shares on the host.

function udp_query(host)

       local l, sharename, message
       local WildCard = 
       string.char(0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
                   0x00, 0x00, 0x20, 0x43, 0x4b, 0x41, 0x41, 0x41, 0x41, 0x41, 
                   0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
                   0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 
                   0x41, 0x41, 0x41, 0x41, 0x41, 0x00, 0x00, 0x21, 0x00, 0x00)

       local socket = nmap.new_socket()

       socket:connect(host.ip, 137, "udp")
       socket:send(WildCard)
       socket:set_timeout(100)

       local status, result = socket:receive_bytes(1);

       socket:close()

       if (result ~= nil) then
           l = string.len(result)
           sharename = extract_sharename(result)
           if (sharename ~= 0)  then
              return sharename, 1
           else
              message = "Failed to find NetBIOS share name in response to UDP NetBIOS wildcard query"
              return 0, message
           end
       end
end

-----------------------------------------------------------------------
-- This function extracts the name of a "workstation" share from the
-- response to the UDP NetBIOS wildcard query.  Typically, there are
-- several share types returned, but only one with a "workstation"
-- type/code can be queried later for the OS version.  The workstation
-- type/code is 0x44 0x00 for OS versions prior to Vista.  The type/code
-- for Vista is 0x04 0x00.  

function extract_sharename(resp)
 
    local lenpay, beg, eend, typebeg, typeend, temp, name, nametype, ntgeneric, ntvista, ename, myname, eename, ntunix

    beg = 58
    eend = beg + 15
    typebeg = eend + 1
    lenpay = string.len(resp)

    while (eend <= lenpay) do

        myname = string_concatenate(resp, beg, eend - 1)
        nametype = string.byte(resp, typebeg) .. string.byte(resp, typebeg + 1)
        ntgeneric = string.find(nametype, 0x44,0x00) 
        ntvista = string.find(nametype, 0x04, 0x00)
        ntunix = string.find(nametype, 0x64, 0x00)

        if  (ntgeneric == 1) or (ntvista == 1) or (ntunix == 1) then
           ename = encode(myname)
        end

        if (ename ~= nil) then 
            do
              ename = string.char(0x20) .. ename .. string.char(0x43, 0x41, 0x00) 
              return(ename)
            end
        end

        beg = beg + 18
        eend = beg + 15
        typebeg = eend + 1
    end
    return(0)
end

-----------------------------------------------------------------------
-- Extract multiple bytes from a string and return concatenated result

function string_concatenate(mystring, start, stop)
    local x, temp, newname

    for x = start, stop, 1 do
        temp = string.byte(mystring,x)
        if (x > start) then
            newname = newname .. string.char(temp)
          else
            newname = string.char(temp)
        end
    end
    return(newname)
end

-----------------------------------------------------------------------
-- This function encodes the workstation share name returned from the
-- UDP wildcard NetBIOS query. Each character from the NetBIOS share
-- name is encoded/mangled using a special algorithm.  Rather than 
-- implementing the algorithm, Microsoft offers a conversion table for
-- any valid character found in a share name. I could not figure out
-- how to use a Lua dictionary where the key value included a 
-- non-alphanumeric character.  The static variable chars represents
-- most of the characters that can be found in a share and the position
-- in the string "chars" is the corresponding position in the trtable
-- table. The character " had to be handled separately as it is used
-- to delimit the value of chars.  

encode = function(name)

    local ln, y, nchar, newname, pos, temp, trtable
    local chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 !#$%&'()*+,-.=:;@^_{}~"

    local trtable = 
    {
     string.char(0x45,0x42), string.char(0x45,0x43), string.char(0x45,0x44), string.char(0x45,0x45), string.char(0x45,0x46),
     string.char(0x45,0x47), string.char(0x45,0x48), string.char(0x45,0x49), string.char(0x45,0x4A), string.char(0x45,0x4B),
     string.char(0x45,0x4C), string.char(0x45,0x4D), string.char(0x45,0x4E), string.char(0x45,0x4F), string.char(0x45,0x50),
     string.char(0x46,0x41), string.char(0x46,0x42), string.char(0x46,0x43), string.char(0x46,0x44), string.char(0x46,0x45),
     string.char(0x46,0x46), string.char(0x46,0x47), string.char(0x46,0x48), string.char(0x46,0x49), string.char(0x46,0x4A),
     string.char(0x46,0x4B), string.char(0x44,0x41), string.char(0x44,0x42), string.char(0x44,0x43), string.char(0x44,0x44),
     string.char(0x44,0x45), string.char(0x44,0x46), string.char(0x44,0x47), string.char(0x44,0x48), string.char(0x44,0x49),
     string.char(0x44,0x4A), string.char(0x43,0x41), string.char(0x43,0x42), string.char(0x43,0x44), string.char(0x43,0x45),
     string.char(0x43,0x46), string.char(0x43,0x47), string.char(0x43,0x48), string.char(0x43,0x49), string.char(0x43,0x4A),
     string.char(0x43,0x4B), string.char(0x43,0x4C), string.char(0x43,0x4D), string.char(0x43,0x4E), string.char(0x43,0x4F),
     string.char(0x44,0x4E), string.char(0x44,0x4B), string.char(0x44,0x4C), string.char(0x45,0x41), string.char(0x46,0x4F),
     string.char(0x46,0x50), string.char(0x48,0x4C), string.char(0x48,0x4E), string.char(0x48,0x4F)
     }

     ln = string.len(name)
     y = 1

     while (y <= ln) do 
         temp = string.byte(name, y)

         if (temp == 0x00) then  --Sharename must be followed by spaces not null's to be acceptable
               return(nil)
             elseif (temp == '"') then
               nchar = string.char(0x43,0x43)
             else do
               temp = string.char(temp)
               pos = string.find(chars, temp)
               nchar = trtable[pos] 
               if (y > 1) then
                  newname = newname .. nchar 
               else
                  newname = nchar
               end 
               y = y + 1     
             end
         end
     end
     return(newname)
end

-----------------------------------------------------------------------
-- This function invokes the TCP traffic that is generated to get
-- a response that yields the OS version information.  The first
-- payload is an SMB session initiation request followed by a 
-- negotiate payload, and followed by a Session Setup AndX request.
-- The workstation share name extracted from the UDP wildcard NetBIOS
-- response must be used in the SMB session initiation request(payload 1).
-- Payload for the requests that follow is static.

function tcp_session(ename, host, use_raw)

    local catch = function()
                socket:close()
    end

    local rec1_payload, rec2_payload, rec3_payload, status, line1, line2, line3, currenttime, osversion, winshare, pos, message
 
    message = 0
    local win5 =  "Windows 5.0"
    local win51 = "Windows 5.1"

    winshare = string.char(0x20, 0x46, 0x48, 0x45, 0x4A, 0x45, 0x4F, 0x45, 0x45, 0x45, 0x50, 0x46, 0x48, 0x46, 0x44, 0x43, 0x41, 0x43, 0x41, 0x43, 0x41, 0x43, 0x41, 0x43, 0x41, 0x43, 0x41, 0x43, 0x41, 0x43, 0x41, 0x43, 0x41, 0x00)

   rec1_payload = string.char(0x81, 0x00, 0x00, 0x44) .. ename  ..  winshare

   rec2_payload = string.char( 0x00, 0x00, 0x00, 0x85, 0xff, 0x53, 0x4d, 0x42, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0 ) ..
                  string.char( 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0xfa ) ..
                  string.char( 0x00, 0x00, 0x17, 0x62, 0x00, 0x61, 0x00, 0x02, 0x50, 0x43, 0x20, 0x4e, 0x45, 0x54, 0x57, 0x4f ) ..
                  string.char( 0x52, 0x4b, 0x20, 0x50, 0x52, 0x4f, 0x47, 0x52, 0x41, 0x4d, 0x20, 0x31, 0x2e, 0x30, 0x00, 0x02 ) ..
                  string.char( 0x4c, 0x41, 0x4e, 0x4d, 0x41, 0x4e, 0x31, 0x2e, 0x30, 0x00, 0x02, 0x57, 0x69, 0x6e, 0x64, 0x6f ) ..
                  string.char( 0x77, 0x73, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x57, 0x6f, 0x72, 0x6b, 0x67, 0x72, 0x6f, 0x75, 0x70 ) ..
                  string.char( 0x73, 0x20, 0x33, 0x2e, 0x31, 0x61, 0x00, 0x02, 0x4c, 0x4d, 0x31, 0x2e, 0x32, 0x58, 0x30, 0x30 ) ..
                  string.char( 0x32, 0x00, 0x02, 0x4c, 0x41, 0x4e, 0x4d, 0x41, 0x4e, 0x32, 0x2e, 0x31, 0x00, 0x02, 0x4e, 0x54 ) ..
                  string.char( 0x20, 0x4c, 0x4d, 0x20, 0x30, 0x2e, 0x31, 0x32, 0x00)

   rec3_payload =  string.char( 0x00, 0x00, 0x00, 0xab, 0xff, 0x53, 0x4d, 0x42, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0 ) ..
                   string.char( 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0xfa ) ..
                   string.char( 0x00, 0x00, 0x17, 0x62, 0x0d, 0xff, 0x00, 0x00, 0x00, 0x04, 0x11, 0x0a, 0x00, 0x00, 0x00, 0x00 ) ..
                   string.char( 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x6d ) ..
                   string.char( 0x00, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x69, 0x00, 0x63, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x73, 0x00 ) ..
                   string.char( 0x6f, 0x00, 0x66, 0x00, 0x74, 0x00, 0x00, 0x00, 0x57, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x64, 0x00 ) ..
                   string.char( 0x6f, 0x00, 0x77, 0x00, 0x73, 0x00, 0x20, 0x00, 0x39, 0x00, 0x35, 0x00, 0x2f, 0x00, 0x39, 0x00 ) ..
                   string.char( 0x38, 0x00, 0x2f, 0x00, 0x4d, 0x00, 0x65, 0x00, 0x2f, 0x00, 0x4e, 0x00, 0x54, 0x00, 0x2f, 0x00 ) ..
                   string.char( 0x32, 0x00, 0x6b, 0x00, 0x2f, 0x00, 0x58, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00 ) ..
                   string.char( 0x53, 0x00, 0x68, 0x00, 0x61, 0x00, 0x72, 0x00, 0x65, 0x00, 0x7a, 0x00, 0x20, 0x00, 0x53, 0x00 ) ..
                   string.char( 0x63, 0x00, 0x61, 0x00, 0x6e, 0x00, 0x6e, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00, 0x00, 0x00)
 
   local socket = nmap.new_socket()
   local try = nmap.new_try(catch)  

   if(use_raw) then
      try(socket:connect(host.ip,445,"tcp"))
   else
      try(socket:connect(host.ip,139,"tcp"))
   end

   if(not use_raw) then
      socket:set_timeout(100)
      try(socket:send(rec1_payload))
      status, line1 = socket:receive_lines(1)
   
      if (not status) then
          socket:close()
          message = "Never received a response to SMB Session Request"
          return 0, 0, message
      end
   end

   socket:set_timeout(100)
   try(socket:send(rec2_payload))
   status, line2 = socket:receive_lines(1)

   if (not status) then
       socket:close()
       message = "Never received a response to SMB Negotiate Protocol Request"
       return 0, 0, message
   end

   currenttime, message = extract_time(line2);

   -- Check for an error parsing line2
   if(currenttime == 0) then
      return 0, 0, message
   end

   socket:set_timeout(100)
   try(socket:send(rec3_payload))
   status, line3 = socket:receive_lines(1)

   if (not status) then
       socket:close()
       message = "Never received a response to SMB Setup AndX Request"
       return 0, currenttime, message
   end
   socket:close()

   -- Check for an error parsing line3
   osversion, message = extract_version(line3)
   if (osversion ~= 0) then
       pos = string.find(osversion, win5) 
       if (pos ~= nil) then
           osversion = "Windows 2000"
       else
           pos = string.find(osversion, win51) 
           if (pos ~= nil) then
               osversion = "Windows XP"
           end
       end
   end 

   return  osversion, currenttime, message

end

-----------------------------------------------------------------------
-- Response from Session Setup AndX Request (TCP payload 3)
-- Must be SMB response.  Extract the OS version from it from a fixed
-- offset in the payload.

function extract_version(line)

    local temp, smb, ltemp, go, x, osversion, mychar, message

    smb = "SMB" .. string.char(0x73)
    temp = string_concatenate(line, 6, 9)
    message = 0
    
    if (temp ~= smb) then
        message = "Didn't find correct SMB record as a response to the Session Setup AndX request"
        return 0, message
    end

    ltemp = string.len(line)
    temp = string_concatenate(line, 47, ltemp)
    x=1

    osversion = ""
    while (x < ltemp) do
        mychar = string.byte(temp,x)
        if (mychar == 0) then
            return osversion, message
        else
            osversion = osversion .. string.char(mychar)
        end
        x = x + 2
    end

    if (x >= ltemp) then 
        message = "OS version not found in expected record Session Setup AndX response"
        return 0, message
    end
    
end

-----------------------------------------------------------------------
-- Response from Negotiate Protocol Response (TCP payload 2)
-- Must be SMB response.  Extract the time from it from a fixed
-- offset in the payload.

function extract_time(line)

    local smb, tmp, message, i, timebuf, timezonebuf, time, timezone

    message = 0

    if(string.sub(line, 6, 8) ~= "SMB") then
        message = "Didn't find correct SMB record as a response to the Negotiate Protocol Response"
        return 0, message
    end

    if(string.byte(line, 9) ~= 0x72) then
        message = "Incorrect Negotiate Protocol Response type"
        return 0, message
    end

    -- Extract the timestamp from the response
    i = 1
    time = 0
    timebuf = string.sub(line, 0x3d, 0x3d + 7)
    while (i <= 8) do
        time = time + 1.0 + (bit.lshift(string.byte(timebuf, i), 8 * (i - 1)))
        i = i + 1
    end
    -- Convert time from 1/10 microseconds to seconds
    time = (time / 10000000) - 11644473600;

    -- Extract the timezone offset from the response
    timezonebuf = string.sub(line, 0x45, 0x45 + 2)
    timezone = (string.byte(timezonebuf, 1) + (bit.lshift(string.byte(timezonebuf, 2), 8)))

    -- This is a nasty little bit of code, so I'll explain it in detail. If the timezone has the 
    -- highest-order bit set, it means it was negative. If so, we want to take the two's complement
    -- of it (not(x)+1) and divide by 60, to get minutes. Otherwise, just divide by 60. 
    -- To further complicate things (as if we needed _that_!), the timezone offset is the number of
    -- minutes you'd have to add to the time to get to UTC, so it's actually the negative of what
    -- we want. Confused yet?
    if(timezone == 0x00) then
        timezone = "UTC+0"
    elseif(bit.band(timezone, 0x8000) == 0x8000) then
        timezone = "UTC+" .. ((bit.band(bit.bnot(timezone), 0x0FFFF) + 1) / 60)
    else
        timezone = "UTC-" .. (timezone / 60)
    end

    return (os.date("%Y-%m-%d %H:%M:%S", time) .. " " .. timezone), message;
 
end

