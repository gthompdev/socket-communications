/* name: Greg Thompson

   Description: The client side of the communication. A message containing a college major will be sent from this program to
   the server, and will wait for the server to return the corresponding early and mid career salaries.
*/

#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

int main(int argc, char *argv[])
{
  int sockfd, portno, n;

  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[256];
  string hostname;
  cout << "Enter server host name: ";
  cin >> hostname;
  //must have char array for gethostbyname()
  const char* host = hostname.c_str();

  cout << "Enter server port number: ";
  cin >> portno;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    cerr << "ERROR opening socket" << endl;
    exit(1);
  }
  server = gethostbyname(host);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(1);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,  (char *)&serv_addr.sin_addr.s_addr,  server->h_length);
  serv_addr.sin_port = htons(portno);

  cin.ignore();
  string major;
  //loop for client entry
  while(true) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
      cerr << "ERROR connecting" << endl;
      exit(1);
    }
    cout << "Enter a college major: ";
    //use cin.ignore() before loop.
    getline(cin, major);
    //write to socket
    n = write(sockfd,major.c_str(),major.size());
    if (n < 0) {
      cerr << "ERROR writing to socket" << endl;
      exit(1);
    }
    if(major == "") {
      exit(1);
    }
    //read message from server
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) {
      cout << "ERROR reading from socket" << endl;
    }
    printf("The average early career pay for a %s major is %s\n", major.c_str(), buffer);
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) {
      cout << "ERROR reading from socket" << endl;
    }
    printf("The average mid career pay for a %s major is %s\n", major.c_str(), buffer);
    //socket close
    close(sockfd);
  }
  return 0;
}
