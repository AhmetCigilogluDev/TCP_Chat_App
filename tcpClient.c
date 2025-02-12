#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // Sunucunun IP adresi
#define SERVER_PORT 8080 // Sunucunun bağlantı noktası
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char message[BUF_SIZE];

    // Soket oluştur
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Sunucu adresini ayarla
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(sock);
        return -2;
    }

    // Sunucuya bağlan
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        close(sock);
        return -3;
    }

    printf("Connected to the server.\n");

    // Mesaj gönderme ve alma döngüsü
    while (1) {
        printf("Enter message: ");
        fgets(message, BUF_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0'; // Yeni satırı kaldır

        // Mesajı gönder
        if (send(sock, message, strlen(message), 0) < 0) {
            perror("Send failed");
            break;
        }

        // Sunucudan mesaj al
        int received = recv(sock, buffer, BUF_SIZE, 0);
        if (received > 0) {
            buffer[received] = '\0';
            printf("Received: %s\n", buffer);
        } else if (received == 0) {
            printf("Server disconnected.\n");
            break;
        }
    }

    close(sock);
    return 0;
}
