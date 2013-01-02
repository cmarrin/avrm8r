/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * This software implements a  web browser.
 * The the browser tries to access 
 * the webserver at webserver_vhost when a push button is pressed
 * It will upload data to http://tuxgraphics.org/cgi-bin/upld
 * using http-get. You can as well upload to your own server where you
 * have installed the sdat cgi-script.
 *
 * When you connect PD6 with GND then it will upload a line to the 
 * upld page that should look like this:
 * 70.80.220.56 :: 2012-01-28 18:08 GMT :: action=btn :: ethbrd_ip=10.0.0.30 :: pw=sec ::
 *
 * There are two LEDs you can connect to the circuit to be able to
 * see what is going on.
 *
 * LED1 between PB1 and Vcc: this LED will be off during transations and go
 * back on when the transation is finished. Transations are e.g: get IP 
 * via dhcp, resolve tuxgraphics.org via DNS, start a browswe to upload
 * data to http://tuxgraphics.org/cgi-bin/upld
 *
 * LED2 between PD0 and Vcc: this LED will only go on if the DHCP server
 * did not provide a gateway IP address. This software needs absolutely
 * a gateway IP address and can not function without it.
 *
 * Chip type           : Atmega168 or Atmega328 or Atmega644 with ENC28J60
 *********************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// http://www.nongnu.org/avr-libc/changes-1.8.html:
#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include "analog-10bit.h"
#include "../ip_arp_udp_tcp.h"
#include "../websrv_help_functions.h"
#include "../enc28j60.h"
#include "../timeout.h"
#include "../net.h"
#include "../dnslkup.h"
#include "../dhcp_client.h"

//
// Please modify the following lines. mac and ip have to be unique
// in your local area network. You can not have the same numbers in
// two devices:
static uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x29};
// how did I get the mac addr? Translate the first 3 numbers into ascii is: TUX
//
// The name of the virtual host which you want to 
// contact (hostname of the first portion of the URL):
static const char webserver_vhost[]="tuxgraphics.org";
//
// Our DNS resolver can only look up IP addresses in the public internet
// for testing purposes you may as well use a server in your local LAN.
// In that can you can specify just the IP address as a 
// sting in webserver_vhost.
//static const char webserver_vhost[]="10.0.0.1";
//static const char webserver_vhost[]="77.37.2.152";
// 
// if you plan to use the sdat cgi-script instead of upld on tuxgraphics.org
// then replace upld by sdat further down in the function client_browse_url().
//
// -- there should not be any need to change something below this line --
static uint8_t otherside_www_ip[4]; // dns will fill this
// My own IP (DHCP will provide a value for it):
static uint8_t myip[4]={0,0,0,0};
// Default gateway (DHCP will provide a value for it):
static uint8_t gwip[4]={0,0,0,0};
#define TRANS_NUM_GWMAC 1
static uint8_t gwmac[6];
#define TRANS_NUM_WEBMAC 2
static uint8_t otherside_www_gwmac[6];
// Netmask (DHCP will provide a value for it):
static uint8_t netmask[4];
static char urlvarstr[32];
static volatile uint8_t sec=0; // counts up to 6 and goes back to zero
static volatile uint8_t gsec=0; // counts up beyond 6 sec
static  uint8_t gmin=0; 

// set output to VCC, red LED off
#define LEDOFF PORTB|=(1<<PORTB1)
// set output to GND, red LED on
#define LEDON PORTB&=~(1<<PORTB1)
// to test the state of the LED
#define LEDISOFF PORTB&(1<<PORTB1)
// packet buffer
#define BUFFER_SIZE 650
static uint8_t buf[BUFFER_SIZE+1];
static uint8_t start_web_client=0;
static uint8_t web_client_sendok=0;
static int8_t processing_state=0;

// set output to VCC, red LED off
#define LEDOFF PORTB|=(1<<PORTB1)
// set output to GND, red LED on
#define LEDON PORTB&=~(1<<PORTB1)
// to test the state of the LED
#define LEDISOFF PORTB&(1<<PORTB1)
// 

