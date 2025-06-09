/**
 * \file
 *         IPv6 prefix definitions and helper function for second address assignment.
 * \author Steve Cosgrove <steve.cosgrove@rata.co.nz>
 * \date   May 2025
 *
 * \brief
 * This file define one and perhaps could be more
 * additional IPv6 prefixes and helper routines for
 * assigning global addresses based on these prefixes. Specifically,
 * it introduces a second site-local IPv6 prefix (fd02::/64) to enable
 * multi-addressing per node in Contiki-NG.
 *
 * The secondary prefix is used to autoconfigure a second global IPv6
 * address on each node using an Interface Identifier (IID) derived
 * from the node's link-layer address.
 *
 * The helper function \c uip_ds6_add_second_global_address() trieds to
 * check the address is only added if it does not already exist but this
 * generated an error listed below (about line 46) so is removed.
 *
 * \note
 * To use this functionality, include call
 * \c uip_ds6_add_second_global_address() during address configuration,
 * typically from \c set_global_address() when creating each node.
 *
 * \see uip-ds6-prefix.h
 * \see uip-ds6.c
 */

#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "uip-ds6.h"
#include "contiki.h"
#include "net/routing/rpl-classic/rpl.h"

const uip_ipaddr_t UIP_DS6_DEFAULT_PREFIX2_F = {
  .u16 = {UIP_HTONS(UIP_DS6_DEFAULT_PREFIX2), 0, 0, 0, 0, 0, 0, 0}
};

void uip_ds6_add_second_global_address(void) {
 uip_ipaddr_t ipaddr;
 // if(uip_ds6_addr_lookup(&UIP_DS6_DEFAULT_PREFIX2_F) == NULL) {
    uip_ip6addr_copy(&ipaddr, &UIP_DS6_DEFAULT_PREFIX2_F);
    uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
    uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);
 // }
 /** The if(uip... statement above generated an error
  * ../../../os/net/ipv6/uip-ds6-prefix.c:39:26: error: passing argument 1 of 'uip_ds6_addr_lookup' discards 'const' qualifier from pointer target type [-Werror=discarded-qualifiers]
   39 |   if(uip_ds6_addr_lookup(&UIP_DS6_DEFAULT_PREFIX2_F) == NULL)
  * 
  */
}

// The only significant change is this include path!
/*#include "sys/log.h"
#include <stdio.h>

#define LOG_MODULE "RPL-Classic-OF"
#define LOG_LEVEL LOG_LEVEL_INFO


 * This helper function is IDENTICAL to the one for rpl-lite.
 * The RPL_OCP_* definitions are pulled in via rpl.h -> rpl-conf.h -> rpl-const.h
 */
const char *
get_rpl_of_name(rpl_ocp_t ocp)
{
  switch(ocp) {
    case RPL_OCP_OF0:
      return "OF0";
    case RPL_OCP_MRHOF:
      return "MRHOF";
    default:
      return "Unknown/Custom OF";
  }
}
