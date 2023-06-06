#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include<unistd.h>

#define MAX_ATTEMPTS 3
#define MAX_USERS 10
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_LINE_LENGTH 101

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
void addContact( SOCKET clientsock){
    //open the file in append mode
    FILE *f = fopen("contacts.txt", "a+");
    if (f == NULL) {
        printf("Error: Failed to open contacts file.\n");
        sleep(10);

        exit(1);
    }
    
    contacts contacts;
    //wait for amessage from the client

    char buffer[2084];
    recv(clientsock,buffer,sizeof(buffer),0);
    sscanf(buffer,"%s %s %s %s %s %s %s %s",contacts.CIN,contacts.nom,contacts.prenom,contacts.GSM,contacts.email,contacts.rue,contacts.ville,contacts.pays);
    //write the new contact to the file

    fprintf(f,"%s %s %s %s %s %s %s %s\n", contacts.CIN, contacts.nom,contacts.prenom,contacts.GSM , contacts.email, contacts.rue, contacts.ville, contacts.pays);
    //print the new contact to the console
    printf("Contact added successfully.\n");
    printf("CIN: %s\t", contacts.CIN);
    printf("Nom: %s\t", contacts.nom);
    printf("Prenom: %s\t", contacts.prenom);
    printf("GSM: %s\t", contacts.GSM);
    printf("Email: %s\t", contacts.email);
    printf("Rue: %s\t", contacts.rue);
    printf("Ville: %s\t", contacts.ville);
    printf("Pays: %s\t", contacts.pays);
    printf("\n");


    fclose(f);
    sleep(10);

}
void searchContact(SOCKET clientsocket){
    //open the file in read mode
    FILE *f = fopen("contacts.txt", "r");
    if (f == NULL) {
        printf("Error: Failed to open contacts file.\n");
        sleep(10);

        exit(1);
    }
    //wait for the cin from the client
    char Cin[30];
    recv(clientsocket,Cin,sizeof(Cin),0);
    printf("the cin is %s \n",Cin);
    //declare a contact
    contacts contacts;
    //declare a bool to check if the contact is found or not
    int found=0;
    //read the file line by line
    while(fscanf(f,"%s %s %s %s %s %s %s %s", contacts.CIN, contacts.nom,contacts.prenom,contacts.GSM , contacts.email, contacts.rue, contacts.ville, contacts.pays)!=EOF){
        //if the cin is found
        if(strcmp(Cin,contacts.CIN)==0){
            //set the bool to 1
            found=1;
            //print the contact to the console
            printf("Contact found successfully.\n");
            printf("CIN: %s\t", contacts.CIN);
            printf("Nom: %s\t", contacts.nom);
            printf("Prenom: %s\t", contacts.prenom);
            printf("GSM: %s\t", contacts.GSM);
            printf("Email: %s\t", contacts.email);
            printf("Rue: %s\t", contacts.rue);
            printf("Ville: %s\t", contacts.ville);
            printf("Pays: %s\t", contacts.pays);
            printf("\n");
            //send the contact to the client
            char buffer[2084];
            sprintf(buffer,"The contact : %s %s %s %s %s %s %s %s", contacts.CIN, contacts.nom,contacts.prenom,contacts.GSM , contacts.email, contacts.rue, contacts.ville, contacts.pays);
            send(clientsocket,buffer,sizeof(buffer),0);
            break;
        }
    }
    //if the contact is not found
    if(found==0){
        //print the message to the console
        printf("Contact not found.\n");
        //send the message to the client
        char buffer[2084];
        sprintf(buffer,"Contact not found.\n");
        send(clientsocket,buffer,sizeof(buffer),0);
    }


    
}
void DeleteContact(SOCKET clientsocket){
    //recive the cin from the client
    char Cin[30];
    memset(Cin,0,sizeof(Cin));
    recv(clientsocket,Cin,sizeof(Cin),0);
    printf("the cin is %s \n",Cin);
    //open the file in read mode
    FILE *f = fopen("contacts.txt", "r");
    if (f == NULL) {
        printf("Error: Failed to open contacts file.\n");
         exit(1);
    }
    //declare a contact
    contacts contact;
    //declare a bool to check if the contact is found or not
    int found=0;
    //read the file line by line
    while(fscanf(f,"%s %s %s %s %s %s %s %s", contact.CIN, contact.nom,contact.prenom,contact.GSM , contact.email, contact.rue, contact.ville, contact.pays)!=EOF){
        //if the cin is found
        if(strcmp(Cin,contact.CIN)==0){
            //set the bool to 1
            found=1;
            //print the contact to the console
            printf("Contact found successfully.\n");
            printf("CIN: %s\t", contact.CIN);
            printf("Nom: %s\t", contact.nom);
            printf("Prenom: %s\t", contact.prenom);
            printf("GSM: %s\t", contact.GSM);
            printf("Email: %s\t", contact.email);
            printf("Rue: %s\t", contact.rue);
            printf("Ville: %s\t", contact.ville);
            printf("Pays: %s\t", contact.pays);
            printf("\n");
            break;
        }
    }
    //rewind the file
    rewind(f);
    //if the contact is found delete it
    //create a temp file
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error: Failed to open temp file.\n");
        exit(1);
    }
    //copy all the contacts to the temp file except the contact to be deleted
    while(fscanf(f,"%s %s %s %s %s %s %s %s", contact.CIN, contact.nom,contact.prenom,contact.GSM , contact.email, contact.rue, contact.ville, contact.pays)!=EOF){
        if(strcmp(Cin,contact.CIN)!=0){
            fprintf(temp,"%s %s %s %s %s %s %s %s \n", contact.CIN, contact.nom,contact.prenom,contact.GSM , contact.email, contact.rue, contact.ville, contact.pays);
        }
    }
    //close the files
    fclose(f);
    fclose(temp);
    //dekete the original file
    std:remove("contacts.txt");
    //rename the temp file to the original file
    rename("temp.txt","contacts.txt");
    //if the contact is not found
    if(found==0){
        //print the message to the console
        printf("Contact not found.\n");
        //send the message to the client
        char buffer[2084];
        sprintf(buffer,"Contact not found.\n");
        send(clientsocket,buffer,sizeof(buffer),0);
    }
    else{
        //print the message to the console
        printf("Contact deleted successfully.\n");
        //send the message to the client
        char buffer[2084];
        sprintf(buffer,"Contact deleted successfully.\n");
        send(clientsocket,buffer,sizeof(buffer),0);
    }

}
void ModifyContact(SOCKET clientsocket){
    //recive the cin from the client
    char Cin[30];
    memset(Cin,0,sizeof(Cin));
    recv(clientsocket,Cin,sizeof(Cin),0);
    printf("the cin to be modifyed is %s \n",Cin);
    //open the file in read mode
    FILE *f = fopen("contacts.txt", "r");
    if (f == NULL) {
        printf("Error: Failed to open contacts file.\n");
         exit(1);
    }
    //declare a contact
    contacts contact;
    //declare a bool to check if the contact is found or not
    int found=0;
    //read the file line by line
    while(fscanf(f,"%s %s %s %s %s %s %s %s", contact.CIN, contact.nom,contact.prenom,contact.GSM , contact.email, contact.rue, contact.ville, contact.pays)!=EOF){
        //if the cin is found
        if(strcmp(Cin,contact.CIN)==0){
            //set the bool to 1
            found=1;
            //print the contact to the console
            printf("Contact found successfully.\n");
            break;
        }
    }
    //rewind the file
    rewind(f);
    //create a temp file
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error: Failed to open temp file.\n");
        exit(1);
    }
    //copy all the contacts to the temp file except the contact to be modified
    while(fscanf(f,"%s %s %s %s %s %s %s %s", contact.CIN, contact.nom,contact.prenom,contact.GSM , contact.email, contact.rue, contact.ville, contact.pays)!=EOF){
        if(strcmp(Cin,contact.CIN)!=0){
            fprintf(temp,"%s %s %s %s %s %s %s %s \n", contact.CIN, contact.nom,contact.prenom,contact.GSM , contact.email, contact.rue, contact.ville, contact.pays);
        }
    }
    //close the files
    fclose(f);
    //if the contact is found send a Contact found. message to the client
    if(found==1){
        char buffer[2084];
        sprintf(buffer,"Contact found.");
        send(clientsocket,buffer,sizeof(buffer),0);
        //recive the new contact from the client
        char newcontact[2084];
        memset(newcontact,0,sizeof(newcontact));
        recv(clientsocket,newcontact,sizeof(newcontact),0);
        //add the new contact to the temp file
        fprintf(temp,"%s",newcontact);
        //close the temp file
        fclose(temp);
        //delete the original file
        std:remove("contacts.txt");
        //rename the temp file to the original file
        rename("temp.txt","contacts.txt");
        //print the message to the console
        printf("Contact modified successfully.\n");
        printf(" The Modifyed contact is : \n%s", newcontact);
        //send the message to the client
        char buffer2[2084];
        sprintf(buffer2,"Contact modified successfully.\n");
        send(clientsocket,buffer2,sizeof(buffer2),0);
        
    }else{
        //print the message to the console
        printf("Contact not found.\n");
    }

}
void showallcontacts(SOCKET clientsocket){
    //the number of contacts
    int i=0;
    //open the contacts file
    FILE *f = fopen("contacts.txt", "r");
    if (f == NULL) {
        printf("Error: Failed to open contacts file.\n");
        sleep(10);

        exit(1);
    }
    //declare a contact
    contacts contact;
    //read the file line by line
    while(fscanf(f,"%s %s %s %s %s %s %s %s", contact.CIN, contact.nom,contact.prenom,contact.GSM , contact.email, contact.rue, contact.ville, contact.pays)!=EOF){
        //increment the number of contacts
        i++;
    }
    rewind(f);
    //send the number of contacts to the client
    char buffer[2084];
    sprintf(buffer,"%d",i);
    send(clientsocket,buffer,sizeof(buffer),0);
    //send contact after contact to the client
    while(fscanf(f,"%s %s %s %s %s %s %s %s", contact.CIN, contact.nom,contact.prenom,contact.GSM , contact.email, contact.rue, contact.ville, contact.pays)!=EOF){
        char buffer[2084];
        sprintf(buffer,"%s %s %s %s %s %s %s %s", contact.CIN, contact.nom,contact.prenom,contact.GSM , contact.email, contact.rue, contact.ville, contact.pays);
        send(clientsocket,buffer,sizeof(buffer),0);
    }
    //close the file
    fclose(f);

}


