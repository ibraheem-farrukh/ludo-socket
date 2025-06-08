#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
using namespace std;

int main() {

    srand(time(NULL));

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to game. You can start sending messages...\n");

    float* msg = new float[14];

    int bytes_received = recv(sock, msg, 56, 0);
    cout<<bytes_received<<endl;

    float kill_values[2][4] = {
        msg[0],msg[1],msg[2],msg[3],
        msg[4],msg[5],msg[6],msg[7]
    };
    int out[4]={0,0,0,0};

    if(bytes_received > 0)
    {
        cout<<"Your First Turn In : "<<msg[8]<<endl;
    }

    while(1)
    {
        while(msg[8]!=0)
        {
            recv(sock, msg, 56, 0);
            cout<<"Your Turn In : "<<msg[8]<<endl;

            for(int i=0;i<4;i++)
            {
                if(msg[10+i]==-1)
                {
                    msg[i]=kill_values[0][i];
                    msg[i+4]=kill_values[1][i];
                }
            }

        }

        msg[9] = rand() % 6 + 1;
        bool turndone=1;
        while(turndone)
        {
            cout<<"You Rolled : "<<msg[9]<<endl; 
            cout<<"Which Piece Do You Wish To Move ? "; 
            cin>>msg[8];
            cout<<msg[0]<<" "<<msg[1]<<" "<<msg[2]<<" "<<msg[3]<<endl;

            if(msg[8]>3 || msg[8]<0)
            {
                cout<<"Invalid Piece ! Try Again."<<endl;
                continue;
            }

            else if(msg[9]!=6 && (msg[10]==-1) && (msg[11]==-1) && (msg[12]==-1) && (msg[13]==-1) )
            {
                cout<<"Can't Move Anything."<<endl;
                msg[8]=-1;
                turndone=0;
                continue;
            }

            else if(msg[9]!=6 && msg[int(msg[8])]==kill_values[0][int(msg[8])] && msg[int(msg[8])] == kill_values[1][int(msg[8])])
            {
                cout<<"Killed Piece ! Try Again."<<endl;
                continue;
            }

            else if(msg[9]==6 && msg[int(msg[8])]==kill_values[0][int(msg[8])] && msg[int(msg[8])+4] == kill_values[1][int(msg[8])])
            {
                cout<<"ok move"<<endl;
                out[int(msg[8])]=1;
                msg[8]+=10;
                turndone=0;
            }

            else
            {
                cout<<"okest move"<<endl;
                turndone=0;
            }
        }

        send(sock, msg, 56,0);
        recv(sock, msg, 56,0);
    }



    // if(id==0)
    // {
    //     while (1) {
        
    //         printf("Client: ");
    //         fgets(msg, sizeof(msg), stdin);
    //         msg[strcspn(msg, "\n")] = 0;  

    //         if (strcmp(msg, "exit") == 0) {
    //             send(sock, msg, strlen(msg), 0);
    //             break;
    //         }

    //         send(sock, msg, strlen(msg), 0);

            
    //         int bytes_received = recv(sock, buff, sizeof(buff) - 1, 0);
    //         if (bytes_received > 0) {
    //             buff[bytes_received] = '\0';
    //             printf("Server: %s\n", buff);
    //         }
    //     }
    // }


    // else
    // {
    //     while (1) {
    //         if (strcmp(msg, "exit") == 0) {
    //             send(sock, msg, strlen(msg), 0);
    //             break;
    //         }
            
    //         int bytes_received = recv(sock, buff, sizeof(buff) - 1, 0);
    //         if (bytes_received > 0) {
    //             buff[bytes_received] = '\0';
    //             printf("Server: %s\n", buff);
    //         }
    //     }
    // }
    
    

    close(sock);
    return 0;
}

