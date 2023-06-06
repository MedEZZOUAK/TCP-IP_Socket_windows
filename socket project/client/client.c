
#include <winsock2.h>
#include <stdio.h>
#include <string.h>

#define MAX_ATTEMPTS 3
#define MAX_USERS 1000
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char privilege[50];
} User;
typedef struct {
char CIN[30];
char nom[30];
char prenom[30];
char GSM[30];
char email[50];
char rue[20];
char ville[20];
char pays[20];
}contacts;
void addContact(SOCKET clientsock){
   contacts contact ; 
    printf("Donner le numero de la carte nationale du contact: \t");
    scanf("%s",contact.CIN);
    printf("Donner le nom du contact: \t");
    scanf("%s",contact.nom);
    printf("Donner le prenom du contact: \t");
    scanf("%s",contact.prenom);
    printf("Donner le GSM : \t");
    scanf("%s",contact.GSM);

    printf("Donner l'email : \t");
    scanf("%s",contact.email);

    printf("\n saisie de l'adresse : \n");
    printf("Donner le nom de la rue  : \t");
    scanf("%s",contact.rue);

    printf("Donner la ville du contact : \t");
    scanf("%s",contact.ville);

    printf("Donner le pays du contact  : \t");
    scanf("%s",contact.pays); 
    char buffer[2084];
    sprintf(buffer,"%s %s %s %s %s %s %s %s",contact.CIN,contact.nom,contact.prenom,contact.GSM,contact.email,contact.rue,contact.ville,contact.pays);
    printf("le contact saisie : %s \n",buffer);

    send(clientsock,buffer,sizeof(buffer),0);
}
void searchContact(SOCKET clientsocket){
    char Cin[30];
    printf("Give the cin of the user :");
    scanf("%s",&Cin);
    //send the cin to the server
    send(clientsocket,Cin,sizeof(Cin),0);
    //receive the contact from the server
    char buffer[2084];
    recv(clientsocket,buffer,sizeof(buffer),0);
    printf("%s \n",buffer);
}
void DeleteContact(SOCKET clientsocket){
    char Cin[30];
    printf("Give the cin of the user :");
    scanf("%s",&Cin);
    //send the cin to the server
    send(clientsocket,Cin,sizeof(Cin),0);
   //recieve the message from the server
    char buffer[2084];
    recv(clientsocket,buffer,sizeof(buffer),0);
    printf("%s \n",buffer);
}
void ModifyContact(SOCKET clientsocket){
    //get the cin of the contact
    char Cin[30];
    printf("Give the cin of the user :");
    scanf("%s",&Cin);
    //send the cin to the server
    send(clientsocket,Cin,sizeof(Cin),0);
    //recieve the message from the server
    char buffer[2084];
    recv(clientsocket,buffer,sizeof(buffer),0);
    //if the contact exist ask the user to enter the new data
    if(strcmp(buffer,"Contact found.")==0){
        //ask the user to enter the new data
        contacts contact ;
        printf("Donner le numero de la carte nationale du contact: \t");
        scanf("%s",contact.CIN);
        printf("Donner le nouveau nom du contact: \t");
        scanf("%s",contact.nom);
        printf("Donner le nouveau prenom du contact: \t");
        scanf("%s",contact.prenom);
        printf("Donner le nouveau GSM : \t");
        scanf("%s",contact.GSM);
        printf("Donner le nouveau email : \t");
        scanf("%s",contact.email);
        printf("\n saisie de la nouvelle adresse : \n");
        printf("Donner le nouveau nom de la rue  : \t");
        scanf("%s",contact.rue);
        printf("Donner la nouvelle ville du contact : \t");
        scanf("%s",contact.ville);
        printf("Donner le nouveau pays du contact  : \t");
        scanf("%s",contact.pays);
        char buffer2[2084];
        sprintf(buffer2,"%s %s %s %s %s %s %s %s",contact.CIN,contact.nom,contact.prenom,contact.GSM,contact.email,contact.rue,contact.ville,contact.pays);
        send(clientsocket,buffer2,sizeof(buffer2),0);

        //recieve the message from the server
        char buffer3[2084];
        recv(clientsocket,buffer3,sizeof(buffer3),0);
        printf("%s \n",buffer3);
    }else{
      printf("Contact not found.\n");
    }
}
void showall(SOCKET clientsocket){
    //recieve the number of contacts
    char buffer[2084];
    recv(clientsocket,buffer,sizeof(buffer),0);
    printf("le nombre totale des contacts est : %s \n",buffer);
    int n=atoi(buffer);
    //recieve the contacts
    for(int i=0;i<n;i++){
        char buffer2[2084];
        memset(buffer2,0,sizeof(buffer2));
        recv(clientsocket,buffer2,sizeof(buffer2),0);
        printf("%s \n",buffer2);
    }

}




