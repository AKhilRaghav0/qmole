/* Copyright (C) 2007 by Marc Maurer <uwog@uwog.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef __TCPUNIXACCOUNTHANDLER__
#define __TCPUNIXACCOUNTHANDLER__

#include <backends/tcp/xp/TCPAccountHandler.h>

class TCPUnixAccountHandler : public TCPAccountHandler
{
public:
	TCPUnixAccountHandler();
	
	static AccountHandler*					static_constructor();

	void									eventGroupChanged();

	// dialog management 
	virtual void							embedDialogWidgets(void* pEmbeddingParent);
	virtual void							removeDialogWidgets(void* pEmbeddingParent);
	virtual void							storeProperties();
	
private:
	GtkWidget*								vbox;
	GtkWidget*								server_button;
	GtkWidget*								client_button;
	GtkWidget*								server_entry;
	GtkWidget*								port_button;
	GtkWidget*								ssl_button;
	GtkWidget*								autoconnect_button;
};

#endif /* __TCPUNIXACCOUNTHANDLER__ */
