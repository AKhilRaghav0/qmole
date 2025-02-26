id="Skype v2"
description="Determines if remote service is Skype protocol version 2"
author = "Brandon Enright <bmenrigh@ucsd.edu>" 
license = "Same as Nmap--See http://nmap.org/book/man-legal.html"
categories = {"version"}
require "comm"

portrule = function(host, port)
	if 	(port.number == 80 or
		port.number == 443 or
		port.service == nil or
		port.service == "" or
		port.service == "unknown")
		and port.protocol == "tcp"
		and port.state == "open"
		and port.service ~= "http"
		and port.service ~= "ssl/http"
	then
		return true
	else
		return false
	end
end

action = function(host, port)
	local status, result = comm.exchange(host, port, "GET / HTTP/1.0\r\n\r\n", {bytes=26, proto=port.protocol})

	if (not status) then
		return
	end

	if (result ~= "HTTP/1.0 404 Not Found\r\n\r\n") then
		return
	end
	
	-- So far so good, now see if we get random data for another request

	status, result = comm.exchange(host, port, "random data\r\n\r\n", {bytes=15, proto=port.protocol})

	if (not status) then
		return
	end

	if string.match(result, "[^%s!-~].*[^%s!-~].*[^%s!-~]") then
		port.version.name = "skype2"
		port.version.product = "Skype"
		port.version.confidence = 10
		port.version.fingerprint = nil
		nmap.set_port_version(host, port, "hardmatched")
		return	
		-- return "Skype v2 server detected"
	end

	return
end
