#include <unistd.h>  
#include <string.h>  

int main() {
    char buffer[512] = "Hello world!\n";
    write(1, buffer, strlen(buffer)); 
    return 0;
}
