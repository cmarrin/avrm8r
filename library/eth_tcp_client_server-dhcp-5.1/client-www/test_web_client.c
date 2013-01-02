/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * Note: This software implements a web server and a web browser.
 * The web server is at "myip" and the browser tries to access 
 * the webserver at WEBSERVER_VHOST (tuxgraphics.org)
 *
 * Here is how to use this test software:
 * test_web_client.hex is a web client that uploads data to 
 * http://tuxgraphics.org/cgi-bin/upld when you ping it once
 * This is what you need to do:
 * - Change above the myip and gwip paramters 
 * - Compile and load the software
 * - ping the board once at myip from your computer (this will trigger the web-client)
 * - go to http://tuxgraphics.org/cgi-bin/upld and it will tell
 *   you from which IP address the ethernet board was ping-ed.
 *
 * Chip type           : Atmega168 or Atmega328 or Atmega644 with ENC28J60
 *********************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
// http://www.nongnu.org/avr-libc/changes-1.8.html:
#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include "../ip_arp_udp_tcp.h"
#include "../websrv_help_functions.h"
#include "../enc28j60.h"
#include "../timeout.h"
#include "../net.h"
#include "../dnslkup.h"

// Please modify the following lines. mac and ip have to be unique
// in your local area network. You can not have the same numbers in
// two devices:
static uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x29};
// how did I get the mac addr? Translate the first 3 numbers into ascii is: TUX
// This web server's own IP.
static uint8_t myip[4] = {10,0,0,29};
//static uint8_t myip[4] = {192,168,255,100};
//
// The name of the virtual host which you want to 
// contact at websrvip (hostname of the first portion of the URL):
#define WEBSERVER_VHOST "tuxgraphics.org"
// listen port for tcp/www:
#define MYWWWPORT 80

// Default gateway. The ip address of your DSL router. It can be set to 
// the same as the web server in the case where there is no default GW to access the 
// web server (=web server is on the same lan as this host) 
static uint8_t gwip[4] = {10,0,0,2};
//
// --- there should not be any need to changes things below this line ---
#define TRANS_NUM_GWMAC 1
static uint8_t gwmac[6]; 
static uint8_t otherside_www_ip[4]; // will be filled by dnslkup
//
static char urlvarstr[21];
//
//
#define BUFFER_SIZE 650
static uint8_t buf[BUFFER_SIZE+1];
static uint8_t pingsrcip[4];
static uint8_t start_web_client=0;
static uint8_t web_client_attempts=0;
static uint8_t web_client_sendok=0;
static volatile uint8_t sec=0;
static volatile uint8_t cnt2step=0;
static int8_t dns_state=0;
static int8_t gw_arp_state=0;

// set output to VCC, red LED off
#define LEDOFF PORTB|=(1<<PORTB1)
// set output to GND, red LED on
#define LEDON PORTB&=~(1<<PORTB1)
// to test the state of the LED
#define LEDISOFF PORTB&(1<<PORTB1)
// 

