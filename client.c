#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8080
#define IPADD "127.0.0.1"


int main(){

        int sockfunc = 0;
        struct sockaddr_in target_addr; //struct for sockaddr_in

        char reply[10240];
        char cmd[1024];

        //CREATE A SOCKET
        sockfunc = socket (AF_INET, SOCK_STREAM, 0);

        //NONBLOCK - so we can continue even no response from server or sending wrong command
        fcntl(sockfunc, F_SETFL, O_NONBLOCK);

        //DEFINE THE REQUIREED SOCKADD_IN STRUC VALUES
        target_addr.sin_family = AF_INET;
        target_addr.sin_port = htons(PORT);

        //CONVERTS THE IP ADDRESS STRING INTO NETWORK BYTES ORDER
        //AND COPIES IT INTO SOCKADDR_IN SIN_ADDR VARIABLE
        inet_pton(AF_INET, IPADD, &target_addr.sin_addr);

        //OPEN CONNECTION TO TARGET
        connect(sockfunc, (struct sockaddr *) &target_addr, sizeof(target_addr));

        while(1){
                memset(cmd, 0, sizeof(cmd));
                memset(reply, 0, sizeof(reply));

                printf("shell#");
                fgets(cmd, sizeof(cmd), stdin);

                if(cmd == "exit"){
                        return 0;
                }
                //SEND THE CMD TO TARGET MACHINE
                write(sockfunc, cmd, sizeof(cmd));
                //send(sockfunc, cmd, sizeof(cmd), MSG_DONTWAIT);


                //Read the reply buffer, MSG_DONTWAIT - to continue even without response from server
                recv(sockfunc, reply, sizeof(reply), 0);
                //read(sockfunc, reply, sizeof(reply));


                //Need to press ENTER twice so the data is copied into reply buffer
                //This can be improved
                printf("%s\n",reply);
        }
        return 0;
}
