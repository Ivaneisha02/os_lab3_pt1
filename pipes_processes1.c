// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int pipe1to2[2];  // Used to store two ends of first pipe 
    int pipe2to1[2];  // Used to store two ends of second pipe 
  
    //char fixed_str[] = "howard.edu"; 
    char input_str[100]; 
    pid_t p; 
  
    if (pipe(pipe1to2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(pipe2to1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate:");
    scanf("%s", input_str); 
    input_str[101]='\0';
    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
  
        close(pipe1to2[0]);  // Close reading end of pipes 
        close(pipe2to1[0]);
        write(pipe1to2[1], input_str, strlen(input_str)+1); 
        read(pipe2to1[0],input_str,100);

        close(pipe2to1[0]);
  
        // Write input string and close writing end of first 
        // pipe. 
        //write(fd1[1], input_str, strlen(input_str)+1); 
        
  
        // Wait for child to send a string 
        wait(NULL); 
        strcat(input_str, "gobison.org");
        printf("Parent concatenated string: %s\n", input_str);
        close(pipe2to1[1]); // Close writing end of pipes 
        close(pipe1to2[0]); 
    } 
  
    // child process 
    else
    { 
        close(pipe1to2[1]);  // Close writing end of first pipes IN
        close(pipe2to1[0]); 
      
        // Read a string using first pipe 
        //char concat_str[100]; 
        read(pipe1to2[0], input_str, 100); 
        strcat(input_str, "howard.edu");
  
        // Concatenate a fixed string with it 
        /*
        int k = strlen(concat_str); 
        int i; 
        for (i=0; i<strlen(fixed_str); i++) 
            concat_str[k++] = fixed_str[i]; 
  
        concat_str[k] = '\0';   // string ends with '\0' 
        */
  
        printf("Concatenated string %s\n", input_str);
        printf("Enter string to concatenate:");
        scanf("%s", input_str);
        write(pipe2to1[1],input_str,strlen(input_str)+1);
        // Close both reading ends 
        close(pipe1to2[0]); 
        close(pipe2to1[1]); 

  
        exit(0); 
    } 
} 