// set output to VCC, red LED off
#define PD0LEDOFF PORTD|=(1<<PORTD0)
// set output to GND, red LED on
#define PD0LEDON PORTD&=~(1<<PORTD0)
// to test the state of the LED
#define PD0LEDISOFF PORTD&(1<<PORTD0)
// 

// timer interrupt, called automatically every second
ISR(TIMER1_COMPA_vect){
        sec++;
        gsec++;
        if (sec>5){
                sec=0;
                dhcp_6sec_tick();
        }
}

// Generate an interrup about ever 1s form the 12.5MHz system clock
// Since we have that 1024 prescaler we do not really generate a second
// (1.00000256000655361677s) 
void timer_init(void)
{
        /* write high byte first for 16 bit register access: */
        TCNT1H=0;  /* set counter to zero*/
        TCNT1L=0;
        // Mode 4 table 14-4 page 132. CTC mode and top in OCR1A
        // WGM13=0, WGM12=1, WGM11=0, WGM10=0
        TCCR1A=(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11);
        TCCR1B=(1<<CS12)|(1<<CS10)|(1<<WGM12)|(0<<WGM13); // crystal clock/1024
        // divide clock by 1024: 12.5MHz/1024=12207.0313 Hz

        // At what value to cause interrupt. You can use this for calibration
        // of the clock. Theoretical value for 12.5MHz: 12207=0x2f and 0xaf
        OCR1AH=0x2f;
        OCR1AL=0xaf;
        // interrupt mask bit:
        TIMSK1 = (1 << OCIE1A);
}

// we were ping-ed by somebody, store the ip of the ping sender
// and trigger an upload to http://tuxgraphics.org/cgi-bin/upld
// This is just for testing and demonstration purpose
//
// the __attribute__((unused)) is a gcc compiler directive to avoid warnings about unsed variables.
void browserresult_callback(uint16_t webstatuscode,uint16_t datapos __attribute__((unused)), uint16_t len __attribute__((unused))){
        if (webstatuscode==200){
                web_client_sendok++;
                LEDOFF;
        }
}

// the __attribute__((unused)) is a gcc compiler directive to avoid warnings about unsed variables.
void arpresolver_result_callback(uint8_t *ip __attribute__((unused)),uint8_t reference_number,uint8_t *mac){
        uint8_t i=0;
        if (reference_number==TRANS_NUM_GWMAC){
                // copy mac address over:
                while(i<6){gwmac[i]=mac[i];i++;}
        }
        if (reference_number==TRANS_NUM_WEBMAC){
                // copy mac address over:
                while(i<6){otherside_www_gwmac[i]=mac[i];i++;}
        }
}