uint16_t http200ok(void)
{
        return(fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n")));
}


// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage(uint8_t *buf)
{
        uint16_t plen;
        char vstr[17];
        uint8_t err;
        plen=http200ok();
        plen=fill_tcp_data_p(buf,plen,PSTR("<h2>web client status</h2>\n<pre>\n"));
        if (gw_arp_state==1){
                plen=fill_tcp_data_p(buf,plen,PSTR("waiting for GW "));
                mk_net_str(vstr,gwip,4,'.',10);
                plen=fill_tcp_data(buf,plen,vstr);
                plen=fill_tcp_data_p(buf,plen,PSTR(" to answer arp.\n"));
                return(plen);
        }
        if (dns_state==1){
                plen=fill_tcp_data_p(buf,plen,PSTR("waiting for DNS answer.\n"));
                err=dnslkup_get_error_info();
                plen=fill_tcp_data_p(buf,plen,PSTR("Error code: "));
                itoa(err,vstr,10);
                plen=fill_tcp_data(buf,plen,vstr);
                plen=fill_tcp_data_p(buf,plen,PSTR(" (0=no error)\n"));
                return(plen);
        }
        plen=fill_tcp_data_p(buf,plen,PSTR("Number of data uploads started by ping: "));
        // convert number to string:
        itoa(web_client_attempts,vstr,10);
        plen=fill_tcp_data(buf,plen,vstr);
        plen=fill_tcp_data_p(buf,plen,PSTR("\nNumber successful data uploads to web: "));
        // convert number to string:
        itoa(web_client_sendok,vstr,10);
        plen=fill_tcp_data(buf,plen,vstr);
        plen=fill_tcp_data_p(buf,plen,PSTR("\ncheck result: <a href=http://tuxgraphics.org/cgi-bin/upld>http://tuxgraphics.org/cgi-bin/upld</a>"));
        plen=fill_tcp_data_p(buf,plen,PSTR("\n</pre><br><hr>"));
        return(plen);
}

/* setup timer T2 as an interrupt generating time base.
* You must call once sei() in the main program */
void init_cnt2(void)
{
	cnt2step=0;
	PRR&=~(1<<PRTIM2); // write power reduction register to zero
	TIMSK2=(1<<OCIE2A); // compare match on OCR2A
	TCNT2=0;  // init counter
	OCR2A=244; // value to compare against
	TCCR2A=(1<<WGM21); // do not change any output pin, clear at compare match
	// divide clock by 1024: 12.5MHz/128=12207 Hz
	TCCR2B=(1<<CS22)|(1<<CS21)|(1<<CS20); // clock divider, start counter
	// 12207/244=50Hz
}

// called when TCNT2==OCR2A
// that is in 50Hz intervals
ISR(TIMER2_COMPA_vect){
	cnt2step++;
	if (cnt2step>50){
                cnt2step=0;
                sec++; // stepped every second
	}
}

// we were ping-ed by somebody, store the ip of the ping sender
// and trigger an upload to http://tuxgraphics.org/cgi-bin/upld
// This is just for testing and demonstration purpose
void ping_callback(uint8_t *ip){
        uint8_t i=0;
        // trigger only first time in case we get many ping in a row:
        if (start_web_client==0){
                start_web_client=1;
                // save IP from where the ping came:
                while(i<4){
                        pingsrcip[i]=ip[i];
                        i++;
                }
        }
}

// the __attribute__((unused)) is a gcc compiler directive to avoid warnings about unsed variables.
void browserresult_callback(uint16_t webstatuscode,uint16_t datapos __attribute__((unused)), uint16_t len __attribute__((unused))){
        if (webstatuscode==200){
                web_client_sendok++;
                LEDOFF;
        }
}

// the __attribute__((unused)) is a gcc compiler directive to avoid warnings about unsed variables.
void arpresolver_result_callback(uint8_t *ip __attribute__((unused)),uint8_t transaction_number,uint8_t *mac){
        uint8_t i=0;
        if (transaction_number==TRANS_NUM_GWMAC){
                // copy mac address over:
                while(i<6){gwmac[i]=mac[i];i++;}
        }
}

int main(void){

        
        uint16_t dat_p,plen;
        char str[20]; 

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
        
        init_cnt2();
        sei();

        /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
        // LEDB=yellow LEDA=green
        //
        // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
        // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
        enc28j60PhyWrite(PHLCON,0x476);

        DDRB|= (1<<DDB1); // LED, enable PB1, LED as output
        LEDOFF;
        
        //init the web server ethernet/ip layer:
        init_udp_or_www_server(mymac,myip);
        www_server_port(MYWWWPORT);

        // register to be informed about incomming ping:
        register_ping_rec_callback(&ping_callback);

        while(1){
                // handle ping and wait for a tcp packet
                plen=enc28j60PacketReceive(BUFFER_SIZE, buf);
                dat_p=packetloop_arp_icmp_tcp(buf,plen);
                if(plen==0){
                        // we are idle here trigger arp and dns stuff here
                        if (gw_arp_state==0){
                                // find the mac address of the gateway (e.g your dsl router).
                                get_mac_with_arp(gwip,TRANS_NUM_GWMAC,&arpresolver_result_callback);
                                gw_arp_state=1;
                        }
                        if (get_mac_with_arp_wait()==0 && gw_arp_state==1){
                                // done we have the mac address of the GW
                                gw_arp_state=2;
                        }
                        if (dns_state==0 && gw_arp_state==2){
                                if (!enc28j60linkup()) continue; // only for dnslkup_request we have to check if the link is up. 
                                sec=0;
                                dns_state=1;
                                dnslkup_request(buf,WEBSERVER_VHOST,gwmac);
                                continue;
                        }
                        if (dns_state==1 && dnslkup_haveanswer()){
                                dns_state=2;
                                dnslkup_get_ip(otherside_www_ip);
                        }
                        if (dns_state!=2){
                                // retry every minute if dns-lookup failed:
                                if (sec > 60){
                                        dns_state=0;
                                }
                                // don't try to use web client before
                                // we have a result of dns-lookup
                                continue;
                        }
                        //----------
                        if (start_web_client==1){
                                LEDON;
                                sec=0;
                                start_web_client=2;
                                web_client_attempts++;
                                mk_net_str(str,pingsrcip,4,'.',10);
                                urlencode(str,urlvarstr);
                                client_browse_url(PSTR("/cgi-bin/upld?pw=sec&pingIP="),urlvarstr,PSTR(WEBSERVER_VHOST),&browserresult_callback,otherside_www_ip,gwmac);
                        }
                        // reset after a delay to prevent permanent bouncing
                        if (sec>60 && start_web_client==2){
                               start_web_client=0;
                        }
                        continue;
                }
                if(dat_p==0){ // plen!=0
                        // check for incomming messages not processed
                        // as part of packetloop_arp_icmp_tcp, e.g udp messages
                        udp_client_check_for_dns_answer(buf,plen);
                        continue;
                }
                        
                if (strncmp("GET ",(char *)&(buf[dat_p]),4)!=0){
                        // head, post and other methods:
                        //
                        // for possible status codes see:
                        // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
                        dat_p=http200ok();
                        dat_p=fill_tcp_data_p(buf,dat_p,PSTR("<h1>200 OK</h1>"));
                        goto SENDTCP;
                }
                if (strncmp("/ ",(char *)&(buf[dat_p+4]),2)==0){
                        dat_p=http200ok();
                        dat_p=print_webpage(buf);
                        goto SENDTCP;
                }else{
                        dat_p=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
                        goto SENDTCP;
                }
                //
SENDTCP:
                www_server_reply(buf,dat_p); // send data

        }
        return (0);
}
