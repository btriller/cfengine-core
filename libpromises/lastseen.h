/*
  Copyright 2024 Northern.tech AS

  This file is part of CFEngine 3 - written and maintained by Northern.tech AS.

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; version 3.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA

  To the extent this program is licensed as part of the Enterprise
  versions of CFEngine, the applicable Commercial Open Source License
  (COSL) may apply to this file if you as a licensee so wish it. See
  included file COSL.txt.
*/

#ifndef CFENGINE_LASTSEEN_H
#define CFENGINE_LASTSEEN_H

#include <statistics.h>

typedef struct
{
    bool acknowledged; // True when acknowledged by cf-hub, false when updated
    time_t lastseen;
    QPoint Q; // Average time between connections (rolling weighted average)
} KeyHostSeen;

typedef enum
{
    LAST_SEEN_ROLE_CONNECT,
    LAST_SEEN_ROLE_ACCEPT
} LastSeenRole;


bool Address2Hostkey(char *dst, size_t dst_size, const char *address);
char *HostkeyToAddress(const char *hostkey);

void LastSaw1(const char *ipaddress, const char *hashstr, LastSeenRole role);
void LastSaw(const char *ipaddress, const unsigned char *digest, LastSeenRole role);

bool DeleteIpFromLastSeen(const char *ip, char *digest, size_t digest_size);
bool DeleteDigestFromLastSeen(const char *key, char *ip, size_t ip_size, bool a_entry_required);

/*
 * Return false in order to stop iteration
 */
typedef bool (*LastSeenQualityCallback)(const char *hostkey, const char *address,
                                        bool incoming, const KeyHostSeen *quality,
                                        void *ctx);

bool ScanLastSeenQuality(LastSeenQualityCallback callback, void *ctx);
int LastSeenHostKeyCount(void);
bool IsLastSeenCoherent(void);
int RemoveKeysFromLastSeen(const char *input, bool must_be_coherent,
                           char *equivalent, size_t equivalent_size);

/**
 * @brief Acknowledge that lastseen host entry is observed.
 * @param host_key The host key of the lastseen entry.
 * @param incoming Whether it is an incoming or outgoing entry.
 * @return true if host entry was successfully acknowledged, otherwise false.
 */
bool LastSeenHostAcknowledge(const char *host_key, bool incoming);

#endif
