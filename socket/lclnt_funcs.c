/******************************************************************
 * lclnt_funcs.c: functions for the client of the lincense server
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "dgram.h"


// Important variables uesd throughout
static int pid = -1;				// our PID
static int sd = -1;				// out communctaion sokcet
static struct sockaddr serv_addr;		// Server address
static socklen_t serv_alen;			// length of address
static char ticket_buf[128];			// Buffer to hold our ticket
static int have_ticket = 0;			// Set when we have a ticket

#define MSGLEN 128				// Size of out datagrams
#define SERVER_PORTNUM 2020			// Our server's port number
#define HOSTLEN 512

#define oops(p) {perror(p);exit(1);}

char * do_transaction();
void narrate(char *,char *);
void syserr(char *);

/******************************************************************
 * setup: get pid, socket, and address of license server
 * IN no args
 * RET nothing, dies on error
 * notes: assumes server is on the same host as client
 */
void setup() {
	char hostname[BUFSIZ];

	pid = getpid();				// for ticks and msgs
	sd = make_dgram_client_socket();	// to talk to server

	if(sd == -1) 
		oops("Cannot create scoket");

	gethostname(hostname,HOSTLEN);		// server on the same host
	make_internet_address(hostname,SERVER_PORTNUM,
		(struct sockaddr_in *)&serv_addr);
	serv_alen = sizeof(serv_addr);
}

/*******************************************************************
 * shut down: close sd
 */
void shut_down() {
	close(sd);
}

/*******************************************************************
 * get_ticket
 * get a ticket from the license server
 * Result: 0 for scuccess, -1 for failure
 */
int get_ticket() {
	char *response;
	char buf[MSGLEN];

	if(have_ticket)
		return 0;				// dont' be greedy

	sprintf(buf,"HELLO %d",pid);

	if((response = do_transaction(buf)) == NULL) {
		return -1;
	}

	/* parse the response and if we got a ticket.
	 * on success,the message is: TICK ticket-string
	 * on failure,the message is: FAIL failute-msg
	*/
	if(strncmp(response,"TICK",4) == 0) {
		strcpy(ticket_buf,response + 5);;	// grab ticket -id
		have_ticket = 1;			// set the flag
		narrate("got ticket",ticket_buf);

		return 0;
	}

	if(strncmp(response,"FAIL",4) == 0) 
		narrate("Could not get ticket",response);
	else 
		narrate("Unknowm message:",response);

	return -1;
}// get_ticket

/*******************************************************************
 * release_ticket
 * Give a ticket back to the server
 * Result: 0 for success, -1 for failure
 */
int release_ticket() {
	char buf[MSGLEN];
	char *response;

	if(!have_ticket)
		return 0;				// don't have a ticket
							// nothing to release
	sprintf(buf,"GBYE %s",ticket_buf);
	if((response = do_transaction(buf)) == NULL)
		return -1;
	
	/* examine response
	 * success: THNX info-string
	 * failure: FAIL error-string
	 */
	if(strncmp(response,"THNX",4) == 0) {
		narrate("released ticket OK","");

		return 0;
	}

	if(strncmp(response,"FAIL",4) == 0)
		narrate("release failed",response + 5);
	else
		narrate("Unknown message",response);
	
	return -1;
}

/*******************************************************************
 * do_transaction
 * Send a request to the server and get a response back
 * IN msg_p	message to send
 * Result: pointer to message string, or NULL for error
 * 	NOTE: pointer returned is to static storge
 * 		overwritten by each successive call.
 * note: for extra sercurity, compare retaddr to serv_addr(why?)
 */
char *do_transaction(char *msg) {
	static char buf[MSGLEN];
	struct sockaddr retaddr;
	socklen_t addrlen = sizeof(retaddr);
	int ret;

	ret = sendto(sd,msg,strlen(msg),0,&serv_addr,serv_alen);
	if(ret == -1) {
		syserr("sendto");

		return NULL;
	}

	// Get the response back
	ret = recvfrom(sd,buf,MSGLEN,0,&retaddr,&addrlen);
	if(ret == -1) {
		syserr("recvfrom");

		return NULL;
	}

	// Now retrun the messgae itself
	return buf;
} // do_transaction
/*******************************************************************
 * narrate: print message t stderr for debugging and demo purpose
 * IN msg1,msg2: string to print along with pid and title
 * RET nothing, dies on error
 */
void narrate(char *msg1,char *msg2) {
	fprintf(stderr,"CLIENT[%d]: %s %s\n",pid,msg1,msg2);
}

/*******************************************************************
 * syserr: print the error message using perror
 */
void syserr(char *msg1) {
	char buf[MSGLEN];

	sprintf(buf,"CLIENT[%d]: %s",pid,msg1);
	perror(buf);
}

/*******************************************************************
 * do_validate
 * validate the ticket  that hold on
 * Result: 0 for success, -1 for failure
 */
int do_validate() {
	char buf[BUFSIZ];
	char *response;


	sprintf(buf,"VALD %s",ticket_buf);
	if((response = do_transaction(buf)) == NULL)
		return -1;
	
	/* parse the response and see if we the ticket is valid
	 * on success, the message is: GOOD vaild-msg
	 * on failure, the message is: FIAL failure-msg
	 */
	if(strncmp(response,"GOOD",4) == 0) {
		narrate("Validate success",ticket_buf);

		return 0;
	}

	if(strncmp(response,"FAIL",4) == 0)
		narrate("Invalid ticket",response);
	else
		narrate("Unknown message:",response);
		
	return -1;
}