int main(){
    // Initialize Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Error initializing Winsock.\n");
        return 1;
    }

    // Create socket
    SOCKET serversock = socket(AF_INET, SOCK_STREAM, 0);
    if (serversock == INVALID_SOCKET) {
        printf("Error creating socket.\n");
        WSACleanup();
        return 1;
    }

    // Bind socket to port 6000 and the ip 100.104.161.220
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(6000);
    serveraddr.sin_addr.s_addr = inet_addr("192.168.214.221");

    

    if (bind(serversock, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("Error binding socket.\n");
        closesocket(serversock);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(serversock, SOMAXCONN) == SOCKET_ERROR) {
        printf("Error listening for connections.\n");
        closesocket(serversock);
        WSACleanup();
        return 1;
    }

    printf("Listening for incoming connections...\n");
    // Accept client connection
    struct sockaddr_in clientaddr;
    int addrlen = sizeof(clientaddr);
    SOCKET clientsock = accept(serversock, (struct sockaddr *)&clientaddr, &addrlen);

    if (clientsock == INVALID_SOCKET) {
        printf("Error accepting client connection.\n");
        closesocket(serversock);
        WSACleanup();
        return 1;
    }
    printf("Client connected.\n");
    //print the ip address of the client
    printf("Client IP address: %s\n", inet_ntoa(clientaddr.sin_addr));
    //print the port
    printf("Port : %d\n",serveraddr.sin_port);
    //scan the file for the username and password and privilege
    //read login info from file
   
    //receive privlige from client
    char privilege[50];
    recv(clientsock, privilege, sizeof(privilege), 0);
    //receive username from client
    char username[MAX_USERNAME_LENGTH];
    recv(clientsock, username, sizeof(username), 0);
    //print the username is connected as privlege
    printf("%s connected as %s \n",username,privilege);
    //declare a bool to check if the user is admin or not
    int admin=0;
    //if the user is admin print match 
    if(strcmp(privilege,"admin")==0){
        printf("match admin \n");
        //set the bool to 1
        admin=1;
        
    }else if(strcmp(privilege,"user")==0){
        printf("match  user \n");
        //set the bool to 1
        admin=2;
        
    }else{
        printf("no match \n");
    }
    if(admin==1){
        char choice[50]; 
        int nmbr=0; 
        //as match as the choice is not 6
        do{
        //ckear the buffer
        memset(choice,0,sizeof(choice));
        //receive the choice from the client
        recv(clientsock, choice, sizeof(choice), 0);
        //transfotm the char to int 
        
        nmbr=atoi(choice);
        printf("choice is %d \n",nmbr);
        //switch case to call the function according to the choice
     switch(nmbr){
        case 1:
            //call the addContact function
            printf("	%s trying to add a contact \n",username);
            addContact(clientsock);
            break;

        case 2:
        	printf("	%s trying to search a contact \n",username);
            searchContact(clientsock);
            break;
        case 3:
        	printf("	%s trying to delete a contact \n",username);
            DeleteContact(clientsock);
            break;
        case 4:
        	printf("	%s trying to modify a contact \n",username);
            ModifyContact(clientsock);
            break;
        case 5:
        	printf("	%s trying to view all contacts a contact \n",username);
            showallcontacts(clientsock);
            break;
        case 6:
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;}
    }while(nmbr!=6);
        

  }else if (strcmp(privilege,"user")==0){
        char choice[50]; 
        int nmbr=0; 
        //as match as the choice is not 6
        do{
        //ckear the buffer
        memset(choice,0,sizeof(choice));
        //receive the choice from the client
        recv(clientsock, choice, sizeof(choice), 0);
        //transfotm the char to int 
        
        nmbr=atoi(choice);
        printf("choice is %d \n",nmbr);
        //switch case to call the function according to the choice
        switch(nmbr){
            case 1:
                printf("	%s trying to search a contact \n",username);
                searchContact(clientsock);
                break;
            case 2:
                printf("	%s trying to view all contacts a contact \n",username);
                showallcontacts(clientsock);
                break;
            case 3:
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
        


    }while(nmbr!=3);



    }else{
        printf("you are not allowed to connect \n");
    }
}

        