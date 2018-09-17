/* name: Greg Thompson
   PsId: 1569808
   Description: A server which prompts the user for a text file to be read into a table, which will then be queried
   by the client. The server will return the major's corresponding early and mid career salaries through the socket.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>

using namespace std;


int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno, clilen;
  //buffer which will be used for exchanging messages in the socket.
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  //n bytes in the message
  int n;
  string filename;
  cout << "Please enter the name of the file you would like to open: " ;
  cin >> filename;
  ifstream infile;
  infile.open(filename.c_str());
  if(infile.fail()){
    cout << "Error opening file." << endl;
    exit(1);
  }
  //tokenize file by tabs and push tokens into vector.
  string line;
  string token;
  vector<string> tokens;
  while(getline(infile,line)){
    stringstream ss(line);
    while(getline(ss,token,'\t'))
    tokens.push_back(token);
  }
  infile.close();

  cout << "Enter server port number: ";
  cin >> portno;

  //create socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    cout << "ERROR opening socket" << endl;
    exit(1);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    cout << "ERROR on binding" << endl;
    exit(1);
  }
  listen(sockfd,5);
  clilen = sizeof(cli_addr);

  while(true) {
    //accepts connection
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen);
    if (newsockfd < 0) {
      cerr << "ERROR on accept" << endl;
      exit(1);
    }
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) {
      cerr << "ERROR reading from socket" << endl;
      exit(1);
    }
    string str(buffer);
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    string earlySalary, midSalary;
    for(int i = 0; i < tokens.size(); i++) {

      if(tokens[i] == str) {
        int j = i + 1;
        int k = i + 2;
        earlySalary = tokens[j];
        midSalary = tokens[k];
        break;
      }
    }

    if(earlySalary.size() == 0)
    {
      n = write(newsockfd,"-1",2);
      n = write(newsockfd,"-1",2);
    }
    else{
      //make char arrays
      int e = earlySalary.length();
      char earlySalArray[e+1];
      strcpy(earlySalArray, earlySalary.c_str());
      n = write(newsockfd,earlySalArray,strlen(earlySalArray));
      int m = midSalary.length();
      char midSalArray[m+1]; // {}?
      strcpy(midSalArray, midSalary.c_str());
      n = write(newsockfd,midSalArray,strlen(midSalArray));
    }
    if (n < 0) {
      cerr << "ERROR writing to socket" << endl;
      exit(1);
    }
  }

  return 0;
}