int main(void){
        uint16_t dat_p,plen;
        char str[20]; 
        uint8_t rval;
        uint8_t i;

        DDRB|= (1<<DDB1); // LED, enable PB1 as output
        LEDON;
        DDRD|= (1<<DDD0); // second LED, enable PD0 as output
        PD0LEDOFF;
        // Set the clock speed to "no pre-scaler" (8MHz with internal osc or 
        // full external speed)
        // set the clock prescaler. First write CLKPCE to enable setting 
        // of clock the next four instructions.
        // Note that the CKDIV8 Fuse determines the initial
        // value of the CKKPS bits.
        CLKPR=(1<<CLKPCE); // change enable
        CLKPR=0; // "no pre-scaler"
        _delay_loop_1(0); // 60us

        /*initialize enc28j60*/
        enc28j60Init(mymac);
        enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
        _delay_loop_1(0); // 60us
        
        timer_init();
        sei();

        /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
        // LEDB=yellow LEDA=green
        //
        // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
        // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
        enc28j60PhyWrite(PHLCON,0x476);

        LEDON;
        // DHCP handling. Get the initial IP
        rval=0;
        init_mac(mymac);
        while(rval==0){
                plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
                buf[BUFFER_SIZE]='\0';
                rval=packetloop_dhcp_initial_ip_assignment(buf,plen,mymac[5]);
        }
        // we have an IP:
        dhcp_get_my_ip(myip,netmask,gwip); 
        client_ifconfig(myip,netmask);
        LEDOFF;

        if (gwip[0]==0){
                // we must have a gateway returned from the dhcp server
                // otherwise this code will not work
                PD0LEDON; // error
                while(1); // stop here
        }
        
        LEDON;
        // we have a gateway.
        // find the mac address of the gateway (e.g your dsl router).
        get_mac_with_arp(gwip,TRANS_NUM_GWMAC,&arpresolver_result_callback);
        while(get_mac_with_arp_wait()){
                // to process the ARP reply we must call the packetloop
                plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
                packetloop_arp_icmp_tcp(buf,plen);
        }
        LEDOFF;

        if (string_is_ipv4(webserver_vhost)){
                // the the webserver_vhost is not a domain name but alreay
                // an IP address written as sting
                parse_ip(otherside_www_ip,webserver_vhost);
                processing_state=2; // no need to do any dns look-up
        }

        while(1){
                plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
                buf[BUFFER_SIZE]='\0'; // http is an ascii protocol. Make sure we have a string terminator.
                // DHCP renew IP:
                plen=packetloop_dhcp_renewhandler(buf,plen); // for this to work you have to call dhcp_6sec_tick() every 6 sec
                dat_p=packetloop_arp_icmp_tcp(buf,plen);
                if(plen==0){
                        // we are idle here
                        if (processing_state==0){
                                if (!enc28j60linkup()) continue; // only for dnslkup_request we have to check if the link is up. 
                                gsec=0;
                                processing_state=1;
                                dnslkup_request(buf,webserver_vhost,gwmac);
                                LEDON;
                                continue;
                        }
                        if (processing_state==1 && dnslkup_haveanswer()){
                                dnslkup_get_ip(otherside_www_ip);
                                processing_state=2;
                                LEDOFF;
                        }
                        if (processing_state==2){
                                if (route_via_gw(otherside_www_ip)){
                                        // otherside_www_ip is behind the GW
                                        i=0;
                                        while(i<6){
                                                otherside_www_gwmac[i]=gwmac[i];
                                                i++;
                                        }
                                        processing_state=4;
                                }else{
                                        // the web server we want to contact is on the local lan. find the mac:

                                        LEDON;
                                        get_mac_with_arp(otherside_www_ip,TRANS_NUM_WEBMAC,&arpresolver_result_callback);
                                        processing_state=3;
                                }
                                continue;
                        }
                        if (processing_state==3 && get_mac_with_arp_wait()==0){
                                // we have otherside_www_gwmac
                                LEDOFF;
                                processing_state=4;
                        }
                        if (processing_state==4){
                                processing_state=5;
                                // we are ready to upload the first measurement data after startup:
                                if (start_web_client==0) start_web_client=1;

                        }
                        if (processing_state==1){
                                // retry every 20 sec if dns-lookup failed:
                                if (gsec > 20){
                                        processing_state=0;
                                }
                                // don't try to use web client before
                                // we have a result of dns-lookup
                                continue;
                        }
                        //----------
                        if (start_web_client==1){
                                LEDON;
                                gsec=0;
                                gmin=0;
                                start_web_client=2;
                                mk_net_str(str,myip,4,'.',10);
                                urlencode(str,urlvarstr);
                                strcat(urlvarstr,"&adc0=");
                                itoa(convertanalog(0),str,10);
                                strcat(urlvarstr,str);
                                //client_browse_url(PSTR("/cgi-bin/sdat?pw=sec&action=btn&ethbrd_ip="),urlvarstr,webserver_vhost,&browserresult_callback,otherside_www_ip,otherside_www_gwmac);
                                client_browse_url(PSTR("/cgi-bin/upld?pw=sec&action=btn&ethbrd_ip="),urlvarstr,webserver_vhost,&browserresult_callback,otherside_www_ip,otherside_www_gwmac);
                        }
                        // our clock:
                        if (gsec>59){
                                gsec=0;
                                gmin++;
                        }
                        if (gmin>59){
                                gmin=0;
                                start_web_client=1; // upload one measurement every hour
                        }
                        continue;
                }
                if(dat_p==0){ // plen!=0
                        // check for incomming messages not processed
                        // as part of packetloop_arp_icmp_tcp, e.g udp messages
                        udp_client_check_for_dns_answer(buf,plen);
                        continue;
                }
                // when dat_p!=0 then we get normally a http 
                // request but this is not a web server so we do nothing.
        }
        return (0);
}
