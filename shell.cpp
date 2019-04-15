#include "shell.h"

class Command
{
    char * path;
    char * inputArray[MAX_ARG_SIZE];
    char inputStr[MAX_BUF_SIZE];
    int arguments;
    int jobNumber;
    
    public:
    Command(void)
    {
        path = NULL;
        for( int i = 0; i < MAX_ARG_SIZE; i++ )
        {
            inputArray[i] = NULL;
        }
        jobNumber = 0;
        arguments = 0;
    }

    void setInput( string inStr )
    {
        // Parses inStr and populates inputArray and path accordingly
        char * end;
        int i = 1;
        strcpy( inputStr, inStr.c_str() );

        if( *inputStr == '\n' )
            // No input, just return
            return;

        inputArray[0] = inputStr;
        path = inputStr;
        arguments = 1;
        end = inputStr;

        while( *end != '\0' )
        {
            // while we haven't reached the end of the input
            if( *end == ' ' )
            {
                // if we are not reading a argument..
                // TODO: add other whitespace cases
                *end = '\0';
                if( *( end + 1 ) != '\n' || *( end + 1 ) != ' ' )
                {
                    // ..but we are about start
                    // add next argument to array and incrememnt arguments
                    inputArray[i++] = ++end;
                    arguments++;
                }
            }
            else if( *end == '\n' )
            {
                // if we are about to reach the end of the array,
                // just prepare to finish
                inputArray[i] = NULL;
                *end++ = '\0';
            }
            else
            {
                end++;
            }
        }
        
        return;
    }

    void execute(void)
    {
        // Execute the command in inputArray
        if( path == NULL )
            // don't do anything if no path is given
            // since we are in the child process, kill yourself
            exit(0);

        execv( path, inputArray );
    }
};

class Shell
{
    string strBuf;
    //char strBuf[MAX_BUF_SIZE];
    Command currentCmd;

    public:
    void getInput( void )
    {
        // Gets an input from the user, and creates a command based on it
        cout << "$>";
        getline( cin, strBuf );
        moveInputToCommand( strBuf );
    }

    void moveInputToCommand( string inString )
    {
        // Creates a new command object based on the input
        currentCmd.setInput( inString );
    }

    void forkAndExecute()
    {
        // Executes the current cmd
        if( fork() == 0 )
        {
            currentCmd.execute();
        }
    }
};



int main( int argc, char * argv[] )
{
    Shell activeShell;

    while( 1 )
    {
        activeShell.getInput();
        activeShell.forkAndExecute();
    }
}

/*
command object?
helpful for background tasks

lets implement it, barebones, and build on its functionality later
*/
