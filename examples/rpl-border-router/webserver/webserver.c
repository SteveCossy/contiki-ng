/*
 * Copyright (c) 2017, RISE SICS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

#include "contiki.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-sr.h"

/* SC-Oct-24 */
#include "net/ipv6/uip-ds6.h"
#include "contiki-net.h"
#include "httpd-simple.h"
#include "net/nbr-table.h"

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "WEBSVR"
#define LOG_LEVEL LOG_LEVEL_INFO


#include <stdio.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
static const char *TOP = "<html>\n  <head>\n    <title>Contiki-NG</title>\n  </head>\n<body>\n";
static const char *BOTTOM = "\n</body>\n</html>\n";
static char tmp[64]; /* Temporary buffer for IP addresses SC-Oct-24 */
static char buf[2048]; /* Increased from 256 to 1024  SC-Oct-24 */
static int blen;
/* 
#define ADD(...) do {                                                   \
    blen += snprintf(&buf[blen], sizeof(buf) - blen, __VA_ARGS__);      \
  } while(0)
*/
#define SEND(s) do { \
  SEND_STRING(s, buf); \
  blen = 0; \
} while(0);

/* Use simple webserver with only one page for minimum footprint.
 * Multiple connections can result in interleaved tcp segments since
 * a single static buffer is used for all segments.
 */
#include "httpd-simple.h"

/*- SC-Oct-24 ---------------------------------------------------------------
The ADD and ADD_IP Macros: These macros will help in appending content and 
formatting IP addresses into the output buffer:
*/
#define ADD(str) snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%s", str)
/*  First version of ADD_IP macro
#define ADD_IP(addr) do { snprintf(tmp, sizeof(tmp), "%02x%02x:%02x%02x:%02x%02x:%02x%02x:" \
                           "%02x%02x:%02x%02x:%02x%02x:%02x%02x", \
                           addr->u8[0], addr->u8[1], addr->u8[2], addr->u8[3], \
                           addr->u8[4], addr->u8[5], addr->u8[6], addr->u8[7], \
                           addr->u8[8], addr->u8[9], addr->u8[10], addr->u8[11], \
                           addr->u8[12], addr->u8[13], addr->u8[14], addr->u8[15]); \
                           ADD(tmp); } while(0)
Updated version follows  */
#define ADD_IP(addr) snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), \
                             "%02x%02x:%02x%02x:%02x%02x:%02x%02x", \
                             (addr)->u8[0], (addr)->u8[1], (addr)->u8[2], (addr)->u8[3], \
                             (addr)->u8[4], (addr)->u8[5], (addr)->u8[6], (addr)->u8[7])


/*---------------------------------------------------------------------------*/
static void
ipaddr_add(const uip_ipaddr_t *addr)
{
  uint16_t a;
  int i, f;
  for(i = 0, f = 0; i < sizeof(uip_ipaddr_t); i += 2) {
    a = (addr->u8[i] << 8) + addr->u8[i + 1];
    if(a == 0 && f >= 0) {
      if(f++ == 0) {
        ADD("::");
      }
    } else {
      if(f > 0) {
        f = -1;
      } else if(i > 0) {
        ADD(":");
      }
      /* ADD("%x", a); */
      snprintf(tmp, sizeof(tmp), "%x", a);  // Format the variable 'a' as hexadecimal into 'tmp'
      ADD(tmp);                             // Add the formatted string to the buffer using 'ADD'

    }
  }
}

/*---------------------------------------------------------------------------
Create the Function to List Neighbours and Routes:
  Add the list_neighbours_and_routes() function to the file. 
  This function will dynamically generate the HTML content 
  listing the neighbours and associated routes, as described 
  in the earlier code:
  SC-Oct-24
*/

//tatic void list_neighbors_and_routes(void) {


//}