int main(){


    // Initialize Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Error initializing Winsock.\n");
        return 1;
    }

    // Create socket
    SOCKET clientsock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsock == INVALID_SOCKET) {
        printf("Error creating socket.\n");
        WSACleanup();
        return 1;
    }
    // ASK the user for the ip adress of the server
    char ip[30];
    printf("Give the ip adress of the server :");
    scanf("%s",&ip);
    fflush(stdin);

    // Connect to server
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(ip);
    serveraddr.sin_port = htons(1234); // replace with actual port number

    if (connect(clientsock, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("Error connecting to server.\n");
        closesocket(clientsock);
        WSACleanup();
        return 1;
    }

    printf("Connected to server.\n");

    Sleep(10);
    User users[MAX_USERS];
    int num_users = 0;
    FILE *fp = fopen("C:\\Users\\Asus\\Desktop\\socket project\\server\\login.txt", "r");
    if (fp == NULL) {
        printf("Error: Failed to open login file.\n");
        exit(1);
    }
    

    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        // Split the line into fields
        char *username = strtok(line, " ");
        char *password = strtok(NULL, " ");
        char *privilege_str = strtok(NULL, " ");

        // Remove newline character from privilege string
        privilege_str[strcspn(privilege_str, "\n")] = '\0';


        // Create a new user struct
        User user;
        strcpy(user.username, username);
        strcpy(user.password, password);
        strcpy(user.privilege, privilege_str);

        // Add the user to the table of users
        if (num_users < MAX_USERS) {
            users[num_users] = user;
            num_users++;
        } else {
            printf("Warning: Maximum number of users exceeded.\n");
            break;
        }
    }

    fclose(fp);

    // Loop to allow the user 3 attempts to login
    int attempts_left = 3;
    int is_authenticated = 0;
    int is_admin = 0;
    char temp_privilege[50];
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    while (attempts_left > 0 && !is_authenticated) {
        
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);

        // Check if the entered credentials match any of the users in the table
        for (int i = 0; i < num_users; i++) {
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
                is_authenticated = 1;
                strcpy(temp_privilege, users[i].privilege);
                
                break;
            }
        }

        if (!is_authenticated) {
            attempts_left--;
            if (attempts_left > 0) {
                printf("Login failed. You have %d attempts left.\n", attempts_left);
            } else {
                printf("Login failed. No attempts left.\n");
                printf("Exiting...\n");
                Sleep(10);
                exit(1);
            }
        }
    }
    
    is_admin=strcmp(temp_privilege,"admin");

    if (is_authenticated) {
        printf("Login successful.\n");
        if (!is_admin) {
            printf("Welcome, admin user!\n");
        } else {
            printf("Welcome, regular user!\n");
        }
    }
    //send the privilege to the server
    send(clientsock, temp_privilege, sizeof(temp_privilege), 0);
    //send the username to the server
    send(clientsock, username, sizeof(username), 0);
    //if the user is admin, show the admin menu
    if(!is_admin){
       int choice;
    do{
    printf("**********ADMIN MENU**********\n");
    printf("1. Add Contact\n");
    printf("2. Search Contact\n");
    printf("3. Delete Contact\n");
    printf("4. Modify Contact\n");
    printf("5. View All Contacts\n");
    printf("6. Exit\n");
    printf("*****************************\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    int temp=choice;
    //transform choice to char 
    char choice_char[1];
    sprintf(choice_char, "%d", choice);
    //send it to server
    send(clientsock, choice_char, sizeof(choice_char), 0);
    choice=temp;
    

    switch(choice){
        case 1:
            addContact(clientsock);
            getchar();
            break;
        case 2:
            searchContact(clientsock);
            getchar();
            break;
        case 3:
            DeleteContact(clientsock);
            break;
        case 4:
            ModifyContact(clientsock);
            break;
        case 5:
            showall(clientsock);
            break;
        case 6:
            exit(0);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }
   }while(choice!=6);
        


 }
    //if the user is not admin, show the user menu
    else{
        int choice;
    do{
    printf("**********USER MENU**********\n");
    printf("1. Search Contact\n");
    printf("2. View All Contacts\n");
    printf("3. Exit\n");
    printf("*****************************\n");
    printf("Enter your choice: ");
    
    scanf("%d", &choice);
    int temp=choice;
    //transform choice to char 
    char choice_char[1];
    sprintf(choice_char, "%d", choice);
    //send it to server
    send(clientsock, choice_char, sizeof(choice_char), 0);
    choice=temp;
    switch(choice){
        case 1:
            searchContact(clientsock);
            getchar();
            break;
        case 2:
            showall(clientsock);
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            
    }
  }while(choice!=3);
 }

    


}    

    

