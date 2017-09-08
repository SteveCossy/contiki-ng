/*
 * Copyright (c) 2015, SICS Swedish ICT.
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
 */

/**
 * \author Simon Duquennoy <simonduq@sics.se>
 */

#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__

/* Set to use the Contiki shell */
#define WITH_SHELL 1

/* Set to run orchestra */
#ifndef WITH_ORCHESTRA
#define WITH_ORCHESTRA 0
#endif /* WITH_ORCHESTRA */

/* Set to enable TSCH security */
#ifndef WITH_SECURITY
#define WITH_SECURITY 0
#endif /* WITH_SECURITY */

/*******************************************************/
/********* Enable RPL non-storing mode *****************/
/*******************************************************/

#undef RPL_CONF_MOP
#define RPL_CONF_MOP RPL_MOP_NON_STORING /* Mode of operation*/
#undef ORCHESTRA_CONF_RULES
#define ORCHESTRA_CONF_RULES { &eb_per_time_source, &unicast_per_neighbor_rpl_ns, &default_common } /* Orchestra in non-storing */

/*******************************************************/
/********************* Configure TSCH *********************/
/*******************************************************/

/* TSCH and RPL callbacks */
#define RPL_CALLBACK_PARENT_SWITCH tsch_rpl_callback_parent_switch
#define RPL_CALLBACK_NEW_DIO_INTERVAL tsch_rpl_callback_new_dio_interval
#define TSCH_CALLBACK_KA_SENT tsch_rpl_callback_ka_sent
#define TSCH_CALLBACK_JOINING_NETWORK tsch_rpl_callback_joining_network
#define TSCH_CALLBACK_LEAVING_NETWORK tsch_rpl_callback_leaving_network

#if WITH_SHELL
/* Needed for CC2538 platforms, for serial */
#define USB_SERIAL_CONF_ENABLE 1

/* USB serial takes space, free more space elsewhere */
#undef SICSLOWPAN_CONF_FRAG
#define SICSLOWPAN_CONF_FRAG 0
#undef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE 160

#endif /* WITH_SHELL */

/*******************************************************/
/******************* Configure TSCH ********************/
/*******************************************************/

/* IEEE802.15.4 PANID */
#undef IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID 0x81a5

/* Do not start TSCH at init, wait for NETSTACK_MAC.on() */
#undef TSCH_CONF_AUTOSTART
#define TSCH_CONF_AUTOSTART 0

/* 6TiSCH minimal schedule length.
 * Larger values result in less frequent active slots: reduces capacity and saves energy. */
#undef TSCH_SCHEDULE_CONF_DEFAULT_LENGTH
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 3

#if WITH_SECURITY

/* Enable security */
#undef LLSEC802154_CONF_ENABLED
#define LLSEC802154_CONF_ENABLED 1

#endif /* WITH_SECURITY */

#if WITH_ORCHESTRA

/* See os/services//orchestra/README.md for more Orchestra configuration options */
#define TSCH_SCHEDULE_CONF_WITH_6TISCH_MINIMAL 0 /* No 6TiSCH minimal schedule */
#define TSCH_CONF_WITH_LINK_SELECTOR 1 /* Orchestra requires per-packet link selection */
/* Orchestra callbacks */
#define TSCH_CALLBACK_NEW_TIME_SOURCE orchestra_callback_new_time_source
#define TSCH_CALLBACK_PACKET_READY orchestra_callback_packet_ready
#define NETSTACK_CONF_ROUTING_NEIGHBOR_ADDED_CALLBACK orchestra_callback_child_added
#define NETSTACK_CONF_ROUTING_NEIGHBOR_REMOVED_CALLBACK orchestra_callback_child_removed

#endif /* WITH_ORCHESTRA */

/*******************************************************/
/************* Other system configuration **************/
/*******************************************************/

/* Logging */
#define LOG_CONF_LEVEL_RPL                         LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_TCPIP                       LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_IPV6                        LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_6LOWPAN                     LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_MAC                         LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_FRAMER                      LOG_LEVEL_DBG
#define TSCH_LOG_CONF_PER_SLOT                     1

#endif /* __PROJECT_CONF_H__ */