/*---------------------------------------------------------------------------*/
static
PT_THREAD(generate_routes(struct httpd_state *s))
{
 // static uip_ds6_nbr_t *nbr;

  PSOCK_BEGIN(&s->sout);
  SEND_STRING(&s->sout, TOP);

  ADD("  </ul>\n");
  SEND(&s->sout);


      /* Add the HTML header */
      ADD("<html><head><title>RPL Border Router</title></head><body>");
      ADD("<h1>RPL Border Router</h1>");

      /* Add dynamic content: Neighbours and Routes */
      // list_neighbors_and_routes();

  uip_ds6_nbr_t *nbr;
   uip_ipaddr_t child_ipaddr;
  uip_ipaddr_t parent_ipaddr;
  uip_ipaddr_t ip_buffer;

  /* Add a heading for neighbours */
  ADD("<h2>Neighbours and Associated Routes</h2>");
  ADD("<pre>");  /* Start preformatted text */
  // LOG_INFO("ADDed Stuff\n"); /* Debug */
  // Iterate over the neighbor table using nbr_table_ds6_neighbors()
  for(nbr = uip_ds6_nbr_head();
      nbr != NULL;
      nbr = uip_ds6_nbr_next(nbr)) {

    static uip_sr_node_t *link;

    ADD("Neighbour IP Address: ");
      ipaddr_add(&nbr->ipaddr);
    ADD("\n");
   
    /* Check for routes associated with this neighbour */
    ADD("  Routes associated with this neighbour:\n");
    /* Debug 
    LOG_INFO("Neighbour IP:");*/
    char ip_buf[UIPLIB_IPV6_MAX_STR_LEN];
    uiplib_ipaddr_snprint(ip_buf, sizeof(ip_buf), &nbr->ipaddr); // ADD this neighbour
    
    for ( link = uip_sr_node_head(); 
      link != NULL; 
      link = uip_sr_node_next(link)) {
      if(link->parent != NULL) {
        NETSTACK_ROUTING.get_sr_node_ipaddr(&child_ipaddr, link);
        NETSTACK_ROUTING.get_sr_node_ipaddr(&parent_ipaddr,link->parent);
        if(uiplib_ipaddrconv(ip_buf, &ip_buffer) == 0) {
          printf("Invalid IP address format\n");
        } else {
      // Now ipaddr contains the parsed IPv6 address
          uiplib_ipaddr_print(&ip_buffer);  // For example, print the address
        }
        printf("IP buffer is currently:");
        uiplib_ipaddr_print(&ip_buffer);
        printf("  parent is:");
        uiplib_ipaddr_print(&parent_ipaddr);
        printf("\n");
        if(uip_ipaddr_cmp(&ip_buffer,&parent_ipaddr)) {
          printf("link->parent points to ");
          uiplib_ipaddr_print(&parent_ipaddr);
          printf("\n");
          printf("link points to ");
          uiplib_ipaddr_print(&child_ipaddr);
          printf("\n");
          // Add Route IP Address if the nexthop matches the neighbour's IP 
          ADD("    Route IP Address: ");
          ADD_IP(&child_ipaddr);
          ADD("\n");
        }
//        printf(buf);
      }
    } 

    ADD("\n");  /* Add space between neighbours */
    SEND(&s->sout)
  printf(buf); // Debug
  }
  ADD("</pre>");  /* End preformatted text */

      /* Add closing tags */
      ADD("</body></html>");

      /* Send the generated HTML content */
      /* doesn't work httpd_simple_serve(buf, strlen(buf)); */
    httpd_appcall(buf);
    

  SEND_STRING(&s->sout, BOTTOM);

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
PROCESS(webserver_nogui_process, "Web server");
PROCESS_THREAD(webserver_nogui_process, ev, data)
{
  PROCESS_BEGIN();

/*- SC-Oct-24 ---------------------------------------------------------------
Replace existing code to integrate the new function into the existing web server logic:
  httpd_init();
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
  }
*/
  /* doesn't work httpd_simple_init(); */
  httpd_init();
  LOG_INFO("Contiki-NG Webserver started\n");
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    httpd_appcall(data);
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
httpd_simple_script_t
httpd_simple_get_script(const char *name)
{
  return generate_routes;
}
/*---------------------------------------------------------------------------*/
