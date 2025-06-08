#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/Window.hpp>
using namespace sf;
#include <iostream>
using namespace std;

int* client_sockets;
float block_size = 1224/15;
float redroute[2][57]={
    {
        (block_size*1)+10,(block_size*2)+10,(block_size*3)+10,(block_size*4)+10,(block_size*5)+10,

        (block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,
        (block_size*7)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,
        (block_size*8)+10,
        
        (block_size*9)+10,(block_size*10)+10,(block_size*11)+10,(block_size*6)+12,(block_size*13)+10,(block_size*14)+10,
        (block_size*14)+10,(block_size*14)+10,(block_size*13)+10,(block_size*12)+10,(block_size*11)+10,(block_size*10)+10,
        (block_size*9)+10,
        
        (block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,
        (block_size*7)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,
        (block_size*6)+10,
        
        (block_size*5)+10,(block_size*4)+10,(block_size*3)+10,(block_size*2)+10,(block_size*1)+10,(block_size*0)+10,
        (block_size*0)+10,(block_size*1)+10, (block_size*2)+10,(block_size*3)+10,(block_size*4)+10,(block_size*5)+10,
        (block_size*6)+10
    },
    {
        (block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,

        (block_size*5)+18,(block_size*4)+18,(block_size*3)+18,(block_size*2)+18,(block_size*1)+18,(block_size*0)+18,
        (block_size*0)+18,(block_size*0)+18,(block_size*1)+18,(block_size*2)+18,(block_size*3)+18,(block_size*4)+18,
        (block_size*5)+18,
        
        (block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,
        (block_size*7)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,
        (block_size*8)+18,
        
        (block_size*9)+18,(block_size*10)+18,(block_size*11)+18,(block_size*12)+18,(block_size*13)+18,(block_size*14)+18,
        (block_size*14)+18,(block_size*14)+18,(block_size*13)+18,(block_size*12)+18,(block_size*11)+18,(block_size*10)+18,
        (block_size*9)+18,
        
        (block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,
        (block_size*7)+18,(block_size*7)+18, (block_size*7)+18,(block_size*7)+18,(block_size*7)+18,(block_size*7)+18,
        (block_size*7)+18
    },
};

float greenroute[2][57]={
    {
        (block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,

        (block_size*9)+10,(block_size*10)+10,(block_size*11)+10,(block_size*6)+12,(block_size*13)+10,(block_size*14)+10,
        (block_size*14)+10,(block_size*14)+10,(block_size*13)+10,(block_size*12)+10,(block_size*11)+10,(block_size*10)+10,
        (block_size*9)+10,
        
        (block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,
        (block_size*7)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,
        (block_size*6)+10,
        
        (block_size*5)+10,(block_size*4)+10,(block_size*3)+10,(block_size*2)+10,(block_size*1)+10,(block_size*0)+10,
        (block_size*0)+10,(block_size*0)+10, (block_size*1)+10,(block_size*2)+10,(block_size*3)+10,(block_size*4)+10,
        (block_size*5)+10,
        
        (block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,
        (block_size*7)+10,(block_size*7)+10,(block_size*7)+10,(block_size*7)+10,(block_size*7)+10,(block_size*7)+10,
        (block_size*7)+10
    },
    {
        (block_size*1)+18,(block_size*2)+18,(block_size*3)+18,(block_size*4)+18,(block_size*5)+18,

        (block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,
        (block_size*7)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,
        (block_size*8)+18,
        
        (block_size*9)+18,(block_size*10)+18,(block_size*11)+18,(block_size*12)+18,(block_size*13)+18,(block_size*14)+18,
        (block_size*14)+18,(block_size*14)+18,(block_size*13)+18,(block_size*12)+18,(block_size*11)+18,(block_size*10)+18,
        (block_size*9)+18,
        
        (block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,
        (block_size*7)+18,(block_size*6)+18, (block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,
        (block_size*6)+18,
        
        (block_size*5)+18,(block_size*4)+18,(block_size*3)+18,(block_size*2)+18,(block_size*1)+18,(block_size*0)+18,
        (block_size*0)+18,(block_size*1)+18,(block_size*2)+18,(block_size*3)+18,(block_size*4)+18,(block_size*5)+18,
        (block_size*6)+18
    },
};

float yellowroute[2][57]={
    {
        (block_size*13)+10,(block_size*12)+10,(block_size*11)+10,(block_size*10)+10,(block_size*9)+10,

        (block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,
        (block_size*7)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,
        (block_size*6)+10,
        
        (block_size*5)+10,(block_size*4)+10,(block_size*3)+10,(block_size*2)+10,(block_size*1)+10,(block_size*0)+10,
        (block_size*0)+10,(block_size*0)+10, (block_size*1)+10,(block_size*2)+10,(block_size*3)+10,(block_size*4)+10,
        (block_size*5)+10,

        (block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,
        (block_size*7)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,
        (block_size*8)+10,
        
        (block_size*9)+10,(block_size*10)+10,(block_size*11)+10,(block_size*6)+12,(block_size*13)+10,(block_size*14)+10,
        (block_size*14)+10,(block_size*13)+10,(block_size*12)+10,(block_size*11)+10,(block_size*10)+10,(block_size*9)+10,
        (block_size*8)+10
    },
    {
        (block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,  
        
        (block_size*9)+18,(block_size*10)+18,(block_size*11)+18,(block_size*12)+18,(block_size*13)+18,(block_size*14)+18,
        (block_size*14)+18,(block_size*14)+18,(block_size*13)+18,(block_size*12)+18,(block_size*11)+18,(block_size*10)+18,
        (block_size*9)+18,

        (block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,
        (block_size*7)+18,(block_size*6)+18, (block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,
        (block_size*6)+18,

        (block_size*5)+18,(block_size*4)+18,(block_size*3)+18,(block_size*2)+18,(block_size*1)+18,(block_size*0)+18,
        (block_size*0)+18,(block_size*0)+18,(block_size*1)+18,(block_size*2)+18,(block_size*3)+18,(block_size*4)+18,
        (block_size*5)+18,
        
        (block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,
        (block_size*7)+18,(block_size*7)+18,(block_size*7)+18,(block_size*7)+18,(block_size*7)+18,(block_size*7)+18,
        (block_size*7)+18
    },
};
float blueroute[2][57]={
    {
        (block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,
        
        (block_size*5)+10,(block_size*4)+10,(block_size*3)+10,(block_size*2)+10,(block_size*1)+10,(block_size*0)+10,
        (block_size*0)+10,(block_size*0)+10, (block_size*1)+10,(block_size*2)+10,(block_size*3)+10,(block_size*4)+10,
        (block_size*5)+10,

        (block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,(block_size*6)+10,
        (block_size*7)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,
        (block_size*8)+10,
        
        (block_size*9)+10,(block_size*10)+10,(block_size*11)+10,(block_size*6)+12,(block_size*13)+10,(block_size*14)+10,
        (block_size*14)+10,(block_size*14)+10,(block_size*13)+10,(block_size*12)+10,(block_size*11)+10,(block_size*10)+10,
        (block_size*9)+10,
        
        (block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,(block_size*8)+10,
        (block_size*7)+10,(block_size*7)+10,(block_size*7)+10,(block_size*7)+10,(block_size*7)+10,(block_size*7)+10,
        (block_size*7)+10
    },
    {
        (block_size*13)+18,(block_size*12)+18,(block_size*11)+18,(block_size*10)+18,(block_size*9)+18,
        
        (block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,
        (block_size*7)+18,(block_size*6)+18, (block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,
        (block_size*6)+18,

        (block_size*5)+18,(block_size*4)+18,(block_size*3)+18,(block_size*2)+18,(block_size*1)+18,(block_size*0)+18,
        (block_size*0)+18,(block_size*0)+18,(block_size*1)+18,(block_size*2)+18,(block_size*3)+18,(block_size*4)+18,
        (block_size*5)+18,
        
        (block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,(block_size*6)+18,
        (block_size*7)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,(block_size*8)+18,
        (block_size*8)+18,
        
        (block_size*9)+18,(block_size*10)+18,(block_size*11)+18,(block_size*12)+18,(block_size*13)+18,(block_size*14)+18,
        (block_size*14)+18,(block_size*13)+18,(block_size*12)+18,(block_size*11)+18,(block_size*10)+18,(block_size*9)+18,
        (block_size*8)+18,
    },
};

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Socket creation failed");
        return 1;
    }
    
    int option = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Bind failed");
        return 1;
    }

    listen(sock,4);

    int window_x=1224;
    int window_y=1224;
    RenderWindow window(VideoMode(612*2,612*2), "MAIN GAME");
    Event e;
    Texture bgtexture;
    Sprite bgsprite;
    bgtexture.loadFromFile("ludo.jpg");
    bgsprite.setTexture(bgtexture);
    bgsprite.setScale(2,2);
    bgsprite.setPosition(0,0);
    Text pieces[4][4];
    Color colors[4] = {Color::Red,Color::Green,Color::Yellow,Color::Blue};
    Font font;
    font.loadFromFile("balls.ttf");
    float coordsx[4][4] = {
        (block_size*1.75),(block_size*1.75),(block_size*3.75),(block_size*3.75),
        (block_size*10.75),(block_size*10.75),(block_size*12.75),(block_size*12.75),
        (block_size*10.75),(block_size*10.75),(block_size*12.75),(block_size*12.75),
        (block_size*1.75),(block_size*1.75),(block_size*3.75),(block_size*3.75)
    };
    float coordsy[4][4] = {
        (block_size*1.6),(block_size*3.6),(block_size*1.6),(block_size*3.6),
        (block_size*1.6),(block_size*3.6),(block_size*1.6),(block_size*3.6),
        (block_size*10.6),(block_size*12.6),(block_size*10.6),(block_size*12.6),
        (block_size*10.6),(block_size*12.6),(block_size*10.6),(block_size*12.6)
    };

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            pieces[i][j].setColor(colors[i]);
            pieces[i][j].setPosition(coordsx[i][j],coordsy[i][j]);
            pieces[i][j].setString(to_string(j+1));
            pieces[i][j].setCharacterSize(60);
            pieces[i][j].setFont(font);
        }
    }

    window.draw(bgsprite);
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            window.draw(pieces[i][j]);
        }
    }
    window.display();

    printf("Waiting for clients to connect...\n");

    key_t key = ftok("sockets", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    client_sockets = (int *)shmat(shmid, (void *)0, 0);
    int client_count = 0;
    float** msg = new float*[4];
    msg[0] = new float[14];
    msg[1] = new float[14];
    msg[2] = new float[14];
    msg[3] = new float[14];

    for(int i=0;i<4;i++)
    {
        msg[i][0]=coordsx[i][0];
        msg[i][1]=coordsx[i][1];
        msg[i][2]=coordsx[i][2];
        msg[i][3]=coordsx[i][3];
        msg[i][4]=coordsy[i][0];
        msg[i][5]=coordsy[i][1];
        msg[i][6]=coordsy[i][2];
        msg[i][7]=coordsy[i][3];
        msg[i][8]=i;
        msg[i][9]=0;
        msg[i][10]=-1;
        msg[i][11]=-1;
        msg[i][12]=-1;
        msg[i][13]=-1;
    }

    for(int i=0;i<4;i++)
    {
        client_sockets[i] = accept(sock, NULL, NULL);
        if (client_sockets[i] < 0)
        {
            perror("Accept failed");
            continue;
        }

        printf("Player %d connected.\n",i);
        send(client_sockets[i],msg[i],56, 0);
    }

    int pwd=0;
    int x=0;

    while(window.isOpen())
    {
        while (window.pollEvent(e))
        {  
            if (e.type == Event::Closed) 
            {
                window.close();  
            }                	    
        }


        cout<<endl<<pwd<<endl;
        
        int bytes_received = recv(client_sockets[pwd], msg[pwd], 56, 0);

        cout<<"Player "<<pwd<<" rolled "<<msg[pwd][9]<<endl;

        // handle player logic

        if(msg[pwd][8]<0)
        {

        }

        else if(msg[pwd][8]<4)
        {
            switch(pwd)
            {
                case 0:
                    if(msg[pwd][int(msg[pwd][8]+10)]+msg[pwd][9] < 57)
                    {
                        msg[pwd][int(msg[pwd][8])] = redroute[0][int(msg[pwd][int(msg[pwd][8])+10]+msg[pwd][9])];
                        msg[pwd][int(msg[pwd][8])+4] = redroute[1][int(msg[pwd][int(msg[pwd][8])+10]+msg[pwd][9])];
                        msg[pwd][int(msg[pwd][8])+10]+=msg[pwd][9];
                    }
                    break;
                case 1:
                    if(int(msg[pwd][int(msg[pwd][8])+10]+msg[pwd][9]) < 57)
                    {
                        msg[pwd][int(msg[pwd][8])] = greenroute[0][int(msg[pwd][int(msg[pwd][8])+10]+msg[pwd][9])];
                        msg[pwd][int(msg[pwd][8])+4] = greenroute[1][int(msg[pwd][int(msg[pwd][8])+10]+msg[pwd][9])];
                        msg[pwd][int(msg[pwd][8])+10]+=msg[pwd][9];
                    }
                    break;
                case 2:
                    if(int(msg[pwd][int(msg[pwd][8])+10]+msg[pwd][9]) < 57)
                    {
                        msg[pwd][int(msg[pwd][8])] = yellowroute[0][int(msg[pwd][int(msg[pwd][8])+10]+msg[pwd][9])];
                        msg[pwd][int(msg[pwd][8])+4] = yellowroute[1][int(msg[pwd][int(msg[pwd][8])+10]+msg[pwd][9])];
                        msg[pwd][int(msg[pwd][8])+10]+=msg[pwd][9];
                    }
                    break;
                case 3:
                    if(int(msg[pwd][int(msg[pwd][8])+10]+msg[pwd][9]) < 57)
                    {
                        msg[pwd][int(msg[pwd][8])] = blueroute[0][int(msg[pwd][int(msg[pwd][8])+10]+msg[pwd][9])];
                        msg[pwd][int(msg[pwd][8])+4] = blueroute[1][int(msg[pwd][int(msg[pwd][8])+10]+msg[pwd][9])];
                        msg[pwd][int(msg[pwd][8])+10]+=msg[pwd][9];
                    }
                    break;
            }

        }

        else
        {
            cout<<msg[pwd][8]-10<<" "<<int(msg[pwd][8]-10)+4<<endl;
            switch(pwd)
            {
                case 0:
                        msg[pwd][int(msg[pwd][8]-10)] = redroute[0][0];
                        msg[pwd][int(msg[pwd][8]-10)+4] = redroute[1][0];
                        msg[pwd][int(msg[pwd][8]-10)+10]=0;
                    break;
                case 1:
                        msg[pwd][int(msg[pwd][8]-10)] = greenroute[0][0];
                        msg[pwd][int(msg[pwd][8]-10)+4] = greenroute[1][0];
                        msg[pwd][int(msg[pwd][8]-10)+10]=0;
                    break;
                case 2:
                        msg[pwd][int(msg[pwd][8]-10)] = yellowroute[0][0];
                        msg[pwd][int(msg[pwd][8]-10)+4] = yellowroute[1][0];
                        msg[pwd][int(msg[pwd][8]-10)+10]=0;
                    break;
                case 3:
                        msg[pwd][int(msg[pwd][8]-10)] = blueroute[0][0];
                        msg[pwd][int(msg[pwd][8]-10)+4] = blueroute[1][0];
                        msg[pwd][int(msg[pwd][8]-10)+10]=0;
                    break;
            }
        }

        msg[pwd][8]=4;

        // handle player logic

        for(int j=0;j<4;j++)
        {
            if(j!=pwd)
            {
                for(int k=0;k<4;k++)
                {
                    for(int p=0;p<4;p++)
                    {
                        if(msg[j][k]==msg[pwd][p] && msg[j][k+4]==msg[pwd][p+4])
                        {
                            cout<<"death "<<j<<" "<<k<<pwd<<" "<<p<<endl;
                            msg[j][k]=coordsx[j][k];
                            msg[j][k+4]=coordsy[j][k];
                            msg[j][k+10]=-1;
                        }
                    }
                }
            }
            msg[j][8]--;
            send(client_sockets[j], msg[j], 56, 0);
        }
        pwd = (pwd+1)%4;

        window.clear(Color::Black);
        window.draw(bgsprite);
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                pieces[i][j].setPosition(msg[i][j],msg[i][j+4]);
                window.draw(pieces[i][j]);
            }
        }
        window.display();
    }

    close(sock);

    return 0;
}
